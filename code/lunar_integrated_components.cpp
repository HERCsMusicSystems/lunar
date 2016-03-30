///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
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

/////////////////////////////////////////////////////////////////////////
// This file was created on Monday, 14th December 2015 at 11:37:02 AM. //
/////////////////////////////////////////////////////////////////////////

#include "lunar_integrated_components.h"
#include <math.h>

#define DIV_16384 0.00006103515625
#define DIV_8192  0.0001220703125
#define DIV_4096  0.000244140625

void integrated_vco :: move_sine (void) {
	if (trigger >= 16384.0) time = 0.0;
	this -> signal = core -> Amplitude (amp) * core -> Sine (time + shift);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}

void integrated_vco :: move_sawtooth (void) {
	if (trigger >= 16384.0) time = 0.0;
	double delta = core -> TimeDelta (freq) * ratio;
	signal = core -> MinBlep (blep_index) - 1.0 - 2.0 * time;
	signal *= core -> Amplitude (amp);
	time += delta;
	blep_index += 512;
	while (time >= 1.0) {time -= 1.0; blep_index = (int) (time * 512.0 / (delta > 0.0 ? delta : 1.0));}
}

void integrated_vco :: move_square (void) {
	if (trigger >= 16384.0) time = 0.0;
	double delta = core -> TimeDelta (freq) * ratio;
	if (stage) signal = core -> MinBlep (blep_index) - 1.0;
	else signal = 1.0 - core -> MinBlep (blep_index);
	time += delta;
	blep_index += 512;
	if (stage && time > 0.5) {stage = false; blep_index = (int) ((time - 0.5) * 512.0 / (delta > 0.0 ? delta : 1.0));}
	while (time >= 1.0) {stage = true; time -= 1.0; blep_index = (int) (time * 512.0 / (delta > 0.0 ? delta : 1.0));}
	signal *= core -> Amplitude (amp);
}

void integrated_vco :: move_noise (void) {signal = core -> Amplitude (amp) * (0.00000011920928955078125 * (double) core -> noise24b - 1.0);}

void integrated_vco :: move (void) {
	switch ((int) wave) {
	case 1: move_sawtooth (); return; break;
	case 2: move_square (); return; break;
	case 3: move_noise (); return; break;
	default: move_sine (); return; break;
	}
}

integrated_vco :: integrated_vco (orbiter_core * core) {
	this -> core = core;
	signal = 0.0;
	freq = amp = trigger = time = 0.0;
	shift = 0.0;
	ratio = 1.0;
	wave = 0.0;
	blep_index = 0;
	stage = true;
}

void integrated_noise :: move (void) {signal = gain * core -> Amplitude (amp) * (0.00000011920928955078125 * (double) core -> noise24b - 1.0);}
integrated_noise :: integrated_noise (orbiter_core * core) {this -> core = core; gain = 1.0; amp = 0.0;}

double integrated_sensitivity (double breakpoint, double left, double right, double enter, double gain) {
	if (enter >= breakpoint) return (enter - breakpoint) * right * gain;
	return (breakpoint - enter) * left * gain;
}

double integrated_morph (double x, double * vector) {return vector [1] * x + * vector - * vector * x;}
double integrated_morph (double x, double y, double * vector) {
	double mx = 1.0 - x;
	double ab = vector [1] * x + * vector * mx;
	double cd = vector [3] * x + vector [2] * mx;
	return cd * y + ab - ab * y;
}
double integrated_morph (double x, double y, double z, double * vector) {
	double mx = 1.0 - x;
	double my = 1.0 - y;
	double ab = vector [1] * x + * vector * mx;
	double cd = vector [3] * x + vector [2] * mx;
	double ef = vector [5] * x + vector [4] * mx;
	double gh = vector [7] * x + vector [6] * mx;
	double abcd = cd * y + ab * my;
	double efgh = gh * y + ef * my;
	return efgh * z + abcd - abcd * z;
}

#define FRAC 0.00006103515625
double algo1 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> feedback3 * block -> signal3 * FRAC);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> feedback1 * block -> signal1 * FRAC);
	return block -> signal1 + block -> signal2 + block -> signal3 + block -> signal4;
}
double algo2 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> feedback1 * block -> signal1 * FRAC);
	return block -> signal1 + block -> signal2 + block -> signal3;
}
double algo3 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> signal4);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal4);
	return block -> signal1 + block -> signal2 + block -> signal3;
}
double algo4 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2);
	return block -> signal1 + block -> signal3;
}
double algo5 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> feedback3 * block -> signal3 * FRAC);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) *
														block -> core -> Sine (block -> time1 + block -> signal2 + block -> signal3 + block -> signal4);
}
double algo6 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2 + block -> signal3);
}
double algo7 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> feedback3 * block -> signal3 * FRAC);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> signal3 + block -> signal4);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2);
}
double algo8 (integrated_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> signal3);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2);
}
void integrated_fm4_block :: move (void) {
	if (algo != previous_algo) {
		switch ((int) algo) {
		case 0: current_algo = algo1; break;
		case 1: current_algo = algo2; break;
		case 2: current_algo = algo3; break;
		case 3: current_algo = algo4; break;
		case 4: current_algo = algo5; break;
		case 5: current_algo = algo6; break;
		case 6: current_algo = algo7; break;
		case 7: current_algo = algo8; break;
		default: current_algo = algo1; break;
		}
		previous_algo = algo;
	}
	if (trigger >= 16384.0) time1 = time2 = time3 = time4 = 0.0;
	signal = current_algo (this);
	time1 += core -> TimeDelta (freq1) * ratio1;
	time2 += core -> TimeDelta (freq2) * ratio2;
	time3 += core -> TimeDelta (freq3) * ratio3;
	time4 += core -> TimeDelta (freq4) * ratio4;
	while (time1 >= 1.0) time1 -= 1.0;
	while (time2 >= 1.0) time2 -= 1.0;
	while (time3 >= 1.0) time3 -= 1.0;
	while (time4 >= 1.0) time4 -= 1.0;
}
integrated_fm4_block :: integrated_fm4_block (orbiter_core * core) {
	this -> core = core;
	current_algo = algo1;
	algo = previous_algo = 0.0;
	trigger = 0.0;
	signal1 = signal2 = signal3 = signal4 = 0.0;
	time1 = time2 = time3 = time4 = 0.0;
	freq1 = freq2 = freq3 = freq4 = 0.0;
	amp1 = 0.0; amp2 = amp3 = amp4 = -16384.0;
	gain1 = gain2 = gain3 = gain4 = 1.0;
	ratio1 = ratio2 = ratio3 = ratio4 = 1.0;
	feedback1 = feedback2 = feedback3 = feedback4 = 0.0;
	signal = 0.0;
}

