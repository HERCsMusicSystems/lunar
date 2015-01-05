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

////////////////////////////////////////////////////////////////////
// This file was created on Tuesday, 29th July 2014, 13:47:30 PM. //
////////////////////////////////////////////////////////////////////

#include "lunar_landers.h"

#define DIV_16384 0.00006103515625
#define DIV_8192  0.0001220703125

int lunar_parameter_block :: numberOfInputs (void) {return 1;}
char * lunar_parameter_block :: inputName (int ind) {if (ind == 0) return "SIGNAL"; else return  orbiter :: inputName (ind);}
double * lunar_parameter_block :: inputAddress (int ind) {return ind != 0 ? orbiter :: inputAddress (ind) : active ? & enter : orbiter :: outputAddress (ind);}
void lunar_parameter_block :: move (void) {
	if (maximum_change == 0.0) {signal = enter; return;}
	if (enter == signal) return;
	if (enter > signal) {signal += maximum_change * core -> gate_delay; if (signal > enter) signal = enter; return;}
	signal -= maximum_change * core -> gate_delay;
	if (signal < enter) signal = enter;
}
lunar_parameter_block :: lunar_parameter_block (orbiter_core * core, int style, bool active, double maximum_change) : orbiter (core) {
	this -> active = active;
	this -> style = style;
	this -> maximum_change = maximum_change >= 0.0 ? maximum_change : 0.0;
	initialise ();
	if (active) activate ();
}

auto_frame :: auto_frame (double value, double time, auto_frame * prevoius) {this -> value = value; this -> time = time; this -> previous = previous; next = 0;}
auto_frame :: ~ auto_frame (void) {if (next != 0) delete next;}

