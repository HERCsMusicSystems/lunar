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
// This file was created on Tuesday, 29th July 2014, 13:48:00 PM. //
////////////////////////////////////////////////////////////////////

#include "lunar_landers.h"

int lunar_parameter_block :: numberOfInputs (void) {return 1;}
char * lunar_parameter_block :: inputName (int ind) {return ind == 0 ? "SIGNAL": orbiter :: inputName (ind);}
double * lunar_parameter_block :: inputAddress (int ind) {return ind == 0 ? & enter : 0;}
lunar_parameter_block :: lunar_parameter_block (orbiter_core * core, double maximum_change) : orbiter (core) {
	if (maximum_change < 0.0) maximum_change = 0.0;
	this -> maximum_change = maximum_change * 48000.0 / core -> sampling_frequency;
}
void lunar_parameter_block :: move (void) {
	if (maximum_change == 0.0) {signal = enter; return;}
	if (enter == signal) return;
	if (enter > signal) {signal += maximum_change; if (signal > enter) signal = enter;}
	signal -= maximum_change;
	if (signal < enter) signal = enter;
}