void integrated_filter :: move (void) {
	double F = core -> FilterFreq (freq);
	double Q = 2.0 - resonance * 0.0001220703125;
	running_signal += running_band_pass_signal * F;
	running_high_pass_signal = enter - running_signal - running_band_pass_signal * Q;
	running_band_pass_signal += running_high_pass_signal * F;
	running_signal += running_band_pass_signal * F;
	running_high_pass_signal = enter - running_signal - running_band_pass_signal * Q;
	running_band_pass_signal += running_high_pass_signal * F;
	if (running_band_pass_signal > 10.0) running_band_pass_signal = 10.0; if (running_band_pass_signal < -10.0) running_band_pass_signal = -10.0;
	double amplitude = core -> Amplitude (amp);
	signal = running_signal * amplitude;
	high_pass_signal = running_high_pass_signal * amplitude;
	band_pass_signal = running_band_pass_signal * amplitude;
	band_reject_signal = high_pass_signal + signal;
}

integrated_filter :: integrated_filter (orbiter_core * core) {
	this -> core = core;
	high_pass_signal = band_pass_signal = band_reject_signal = running_signal = 0.0;
	running_high_pass_signal = running_band_pass_signal = 0.0;
	freq = amp = 0.0; resonance = 0.0;
	enter = signal = 0.0;
}

void integrated_lfo :: move (void) {
	if (sync > 0.0 && trigger > 0.0 && previous_trigger == 0.0) time = phase * 0.00006103515625; previous_trigger = trigger;
	double t = 0.5 + pulse * 0.00006103515625;
	switch ((int) wave) {
	case 0: // sine
		if (t <= 0) signal = core -> SineApproximated (0.5 * time + 0.25);
		else if (t >= 1.0) signal = core -> SineApproximated (0.5 * time - 0.25);
		else if (time < t) signal = core -> SineApproximated (time * 0.5 / t - 0.25);
		else signal = core -> SineApproximated ((time - t) * 0.5 / (1.0 - t) + 0.25);
		positive = 0.5 + signal * 0.5;
		negative = positive - 1.0;
		break;
	case 1: // triangle
		if (t <= 0) positive = 1.0 - time;
		else if (t >= 1.0) positive = time;
		else if (time < t) positive = time / t;
		else positive = (1.0 - time) / (1.0 - t);
		signal = positive * 2.0 - 1.0;
		negative = positive - 1.0;
		break;
	case 2: // square
		if (time < t) {signal = positive = 1.0; negative = 0.0;}
		else {signal = -1.0; positive = 0.0; negative = -1.0;}
		break;
	case 3: // random
		if (time < t) {if (! stage_one) {
			origin = target;
			target = 0.000000059604644775390625 * (double) core -> noise24b;
			delta = target - origin;
			stage_one = true;
		}} else {if (stage_one) {
			origin = target;
			target = 0.000000059604644775390625 * (double) core -> noise24b;
			delta = target - origin;
			stage_one = false;
		}}
		if (t <= 0) positive = origin + delta * time;
		else if (t >= 1.0) positive = origin + delta * time;
		else if (time < t) positive = origin + delta * time / t;
		else positive = origin + delta * (time - t) / (1.0 - t);
		signal = positive * 2.0 - 1.0;
		negative = positive - 1.0;
		break;
	case 4: // S/H
		if (time < t) {if (! stage_one) {
			signal = 0.00000011920928955078125 * (double) core -> noise24b - 1.0;
			positive = 0.000000059604644775390625 * (double) core -> noise24b;
			stage_one = true;
		}} else {if (stage_one) {
			signal = 0.00000011920928955078125 * (double) core -> noise24b - 1.0;
			positive = 0.000000059604644775390625 * (double) core -> noise24b;
			stage_one = false;
		}}
		negative = positive - 1.0;
		break;
	default: signal = positive = negative = 0.0; break;
	}
	time += core -> ControlTimeDelta (speed);
	while (time >= 1.0) time -= 1.0;
	vibrato_signal = signal * vibrato;
	tremolo_signal = negative * tremolo;
	wahwah_signal = negative * wahwah;
	pan_signal = signal * pan;
}

integrated_lfo :: integrated_lfo (orbiter_core * core) {
	this -> core = core;
	stage_one = true;
	origin = target = delta = 0.0;
	time = speed = wave = pulse = phase = sync = positive = negative = 0.0;
	vibrato = tremolo = wahwah = pan = 0.0;
	vibrato_signal = tremolo_signal = wahwah_signal = pan_signal = 0.0;
	signal = 0.0;
	trigger = previous_trigger = 0.0;
}

void integrated_trigger :: move (void) {
	pthread_mutex_lock (& critical);
	if (request != 0) {
		switch (request) {
		case 5: ground_request (); break;
		case 4: keyon_velocity_request (); break;
		case 1: keyoff_request (); break;
		case 2: keyoff_all_request (); break;
		case 3: sub_keyon (request_key); break;
		default: break;
		}
		request = 0;
	}
	if (trigger != 0.0 && keystack_pointer == 0 && hold == 0.0) trigger = 0.0;
	if (signal == target) {
		if (trigger >= 16384.0) {if (time > 0.0) trigger = 1.0; else time += 1.0;}
		pthread_mutex_unlock (& critical); return;
	}
	if (time > 0.0) {
		if (trigger >= 16384.0) trigger = 1.0;
		if (time >= 1.0) {signal = target; pthread_mutex_unlock (& critical); return;}
	}
	signal = origin + delta * time;
	time += core -> WaitingTime (porta_time);
	pthread_mutex_unlock (& critical);
}

void integrated_trigger :: add_stack (int key) {
	if (keystack_pointer < 16) {keystack [keystack_pointer++] = key; return;}
	for (int ind = 0; ind < 14; ind++) keystack [ind] = keystack [ind + 1];
	keystack [15] = key;
	keystack_pointer = 16;
}
void integrated_trigger :: drop_stack (int key) {
	bool continue_playing = keystack_pointer > 0 && keystack [keystack_pointer - 1] != key;
	for (int ind = 0; ind < keystack_pointer; ind++) {
		if (keystack [ind] == key) {
			for (int sub = ind; sub < keystack_pointer - 1; sub++) keystack [sub] = keystack [sub + 1];
			keystack_pointer--;
		}
	}
	if (continue_playing) return;
	if (keystack_pointer > 0) sub_keyon (keystack [--keystack_pointer]);
}

