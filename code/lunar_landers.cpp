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

void lunar_trigger :: set_map (lunar_map * map) {
	if (this -> map != 0) this -> map -> release ();
	this -> map = map;
	if (map != 0) map -> hold ();
}
lunar_trigger :: lunar_trigger (orbiter_core * core) : orbiter (core) {map = 0; initialise ();}
lunar_trigger :: ~ lunar_trigger (void) {if (map != 0) map -> release ();}
lunar_impulse :: lunar_impulse (orbiter_core * core) : orbiter (core) {initialise (); activate ();}
