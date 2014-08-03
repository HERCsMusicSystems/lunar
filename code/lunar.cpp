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

int orbiter_count = 0;

orbiter_core :: orbiter_core (void) {
	root = 0;
	this -> centre_frequency = 330.0;
	this -> sampling_frequency = 48000.0;
	this -> latency_block_size = 128;
	time_delta = time_deltas + 16384;
	amplitude = amplitudes + 16383;
	for (int ind = 0; ind > -16384; ind--) * (amplitude + ind) = pow (2.0, (double) ind / 1536.0); * amplitudes = 0.0;
	for (int ind = 0; ind <= 16384; ind++) sine_wave [ind] = sin ((double) ind * M_PI * 2.0 / 16384.0);
	recalculate ();
	pthread_mutex_init (& main_mutex, 0);
	pthread_mutex_init (& maintenance_mutex, 0);
}

orbiter_core :: ~ orbiter_core (void) {
	pthread_mutex_destroy (& main_mutex);
	pthread_mutex_destroy (& maintenance_mutex);
}

void orbiter_core :: move_modules (void) {
	if (root == 0) return;
	root -> move ();
	orbiter * orp = root -> next;
	while (orp != root) {orp -> move (); orp = orp -> next;}
}

void orbiter_core :: propagate_signals (void) {
	if (root == 0) return;
	root -> propagate_signals ();
	orbiter * orp = root -> next;
	while (orp != root) {orp -> propagate_signals (); orp = orp -> next;}
}

double orbiter_core :: TimeDelta (double index) {
	int ind = (int) index;
	if (ind > 16383) return * (time_delta + 16383);
	if (ind < -16384) return * (time_delta - 16384);
	return * (time_delta + ind);
}

double orbiter_core :: Amplitude (double index) {
	int ind = (int) index;
	if (ind > 0) return * amplitude;
	if (ind < -16383) return * (amplitude - 16383);
	return * (amplitude + ind);
}

double orbiter_core :: Sine (double index) {
	int ind = (int) (index * 16384.0);
	while (ind < 0) ind += 16384;
	while (ind > 16383) ind -= 16384;
	return sine_wave [ind];
}

double orbiter_core :: SineApproximated (double index) {
	int ind = (int) (index * 16384.0);
	double sub = index - (double) ind;
	while (ind < 0) ind += 16384;
	while (ind > 16384) ind -= 16384;
	return sine_wave [ind] * (1.0 - sub) + sine_wave [ind + 1] * sub;
}

void orbiter_core :: recalculate (void) {
	gate_gap = sampling_frequency / 2048.0;
	gate_delay = 48000.0 / sampling_frequency;
	double delay = sampling_frequency > 0.0 ? centre_frequency  / sampling_frequency : centre_frequency;
	for (int ind = 0; ind < 32768; ind++) time_deltas [ind] = delay * pow (2.0, ((double) (ind - 16384) / 1536.0));
}

void orbiter_core :: activate (orbiter * module) {}
void orbiter_core :: deactivate (orbiter * module) {}

int orbiter :: numberOfInputs (void) {return 0;}
int orbiter :: numberOfOutputs (void) {return 1;}
char * orbiter :: inputName (int ind) {return "VOID";}
char * orbiter :: outputName (int ind) {return "SIGNAL";}
int orbiter :: inputIndex (char * name) {for (int ind = 0; ind < numberOfInputs (); ind++) {if (strcmp (name, inputName (ind)) == 0) return ind;} return -1;}
int orbiter :: outputIndex (char * name) {for (int ind = 0; ind < numberOfOutputs (); ind++) {if (strcmp (name, outputName (ind)) == 0) return ind;} return -1;}
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
	number_of_connections = numberOfInputs ();
	connectors = number_of_connections > 0 ? new dock_pointer [number_of_connections] : 0;
	connection_addresses = number_of_connections > 0 ? new connection_address [number_of_connections] : 0; 
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
	printf ("ORBITER ACTIVATED [%i]\n", orbiter_count);
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
	printf ("ORBITER DESTROYED [%i]\n", orbiter_count);
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