void integrated_trigger :: sub_keyon (int key) {
	if (key < 0) key = 0;
	if (key > 127) key = 127;
	index = (double) (key - 64);
	target = key_map == 0 ? (double) (key - 64) * 128.0 : key_map -> map [key];
	this -> key = key;
	if (! active || porta == 0.0 || porta_time == 0.0 || (legato != 0.0 && trigger == 0)) this -> signal = target;
	else {origin = this -> signal; delta = target - origin; time = 0.0;}
	trigger = 1.0;
	add_stack (key);
}

void integrated_trigger :: sub_velocity (int velocity) {
	if (velocity < 0) velocity = 0; if (velocity > 127) velocity = 127;
	this -> velocity = velocity_map == 0 ? (double) velocity * 128.0 : velocity_map -> map [velocity];
}

void integrated_trigger :: keyon_velocity_request (void) {
	if (keystack_pointer < 1) sub_velocity (request_velocity); sub_keyon (request_key);
}

void integrated_trigger :: ground_request (void) {
	sub_velocity (request_velocity);
	if (request_key < 0) request_key = 0; if (request_key > 127) request_key = 127;
	index = (double) (request_key - 64);
	target = key_map == 0 ? (double) (request_key - 64) * 128.0 : key_map -> map [request_key];
	this -> signal = request_key; trigger = 16384.0; time = 0.0;
	if (! active || porta == 0.0 || porta_time == 0.0) this -> signal = target;
	else {
		if (legato == 0.0) request_base = request_previous;
		this -> signal = origin = key_map == 0 ? (double) (request_base - 64) * 128.0 : key_map -> map [request_base];
		delta = target - origin;
	}
	keystack_pointer = 0; add_stack (request_key);
}

void integrated_trigger :: keyon (int key) {
	pthread_mutex_lock (& critical);
	request_key = key; request = 3;
	if (! active) sub_keyon (key);
	pthread_mutex_unlock (& critical);
}

void integrated_trigger :: keyon (int key, int velocity) {
	if (velocity < 1) {keyoff (key); return;}
	pthread_mutex_lock (& critical);
	request_key = key; request_velocity = velocity; request = 4;
	if (! active) keyon_velocity_request ();
	pthread_mutex_unlock (& critical);
}

void integrated_trigger :: ground (int key, int velocity, int base, int previous) {
	if (velocity < 1) {keyoff (key); return;}
	pthread_mutex_lock (& critical);
	request_key = key; request_velocity = velocity; request_base = base; request_previous = previous; request = 5;
	if (! active) ground_request ();
	pthread_mutex_unlock (& critical);
}

void integrated_trigger :: keyoff (int key) {
	pthread_mutex_lock (& critical);
	request_key = key; request = 1;
	drop_stack (request_key);
	this -> key = -1;
	if (! active) keyoff_request ();
	pthread_mutex_unlock (& critical);
}

void integrated_trigger :: keyoff (void) {
	pthread_mutex_lock (& critical);
	request = 2;
	this -> key = -1;
	if (! active) keyoff_all_request ();
	pthread_mutex_unlock (& critical);
}

void integrated_trigger :: keyoff_request (void) {if (keystack_pointer == 0 && hold == 0.0) trigger = 0.0;}
void integrated_trigger :: keyoff_all_request (void) {keystack_pointer = 0; trigger = 0.0;}
integrated_trigger :: integrated_trigger (orbiter_core * core, bool active, integrated_trigger * next) {
	this -> core = core;
	this -> next = next;
	key = -1;
	signal = index = trigger = busy = 0.0;
	origin = delta = target = 0.0;
	porta = porta_time = legato = 0.0;
	hold = 0.0;
	time = 2.0;
	velocity = 12800.0;
	key_map = 0;
	velocity_map = 0;
	for (int ind = 0; ind < 16; ind++) keystack [ind] = 0; keystack_pointer = 0;
	this -> active = active;
	request = 0;
	pthread_mutex_init (& critical, 0);
}
integrated_trigger :: ~ integrated_trigger (void) {pthread_mutex_destroy (& critical);}

