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

char * moonbase :: outputName (int ind) {if (ind == 0) return "POLY"; return orbiter :: outputName (ind);}
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
bool moonbase :: insert_controller (orbiter * controller, int location) {
	if (location < 0 || location > 128) return false;
	if (controller != 0 && controller -> numberOfInputs () < 1) return false;
	if (controllers [location] != 0) controllers [location] -> release ();
	controllers [location] = controller;
	if (controller != 0) controller -> hold ();
	return true;
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
	previous_key = key;
	pthread_mutex_unlock (& critical);
}
void moonbase :: keyon (int key, int velocity) {
	if (velocity == 0) {keyoff (key, 0); return;}
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = select ();
	if (trigger != 0) trigger -> keyon (key, velocity);
	previous_key = key;
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
void moonbase :: mono (void) {mono_mode = true; signal = 0.0; keyoff (); previous_key = -1;}
void moonbase :: poly (void) {mono_mode = false; signal = 1.0; keyoff (); previous_key = -1;}
bool moonbase :: isMonoMode (void) {return mono_mode;}
void moonbase :: portamento (void) {legato_mode = false; previous_key = -1;}
void moonbase :: legato (void) {legato_mode = true; previous_key = -1;}
bool moonbase :: isLegatoMode (void) {return legato_mode;}
void moonbase :: control (int ctrl, int value) {
	if (ctrl < 0 || ctrl > 128) return;
	if (controllers [ctrl] != 0) {
		* (controllers [ctrl] -> inputAddress (0)) = (double) ((value << 7) + ctrl_lsbs [ctrl]);
		ctrl_lsbs [ctrl] = 0;
	} else if (ctrl > 31) ctrl_lsbs [ctrl - 32] = value;
	if (ctrl == 126) mono ();
	if (ctrl == 127) poly ();
}
double moonbase :: getControl (int ctrl) {
	if (ctrl < 0 || ctrl > 128) return 0.0;
	if (controllers [ctrl] != 0) return * (controllers [ctrl] -> outputAddress (0));
	if (ctrl == 126) return mono_mode ? 1.0 : 0.0;
	if (ctrl == 127) return mono_mode ? 0.0 : 1.0;
	return 0.0;
}
bool moonbase :: release (void) {
	lunar_map * map_to_delete = map;
	lunar_trigger * triggers_to_delete = triggers;
	orbiter * controllers_to_delete [129];
	for (int ind = 0; ind < 129; ind++) controllers_to_delete [ind] = controllers [ind];
	bool ret = orbiter :: release ();
	if (ret) {
		if (map_to_delete != 0) map_to_delete -> release ();
		if (triggers_to_delete != 0) triggers_to_delete -> release ();
		for (int ind = 0; ind < 129; ind++) {if (controllers_to_delete [ind] != 0) controllers_to_delete [ind] -> release ();}
	}
	return ret;
}
moonbase :: moonbase (orbiter_core * core) : orbiter (core) {
	pthread_mutex_init (& critical, 0);
	map = 0; choice = triggers = 0; mono_mode = false; legato_mode = true; signal = 1.0; previous_key = -1;
	for (int ind = 0; ind < 129; ind++) {controllers [ind] = 0; ctrl_lsbs [ind] = 0;}
	initialise ();
}

moonbase :: ~ moonbase (void) {pthread_mutex_destroy (& critical);}

