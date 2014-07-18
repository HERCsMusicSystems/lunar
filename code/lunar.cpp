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

#include "lunar.h"
#include <stdio.h>
#include <string.h>

int orbiter_count = 0;

int orbiter :: numberOfInputs (void) {return 0;}
int orbiter :: numberOfOutputs (void) {return 0;}
char * orbiter :: inputName (int ind) {return "VOID";}
char * orbiter :: outputName (int ind) {return "VOID";}
int orbiter :: inputIndex (char * name) {for (int ind = 0; ind < numberOfInputs (); ind++) {if (strcmp (name, inputName (ind)) == 0) return ind;} return -1;}
int orbiter :: outputIndex (char * name) {for (int ind = 0; ind < numberOfOutputs (); ind++) {if (strcmp (name, outputName (ind)) == 0) return ind;} return -1;}
double orbiter :: output (int ind) {return 0.0;}
double orbiter :: output (char * name) {int ind = outputIndex (name); if (ind < 0 || ind >= numberOfOutputs ()) return 0.0; return output (ind);}
void orbiter :: input (double signal, int ind) {}
void orbiter :: input (double signal, char * name) {int ind = inputIndex (name); if (ind < 0 || ind >= numberOfInputs ()) return; input (signal, ind);}
void orbiter :: move (void) {}
void orbiter :: hold (void) {references++;}
void orbiter :: release (void) {if (--references < 1) delete this;}

orbiter :: orbiter (orbiter_core * core) {
	this -> core = core;
	orbiter_count++;
	printf ("ORBITER CREATED [%i]\n", orbiter_count);
}
orbiter :: ~ orbiter (void) {
	orbiter_count--;
	printf ("ORBITER DESTROYED [%i]\n", orbiter_count);
}