int auto_data :: numberOfInputs (void) {return 3;}
char * auto_data :: inputName (int ind) {
	switch (ind) {
	case 0: return "ENTER"; break;
	case 1: return "TRIGGER"; break;
	case 2: return "CONTROL"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * auto_data :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & signal; break;
	case 1: return & trigger; break;
	case 2: return & control; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int auto_data :: numberOfOutputs (void) {return 0;}
void auto_data :: move (void) {
	if (control < 16) return; // OPTIMISATION = RETURN MOST OF THE TIME
	pthread_mutex_lock (& critical);
	if (trigger > 0.0) {
		if (record == 0.0) {
			// START RECORDING (AND MAKE SURE THAT THE NEW FRAMES POINTS TO A DIFFERENT LOCATION)
			auto_frame * ptr = frames;
			frames = current_frame = new auto_frame (signal);
			if (ptr != 0) delete ptr;
			time = core -> sample_duration;
			record = trigger;
		} else {
			// KEEP RECORDING
			if (current_frame != 0 && current_frame -> value != signal) {
				current_frame -> time = time;
				current_frame = current_frame -> next = new auto_frame (signal, time, current_frame);
			}
			time += core -> sample_duration;
		}
	} else {
		// STOP RECORDING, MARK THE TIME
		if (record != 0.0) {record = 0.0; if (current_frame != 0) current_frame -> time = time;}
	}
	pthread_mutex_unlock (& critical);
}
void auto_data :: clear_frames (void) {
	pthread_mutex_lock (& critical);
	if (frames != 0) delete frames; frames = current_frame = 0;
	pthread_mutex_unlock (& critical);
}
auto_frame * auto_data :: insert_frame (double value, double time) {
	pthread_mutex_lock (& critical);
	if (frames == 0) {frames = current_frame = new auto_frame (value, time); pthread_mutex_unlock (& critical); return frames;}
	if (current_frame == 0) current_frame = frames;
	while (current_frame -> next != 0) current_frame = current_frame -> next;
	current_frame = current_frame -> next = new auto_frame (value, time);
	pthread_mutex_unlock (& critical);
	return current_frame;
}
auto_data :: auto_data (orbiter_core * core) : orbiter (core) {
	frames = current_frame = 0;
	trigger = record = time = 0.0;
	pthread_mutex_init (& critical, 0);
	initialise (); activate ();
}
auto_data :: ~ auto_data (void) {if (frames != 0) delete frames; frames = 0; pthread_mutex_destroy (& critical);}

int auto_player :: numberOfInputs (void) {return 1;}
char * auto_player :: inputName (int ind) {if (ind == 0) return "TRIGGER"; return orbiter :: inputName (ind);}
double * auto_player :: inputAddress (int ind) {if (ind == 0) return & trigger; return orbiter :: inputAddress (ind);}
void auto_player :: filter (double enter) {
	if (maximum_change == 0.0) {signal = enter; return;}
	if (enter == signal) return;
	if (enter > signal) {signal += maximum_change * core -> gate_delay; if (signal > enter) signal = enter; return;}
	signal -= maximum_change * core -> gate_delay;
	if (signal < enter) signal = enter;
}
void auto_player :: move (void) {
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
bool auto_player :: release (void) {
	auto_data * data_to_delete = this -> data;
	bool ret = orbiter :: release ();
	if (ret) {if (data_to_delete != 0) data_to_delete -> release ();}
	return ret;
}
auto_player :: auto_player (orbiter_core * core, auto_data * data, double maximum_change) : orbiter (core) {
	if (data != 0) data -> hold ();
	this -> data = data;
	this -> maximum_change = maximum_change;
	frames = current_frame = 0;
	trigger = time = 0.0; active_playback = returning = false;
	initialise ();
	if (data != 0) activate ();
}

int lunar_mixer :: numberOfInputs (void) {return 1;}
char * lunar_mixer :: inputName (int ind) {if (ind == 0) return "ENTER"; else return orbiter :: inputName (ind);}
double * lunar_mixer :: inputAddress (int ind) {return ind == 0 ? & enter : orbiter :: inputAddress (ind);}
void lunar_mixer :: move (void) {signal = enter;}
lunar_mixer :: lunar_mixer (orbiter_core * core) : orbiter (core) {enter = 0.0; initialise (); activate ();}

int lunar_stereo_mixer :: numberOfInputs (void) {return 2;}
char * lunar_stereo_mixer :: inputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_stereo_mixer :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & enter; break;
	case 1: return & enter_right; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_stereo_mixer :: numberOfOutputs (void) {return numberOfInputs ();}
char * lunar_stereo_mixer :: outputName (int ind) {return inputName (ind);}
double * lunar_stereo_mixer :: outputAddress (int ind) {if (ind == 1) return & signal_right; return orbiter :: outputAddress (ind);}
void lunar_stereo_mixer :: move (void) {signal = enter; signal_right = enter_right;}
lunar_stereo_mixer :: lunar_stereo_mixer (orbiter_core * core) : orbiter (core) {signal_right = enter = enter_right = 0.0; initialise (); activate ();}

int lunar_gateway :: numberOfInputs (void) {return 2;}
char * lunar_gateway :: inputName (int ind) {if (ind == 0) return "ENTER"; if (ind == 1) return "GATEWAY"; return orbiter :: inputName (ind);}
double * lunar_gateway :: inputAddress (int ind) {if (ind == 0) return & enter; if (ind == 1) return & gateway; return orbiter :: inputAddress (ind);}
void lunar_gateway :: move (void) {signal = enter * gateway * DIV_16384;}
lunar_gateway :: lunar_gateway (orbiter_core * core) : orbiter (core) {enter = 0.0; gateway = 1.0; initialise (); activate ();}

int lunar_stereo_gateway :: numberOfInputs (void) {return 3;}
char * lunar_stereo_gateway :: inputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	case 2: return "GATEWAY"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_stereo_gateway :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & enter; break;
	case 1: return & enter_right; break;
	case 2: return & gateway; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_stereo_gateway :: numberOfOutputs (void) {return 2;}
char * lunar_stereo_gateway :: outputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_stereo_gateway :: outputAddress (int ind) {
	switch (ind) {
	case 0: return & signal; break;
	case 1: return & signal_right; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
void lunar_stereo_gateway :: move (void) {double dv = gateway * DIV_16384; signal = enter * dv; signal_right = enter_right * dv;}
lunar_stereo_gateway :: lunar_stereo_gateway (orbiter_core * core) : orbiter (core) {
	enter = enter_right = signal_right = 0.0; gateway = 1.0;
	initialise (); activate ();
}

void lunar_amplifier :: move (void) {signal = enter * core -> Amplitude (gateway);}
lunar_amplifier :: lunar_amplifier (orbiter_core * core) : lunar_gateway (core) {gateway = 0.0;}

void lunar_stereo_amplifier :: move (void) {double dv = core -> Amplitude (gateway); signal = enter * dv; signal_right = enter_right * dv;}
lunar_stereo_amplifier :: lunar_stereo_amplifier (orbiter_core * core) : lunar_stereo_gateway (core) {gateway = 0.0;}

char * lunar_mono_volume :: inputName (int ind) {if (ind == 1) return "VOLUME"; return lunar_gateway :: inputName (ind);}
void lunar_mono_volume :: move (void) {signal = enter * gateway * core -> Volume (gateway);}
lunar_mono_volume :: lunar_mono_volume (orbiter_core * core) : lunar_gateway (core) {gateway = 1280.0;}

char * lunar_volume :: inputName (int ind) {if (ind == 2) return "VOLUME"; return lunar_stereo_gateway :: inputName (ind);}
void lunar_volume :: move (void) {double dv = core -> Volume (gateway); signal = enter * dv; signal_right = enter_right * dv;}
lunar_volume :: lunar_volume (orbiter_core * core) : lunar_stereo_gateway (core) {gateway = 12800.0;}

void lunar_control :: move (void) {signal = enter * gateway;}
lunar_control :: lunar_control (orbiter_core * core) : lunar_gateway (core) {gateway = 0.0;}

int lunar_map :: numberOfOutputs (void) {return 0;}
lunar_map :: lunar_map (orbiter_core * core, int initial) : orbiter (core) {
	for (int ind = 0; ind < 128; ind++) map [ind] = (double) (initial++) * 128.0;
	initialise ();
}

int lunar_trigger :: numberOfInputs (void) {return 5;}
char * lunar_trigger :: inputName (int ind) {
	switch (ind) {
	case 0: return "BUSY"; break;
	case 1: return "HOLD"; break;
	case 2: return "PORTA"; break;
	case 3: return "TIME"; break;
	case 4: return "LEGATO"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_trigger :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & busy; break;
	case 1: return & hold_ctrl; break;
	case 2: return & porta_switch; break;
	case 3: return & porta_time; break;
	case 4: return & porta_control; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_trigger :: numberOfOutputs (void) {return 3;}
char * lunar_trigger :: outputName (int ind) {
	switch (ind) {
	case 0: return "KEY"; break;
	case 1: return "VELOCITY"; break;
	case 2: return "TRIGGER"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_trigger :: outputAddress (int ind) {
	switch (ind) {
	case 0: return & signal; break;
	case 1: return & velocity; break;
	case 2: return & trigger; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
void lunar_trigger :: set_key_map (lunar_map * map) {
	if (this -> key_map != 0) this -> key_map -> release ();
	this -> key_map = map;
	if (map != 0) map -> hold ();
}
void lunar_trigger :: set_velocity_map (lunar_map * map) {
	if (this -> velocity_map != 0) this -> velocity_map -> release ();
	this -> velocity_map = map;
	if (map != 0) map -> hold();
}
void lunar_trigger :: add_stack (int key) {
	if (keystack_pointer < 16) {keystack [keystack_pointer++] = key; return;}
	for (int ind = 0; ind < 14; ind++) keystack [ind] = keystack [ind + 1];
	keystack [15] = key;
	keystack_pointer = 16;
}
void lunar_trigger :: drop_stack (int key) {
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
void lunar_trigger :: sub_keyon (int key) {
	if (key < 0) key = 0;
	if (key > 127) key = 127;
	target = key_map == 0 ? (double) (key - 64) * 128.0 : key_map -> map [key];
	this -> key = key;
	if (! active || porta_switch == 0.0 || porta_time == 0.0 || (porta_control != 0.0 && trigger == 0)) this -> signal = target;
	else {origin = this -> signal; delta = target - origin; time = 0.0;}
	trigger = 1.0;
	add_stack (key);
}
void lunar_trigger :: sub_velocity (int velocity) {
	if (velocity < 0) velocity = 0; if (velocity > 127) velocity = 127;
	this -> velocity = velocity_map == 0 ? (double) velocity * 128.0 : velocity_map -> map [velocity];
}
void lunar_trigger :: keyon (int key) {
	pthread_mutex_lock (& critical);
	request_key = key; request = 3;
	if (! active) sub_keyon (key);
	pthread_mutex_unlock (& critical);
}
void lunar_trigger :: keyon (int key, int velocity) {
	if (velocity < 1) {keyoff (key); return;}
	pthread_mutex_lock (& critical);
	request_key = key; request_velocity = velocity; request = 4;
	if (! active) keyon_velocity_request ();
	pthread_mutex_unlock (& critical);
}
void lunar_trigger :: ground (int key, int velocity, int base, int previous) {
	if (velocity < 1) {keyoff (key); return;}
	pthread_mutex_lock (& critical);
	request_key = key; request_velocity = velocity; request_base = base; request_previous = previous; request = 5;
	if (! active) ground_request ();
	pthread_mutex_unlock (& critical);
}
void lunar_trigger :: ground_request (void) {
	sub_velocity (request_velocity);
	if (request_key < 0) request_key = 0; if (request_key > 127) request_key = 127;
	target = key_map == 0 ? (double) (request_key - 64) * 128.0 : key_map -> map [request_key];
	this -> key = request_key; trigger = 16384.0; time = 0.0;
	if (! active || porta_switch == 0.0 || porta_time == 0.0) this -> signal = target;
	else {
		if (porta_control == 0.0) request_base = request_previous;
		this -> signal = origin = key_map == 0 ? (double) (request_base - 64) * 128.0 : key_map -> map [request_base];
		delta = target - origin;
	}
	keystack_pointer = 0; add_stack (request_key);
}
void lunar_trigger :: keyoff (int key) {
	pthread_mutex_lock (& critical);
	request_key = key; request = 1;
	drop_stack (request_key);
	if (! active) keyoff_request ();
	pthread_mutex_unlock (& critical);
}
void lunar_trigger :: keyoff (void) {
	pthread_mutex_lock (& critical);
	request = 2;
	if (! active) keyoff_all_request ();
	pthread_mutex_unlock (& critical);
}
void lunar_trigger :: keyon_velocity_request (void) {
	if (keystack_pointer < 1) sub_velocity (request_velocity); sub_keyon (request_key);
}
void lunar_trigger :: keyoff_request (void) {
	this -> key = -1;
	//drop_stack (request_key);
	if (keystack_pointer == 0 && hold_ctrl == 0.0) trigger = 0.0;
}
void lunar_trigger :: keyoff_all_request (void) {
	this -> key = -1; keystack_pointer = 0; trigger = 0.0;
}
bool lunar_trigger :: release (void) {
	lunar_map * to_delete_key_map = key_map;
	lunar_map * to_delete_velocity_map = velocity_map;
	lunar_trigger * to_delete_next = next;
	bool ret = orbiter :: release ();
	if (ret) {
		if (to_delete_key_map != 0) to_delete_key_map -> release ();
		if (to_delete_velocity_map != 0) to_delete_velocity_map -> release ();
		if (to_delete_next != 0) to_delete_next -> release ();
	}
	return ret;
}
void lunar_trigger :: move (void) {
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
	if (trigger != 0.0 && keystack_pointer == 0 && hold_ctrl == 0.0) trigger = 0.0;
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
lunar_trigger :: lunar_trigger (orbiter_core * core, bool active, lunar_trigger * next) : orbiter (core) {
	this -> next = next;
	if (next != 0) next -> hold ();
	key = -1;
	signal = trigger = busy = 0.0;
	origin = delta = target = 0.0;
	porta_switch = porta_time = porta_control = 0.0;
	hold_ctrl = 0.0;
	time = 2.0;
	velocity = 12800.0;
	key_map = 0;
	velocity_map = 0;
	for (int ind = 0; ind < 16; ind++) keystack [ind] = 0; keystack_pointer = 0;
	this -> active = active;
	request = 0;
	pthread_mutex_init (& critical, 0);
	initialise ();
	if (active) activate ();
}
lunar_trigger :: ~ lunar_trigger (void) {pthread_mutex_destroy (& critical);}

int lunar_impulse :: numberOfInputs (void) {return 1;}
char * lunar_impulse :: inputName (int ind) {if (ind == 0) return "ENTER"; else return orbiter :: inputName (ind);}
double * lunar_impulse :: inputAddress (int ind) {return ind == 0 ? & enter : orbiter :: inputAddress (ind);}
void lunar_impulse :: move (void) {signal = enter > 0.0 && sync == 0.0 ? 1.0 : 0.0; sync = enter;}
lunar_impulse :: lunar_impulse (orbiter_core * core) : orbiter (core) {enter = sync = 0.0; initialise (); activate ();}

int lunar_lfo :: numberOfInputs (void) {return 5;}
char * lunar_lfo :: inputName (int ind) {
	switch (ind) {
	case 0: return "SPEED"; break;
	case 1: return "WAVE"; break;
	case 2: return "PULSE"; break;
	case 3: return "PHASE"; break;
	case 4: return "SYNC"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_lfo :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & speed; break;
	case 1: return & wave; break;
	case 2: return & pulse; break;
	case 3: return & phase; break;
	case 4: return & sync; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_lfo :: numberOfOutputs (void) {return 3;}
char * lunar_lfo :: outputName (int ind) {
	switch (ind) {
	case 1: return "NEGATIVE"; break;
	case 2: return "POSITIVE"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_lfo :: outputAddress (int ind) {
	switch (ind) {
	case 1: return & negative; break;
	case 2: return & positive; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
void lunar_lfo :: move (void) {
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
	}
	time += core -> ControlTimeDelta (speed);
	if (time >= 1.0) time -= 1.0;
}
lunar_lfo :: lunar_lfo (orbiter_core * core) : orbiter (core) {
	stage_one = true;
	origin = target = delta = 0.0;
	time = speed = wave = pulse = phase = sync = positive = negative = 0.0;
	initialise (); activate ();
}

int lunar_adsr :: numberOfInputs (void) {return 5;}
char * lunar_adsr :: inputName (int ind) {
	switch (ind) {
	case 0: return "TRIGGER"; break;
	case 1: return "ATTACK"; break;
	case 2: return "DECAY"; break;
	case 3: return "SUSTAIN"; break;
	case 4: return "RELEASE"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_adsr :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & trigger; break;
	case 1: return & attack; break;
	case 2: return & decay; break;
	case 3: return & sustain; break;
	case 4: return & release; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_adsr :: numberOfOutputs (void) {return 2;}
char * lunar_adsr :: outputName (int ind) {if (ind == 1) return "BUSY"; return orbiter :: outputName (ind);}
double * lunar_adsr :: outputAddress (int ind) {if (ind == 1) return & busy; return orbiter :: outputAddress (ind);}
void lunar_adsr :: move (void) {
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
lunar_adsr :: lunar_adsr (orbiter_core * core) : orbiter (core) {
	attack = decay = sustain = release = trigger = busy = 0.0;
	signal = -16384.0; time = busy = 0.0;
	stage = 0;
	initialise (); activate ();
}

int lunar_eg :: numberOfInputs (void) {return 9;}
char * lunar_eg :: inputName (int ind) {
	switch (ind) {
	case 0: return "TRIGGER"; break;
	case 1: return "LEVEL1"; break;
	case 2: return "LEVEL2"; break;
	case 3: return "LEVEL3"; break;
	case 4: return "LEVEL4"; break;
	case 5: return "TIME1"; break;
	case 6: return "TIME2"; break;
	case 7: return "TIME3"; break;
	case 8: return "TIME4"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_eg :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & trigger; break;
	case 1: return & level1; break;
	case 2: return & level2; break;
	case 3: return & level3; break;
	case 4: return & level4; break;
	case 5: return & time1; break;
	case 6: return & time2; break;
	case 7: return & time3; break;
	case 8: return & time4; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_eg :: numberOfOutputs (void) {return 2;}
char * lunar_eg :: outputName (int ind) {if (ind == 1) return "BUSY"; return orbiter :: outputName (ind);}
double * lunar_eg :: outputAddress (int ind) {if (ind == 1) return & busy; return orbiter :: outputAddress (ind);}
void lunar_eg :: move (void) {
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
		case 0: case 5:
			if (time1 == 0.0) {
				if (time2 == 0.0) {
					if (time3 == 0.0) {signal = level3; stage = 4;}
					else {signal = level2; stage = 3;}
				} else {signal = level1; stage = 2;}
			} else {if (stage == 0) signal = level4; stage = 1;}
			break;
		case 1:
			if (level1 > signal) {
				signal += core -> WaitingTime16384 (time1);
				if (signal > level1) {
					if (time2 == 0.0) {
						if (time3 == 0.0) {signal = level3; stage = 4;}
						else {signal = level2; stage = 3;}
					} else {signal = level1; stage = 2;}
				}
			} else {
				signal -= core -> WaitingTime16384 (time1);
				if (signal < level1) {
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
				if (signal > level2) {
					if (time3 == 0.0) {signal = level3; stage = 4;}
					else {signal = level2; stage = 3;}
				}
			} else {
				signal -= core -> WaitingTime16384 (time2);
				if (signal < level2) {
					if (time3 == 0.0) {signal = level3; stage = 4;}
					else {signal = level2; stage = 3;}
				}
			}
			break;
		case 3:
			if (level3 > signal) {
				signal += core -> WaitingTime16384 (time3);
				if (signal > level3) {signal = level3; stage = 4;}
			} else {
				signal -= core -> WaitingTime16384 (time3);
				if (signal < level3) {signal = level3; stage = 4;}
			}
			break;
		default: break;
		}
	}
}
lunar_eg :: lunar_eg (orbiter_core * core) : orbiter (core) {
	level1 = level2 = level3 = 0.0;
	level4 = -16384.0;
	time1 = time2 = time3 = time4 = 0.0;
	signal = -16384.0; busy = 0.0;
	stage = 0;
	initialise (); activate ();
}

int lunar_delay :: numberOfInputs (void) {return 5;}
char * lunar_delay :: inputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	case 2: return "FEEDBACK"; break;
	case 3: return "TIME"; break;
	case 4: return "HIGHDAMP"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_delay :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & enter; break;
	case 1: return & enter_right; break;
	case 2: return & feedback; break;
	case 3: return & time; break;
	case 4: return & high_dump; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_delay :: numberOfOutputs (void) {return 2;}
char * lunar_delay :: outputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_delay :: outputAddress (int ind) {
	switch (ind) {
	case 0: return & signal; break;
	case 1: return & signal_right; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
#define DIV_16384 0.00006103515625
void lunar_delay :: move (void) {
	double feed = feedback * DIV_16384;
	int sentinel = (int) (time * core -> DSP_time_fraction);
	sentinel |= 1;
	if (sentinel > 131071) sentinel = 131071;
	if (index >= sentinel) index = 0;
	double value = line [index];
	signal = value;
	value *= feed;
	value += enter;
	line [index++] = value;
	if (index >= sentinel) index = 0;
	value = line [index];
	signal_right = value;
	value *= feed;
	value += enter_right;
	line [index++] = value;
}
lunar_delay :: lunar_delay (orbiter_core * core) : orbiter (core) {
	for (int ind = 0; ind < 131072; ind++) line [ind] = 0.0;
	index = 0;
	filter = 0.0;
	enter = enter_right = signal_right = 0.0;
	initialise (); activate ();
}

int lunar_drywet :: numberOfInputs (void) {return 5;}
char * lunar_drywet :: inputName (int ind) {
	switch (ind) {
	case 0: return "DRYLEFT"; break;
	case 1: return "DRYRIGHT"; break;
	case 2: return "WETLEFT"; break;
	case 3: return "WETRIGHT"; break;
	case 4: return "BALANCE"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_drywet :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & dry_left; break;
	case 1: return & dry_right; break;
	case 2: return & wet_left; break;
	case 3: return & wet_right; break;
	case 4: return & balance; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_drywet :: numberOfOutputs (void) {return 2;}
char * lunar_drywet :: outputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_drywet :: outputAddress (int ind) {
	switch (ind) {
	case 0: return & signal; break;
	case 1: return & signal_right; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
void lunar_drywet :: move (void) {
	double dry_balance = 1.0;
	double wet_balance = 1.0;
	if (balance < 0.0) wet_balance = balance <= -8192 ? 0.0 : 1 + balance * DIV_8192;
	else if (balance > 0.0) dry_balance = balance >= 8192 ? 0.0 : 1 - balance * DIV_8192;
	signal = dry_left * dry_balance + wet_left * wet_balance;
	signal_right = dry_right * dry_balance + wet_right * wet_balance;
}
lunar_drywet :: lunar_drywet (orbiter_core * core) : orbiter (core) {
	dry_left = dry_right = wet_left = wet_right = 0.0;
	signal_right = 0.0;
	balance = 0.0;
	initialise (); activate ();
}

int lunar_drywet_mono :: numberOfInputs (void) {return 3;}
char * lunar_drywet_mono :: inputName (int ind) {
	switch (ind) {
	case 0: return "DRY"; break;
	case 1: return "WET"; break;
	case 2: return "BALANCE"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_drywet_mono :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & dry; break;
	case 1: return & wet; break;
	case 2: return & balance; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
void lunar_drywet_mono :: move (void) {
	double dry_balance = 1.0;
	double wet_balance = 1.0;
	if (balance < 0.0) wet_balance = balance <= -8192 ? 0.0 : 1 + balance * DIV_8192;
	else if (balance > 0.0) dry_balance = balance >= 8192 ? 0.0 : 1 - balance * DIV_8192;
	signal = dry * dry_balance + wet * wet_balance;
}
lunar_drywet_mono :: lunar_drywet_mono (orbiter_core * core) : orbiter (core) {
	dry = wet = balance = 0.0;
	initialise (); activate ();
}

int lunar_pan :: numberOfInputs (void) {return 2;}
char * lunar_pan :: inputName (int ind) {
	switch (ind) {
	case 0: return "SIGNAL"; break;
	case 1: return "PAN"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_pan :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & enter; break;
	case 1: return & pan; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_pan :: numberOfOutputs (void) {return 2;}
char * lunar_pan :: outputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_pan :: outputAddress (int ind) {
	switch (ind) {
	case 0: return & signal; break;
	case 1: return & right; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
void lunar_pan :: move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	signal = * (core -> pan - ind) * enter;
	right = * (core -> pan + ind) * enter;
}
lunar_pan :: lunar_pan (orbiter_core * core) : orbiter (core) {enter = pan = right = 0.0; initialise (); activate ();}
void lunar_power_pan :: move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	signal = * (core -> power_pan - ind) * enter;
	right = * (core -> power_pan + ind) * enter;
}
lunar_power_pan :: lunar_power_pan (orbiter_core * core) : lunar_pan (core) {}
void lunar_linear_pan :: move (void) {
	int ind = (int) pan;
	if (ind > 8192) ind = 8192; if (ind < -8192) ind = -8192;
	signal = * (core -> linear_pan - ind) * enter;
	right = * (core -> linear_pan + ind) * enter;
}
lunar_linear_pan :: lunar_linear_pan (orbiter_core * core) : lunar_pan (core) {}

int lunar_sensitivity :: numberOfInputs (void) {return 4;}
char * lunar_sensitivity :: inputName (int ind) {
	switch (ind) {
	case 0: return "SIGNAL"; break;
	case 1: return "BREAKPOINT"; break;
	case 2: return "LEFT"; break;
	case 3: return "RIGHT"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_sensitivity :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & enter; break;
	case 1: return & bp; break;
	case 2: return & lc; break;
	case 3: return & rc; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
void lunar_sensitivity :: move (void) {
	if (original_enter == enter && original_bp == bp && original_lc == lc && original_rc == rc) return;
	original_enter = enter; original_bp = bp; original_lc = lc; original_rc = rc;
	if (enter >= bp) {signal = (enter - bp) * rc / 128.0; return;}
	signal = (bp - enter) * lc / 128.0;
}
lunar_sensitivity :: lunar_sensitivity (orbiter_core * core) : orbiter (core) {
	original_enter = enter = 0.0;
	original_bp = bp = 0.0;
	original_lc = lc = -128.0;
	original_rc = rc = 128.0;
	initialise (); activate ();
}

int lunar_filter :: numberOfInputs (void) {return 4;}
char * lunar_filter :: inputName (int ind) {
	switch (ind) {
	case 0: return "ENTER"; break;
	case 1: return "FREQ"; break;
	case 2: return "RESONANCE"; break;
	case 3: return "AMP"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_filter :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & enter; break;
	case 1: return & freq; break;
	case 2: return & resonance; break;
	case 3: return & amp; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_filter :: numberOfOutputs (void) {return 4;}
char * lunar_filter :: outputName (int ind) {
	switch (ind) {
	case 0: return "LOWPASS"; break;
	case 1: return "HIGHPASS"; break;
	case 2: return "BANDPASS"; break;
	case 3: return "BANDREJECT"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_filter :: outputAddress (int ind) {
	switch (ind) {
	case 1: return & high_pass_signal; break;
	case 2: return & band_pass_signal; break;
	case 3: return & band_reject_signal; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
void lunar_filter :: move (void) {
	double F = core -> FilterFreq (freq);
	double Q = 2.0 - resonance * 0.0001220703125;
	running_signal += running_band_pass_signal * F;
	running_high_pass_signal = enter - running_signal - running_band_pass_signal * Q;
	running_band_pass_signal += running_high_pass_signal * F;
	running_signal += running_band_pass_signal * F;
	running_high_pass_signal = enter - running_signal - running_band_pass_signal * Q;
	running_band_pass_signal += running_high_pass_signal * F;
	double amplitude = core -> Amplitude (amp);
	signal = running_signal * amplitude;
	high_pass_signal = running_high_pass_signal * amplitude;
	band_pass_signal = running_band_pass_signal * amplitude;
	band_reject_signal = high_pass_signal + signal;
}
lunar_filter :: lunar_filter (orbiter_core * core) : orbiter (core) {
	high_pass_signal = band_pass_signal = band_reject_signal = running_signal = 0.0;
	running_high_pass_signal = running_band_pass_signal = 0.0;
	freq = amp = 0.0; resonance = 8192.0;
	enter = 0.0;
	initialise (); activate ();
}

