///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2014 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// This file was created on Tuesday, 15th July 2014 at 15:55:28 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar.h"
#include <stdio.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <omp.h>

int orbiter_count = 0;

orbiter_core :: orbiter_core (void) {
	pthread_mutex_init (& main_mutex, 0);
	noise24b = 0;
	root = 0;
	requested_active_size = 4096;
	active_pointer = 0; actives = 0; active_limit = 0;
	this -> centre_frequency = 330.0;
	this -> sampling_frequency = 48000.0;
	this -> headroom_fraction = 0.4;
	this -> latency_block_size = 128;
	output_device = input_device = 0;
	time_delta = time_deltas + 16384;
	sampler_time_delta = sampler_time_deltas + 16384;
	control_time_delta = control_time_deltas + 8192;
	amplitude = amplitudes + 16383;
	filter_freq = filter_freqs + 8192;
	pan = pans + 8192;
	power_pan = power_pans + 8192;
	linear_pan = linear_pans + 8192;
	for (int ind = 0; ind > -16384; ind--) * (amplitude + ind) = pow (2.0, (double) ind / 1536.0); * amplitudes = 0.0;
	//for (int ind = 0; ind > -16384; ind--) * (amplitude + ind) = pow (2.0, (double) ind / 4608.0) * (1.0 + (double) ind / 16384.0);
	for (int ind = 0; ind <= 16384; ind++) sine_wave [ind] = sin ((double) ind * M_PI * 2.0 / 16384.0);
	for (int ind = 0; ind < 16384; ind++) volumes [ind] = pow (2.0, (double) (ind - 12800) / 12800.0) * (double) ind / 12800.0;
	double blep = 0.0;
	for (int ind = 0; ind < 16384; ind++) {
		double angle = (double) (ind - 512) * M_PI / 512.0;
		double sinc = angle == 0.0 ? 1.0 : sin (angle) / angle;
		blep += sinc / 512.0;
		min_blep [ind] = 2.0 * blep / 1.08949;
	}
	for (int ind = 0; ind <= 16384; ind++) {
		pans [ind] = sin (M_PI * (double) ind / 32768.0);
		power_pans [ind] = sqrt ((double) ind / 16384.0);
		linear_pans [ind] = (double) ind / 16384.0;
	}
	amplitude_zero = pow (2.0, -16383.0 / 1536.0);
	recalculate ();
}

orbiter_core :: ~ orbiter_core (void) {pthread_mutex_destroy (& main_mutex); if (actives != 0) delete [] actives; actives = 0;}

int orbiter_core :: size_limit (int size, int limit) {
	size = (int) ((double) size * sampling_frequency / 44100.0);
	if (size < 1) return 1;
	if (size > limit) return limit;
	return size;
}