bool integrated_moonbase :: insert_trigger (integrated_trigger * trigger) {
	if (trigger == 0) return false;
	trigger -> next = triggers;
	choice = triggers = trigger;
	return true;
}
bool integrated_moonbase :: insert_controller (double * controller, int location, double shift) {
	if (location < 0 || location > 128) return false;
	controllers [location] = controller;
	shifts [location] = shift;
	return true;
}
void integrated_moonbase :: keyon (int key) {
	pthread_mutex_lock (& critical);
	integrated_trigger * trigger = select ();
	if (trigger != 0) trigger -> keyon (key);
	previous_key = key;
	if (key_counter++ == 0) base_key = key;
	pthread_mutex_unlock (& critical);
}
void integrated_moonbase :: keyon (int key, int velocity) {
	if (velocity == 0) {keyoff (key, 0); return;}
	pthread_mutex_lock (& critical);
	integrated_trigger * trigger = select ();
	if (key_counter++ == 0) base_key = key;
	if (trigger != 0) {
		if (mono_mode) trigger -> keyon (key, velocity);
		else trigger -> ground (key, velocity, base_key, previous_key >= 0 ? previous_key : key);
	}
	previous_key = key;
	pthread_mutex_unlock (& critical);
}
integrated_trigger * integrated_moonbase :: select (void) {
	if (mono_mode) return triggers;
	if (choice == 0) return 0;
	integrated_trigger * ret = choice;
	do {
		if (ret -> busy <= 0.0 && ret -> key < 0 && ret -> request < 3) {choice = ret -> next; if (choice == 0) choice = triggers; return ret;}
		ret = ret -> next; if (ret == 0) ret = triggers;
	} while (ret != choice);
	do {
		if (ret -> key < 0 && ret -> request < 3) {choice = ret -> next; if (choice == 0) choice = triggers; return ret;}
		ret = ret -> next; if (ret == 0) ret = triggers;
	} while (ret != choice);
	choice = choice -> next; if (choice == 0) choice = triggers;
	return ret;
}
integrated_trigger * integrated_moonbase :: select (int key) {
	if (mono_mode) return triggers;
	if (choice == 0) return 0;
	integrated_trigger * ret = choice;
	do {
		if (ret -> key == key) return ret;
		ret = ret -> next; if (ret == 0) ret = triggers;
	} while (ret != choice);
	return 0;
}
void integrated_moonbase :: keyoff (void) {
	pthread_mutex_lock (& critical);
	integrated_trigger * trigger = triggers;
	while (trigger != 0) {trigger -> keyoff (); trigger = trigger -> next;}
	choice = triggers;
	key_counter = 0;
	pthread_mutex_unlock (& critical);
}
void integrated_moonbase :: keyoff (int key, int velocity) {
	pthread_mutex_lock (& critical);
	integrated_trigger * trigger = select (key);
	if (trigger != 0) {
		if (mono_mode) trigger -> keyoff (key);
		else trigger -> keyoff ();
	}
	key_counter--; if (key_counter < 0) key_counter = 0;
	pthread_mutex_unlock (& critical);
}
void integrated_moonbase :: mono (void) {mono_mode = true; keyoff (); base_key = 64; previous_key = -1;}
void integrated_moonbase :: poly (void) {mono_mode = false; keyoff (); base_key = 64; previous_key = -1;}
bool integrated_moonbase :: isMonoMode (void) {return mono_mode;}
void integrated_moonbase :: control (int ctrl, double value) {
	if (ctrl < 0 || ctrl > 128) return;
	if (controllers [ctrl] != 0) {
		* controllers [ctrl] = (value + shifts [ctrl]) * 128.0 + ctrl_lsbs [ctrl];
		ctrl_lsbs [ctrl] = 0;
	} else if (ctrl > 31) ctrl_lsbs [ctrl - 32] = value;
	if (ctrl == 126) mono ();
	if (ctrl == 127) poly ();
}
double integrated_moonbase :: getControl (int ctrl) {
	if (ctrl < 0 || ctrl > 128) return 0.0;
	if (controllers [ctrl] != 0) return * controllers [ctrl] * 0.0078125 - (double) shifts [ctrl];
	switch (ctrl) {
	case 0: return 0.0; break;
	case 1: return 0.0; break;
	case 7: return 100.0; break;
	case 65: return 0.0; break;
	case 126: return mono_mode ? 1.0 : 0.0; break;
	case 127: return mono_mode ? 0.0 : 1.0; break;
	case 128: return 64.0; break;
	default: break;
	}
	return 64.0;
}
void integrated_moonbase :: timing_clock (void) {}
integrated_moonbase :: integrated_moonbase (orbiter_core * core) {
	pthread_mutex_init (& critical, 0);
	choice = triggers = 0; mono_mode = false; base_key = 64; previous_key = -1; key_counter = 0;
	for (int ind = 0; ind < 129; ind++) {controllers [ind] = 0; ctrl_lsbs [ind] = 0; shifts [ind] = 0.0;}
}

