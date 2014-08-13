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

int lunar_active_parameter_block :: numberOfInputs (void) {return 1;}
char * lunar_active_parameter_block :: inputName (int ind) {if (ind == 0) return "SIGNAL"; else return  orbiter :: inputName (ind);}
double * lunar_active_parameter_block :: inputAddress (int ind) {return ind == 0 ? & enter : 0;}
lunar_active_parameter_block :: lunar_active_parameter_block (orbiter_core * core, double maximum_change) : orbiter (core) {
	if (maximum_change < 0.0) maximum_change = 0.0;
	initialise (); activate ();
}
void lunar_active_parameter_block :: move (void) {
	if (maximum_change == 0.0) {signal = enter; return;}
	if (enter == signal) return;
	if (enter > signal) {signal += maximum_change * core -> gate_delay; if (signal > enter) signal = enter;}
	signal -= maximum_change * core -> gate_delay;
	if (signal < enter) signal = enter;
}

int lunar_inactive_parameter_block :: numberOfInputs (void) {return orbiter :: numberOfOutputs ();}
char * lunar_inactive_parameter_block :: inputName (int ind) {return orbiter :: outputName (ind);}
double * lunar_inactive_parameter_block :: inputAddress (int ind) {return orbiter :: outputAddress (ind);}
lunar_inactive_parameter_block :: lunar_inactive_parameter_block (orbiter_core * core) : orbiter (core) {initialise ();}

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
void lunar_gateway :: move (void) {
	signal = enter * gateway;
}
lunar_gateway :: lunar_gateway (orbiter_core * core) : orbiter (core) {enter = 0.0; gateway = 1.0; initialise (); activate ();}

int lunar_map :: numberOfOutputs (void) {return 0;}
lunar_map :: lunar_map (orbiter_core * core, int initial) : orbiter (core) {
	for (int ind = 0; ind < 128; ind++) map [ind] = (double) (initial++) * 128.0;
	initialise ();
}

