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

/////////////////////////////////////////////////////////////////////
// This file was created on Monday, 21st July 2014 at 11:24:55 AM. //
/////////////////////////////////////////////////////////////////////

#include "lunar_operator.h"

//////////////////////
// LUNAR OSCILLATOR //
//////////////////////

int lunar_oscillator :: numberOfInputs (void) {return 5;}
char * lunar_oscillator :: inputName (int ind) {
	switch (ind) {
	case 0: return "FREQ"; break;
	case 1: return "AMP"; break;
	case 2: return "RATIO"; break;
	case 3: return "SYNC"; break;
	case 4: return "TRIGGER"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_oscillator :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & freq; break;
	case 1: return & amp; break;
	case 2: return & ratio; break;
	case 3: return & sync; break;
	case 4: return & trigger; break;
	default: break;
	}
	return 0;
}
lunar_oscillator :: lunar_oscillator (orbiter_core * core) : orbiter (core) {
	freq = amp = sync = trigger = slope = time = 0.0;
	ratio = 1.0;
	initialise (); activate ();
}

///////////////////////
// LUNAR OPERATOR FM //
///////////////////////

int lunar_operator :: numberOfInputs (void) {return 6;}
char * lunar_operator :: inputName (int ind) {if (ind == 5) return "SHIFT"; return lunar_oscillator :: inputName (ind);}
double * lunar_operator :: inputAddress (int ind) {if (ind == 5) return & shift; return lunar_oscillator :: inputAddress (ind);}
void lunar_operator :: move (void) {
	RETRIGGER_OSCILLATOR;
	this -> signal = core -> Amplitude (amp) * core -> Sine (time + shift);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}
lunar_operator :: lunar_operator (orbiter_core * core) : lunar_oscillator (core) {shift = 0.0;}

///////////////
// LUNAR SAW //
///////////////

void lunar_aliased_saw_operator :: move (void) {
	RETRIGGER_OSCILLATOR;
	signal = 1.0 - 2.0 * time;
	signal *= core -> Amplitude (amp);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}
lunar_aliased_saw_operator :: lunar_aliased_saw_operator (orbiter_core * core) : lunar_oscillator (core) {}
void lunar_saw_operator :: move (void) {
	RETRIGGER_OSCILLATOR;
	double delta = core -> TimeDelta (freq) * ratio;
	signal = core -> MinBlep (blep_index) - 1.0 - 2.0 * time;
	time += delta;
	while (time >= 1.0) {time -= 1.0; blep_index = (int) (time * 512.0 / (delta > 0.0 ? delta : 1.0));}
}
lunar_saw_operator :: lunar_saw_operator (orbiter_core * core) : lunar_oscillator (core) {blep_index = 0;}

//////////////////
// LUNAR SQUARE //
//////////////////

void lunar_aliased_square_operator :: move (void) {
	RETRIGGER_OSCILLATOR;
	if (time < 0.5) signal = core -> Amplitude (amp);
	else signal = - core -> Amplitude (amp);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}
lunar_aliased_square_operator :: lunar_aliased_square_operator (orbiter_core * core) : lunar_oscillator (core) {}
void lunar_square_operator :: move (void) {
	RETRIGGER_OSCILLATOR;
	double delta = core -> TimeDelta (freq) * ratio;
	if (stage) signal = core -> MinBlep (blep_index) - 1.0;
	else signal = 1.0 - core -> MinBlep (blep_index);
	time += delta;
	blep_index += 512;
	if (stage && time > 0.5) {stage = false; blep_index = (int) ((time - 0.5) * 512.0 / (delta > 0.0 ? delta : 1.0));}
	while (time >= 1.0) {stage = true; time -= 1.0; blep_index = (int) (time * 512.0 / (delta > 0.0 ? delta : 1.0));}
	signal *= core -> Amplitude (amp);
}
lunar_square_operator :: lunar_square_operator (orbiter_core * core) : lunar_saw_operator (core) {stage = true;}
