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
void moonbase :: insert_trigger (lunar_trigger * trigger) {
	if (trigger == 0) return;
	trigger -> next = triggers;
	choice = triggers = trigger;
	trigger -> hold ();
}
lunar_trigger * moonbase :: select (void) {
	if (mono_mode) return triggers;
	if (choice == 0) return 0;
	lunar_trigger * ret = choice;
	do {
		if (ret -> busy <= 0.0) {choice = ret -> next; if (choice == 0) choice = triggers; return ret;}
		ret = ret -> next; if (ret == 0) ret = triggers;
	} while (ret != choice);
	do {
		if (ret -> key < 0) {choice = ret -> next; if (choice == 0) choice = triggers; return ret;}
		ret = ret -> next; if (ret == 0) ret = triggers;
	} while (ret != choice);
	choice = choice -> next; if (choice == 0) choice = triggers;
	return ret;
}
lunar_trigger * moonbase :: select (int key) {
	if (mono_mode) return triggers;
	if (choice == 0) return 0;
	lunar_trigger * ret = choice;
	do {
		if (ret -> key == key) return ret;
		ret = ret -> next; if (ret == 0) ret = triggers;
	} while (ret != choice);
	return 0;
}
void moonbase :: keyon (int key) {
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = select ();
	if (trigger != 0) trigger -> keyon (key);
	pthread_mutex_unlock (& critical);
}
void moonbase :: keyon (int key, int velocity) {
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = select ();
	if (trigger != 0) trigger -> keyon (key, velocity);
	pthread_mutex_unlock (& critical);
}
void moonbase :: keyoff (void) {
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = triggers;
	while (trigger != 0) {trigger -> keyoff (); trigger = trigger -> next;}
	choice = triggers;
	pthread_mutex_unlock (& critical);
}
void moonbase :: keyoff (int key, int velocity) {
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = select (key);
	if (trigger != 0) trigger -> keyoff ();
	pthread_mutex_unlock (& critical);
}
void moonbase :: mono (void) {mono_mode = true;}
void moonbase :: poly (void) {mono_mode = false;}
bool moonbase :: release (void) {
	lunar_map * map_to_delete = map;
	lunar_trigger * triggers_to_delete = triggers;
	bool ret = orbiter :: release ();
	if (ret && map_to_delete != 0) map_to_delete -> release ();
	if (ret && triggers_to_delete != 0) triggers_to_delete -> release ();
	return ret;
}
moonbase :: moonbase (orbiter_core * core) : orbiter (core) {
	pthread_mutex_init (& critical, 0);
	map = 0; choice = triggers = 0; mono_mode = false;
	initialise ();
}

moonbase :: ~ moonbase (void) {pthread_mutex_destroy (& critical);}