void up1 (integrated_arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
	arp -> index++;
}
void up2 (integrated_arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 1) arp -> octave = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	arp -> index++;
}
void up3 (integrated_arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 2) arp -> octave = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	arp -> index++;
}
void up4 (integrated_arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 3) arp -> octave = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	arp -> index++;
}
void down1 (integrated_arpeggiator * arp) {
	if (--arp -> index < 0) arp -> index = arp -> active_key_pointer - 1;
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
}
void down2 (integrated_arpeggiator * arp) {
	if (--arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; if (--arp -> octave < 0) arp -> octave = 1;}
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
}
void down3 (integrated_arpeggiator * arp) {
	if (--arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; if (--arp -> octave < 0) arp -> octave = 2;}
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
}
void down4 (integrated_arpeggiator * arp) {
	if (--arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; if (--arp -> octave < 0) arp -> octave = 3;}
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
}
void updown1 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {arp -> index = 1; arp -> up_direction = true;}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> up_direction = false;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void updown2 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 1) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 0;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 1; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void updown3 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 2) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 1;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 1; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void updown4 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 3) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 2;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 1; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void updowndup1 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void updowndup2 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 1) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 0;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 0; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void updowndup3 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 2) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 1;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 0; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void updowndup4 (integrated_arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 3) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 2;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 0; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}
void randomdup1 (integrated_arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> base -> keyon (arp -> active_keys [key], arp -> active_velocities [key]);
}
void randomdup2 (integrated_arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int octave = (arp -> index * 2) >> 24;
	arp -> base -> keyon (arp -> active_keys [key] + 12 * octave, arp -> active_velocities [key]);
}
void randomdup3 (integrated_arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int octave = (arp -> index * 3) >> 24;
	arp -> base -> keyon (arp -> active_keys [key] + 12 * octave, arp -> active_velocities [key]);
}
void randomdup4 (integrated_arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int octave = (arp -> index * 4) >> 24;
	arp -> base -> keyon (arp -> active_keys [key] + 12 * octave, arp -> active_velocities [key]);
}
void random1 (integrated_arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index];
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}
void random2 (integrated_arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		int octave = (arp -> index * 2) >> 24;
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index] + 12 * octave;
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}
void random3 (integrated_arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		int octave = (arp -> index * 3) >> 24;
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index] + 12 * octave;
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}
void random4 (integrated_arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		int octave = (arp -> index * 4) >> 24;
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index] + 12 * octave;
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}
void integrated_arpeggiator :: move (void) {
	if (active == 0.0) {if (previous_activity != 0.0) {if (base != 0) base -> keyoff (); previous_activity = 0.0;} return;}
	if (active != previous_activity) previous_activity = active;
	if (current_algo != previous_algo) {
		switch ((int) current_algo) {
		case 0: algo = up1; break;
		case 1: algo = up2; break;
		case 2: algo = up3; break;
		case 3: algo = up4; break;
		case 4: algo = down1; break;
		case 5: algo = down2; break;
		case 6: algo = down3; break;
		case 7: algo = down4; break;
		case 8: algo = updown1; break;
		case 9: algo = updown2; break;
		case 10: algo = updown3; break;
		case 11: algo = updown4; break;
		case 12: algo = updowndup1; break;
		case 13: algo = updowndup2; break;
		case 14: algo = updowndup3; break;
		case 15: algo = updowndup4; break;
		case 16: algo = randomdup1; index = core -> noise24b; break;
		case 17: algo = randomdup2; index = core -> noise24b; break;
		case 18: algo = randomdup3; index = core -> noise24b; break;
		case 19: algo = randomdup4; index = core -> noise24b; break;
		case 20: algo = random1; index = core -> noise24b; break;
		case 21: algo = random2; index = core -> noise24b; break;
		case 22: algo = random3; index = core -> noise24b; break;
		case 23: algo = random4; index = core -> noise24b; break;
		default: algo = up1; break;
		}
		previous_algo = current_algo;
	}
	while (time >= 1.0) {time -= 1.0; pthread_mutex_lock (& critical); private_signal (); pthread_mutex_unlock (& critical);}
	time += core -> sample_duration * tempo * 0.4;
}
void integrated_arpeggiator :: insert_key (int key, int velocity) {
	if (number_of_keys < 1 && active_key_pointer > 0) number_of_keys = active_key_pointer = 0;
	int location = 0;
	if (active_key_pointer >= 128) return;
	while (location < active_key_pointer && active_keys [location] < key) location++;
	if (location >= 128) return;
	if (location >= active_key_pointer) {
		active_keys [location] = key; active_velocities [location] = velocity; active_key_pointer = location + 1;
		if (location == 0) {number_of_keys = 1; if (hold == 0.0) ground ();}
		else number_of_keys++;
		return;
	}
	if (active_keys [location] == key) {
		active_key_pointer--;
		for (int ind = location; ind < active_key_pointer; ind++) {
			active_keys [ind] = active_keys [ind + 1];
			active_velocities [ind] = active_velocities [ind + 1];
		}
		if (location <= index) index--;
		number_of_keys--;
		return;
	}
	for (int ind = active_key_pointer; ind > location; ind--) {
		active_keys [ind] = active_keys [ind - 1];
		active_velocities [ind] = active_velocities [ind - 1];
	}
	active_keys [location] = key; active_velocities [location] = velocity; active_key_pointer++; number_of_keys++;
	if (location < index) index++;
}
void integrated_arpeggiator :: remove_key (int key) {
	if (hold != 0.0) {if (number_of_keys > 0) number_of_keys--; return;}
	int location = 0;
	while (location < active_key_pointer && active_keys [location] != key) location++;
	if (location >= active_key_pointer) return;
	active_key_pointer--; number_of_keys--;
	for (int ind = location; ind < active_key_pointer; ind++) {
		active_keys [ind] = active_keys [ind + 1];
		active_velocities [ind] = active_velocities [ind + 1];
	}
	if (location <= index) index--;
}
void integrated_arpeggiator :: private_signal (void) {
	if (base == 0) return;
	if (number_of_keys < 1 && active_key_pointer > 0 && hold == 0.0) active_key_pointer = 0;
	if (active_key_pointer < 1 && ! should_keyoff) return;
	double div = division < 1.0 ? 1.0 : division;
	if (should_keyoff && tick >= div * 0.5) {base -> keyoff (); should_keyoff = false;}
	while (tick >= div) {
		tick -= div;
		if (algo != 0) algo (this);
		should_keyoff = true;
	}
	tick += 1.0;
}
bool integrated_arpeggiator :: insert_trigger (integrated_trigger * trigger) {if (base != 0) return base -> insert_trigger (trigger); return false;}
bool integrated_arpeggiator :: insert_controller (double * controller, int location, double shift) {if (base != 0) return base -> insert_controller (controller, location, shift); return false;}
void integrated_arpeggiator :: keyon (int key) {
	if (active == 0.0 && base != 0) base -> keyon (key);
	pthread_mutex_lock (& critical);
	insert_key (key, 100);
	pthread_mutex_unlock (& critical);
}
void integrated_arpeggiator :: keyon (int key, int velocity) {
	if (velocity < 1) {keyoff (key, 0); return;}
	if (active == 0.0 && base != 0) base -> keyon (key, velocity);
	pthread_mutex_lock (& critical);
	insert_key (key, velocity);
	pthread_mutex_unlock (& critical);
}
void integrated_arpeggiator :: keyoff (void) {
	if (active == 0.0 && base != 0) base -> keyoff ();
	pthread_mutex_lock (& critical);
	number_of_keys = 0;
	if (hold == 0.0) active_key_pointer = 0;
	pthread_mutex_unlock (& critical);
}
void integrated_arpeggiator :: keyoff (int key, int velocity) {
	if (active == 0.0 && base != 0) base -> keyoff (key, velocity);
	pthread_mutex_lock (& critical);
	remove_key (key);
	pthread_mutex_unlock (& critical);
}
void integrated_arpeggiator :: mono (void) {if (base != 0) base -> mono ();}
void integrated_arpeggiator :: poly (void) {if (base != 0) base -> poly ();}
bool integrated_arpeggiator :: isMonoMode (void) {if (base != 0) return base -> isMonoMode (); return false;}
void integrated_arpeggiator :: control (int ctrl, double value) {if (base != 0) base -> control (ctrl, value);}
double integrated_arpeggiator :: getControl (int ctrl) {if (base != 0) return base -> getControl (ctrl); return 0.0;}
void integrated_arpeggiator :: timing_clock (void) {
	pthread_mutex_lock (& critical);
	private_signal ();
	pthread_mutex_unlock (& critical);
}
void integrated_arpeggiator :: ground (void) {
	time = 1.0;
	tick = division;
	should_keyoff = false;
	index = octave = 0;
	up_direction = true;
}
integrated_arpeggiator :: integrated_arpeggiator (orbiter_core * core, IntegratedCommandModule * base) {
	pthread_mutex_init (& critical, 0);
	active_key_pointer = number_of_keys = 0;
	tempo = 140.0; division = 24.0;
	algo = up1; current_algo = previous_algo = 0.0;
	ground ();
	active = previous_activity = 0.0;
	hold = 0.0;
	this -> core = core;
	this -> base = base;
}

