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

int moonbase :: numberOfInputs (void) {return 3;}
char * moonbase :: inputName (int ind) {
	switch (ind) {
	case 0: return "MONO"; break;
	case 1: return "LEFT"; break;
	case 2: return "RIGHT"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * moonbase :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & mono; break;
	case 1: return & left; break;
	case 2: return & right; break;
	default: break;
	}
	return 0;
}
int moonbase :: numberOfOutputs (void) {return numberOfInputs ();}
char * moonbase :: outputName (int ind) {return inputName (ind);}
double * moonbase :: outputAddress (int ind) {return inputAddress (ind);}

moonbase :: moonbase (orbiter_core * core) : orbiter (core) {mono = left = right = 0.0; initialise (); activate ();}

