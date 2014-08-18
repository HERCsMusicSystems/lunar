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

int lunar_operator :: numberOfInputs (void) {return 6;}
char * lunar_operator :: inputName (int ind) {
	switch (ind) {
	case 0: return "FREQ"; break;
	case 1: return "AMP"; break;
	case 2: return "RATIO"; break;
	case 3: return "SHIFT"; break;
	case 4: return "SYNC"; break;
	case 5: return "TRIGGER"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_operator :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & freq; break;
	case 1: return & amp; break;
	case 2: return & ratio; break;
	case 3: return & shift; break;
	case 4: return & sync; break;
	case 5: return & trigger; break;
	default: break;
	}
	return 0;
}

void lunar_operator :: move (void) {
	if (slope != trigger) if (sync != 0.0 && trigger > 0.0) time = 0.0; slope = trigger;
	this -> signal = core -> Amplitude (amp) * core -> Sine (time + shift);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}

lunar_operator :: lunar_operator (orbiter_core * core) : orbiter (core) {
	freq = amp = shift = sync = trigger = slope = 0.0;
	ratio = 1.0;
	time = 0.0;
	initialise (); activate ();
}

int lunar_square_operator :: numberOfInputs (void) {return 5;}
char * lunar_square_operator :: inputName (int ind) {
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
double * lunar_square_operator :: inputAddress (int ind) {
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

void lunar_square_operator :: move (void) {
	if (slope != trigger) if (sync != 0.0 && trigger > 0.0) time = 0.0; slope = trigger;
	double delta = core -> TimeDelta (freq) * ratio;
	if (stage) signal = core -> MinBlep (blep_index) - 1.0;
	else signal = 1.0 - core -> MinBlep (blep_index);
	time += delta;
	blep_index += 512;
	if (stage && time > 0.5) {stage = false; blep_index = (int) ((time - 0.5) * 512.0 / delta);}
	while (time >= 1.0) {stage = true; time -= 1.0; blep_index = (int) (time * 512.0 / delta);}
	signal *= core -> Amplitude (amp);
}

lunar_square_operator :: lunar_square_operator (orbiter_core * core) : orbiter (core) {
	freq = amp = sync = trigger = slope = 0.0;
	ratio = 1.0;
	time = 0.0;
	blep_index = 0;
	stage = true;
	initialise (); activate ();
}
