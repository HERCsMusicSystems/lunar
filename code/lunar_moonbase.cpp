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

int moonbase :: numberOfOutputs (void) {return 0;}
void moonbase :: set_map (lunar_map * map) {
	if (this -> map != 0) this -> map -> release ();
	this -> map = map;
	if (map != 0) map -> hold ();
}
void moonbase :: keyon (int key) {
	if (key < 0) key = 0;
	if (key > 127) key = 127;
	double this_key = map == 0 ? (double) (key - 64) * 128.0 : map -> map [key];
	printf ("=> keyon (%f)\n", this_key);
}
void moonbase :: keyon (int key, int velocity) {
	if (velocity < 1) {keyoff (); return;}
	double this_velocity = (double) velocity * 128.0;
	printf ("=> velocity (%f)\n", this_velocity);
	keyon (key);
}
void moonbase :: keyoff (void) {printf ("=> keyoff ()\n");}
bool moonbase :: release (void) {
	lunar_map * to_delete = map;
	bool ret = orbiter :: release ();
	if (ret && to_delete != 0) to_delete -> release ();
	return ret;
}
moonbase :: moonbase (orbiter_core * core) : orbiter (core) {map = 0; initialise ();}