void integrated_adsr :: move (void) {
	if (trigger >= 16384.0) {
		busy = 1.0;
		if (attack == 0.0) {
			if (decay == 0.0) {stage = 3; signal = (16384.0 + sustain) * DIV_16384; return;}
			stage = 2; signal = 1.0;
			threshold = (16384.0 + sustain) * DIV_16384;
			if (threshold < core -> amplitude_zero) threshold = core -> amplitude_zero;
			return;
		}
		stage = 1; signal = core -> WaitingTime (attack); return;
	}
	if (trigger == 0.0) {
		if (stage == 0) return;
		if (stage == 4) {
			signal *= core -> WaitingPower (release);
			if (signal <= core -> amplitude_zero) {stage = 0; signal = 0.0; busy = 0.0; return;}
			return;
		}
		if (release <= 0.0) {stage = 0; signal = 0.0; busy = 0.0; return;}
		stage = 4;
		return;
	} else {
		switch (stage) {
		case 0:
			busy = 1.0;
			if (attack == 0.0) {
				if (decay == 0.0) {stage = 3; signal = (16384.0 + sustain) * DIV_16384; return;}
				stage = 2; signal = 1.0;
				threshold = (16384.0 + sustain) * DIV_16384;
				if (threshold < core -> amplitude_zero) threshold = core -> amplitude_zero;
				return;
			}
			stage = 1; signal = core -> WaitingTime (attack); return;
			break;
		case 1:
			signal += core -> WaitingTime (attack);
			if (signal >= 1.0) {
				if (decay == 0.0) {stage = 3; signal = (16384.0 + sustain) * DIV_16384; return;}
				stage = 2; signal = 1.0;
				threshold = (16384.0 + sustain) * DIV_16384;
				if (threshold < core -> amplitude_zero) threshold = core -> amplitude_zero;
				return;
			}
			break;
		case 2:
			signal *= core -> WaitingPower (decay);
			if (signal <= threshold) stage = 3;
			return;
			break;
		case 3: return; break;
		case 4: stage = 1; return; break;
		default: break;
		}
	}
}

integrated_adsr :: integrated_adsr (orbiter_core * core) {
	this -> core = core;
	attack = decay = sustain = release = trigger = 0.0;
	signal = 0.0; busy = 0.0; threshold = 0.0;
	stage = 0;
}

static bool stage_between (double x, double from, double to) {
	if (x > from) return x <= to;
	if (x < from) return x >= to;
	return true;
}

void integrated_eg :: move (void) {
	if (trigger >= 16384.0) {
		busy = 1.0;
		if (time1 == 0.0) {
			if (time2 == 0.0) {
				if (time3 == 0) {signal = level3; stage = 4; return;}
				signal = level2; stage = 3; return;
			}
			signal = level1; stage = 2; return;
		}
		signal = level4; stage = 1; return;
	}
	if (trigger == 0.0) {
		if (stage < 1) return;
		if (time4 == 0.0) {signal = level4; stage = 0; return;}
		if (stage < 5) stage = 5;
		if (level4 < signal) {
			signal -= core -> WaitingTime16384 (time4);
			if (signal < level4) {signal = level4; stage = 0;}
		} else {
			signal += core -> WaitingTime16384 (time4);
			if (signal > level4) {signal = level4; stage = 0;}
		}
	} else {
		switch (stage) {
		case 0:
			if (time1 == 0.0) {
				if (time2 == 0.0) {
					if (time3 == 0.0) {signal = level3; stage = 4;}
					else {signal = level2; stage = 3;}
				} else {signal = level1; stage = 2;}
			} else {signal = level4; stage = 1;}
			return; break;
		case 1:
			if (level1 > signal) {
				signal += core -> WaitingTime16384 (time1);
				if (signal >= level1) {
					if (time2 == 0.0) {
						if (time3 == 0.0) {signal = level3; stage = 4;}
						else {signal = level2; stage = 3;}
					} else {signal = level1; stage = 2;}
				}
			} else {
				signal -= core -> WaitingTime16384 (time1);
				if (signal <= level1) {
					if (time2 == 0.0) {
						if (time3 == 0.0) {signal = level3; stage = 4;}
						else {signal = level2; stage = 3;}
					} else {signal = level1; stage = 2;}
				}
			}
			break;
		case 2:
			if (level2 > signal) {
				signal += core -> WaitingTime16384 (time2);
				if (signal >= level2) {
					if (time3 == 0.0) {signal = level3; stage = 4;}
					else {signal = level2; stage = 3;}
				}
			} else {
				signal -= core -> WaitingTime16384 (time2);
				if (signal <= level2) {
					if (time3 == 0.0) {signal = level3; stage = 4;}
					else {signal = level2; stage = 3;}
				}
			}
			break;
		case 3:
			if (level3 > signal) {
				signal += core -> WaitingTime16384 (time3);
				if (signal >= level3) {signal = level3; stage = 4;}
			} else {
				signal -= core -> WaitingTime16384 (time3);
				if (signal <= level3) {signal = level3; stage = 4;}
			}
			break;
		case 5:
			if (stage_between (signal, level4, level1)) {
				if (time1 == 0.0) {
					if (time2 == 0.0) {
						if (time3 == 0.0) {signal = level3; stage = 4;}
						else {signal = level2; stage = 3;}
					} else {signal = level1; stage = 2;}
				} else {stage = 1;}
				return;
			}
			if (stage_between (signal, level1, level2)) {
				if (time2 == 0.0) {
					if (time3 == 0.0) {signal = level3; stage = 4;}
					else {signal = level2; stage = 3;}
				} else {stage = 2;}
				return;
			}
			if (stage_between (signal, level2, level3)) {
				if (time3 == 0.0) {signal = level3; stage = 4;}
				else {stage = 3;}
				return;
			}
			break;
		default: break;
		}
	}
}

integrated_eg :: integrated_eg (orbiter_core * core) {
	this -> core = core;
	time1 = time2 = time3 = time4 = 0.0;
	level1 = level2 = level3 = level4 = 0.0;
	signal = 0.0; busy = 0.0;
	stage = 0;
}

void integrated_pan :: move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	left = * (core -> pan - ind) * enter;
	right = * (core -> pan + ind) * enter;
}

void integrated_pan :: power_move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	left = * (core -> power_pan - ind) * enter;
	right = * (core -> power_pan + ind) * enter;
}

void integrated_pan :: linear_move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	left = * (core -> linear_pan - ind) * enter;
	right = * (core -> linear_pan + ind) * enter;
}

integrated_pan :: integrated_pan (orbiter_core * core) {this -> core = core; enter = left = right = pan = 0.0;}

void integrated_stereo_pan :: move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	left = * (core -> pan - ind) * enter_left;
	right = * (core -> pan + ind) * enter_right;
}

void integrated_stereo_pan :: power_move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	left = * (core -> power_pan - ind) * enter_left;
	right = * (core -> power_pan + ind) * enter_right;
}

