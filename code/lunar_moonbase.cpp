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

CommandModule :: CommandModule (orbiter_core * core) : orbiter (core) {}

int moonbase :: numberOfOutputs (void) {return 0;}
bool moonbase :: set_map (lunar_map * map) {
	if (this -> map != 0) this -> map -> release ();
	this -> map = map;
	if (map != 0) map -> hold ();
	return true;
}
bool moonbase :: insert_trigger (lunar_trigger * trigger) {
	if (trigger == 0) return false;
	trigger -> next = triggers;
	choice = triggers = trigger;
	trigger -> hold ();
	return true;
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
		if (ret -> busy <= 0.0 && ret -> key < 0 && ret -> request < 3) {choice = ret -> next; if (choice == 0) choice = triggers; return ret;}
		ret = ret -> next; if (ret == 0) ret = triggers;
	} while (ret != choice);
	do {
		if (ret -> key < 0 && ret -> request < 3) {choice = ret -> next; if (choice == 0) choice = triggers; return ret;}
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
	if (key_counter++ == 0) base_key = key;
	pthread_mutex_unlock (& critical);
}
void moonbase :: keyon (int key, int velocity) {
	if (velocity == 0) {keyoff (key, 0); return;}
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = select ();
	if (key_counter++ == 0) base_key = key;
	if (trigger != 0) {
		if (mono_mode) trigger -> keyon (key, velocity);
		else trigger -> ground (key, velocity, base_key, previous_key >= 0 ? previous_key : key);
	}
	previous_key = key;
	pthread_mutex_unlock (& critical);
}
void moonbase :: keyoff (void) {
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = triggers;
	while (trigger != 0) {trigger -> keyoff (); trigger = trigger -> next;}
	choice = triggers;
	key_counter = 0;
	pthread_mutex_unlock (& critical);
}
void moonbase :: keyoff (int key, int velocity) {
	pthread_mutex_lock (& critical);
	lunar_trigger * trigger = select (key);
	if (trigger != 0) trigger -> keyoff ();
	key_counter--; if (key_counter < 0) key_counter = 0;
	pthread_mutex_unlock (& critical);
}
void moonbase :: mono (void) {mono_mode = true; signal = 0.0; keyoff (); base_key = 64; previous_key = -1;}
void moonbase :: poly (void) {mono_mode = false; signal = 1.0; keyoff (); base_key = 64; previous_key = -1;}
bool moonbase :: isMonoMode (void) {return mono_mode;}
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
moonbase :: moonbase (orbiter_core * core) : CommandModule (core) {
	pthread_mutex_init (& critical, 0);
	map = 0; choice = triggers = 0; mono_mode = false; signal = 1.0; base_key = 64; previous_key = -1; key_counter = 0;
	for (int ind = 0; ind < 129; ind++) {controllers [ind] = 0; ctrl_lsbs [ind] = 0;}
	initialise ();
}

moonbase :: ~ moonbase (void) {pthread_mutex_destroy (& critical);}

void up1 (arpeggiator * arp) {
	if (arp -> index >= arp -> active_key_pointer) arp -> index = 0;
	if (arp -> base != 0) arp -> base -> keyon (arp -> active_keys [arp -> index], 100);
	arp -> index++;
}

void up2 (arpeggiator * arp) {
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 1) arp -> octave = 0;
	if (arp -> base != 0) arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, 100);
	arp -> index++;
}

void up3 (arpeggiator * arp) {
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 2) arp -> octave = 0;
	if (arp -> base != 0) arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, 100);
	arp -> index++;
}

void up4 (arpeggiator * arp) {
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 3) arp -> octave = 0;
	if (arp -> base != 0) arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, 100);
	arp -> index++;
}