int lunar_trigger :: numberOfInputs (void) {return 1;}
char * lunar_trigger :: inputName (int ind) {if (ind == 0) return "BUSY"; return orbiter :: inputName (ind);}
double * lunar_trigger :: inputAddress (int ind) {if (ind == 0) return & busy; return orbiter :: inputAddress (ind);}
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
	case 0: return & key; break;
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
	if (keystack_pointer > 0) keyon (keystack [--keystack_pointer]);
}
void lunar_trigger :: keyon (int key) {
	if (key < 0) key = 0;
	if (key > 127) key = 127;
	this -> key = key_map == 0 ? (double) (key - 64) * 128.0 : key_map -> map [key];
	trigger = 1.0;
	add_stack (key);
}
void lunar_trigger :: keyon (int key, int velocity) {
	if (velocity < 1) {keyoff (); return;}
	if (velocity < 0) velocity = 0;
	if (velocity > 127) velocity = 127;
	this -> velocity = velocity_map == 0 ? (double) velocity * 128.0 : velocity_map -> map [velocity];
	keyon (key);
}
void lunar_trigger :: keyoff (int key) {drop_stack (key); if (keystack_pointer == 0) trigger = 0.0;}
void lunar_trigger :: keyoff (void) {keystack_pointer = 0; trigger = 0.0;}
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
lunar_trigger :: lunar_trigger (orbiter_core * core, lunar_trigger * next) : orbiter (core) {
	this -> next = next;
	if (next != 0) next -> hold ();
	key = trigger = busy = 0.0;
	velocity = 12800.0;
	key_map = 0;
	velocity_map = 0;
	for (int ind = 0; ind < 16; ind++) keystack [ind] = 0; keystack_pointer = 0;
	initialise ();
}

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
int lunar_lfo :: numberOfOutputs (void) {return 2;}
char * lunar_lfo :: outputName (int ind) {if (ind == 1) return "POSITIVE"; else return orbiter :: outputName (ind);}
double * lunar_lfo :: outputAddress (int ind) {return ind == 1 ? & positive : orbiter :: outputAddress (ind);}
void lunar_lfo :: move (void) {
	double t = 0.5 + pulse * 0.00006103515625;
	switch ((int) wave) {
	case 0: // sine
		if (t <= 0) signal = core -> SineApproximated (0.5 * time + 0.25);
		else if (t >= 1.0) signal = core -> SineApproximated (0.5 * time - 0.25);
		else if (time < t) signal = core -> SineApproximated (time * 0.5 / t - 0.25);
		else signal = core -> SineApproximated ((time - t) * 0.5 / (1.0 - t) + 0.25);
		positive = 0.5 + signal * 0.5;
		break;
	case 1: // triangle
		if (t <= 0) positive = 1.0 - time;
		else if (t >= 1.0) positive = time;
		else if (time < t) positive = time / t;
		else positive = (1.0 - time) / (1.0 - t);
		signal = positive * 2.0 - 1.0;
		break;
	case 2: // square
		if (time < t) signal = positive = 1.0;
		else {signal = -1.0; positive = 0.0;}
		break;
	case 3: // random
		if (time < t) {if (! stage_one) {
			origin = target;
			target = 0.00006103515625 * (double) core -> noise14b;
			delta = target - origin;
			stage_one = true;
		}} else {if (stage_one) {
			origin = target;
			target = 0.00006103515625 * (double) core -> noise14b;
			delta = target - origin;
			stage_one = false;
		}}
		if (t <= 0) positive = origin + delta * time;
		else if (t >= 1.0) positive = origin + delta * time;
		else if (time < t) positive = origin + delta * time / t;
		else positive = origin + delta * (time - t) / (1.0 - t);
		signal = positive * 2.0 - 1.0;
		break;
	case 4: // S/H
		if (time < t) {if (! stage_one) {
			signal = 0.0001220703125 * (double) core -> noise14b - 1.0;
			positive = 0.00006103515625 * (double) core -> noise14b;
			stage_one = true;
		}} else {if (stage_one) {
			signal = 0.0001220703125 * (double) core -> noise14b - 1.0;
			positive = 0.00006103515625 * (double) core -> noise14b;
			stage_one = false;
		}}
		break;
	}
	time += core -> ControlTimeDelta (speed);
	if (time >= 1.0) time -= 1.0;
}
lunar_lfo :: lunar_lfo (orbiter_core * core) : orbiter (core) {
	stage_one = true;
	origin = target = delta = 0.0;
	time = speed = wave = pulse = phase = sync = positive = 0.0;
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
	if (trigger == 0.0) {
		if (stage == 0) return;
		if (stage == 4) {
			if (time < 1.0) {signal = time * -16383.0; time += core -> WaitingTime (release); return;}
			time = 0.0; signal = -16383.0; stage = 0; busy = 0.0; return;
		}
		if (time < 1.0) {
			if (stage == 1) signal = time * 16383.0 - 16383.0;
			if (stage == 2) signal = time * -16383.0;
		}
		time = signal / -16383.0;
		time += core -> WaitingTime (release);
		stage = 4;
		return;
	} else {
		switch (stage) {
		case 0:
			time = 0.0; signal = -16383; busy = 1.0;
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
			if (time < 1.0) {signal = time * 16383.0 - 16383.0; time += core -> WaitingTime (attack); return;}
			time = 0.0;
			if (decay == 0.0) {signal = sustain; stage = 3; return;}
			signal = 0.0;
			stage = 2;
			time += core -> WaitingTime (decay);
			return;
			break;
		case 2:
			if (time < 1.0) signal = time * -16383.0;
			if (signal <= sustain) {time = 0.0; signal = sustain; stage = 3; return;}
			time += core -> WaitingTime (decay);
			return;
			break;
		case 4:
			if (attack == 0.0) {
				if (decay == 0.0) {time = 0.0; signal = sustain; stage = 3; return;}
				signal = 0.0; stage = 2; time += core -> WaitingTime (decay);
				return;
			}
			time = 1 + signal / 16383.0; stage = 1;
			time += core -> WaitingTime (attack);
			return;
			break;
		default: return; break;
		}
	}
}
lunar_adsr :: lunar_adsr (orbiter_core * core) : orbiter (core) {
	attack = decay = sustain = release = trigger = busy = 0.0;
	signal = -16383.0; time = busy = 0.0;
	stage = 0;
	initialise (); activate ();
}

lunar_eg :: lunar_eg (orbiter_core * core) : orbiter (core) {initialise (); activate ();}