void integrated_stereo_pan :: linear_move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	left = * (core -> linear_pan - ind) * enter_left;
	right = * (core -> linear_pan + ind) * enter_right;
}

integrated_stereo_pan :: integrated_stereo_pan (orbiter_core * core) {this -> core = core; enter_left = enter_right = left = right = pan = 0.0;}

void integrated_stereo_amplifier :: amplify_move (void) {double dv = core -> Amplitude (gateway); left = enter_left * dv; right = enter_right * dv;}
void integrated_stereo_amplifier :: control_move (void) {left = enter_left * gateway; right = enter_right * gateway;}
void integrated_stereo_amplifier :: gateway_move (void) {double dv = gateway * DIV_16384; left = enter_left * dv; right = enter_right * dv;}
void integrated_stereo_amplifier :: volume_move (void) {double dv = core -> Volume (gateway); left = enter_left * dv; right = enter_right * dv;}
integrated_stereo_amplifier :: integrated_stereo_amplifier (orbiter_core * core, double gateway) {this -> core = core; enter_left = enter_right = left = right = 0.0; this -> gateway = gateway;}
void integrated_mono_amplifier :: amplify_move (void) {signal = enter * core -> Amplitude (gateway);}
void integrated_mono_amplifier :: control_move (void) {signal = enter * gateway;}
void integrated_mono_amplifier :: gateway_move (void) {signal = enter * gateway * DIV_16384;}
void integrated_mono_amplifier :: volume_move (void) {signal = enter * core -> Volume (gateway);}
integrated_mono_amplifier :: integrated_mono_amplifier (orbiter_core * core, double gateway) {this -> core = core; enter = signal = 0.0; this -> gateway = gateway;}

static double interpolate (double time, double * data) {
	int ind = (int) time;
	time -= (double) ind;
	int sub = ind + 1;
	if (sub > 65535) sub = 0;
	return data [ind] * (1.0 - time) + data [sub] * time;
}
void integrated_chorus :: move (void) {
	double lfo = 1.0 + amp * DIV_16384 * core -> SineApproximated (omega);
	omega += core -> ControlTimeDelta (speed);
	while (omega >= 1.0) omega -= 1.0;
	double location = (double) index - time * lfo * core -> DSP_CHORUS_time_fraction;
	while (location < 0.0) location += 65536.0; while (location >= 65536.0) location -= 65536.0;
	line [index++] = enter;
	if (index > 65535) index = 0;
	signal = enter + level * DIV_16384 * interpolate (location, line);
}
integrated_chorus :: integrated_chorus (orbiter_core * core) {
	this -> core = core;
	for (int ind = 0; ind < 65536; ind++) line [ind] = 0.0;
	signal = enter = omega = 0.0;
	time = 1024.0;
	index = 0;
	level = 0.0;
	speed = 0.0; amp = 8192.0;
}
void integrated_stereo_chorus :: move (void) {
	double amp_divided = amp * DIV_16384;
	double lfo_left = 1.0 + amp_divided * core -> SineApproximated (omega);
	double lfo_right = 1.0 + amp_divided * core -> SineApproximated (omega + phase * DIV_16384);
	omega += core -> ControlTimeDelta (speed);
	while (omega >= 1.0) omega -= 1.0;
	double level_divided = level * DIV_16384;
	double location = (double) index - time * lfo_left * core -> DSP_CHORUS_time_fraction;
	while (location < 0.0) location += 65536.0; while (location >= 65536.0) location -= 65536.0;
	double ml = mono + left;
	signal = ml + level_divided * interpolate (location, line);
	location = (double) index - (time + bias) * lfo_right * core -> DSP_CHORUS_time_fraction;
	while (location < 0.0) location += 65536.0; while (location >= 65536.0) location -= 65536.0;
	double mr = mono + right;
	signal_right = mr + level_divided * interpolate (location, line_right);
	line [index] = ml;
	line_right [index++] = mr;
	if (index > 65535) index = 0;
}
integrated_stereo_chorus :: integrated_stereo_chorus (orbiter_core * core) {
	this -> core = core;
	for (int ind = 0; ind < 65536; ind++) line [ind] = line_right [ind] = 0.0;
	mono = left = right = signal = signal_right = omega = 0.0;
	time = 1024.0;
	index = 0;
	level = 0.0;
	speed = 0.0; amp = 8192.0;
	bias = phase = 0.0;
}

void integrated_delay :: move (void) {
	if (high_damp != previous_high_damp) {
		previous_high_damp = high_damp;
		B = high_damp != 0.0 ? exp (-48000000.0 / (core -> sampling_frequency * high_damp)) : 0.0;
		A = 1.0 - B;
	}
	double feed = feedback * DIV_16384;
	int sentinel = (int) (time * core -> DSP_time_fraction);
	sentinel |= 1;
	if (sentinel > 262143) sentinel = 262143;
	if (index >= sentinel) index = 0;
	double value = line [index];
	signal = value;
	value *= feed;
	value += enter;
	line [index++] = left_1 = A * value + B * left_1;
	if (index >= sentinel) index = 0;
	value = line [index];
	signal_right = value;
	value *= feed;
	value += enter_right;
	line [index++] = right_1 = A * value + B * right_1;
}

integrated_delay :: integrated_delay (orbiter_core * core) {
	this -> core = core;
	for (int ind = 0; ind < 262144; ind++) line [ind] = 0.0;
	index = 0;
	enter = enter_right = signal = signal_right = 0.0;
	time = 8192.0; feedback = 8192.0;
	high_damp = previous_high_damp = 0.0; A = 1.0; B = 0.0;
}

void integrated_drywet :: move (void) {
	double dry_balance = 1.0;
	double wet_balance = 1.0;
	if (balance < 0.0) wet_balance = balance <= -8192 ? 0.0 : 1 + balance * DIV_8192;
	else if (balance > 0.0) dry_balance = balance >= 8192 ? 0.0 : 1 - balance * DIV_8192;
	left = dry_left * dry_balance + wet_left * wet_balance;
	right = dry_right * dry_balance + wet_right * wet_balance;
}
integrated_drywet :: integrated_drywet (void) {
	dry_left = dry_right = wet_left = wet_right = 0.0;
	left = right = 0.0;
	balance = 0.0;
}
void integrated_mono_drywet :: move (void) {
	double dry_balance = 1.0;
	double wet_balance = 1.0;
	if (balance < 0.0) wet_balance = balance <= -8192 ? 0.0 : 1 + balance * DIV_8192;
	else if (balance > 0.0) dry_balance = balance >= 8192 ? 0.0 : 1 - balance * DIV_8192;
	signal = dry * dry_balance + wet * wet_balance;
}
integrated_mono_drywet :: integrated_mono_drywet (void) {dry = wet = balance = signal = 0.0;}

