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
#define _USE_MATH_DEFINES
#include <math.h>

int lunar_operator :: numberOfInputs (void) {return 5;}
char * lunar_operator :: inputName (int ind) {
	switch (ind) {
	case 0: return "FREQ"; break;
	case 1: return "AMP"; break;
	case 2: return "RATIO"; break;
	case 3: return "SHIFT"; break;
	case 4: return "SYNC"; break;
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
	default: break;
	}
	return 0;
}

void lunar_operator :: move (void) {
	if (slope != sync) if (sync > 0.0) time = 0.0; slope = sync;
	this -> signal = core -> Amplitude (amp) * core -> Sine (time + shift);
	time += core -> TimeDelta (freq);
	if (time > 1.0) time -= 1.0;
}

lunar_operator :: lunar_operator (orbiter_core * core) : orbiter (core) {
	freq = amp = ratio = shift = sync = slope = 0.0;
	time = 0.0; omega = 2.0 * M_PI;
	initialise (); activate ();
}