void orbiter_core :: recalculate (void) {
	pthread_mutex_lock (& main_mutex);
	// ACTIVES ....
	if (requested_active_size < 16) requested_active_size = 16;
	if (active_limit != requested_active_size) {
		if (actives != 0) delete [] actives;
		active_limit = requested_active_size;
		actives = new orbiter_pointer [active_limit];
		active_pointer = 0;
		if (root != 0) {
			actives [active_pointer++] = root;
			orbiter * orp = root -> next;
			while (orp != root) {actives [active_pointer++] = orp; orp = orp -> next;}
		}
	}
	// .... ACTIVES
	DSP_time_fraction = sampling_frequency / 8192.0;
	DSP_CHORUS_time_fraction = DSP_time_fraction * 0.1;
	gate_gap = sampling_frequency / 2048.0;
	gate_delay = 48000.0 / sampling_frequency;
	double delay = sampling_frequency > 0.0 ? centre_frequency  / sampling_frequency : centre_frequency;
	for (int ind = 0; ind < 32768; ind++) time_deltas [ind] = delay * pow (2.0, ((double) (ind - 16384) / 1536.0));
	sample_duration = delay = sampling_frequency > 0.0 ? 1.0 / sampling_frequency : 1.0;
	for (int ind = 0; ind < 32768; ind++) sampler_time_deltas [ind] = delay * pow (2.0, ((double) (ind - 16384) / 1536.0));
	for (int ind = 0; ind < 16384; ind++) control_time_deltas [ind] = delay * pow (2.0, ((double) (ind - 8192) / 768.0));
	for (int ind = 0; ind < 16384; ind++) waiting_times_16384 [ind] = 16384.0 * (waiting_times [ind] = delay * pow (2.0, ((double) (ind - 8192) / -768.0)));
	double power_fraction = 16384.0 / 1536.0;
	for (int ind = 0; ind < 16384; ind++) waiting_powers [ind] = pow (0.5, power_fraction / (sampling_frequency * pow (2.0, ((double) ind - 8192.0) / 768.0)));
	for (int ind = 0; ind < 16384; ind++) filter_freqs [ind] = 2.0 * sin (M_PI * 0.5 * centre_frequency * pow (2.0, (double) (ind - 8192) / 1536.0) / sampling_frequency);
	for (int ind = 0; ind < 16384; ind++) if (filter_freqs [ind] > 0.996) filter_freqs [ind] = 0.996;
	// .... FREEVERB SIZES
	int stereo_offset = (int) (23.0 * sampling_frequency / 44100.0);
	left_freeverb_comb_sizes [0] = size_limit (1116, 4096);
	right_freeverb_comb_sizes [0] = size_limit (1116 + stereo_offset, 4096);
	left_freeverb_comb_sizes [1] = size_limit (1188, 4096);
	right_freeverb_comb_sizes [1] = size_limit (1188 + stereo_offset, 4096);
	left_freeverb_comb_sizes [2] = size_limit (1277, 4096);
	right_freeverb_comb_sizes [2] = size_limit (1277 + stereo_offset, 4096);
	left_freeverb_comb_sizes [3] = size_limit (1356, 4096);
	right_freeverb_comb_sizes [3] = size_limit (1356 + stereo_offset, 4096);
	left_freeverb_comb_sizes [4] = size_limit (1422, 4096);
	right_freeverb_comb_sizes [4] = size_limit (1422 + stereo_offset, 4096);
	left_freeverb_comb_sizes [5] = size_limit (1491, 4096);
	right_freeverb_comb_sizes [5] = size_limit (1491 + stereo_offset, 4096);
	left_freeverb_comb_sizes [6] = size_limit (1557, 4096);
	right_freeverb_comb_sizes [6] = size_limit (1557 + stereo_offset, 4096);
	left_freeverb_comb_sizes [7] = size_limit (1617, 4096);
	right_freeverb_comb_sizes [7] = size_limit (1617 + stereo_offset, 4096);
	left_freeverb_allpass_sizes [0] = size_limit (556, 1536);
	right_freeverb_allpass_sizes [0] = size_limit (556 + stereo_offset, 1536);
	left_freeverb_allpass_sizes [1] = size_limit (441, 1536);
	right_freeverb_allpass_sizes [1] = size_limit (441 + stereo_offset, 1536);
	left_freeverb_allpass_sizes [2] = size_limit (341, 1536);
	right_freeverb_allpass_sizes [2] = size_limit (341 + stereo_offset, 1536);
	left_freeverb_allpass_sizes [3] = size_limit (225, 1536);
	right_freeverb_allpass_sizes [3] = size_limit (225 + stereo_offset, 1536);
	pthread_mutex_unlock (& main_mutex);
}

void orbiter_core :: move_modules (void) {
	noise24b = (noise24b * 0x5599d1 + 1) & 0xffffff;
	#pragma omp parallel for
	for (int ind = 0; ind < active_pointer; ind++) {
		//printf ("<%i %i %i> ", ind, omp_get_thread_num (), omp_get_num_threads ());
		actives [ind] -> move ();
	}
}

void orbiter_core :: propagate_signals (void) {
	#pragma omp parallel for
	for (int ind = 0; ind < active_pointer; ind++) {
		//printf ("<%i %i %i> ", ind, omp_get_thread_num (), omp_get_num_threads ());
		actives [ind] -> propagate_signals ();
	}
}

double orbiter_core :: TimeDelta (double index) {
	int ind = (int) index;
	if (ind > 16383) return * (time_delta + 16383);
	if (ind < -16384) return * time_deltas;
	return * (time_delta + ind);
}

double orbiter_core :: FilterFreq (double index) {
	int ind = (int) index;
	if (ind > 8191) return * (filter_freq + 8191);
	if (ind < -8192) return * filter_freq;
	return * (filter_freq + ind);
}

double orbiter_core :: SamplerTimeDelta (double index) {
	int ind = (int) index;
	if (ind > 16383) return * (sampler_time_delta + 16383);
	if (ind < -16384) return * sampler_time_deltas;
	return * (sampler_time_delta + ind);
}

