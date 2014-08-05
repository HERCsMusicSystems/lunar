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
lunar_mixer :: lunar_mixer (orbiter_core * core) : orbiter (core) {enter = 0.0;}

int lunar_gateway :: numberOfInputs (void) {return 2;}
char * lunar_gateway :: inputName (int ind) {if (ind == 0) return "ENTER"; if (ind == 1) return "GATEWAY"; return orbiter :: inputName (ind);}
double * lunar_gateway :: inputAddress (int ind) {if (ind == 0) return & enter; if (ind == 1) return & gateway; return orbiter :: inputAddress (ind);}
void lunar_gateway :: move (void) {
	signal = enter * gateway;
}
lunar_gateway :: lunar_gateway (orbiter_core * core) : orbiter (core) {enter = 0.0; gateway = 1.0; initialise (); activate ();}

int lunar_map :: numberOfOutputs (void) {return 0;}
lunar_map :: lunar_map (orbiter_core * core) : orbiter (core) {
	for (int ind = 0; ind < 128; ind++) map [ind] = (double) (ind - 64) * 128.0;
	initialise ();
}

int lunar_trigger :: numberOfInputs (void) {return 0;}
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
void lunar_trigger :: set_map (lunar_map * map) {
	if (this -> map != 0) this -> map -> release ();
	this -> map = map;
	if (map != 0) map -> hold ();
}
void lunar_trigger :: keyon (int key) {
	if (key < 0) key = 0;
	if (key > 127) key = 127;
	this -> key = map == 0 ? (double) (key - 64) * 128.0 : map -> map [key];
	trigger = 1.0;
}
void lunar_trigger :: keyon (int key, int velocity) {
	if (velocity < 1) {keyoff (); return;}
	this -> velocity = (double) velocity * 128.0;
	keyon (key);
}
void lunar_trigger :: keyoff (void) {trigger = 0.0;}
bool lunar_trigger :: release (void) {
	lunar_map * to_delete = map;
	bool ret = orbiter :: release ();
	if (ret && to_delete != 0) to_delete -> release ();
	return ret;
}
lunar_trigger :: lunar_trigger (orbiter_core * core) : orbiter (core) {key = trigger = 0.0; velocity = 12800.0; map = 0; initialise ();}

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
	double t = 0.5 + pulse * 0.0001220703125;
	switch ((int) wave) {
	case 0: // sine
		signal = positive = 0.0;
		break;
	case 1: // triangle
		if (t == 0) positive = 1.0 - time;
		else if (t == 1.0) positive = time;
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
			reversed_delta = target == origin ? 0.0 : 1.0 / (target - origin);
			stage_one = true;
		}} else {if (stage_one) {
			origin = target;
			target = 0.00006103515625 * (double) core -> noise14b;
			reversed_delta = target == origin ? 0.0 : 1.0 / (target - origin);
			stage_one = false;
		}}
		if (t == 0) positive = origin + reversed_delta * time;
		else if (t == 1.0) positive = origin + reversed_delta * time;
		else if (time < t) positive = origin + reversed_delta * time / t;
		else positive = origin + reversed_delta * (time - t) / (1.0 - t);
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
	origin = target = reversed_delta = 0.0;
	time = speed = wave = pulse = phase = sync = positive = 0.0;
	initialise (); activate ();
}

