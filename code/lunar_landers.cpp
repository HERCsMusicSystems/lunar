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
char * lunar_active_parameter_block :: inputName (int ind) {return ind == 0 ? "SIGNAL": orbiter :: inputName (ind);}
double * lunar_active_parameter_block :: inputAddress (int ind) {return ind == 0 ? & enter : 0;}
lunar_active_parameter_block :: lunar_active_parameter_block (orbiter_core * core, double maximum_change) : orbiter (core) {
	if (maximum_change < 0.0) maximum_change = 0.0;
	this -> maximum_change = maximum_change * 48000.0 / core -> sampling_frequency;
	initialise (); activate ();
}
void lunar_active_parameter_block :: move (void) {
	if (maximum_change == 0.0) {signal = enter; return;}
	if (enter == signal) return;
	if (enter > signal) {signal += maximum_change; if (signal > enter) signal = enter;}
	signal -= maximum_change;
	if (signal < enter) signal = enter;
}

int lunar_inactive_parameter_block :: numberOfInputs (void) {return orbiter :: numberOfOutputs ();}
char * lunar_inactive_parameter_block :: inputName (int ind) {return orbiter :: outputName (ind);}
double * lunar_inactive_parameter_block :: inputAddress (int ind) {return orbiter :: outputAddress (ind);}
lunar_inactive_parameter_block :: lunar_inactive_parameter_block (orbiter_core * core) : orbiter (core) {initialise ();}

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
	this -> key = map == 0 ? (double) key * 128.0 : map -> map [key];
	trigger = 1.0;
}
void lunar_trigger :: keyon (int key, int velocity) {this -> velocity = (double) velocity * 128.0; keyon (key);}
void lunar_trigger :: keyoff (void) {trigger = 0.0;}
lunar_trigger :: lunar_trigger (orbiter_core * core) : orbiter (core) {key = trigger = 0.0; velocity = 12800.0; map = 0; initialise ();}
lunar_trigger :: ~ lunar_trigger (void) {if (map != 0) map -> release ();}

int lunar_impulse :: numberOfInputs (void) {return 1;}
char * lunar_impulse :: inputName (int ind) {return ind == 0 ? "ENTER" : orbiter :: inputName (ind);}
double * lunar_impulse :: inputAddress (int ind) {return ind == 0 ? & enter : orbiter :: inputAddress (ind);}
void lunar_impulse :: move (void) {signal = enter > 0.0 && sync == 0.0 ? 1.0 : 0.0; sync = enter;}
lunar_impulse :: lunar_impulse (orbiter_core * core) : orbiter (core) {enter = sync = 0.0; initialise (); activate ();}