double orbiter_core :: ControlTimeDelta (double index) {
	int ind = (int) index;
	if (ind > 8191) return * (control_time_delta + 8191);
	if (ind < -8192) return * control_time_deltas;
	return * (control_time_delta + ind);
}

double orbiter_core :: Amplitude (double index) {
	int ind = (int) index;
	if (ind > 0) return 1.0;
	if (ind < -16383) return 0.0;
	return * (amplitude + ind);
}

double orbiter_core :: Volume (double index) {
	int ind = (int) index;
	if (ind < 0) return * volumes;
	if (ind > 16383) return volumes [16383];
	return volumes [ind];
}

double orbiter_core :: Sine (double index) {return sine_wave [0x3fff & ((int) (index * 16384.0))];}

double orbiter_core :: SineApproximated (double index) {
	index *= 16384.0;
	int ind = (int) index;
	double sub = index - (double) ind;
	ind &= 0x3fff;
	return sine_wave [ind] * (1.0 - sub) + sine_wave [ind + 1] * sub;
}

double orbiter_core :: WaitingTime (double time) {
	int ind = (int) time;
	if (ind < 0) return * waiting_times;
	if (ind > 16383) return * (waiting_times + 16383);
	return waiting_times [ind];
}

double orbiter_core :: WaitingTime16384 (double time) {
	int ind = (int) time;
	if (ind < 0) return * waiting_times_16384;
	if (ind > 16384) return * (waiting_times_16384 + 16383);
	return waiting_times_16384 [ind];
}

double orbiter_core :: WaitingPower (double time) {
	int ind = (int) time;
	if (ind < 0) return * waiting_powers;
	if (ind > 16384) return * (waiting_powers + 16383);
	return waiting_powers [ind];
}

double orbiter_core :: MinBlep (int index) {
	if (index < 0) return 0.0;
	if (index >= 16384) return min_blep [16383];
	return min_blep [index];
}

void orbiter_core :: activate (orbiter * module) {if (active_pointer >= active_limit) return; actives [active_pointer++] = module;}
void orbiter_core :: deactivate (orbiter * module) {
	for (int ind = 0; ind < active_pointer; ind++) {
		if (actives [ind] == module) {actives [ind] = actives [--active_pointer]; return;}
	}
}

int orbiter :: numberOfInputs (void) {return 0;}
int orbiter :: numberOfOutputs (void) {return 1;}
char * orbiter :: inputName (int ind) {return "VOID";}
char * orbiter :: outputName (int ind) {return "SIGNAL";}
#ifdef WIN32
#define strcasecmp _strcmpi
#endif
int orbiter :: inputIndex (char * name) {for (int ind = 0; ind < numberOfInputs (); ind++) {if (strcasecmp (name, inputName (ind)) == 0) return ind;} return -1;}
int orbiter :: outputIndex (char * name) {for (int ind = 0; ind < numberOfOutputs (); ind++) {if (strcasecmp (name, outputName (ind)) == 0) return ind;} return -1;}
double orbiter :: output (int ind) {return 0.0;}
double orbiter :: output (char * name) {int ind = outputIndex (name); if (ind < 0 || ind >= numberOfOutputs ()) return 0.0; return output (ind);}
void orbiter :: input (double signal, int ind) {}
void orbiter :: input (double signal, char * name) {int ind = inputIndex (name); if (ind < 0 || ind >= numberOfInputs ()) return; input (signal, ind);}
double * orbiter :: inputAddress (int ind) {return 0;}
double * orbiter :: outputAddress (int ind) {return ind == 0 ? & signal : 0;}
void orbiter :: move (void) {}
void orbiter :: hold (void) {
	pthread_mutex_lock (& core -> main_mutex);
	references++;
	pthread_mutex_unlock (& core -> main_mutex);
}
bool orbiter :: release (void) {
	pthread_mutex_t * mt = & core -> main_mutex;
	pthread_mutex_lock (mt);
	if (-- references > 0) {pthread_mutex_unlock (mt); return false;}
	dock_pointer * connectors_to_delete = connectors;
	int number_to_delete = number_of_connections;
	delete this;
	pthread_mutex_unlock (mt);
	if (connectors_to_delete == 0) return true;
	for (int ind = 0; ind < number_to_delete; ind++) {
		if (connectors_to_delete [ind] != 0)
			delete connectors_to_delete [ind];
	}
	delete [] connectors_to_delete;
	return true;
}
bool orbiter :: connect (int destination_port, orbiter * source, int source_port) {
	if (destination_port < 0) return false;
	if (destination_port >= number_of_connections) return false;
	if (source == 0) return false;
	if (source_port < 0) return false;
	if (source_port >= source -> numberOfOutputs ()) return false;
	double * location = source -> outputAddress (source_port);
	if (location == 0) return false;
	connectors [destination_port] = new dock (source, source_port, source -> outputAddress (source_port), connectors [destination_port]);
	return true;
}
void orbiter :: disconnect (void) {
	for (int ind = 0; ind < number_of_connections; ind++) {
		if (connectors [ind] != 0) delete connectors [ind];
		connectors [ind] = 0;
	}
}
bool orbiter :: disconnect (int destination_port) {
	if (destination_port < 0) return false;
	if (destination_port >= number_of_connections) return false;
	if (connectors [destination_port] != 0) delete connectors [destination_port];
	connectors [destination_port] = 0;
	return true;
}
bool orbiter :: disconnect (int destination_port, orbiter * source, int source_port) {
	if (destination_port < 0) return false;
	if (destination_port >= number_of_connections) return false;
	if (source == 0) return false;
	dock * dp = connectors [destination_port];
	if (dp == 0) return false;
	if (dp -> source == source && dp -> source_port == source_port) {
		connectors [destination_port] = dp -> next;
		dp -> next = 0;
		delete dp;
		return true;
	}
	while (dp -> next != 0) {
		if (dp -> next -> source == source && dp -> next -> source_port == source_port) {
			dock * to_delete = dp -> next;
			dp -> next = dp -> next -> next;
			to_delete -> next = 0;
			delete to_delete;
			return true;
		}
		dp = dp -> next;
	}
	return false;
}

