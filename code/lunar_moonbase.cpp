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

///////////////////////////////////////////////////////////////
// This file was created on Friday, 18th July at 8:45:09 AM. //
///////////////////////////////////////////////////////////////

#include "lunar_moonbase.h"
#include <stdio.h>

int moonbase :: numberOfInputs (void) {return 6;}
char * moonbase :: inputName (int ind) {
	switch (ind) {
	case 0: return "MONO"; break;
	case 1: return "LEFT"; break;
	case 2: return "RIGHT"; break;
	case 3: return "MIC"; break;
	case 4: return "MIC_LEFT"; break;
	case 5: return "MIC_RIGHT"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * moonbase :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & mono_input; break;
	case 1: return & left_input; break;
	case 2: return & right_input; break;
	case 3: return & mic_mono_input; break;
	case 4: return & mic_left_input; break;
	case 5: return & mic_right_input; break;
	default: break;
	}
	return 0;
}
int moonbase :: numberOfOutputs (void) {return numberOfInputs ();}
char * moonbase :: outputName (int ind) {return inputName (ind);}
double * moonbase :: outputAddress (int ind) {
	switch (ind) {
	case 0: return & mono_output; break;
	case 1: return & left_output; break;
	case 2: return & right_output; break;
	case 3: return & mic_mono_output; break;
	case 4: return & mic_left_output; break;
	case 5: return & mic_right_output; break;
	default: break;
	}
	return 0;
}
void moonbase :: move (void) {
	mono_output = mono_input; left_output = left_input; right_output = right_input;
	mic_mono_output = mic_mono_input; mic_left_output = mic_left_input; mic_right_output = mic_right_input;
}

moonbase :: moonbase (orbiter_core * core) : orbiter (core) {
	mono_input = left_input = right_input = 0.0;
	mono_output = left_output = right_output = 0.0;
	mic_mono_input = mic_left_input = mic_right_input = 0.0;
	mic_mono_output = mic_left_output = mic_right_output = 0.0;
	initialise (); activate ();
}

