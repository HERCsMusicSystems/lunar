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
}

integrated_lfo :: integrated_lfo (orbiter_core * core) {
	this -> core = core;
	stage_one = true;
	origin = target = delta = 0.0;
	time = speed = wave = pulse = phase = sync = positive = negative = 0.0;
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

void integrated_adsr :: move (void) {
	if (trigger >= 16384.0) {
		busy = 1.0;
		if (attack == 0.0) {
			if (decay == 0.0) {signal = sustain; stage = 3; return;}
			signal = 0.0; stage = 2; time = core -> WaitingTime (decay); return;
		}
		signal = -16384.0; stage = 1; time = core -> WaitingTime (attack); return;
	}
	if (trigger == 0.0) {
		if (stage == 0) return;
		if (stage == 4) {
			if (time < 1.0) {signal = time * -16384.0; time += core -> WaitingTime (release); return;}
			time = 0.0; signal = -16384.0; stage = 0; busy = 0.0; return;
		}
		if (release == 0.0) {time = 0.0; signal = -16384; stage = 0; busy = 0.0; return;}
		if (time < 1.0) {
			if (stage == 1) signal = time * 16384.0 - 16384.0;
			if (stage == 2) signal = time * -16384.0;
		}
		time = signal / -16384.0;
		time += core -> WaitingTime (release);
		stage = 4;
		return;
	} else {
		switch (stage) {
		case 0:
			time = 0.0; signal = -16384; busy = 1.0;
			if (attack == 0.0) {
				if (decay == 0.0) {signal = sustain; stage = 3; return;}
				signal = 0.0;
				stage = 2;
				time += core -> WaitingTime (decay);
				return;
			}
			stage = 1;
			time += core -> WaitingTime (attack);
			return;
		case 1:
			if (time < 1.0) {signal = time * 16384.0 - 16384.0; time += core -> WaitingTime (attack); return;}
			time = 0.0;
			if (decay == 0.0) {signal = sustain; stage = 3; return;}
			signal = 0.0;
			stage = 2;
			time += core -> WaitingTime (decay);
			return;
			break;
		case 2:
			if (time < 1.0) signal = time * -16384.0;
			if (signal <= sustain) {time = 0.0; signal = sustain; stage = 3; return;}
			time += core -> WaitingTime (decay);
			return;
			break;
		case 4:
			if (attack == 0.0) {
				time = 0.0;
				if (decay == 0.0) {signal = sustain; stage = 3; return;}
				signal = 0.0; stage = 2; time += core -> WaitingTime (decay);
				return;
			}
			time = 1 + signal / 16384.0;
			stage = 1;
			time += core -> WaitingTime (attack);
			return;
			break;
		default: return; break;
		}
	}
}

integrated_adsr :: integrated_adsr (orbiter_core * core) {
	this -> core = core;
	attack = decay = sustain = release = trigger = 0.0;
	signal = -16384.0; time = busy = 0.0;
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
	enter = enter_right = signal_right = 0.0;
	time = 8192.0; feedback = 0.0;
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