bool integrated_map :: return_content (PrologElement * parameters) {
	bool clean = true;
	for (int ind = 0; ind < 128; ind++) {if (map [ind] != (double) (initial + ind) * 128.0) clean = false;}
	if (clean) {parameters -> setPair (); return true;}
	for (int ind = 0; ind < 128; ind++) {
		parameters -> setPair ();
		parameters -> getLeft () -> setDouble (map [ind]);
		parameters = parameters -> getRight ();
	}
	return true;
}
bool integrated_map :: read_content (PrologElement * parameters) {
	int double_count = 0;
	PrologElement * doubles [128];
	PrologElement * ret = 0;
	PrologElement * reset = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) if (double_count < 128) doubles [double_count++] = el;
		if (el -> isVar ()) ret = el;
		if (el -> isEarth ()) reset = el;
		if (el -> isPair ()) {
			PrologElement * subel = el;
			while (subel -> isPair ()) {
				PrologElement * ell = subel -> getLeft ();
				if (ell -> isEarth ()) reset = ell;
				if (ell -> isNumber ()) if (double_count < 128) doubles [double_count++] = ell;
				subel = subel -> getRight ();
			}
		}
		parameters = parameters -> getRight ();
	}
	if (reset != 0) {this -> reset (); return true;}
	if (parameters -> isVar ()) ret = parameters;
	if (ret != 0) {
		if (double_count < 1) return_content (ret);
		if (! doubles [0] -> isInteger ()) return false;
		int index = doubles [0] -> getInteger ();
		if (index < 0 || index > 127) return false;
		ret -> setDouble (map [index]);
		return true;
	}
	if (double_count == 2) {
		if (! doubles [0] -> isInteger ()) return false;
		int index = doubles [0] -> getInteger ();
		if (index < 0 || index > 127) return false;
		map [index] = doubles [1] -> getNumber ();
		return true;
	}
	if (double_count == 128) {for (int ind = 0; ind < 128; ind++) map [ind] = doubles [ind] -> getNumber (); return true;}
	return false;
}
void integrated_map :: reset (void) {for (int ind = 0; ind < 128; ind++) map [ind] = (double) (initial + ind) * 128;}
integrated_map :: integrated_map (int initial) {this -> initial = initial; reset ();}

integrated_auto_frame :: integrated_auto_frame (double value, double time, integrated_auto_frame * prevoius) {this -> value = value; this -> time = time; this -> previous = previous; next = 0;}
integrated_auto_frame :: ~ integrated_auto_frame (void) {if (next != 0) delete next;}
bool integrated_auto_data :: return_content (PrologElement * parameters) {return false;}
bool integrated_auto_data :: read_content (PrologElement * parameters) {return false;}
void integrated_auto_data :: move (void) {
	if (control < 16) return; // OPTIMISATION = RETURN MOST OF THE TIME
	pthread_mutex_lock (& critical);
	if (trigger > 0.0) {
		if (record == 0.0) {
			// START RECORDING (AND MAKE SURE THAT THE NEW FRAMES POINTS TO A DIFFERENT LOCATION)
			integrated_auto_frame * ptr = frames;
			frames = current_frame = new integrated_auto_frame (signal);
			if (ptr != 0) delete ptr;
			time = core -> sample_duration;
			record = trigger;
		} else {
			// KEEP RECORDING
			if (current_frame != 0 && current_frame -> value != signal) {
				current_frame -> time = time;
				current_frame = current_frame -> next = new integrated_auto_frame (signal, time, current_frame);
			}
			time += core -> sample_duration;
		}
	} else {
		// STOP RECORDING, MARK THE TIME
		if (record != 0.0) {record = 0.0; if (current_frame != 0) current_frame -> time = time;}
	}
	pthread_mutex_unlock (& critical);
}
integrated_auto_data :: integrated_auto_data (orbiter_core * core) {
	this -> core = core;
	frames = current_frame = 0;
	trigger = record = time = 0.0;
	signal = 0.0;
	pthread_mutex_init (& critical, 0);
}
integrated_auto_data :: ~ integrated_auto_data (void) {if (frames != 0) delete frames; frames = 0; pthread_mutex_destroy (& critical);}

void integrated_auto_player :: filter (double enter) {
	if (maximum_change == 0.0) {signal = enter; return;}
	if (enter == signal) return;
	if (enter > signal) {signal += maximum_change * core -> gate_delay; if (signal > enter) signal = enter; return;}
	signal -= maximum_change * core -> gate_delay;
	if (signal < enter) signal = enter;
}
void integrated_auto_player :: move (void) {
	if (trigger == 0.0 || data -> control < 1.0 || data -> control >= 16.0) {filter (data -> signal); active_playback = false; return;}
	if (trigger > 0.0) {
		if (active_playback) {
			// KEEP PLAYING
			pthread_mutex_lock (& data -> critical);
			if (data -> frames != frames) {pthread_mutex_unlock (& data -> critical); filter (data -> signal); return;}
			while (current_frame != 0 && time >= current_frame -> time) current_frame = current_frame -> next;
			if (current_frame != 0) filter (current_frame -> value);
			else {
				if (data -> control == 2.0) {
					current_frame = frames;
					if (current_frame != 0) filter (current_frame -> value); else active_playback = false;
					time = 0.0;
				}
			}
			pthread_mutex_unlock (& data -> critical);
			time += core -> sample_duration;
		} else {
			// START PLAYBACK
			pthread_mutex_lock (& data -> critical);
			active_playback = true;
			frames = current_frame = data -> frames;
			if (frames == 0) {pthread_mutex_unlock (& data -> critical); filter (data -> signal); return;}
			filter (frames -> value);
			pthread_mutex_unlock (& data -> critical);
			time = core -> sample_duration;
		}
	}
}
integrated_auto_player :: integrated_auto_player (orbiter_core * core, integrated_auto_data * data, double maximum_change) {
	this -> core = core;
	this -> data = data;
	this -> maximum_change = maximum_change;
	frames = current_frame = 0;
	trigger = time = 0.0; active_playback = returning = false;
	signal = 0.0;
}