int arpeggiator :: numberOfInputs (void) {return 4;}
char * arpeggiator :: inputName (int ind) {
	switch (ind) {
	case 0: return "SPEED"; break;
	case 1: return "DIVISION"; break;
	case 2: return "ACTIVE"; break;
	case 3: return "ALGO"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * arpeggiator :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & tempo; break;
	case 1: return & division; break;
	case 2: return & active; break;
	case 3: return & current_algo; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int arpeggiator :: numberOfOutputs (void) {return 0;}

bool arpeggiator :: release (void) {
	moonbase * base_to_delete = base;
	bool ret = orbiter :: release ();
	if (ret && base_to_delete != 0) base_to_delete -> release ();
	return ret;
}

void arpeggiator :: signal (void) {
	if (should_keyoff && tick >= division * 0.5) {if (base != 0) base -> keyoff (); should_keyoff = false;}
	while (tick >= division) {
		tick -= division;
		if (algo != 0) algo (this);
		should_keyoff = true;
	}
	tick += 1.0;
}

void arpeggiator :: propagate_signals (void) {
	orbiter :: propagate_signals ();
	if (active == 0.0) {if (previous_activity != 0.0) {if (base != 0) base -> keyoff (); previous_activity = 0.0;} return;}
	if (active != previous_activity) {previous_activity = active; ground ();}
	if (current_algo != previous_algo) {
		switch ((int) current_algo) {
		case 0: algo = up1; break;
		case 1: algo = up2; break;
		case 2: algo = up3; break;
		case 4: algo = up4; break;
		default: algo = up1; break;
		}
		previous_algo = current_algo;
	}
	while (time >= 1.0) {time -= 1.0; signal ();}
	time += core -> sample_duration * tempo * 0.4;
}

void arpeggiator :: insert_key (int key) {
	int location = 0;
	if (active_key_pointer >= 128) return;
	while (location < active_key_pointer && active_keys [location] < key) location++;
	if (location >= 128) return;
	if (location >= active_key_pointer) {active_keys [location] = key; active_key_pointer = location + 1; return;}
	if (active_keys [location] == key) return;
	for (int ind = active_key_pointer; ind > location; ind--) active_keys [ind] = active_keys [ind - 1];
	active_keys [location] = key; active_key_pointer++;
}

void arpeggiator :: remove_key (int key) {
	int location = 0;
	while (location < active_key_pointer && active_keys [location] != key) location++;
	if (location >= active_key_pointer) return;
	active_key_pointer--;
	for (int ind = location; ind < active_key_pointer; ind++) active_keys [ind] = active_keys [ind + 1];
}

bool arpeggiator :: set_map (lunar_map * map) {return false;}
bool arpeggiator :: insert_trigger (lunar_trigger * trigger) {return false;}
bool arpeggiator :: insert_controller (orbiter * controller, int location) {return false;}
void arpeggiator :: keyon (int key) {
	if (active == 0.0 && base != 0) base -> keyon (key);
	insert_key (key);
}

void arpeggiator :: keyon (int key, int velocity) {
	if (active == 0.0 && base != 0) base -> keyon (key, velocity);
	insert_key (key);
}

void arpeggiator :: keyoff (void) {
	if (active == 0.0 && base != 0) base -> keyoff ();
	active_key_pointer = 0;
}

void arpeggiator :: keyoff (int key, int velocity) {
	if (active == 0.0 && base != 0) base -> keyoff (key, velocity);
	remove_key (key);
}

void arpeggiator :: mono (void) {
	if (base != 0) base -> mono ();
}

void arpeggiator :: poly (void) {
	if (base != 0) base -> poly ();
}

bool arpeggiator :: isMonoMode (void) {
	if (base != 0) return base -> isMonoMode ();
	return false;
}

void arpeggiator :: control (int ctrl, int value) {
	if (base != 0) base -> control (ctrl, value);
}

double arpeggiator :: getControl (int ctrl) {
	if (base != 0) return base -> getControl (ctrl);
	return 0.0;
}

void arpeggiator :: ground (void) {
	time = 1.0;
	tick = division;
	should_keyoff = false;
	index = octave = 0;
}

arpeggiator :: arpeggiator (orbiter_core * core, moonbase * base) : CommandModule (core) {
	active_key_pointer = 0;
	tempo = 140.0; division = 24.0;
	algo = up1; current_algo = previous_algo = 0.0;
	ground ();
	active = previous_activity = 0.0;
	this -> base = base; if (base != 0) base -> hold ();
	initialise (); activate ();
}