void orbiter :: propagate_signals (void) {
	dock_pointer dcp;
	for (int ind = 0; ind < number_of_connections; ind++) {
		if ((dcp = connectors [ind]) != 0) {
			double signal = 0.0;
			while (dcp != 0) {
				signal += * dcp -> source_address;
				dcp = dcp -> next;
			}
			* connection_addresses [ind] = signal;
		}
	}
}

void orbiter :: initialise (void) {
	if (connectors == 0) {
		number_of_connections = numberOfInputs ();
		connectors = number_of_connections > 0 ? new dock_pointer [number_of_connections] : 0;
		connection_addresses = number_of_connections > 0 ? new connection_address [number_of_connections] : 0;
	} else if (number_of_connections != numberOfInputs ()) return;
	for (int ind = 0; ind < number_of_connections; ind++) {connectors [ind] = 0; connection_addresses [ind] = inputAddress (ind);}
}

void orbiter :: activate (void) {
	pthread_mutex_lock (& core -> main_mutex);
	if (core -> root == 0) {
		core -> root = this;
		next = previous = this;
	} else {
		next = core -> root;
		previous = core -> root -> previous;
		next -> previous = this;
		previous -> next = this;
		core -> root = this;
	}
	core -> activate (this);
	pthread_mutex_unlock (& core -> main_mutex);
	//printf ("ORBITER ACTIVATED [%i]\n", orbiter_count);
}

orbiter :: orbiter (orbiter_core * core) {
	signal = 0.0;
	references = 0;
	this -> core = core;
	orbiter_count++;
	number_of_connections = 0;
	connectors = 0;
	connection_addresses = 0;
	previous = next = 0;
}

orbiter :: ~ orbiter (void) {
	core -> deactivate (this);
	if (next == this || previous == this) core -> root = 0;
	else if (next != 0 && previous != 0) {
		next -> previous = previous;
		previous -> next = next;
		if (core -> root == this) core -> root = next;
	}
	if (connection_addresses != 0) delete [] connection_addresses;
	orbiter_count--;
	//printf ("ORBITER DESTROYED [%i]\n", orbiter_count);
}

dock :: dock (orbiter * source, int source_port, double * source_address, dock * next) {
	source -> hold ();
	this -> source = source;
	this -> source_port = source_port;
	this -> source_address = source_address;
	this -> next = next;
}

dock :: ~ dock (void) {
	source -> release ();
	if (next != 0) delete next;
}

