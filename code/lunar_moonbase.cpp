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

CommandModule :: CommandModule (orbiter_core * core) : orbiter (core) {}

int moonbase :: numberOfOutputs (void) {return 0;}
bool moonbase :: insert_trigger (lunar_trigger * trigger) {
	if (trigger == 0) return false;
	trigger -> next = triggers;
	choice = triggers = trigger;
	trigger -> hold ();
	return true;
}
bool moonbase :: insert_controller (orbiter * controller, int location, double shift) {
	if (location < 0 || location > 128) return false;
	if (controller != 0 && controller -> numberOfInputs () < 1) return false;
	if (controllers [location] != 0) controllers [location] -> release ();
	controllers [location] = controller;
	shifts [location] = shift;
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
	if (trigger != 0) {
		if (mono_mode) trigger -> keyoff (key);
		else trigger -> keyoff ();
	}
	key_counter--; if (key_counter < 0) key_counter = 0;
	pthread_mutex_unlock (& critical);
}
void moonbase :: mono (void) {mono_mode = true; signal = 0.0; keyoff (); base_key = 64; previous_key = -1;}
void moonbase :: poly (void) {mono_mode = false; signal = 1.0; keyoff (); base_key = 64; previous_key = -1;}
bool moonbase :: isMonoMode (void) {return mono_mode;}
void moonbase :: control (int ctrl, double value) {
	if (ctrl < 0 || ctrl > 128) return;
	if (controllers [ctrl] != 0) {
		* (controllers [ctrl] -> inputAddress (0)) = (value + shifts [ctrl]) * 128.0 + ctrl_lsbs [ctrl];
		ctrl_lsbs [ctrl] = 0;
	} else if (ctrl > 31) ctrl_lsbs [ctrl - 32] = value;
	if (ctrl == 126) mono ();
	if (ctrl == 127) poly ();
}
double moonbase :: getControl (int ctrl) {
	if (ctrl < 0 || ctrl > 128) return 0.0;
	if (controllers [ctrl] != 0) return * (controllers [ctrl] -> outputAddress (0)) * 0.0078125 - (double) shifts [ctrl];
	switch (ctrl) {
	case 0: return 0.0; break;
	case 1: return 0.0; break;
	case 7: return 100.0; break;
	case 65: return 0.0; break;
	case 66: return 0.0; break;
	case 80: return 0.0; break;
	case 126: return mono_mode ? 1.0 : 0.0; break;
	case 127: return mono_mode ? 0.0 : 1.0; break;
	case 128: return 64.0; break;
	default: break;
	}
	return 64.0;
}
void moonbase :: timing_clock (void) {}
bool moonbase :: release (void) {
	lunar_trigger * triggers_to_delete = triggers;
	orbiter * controllers_to_delete [129];
	for (int ind = 0; ind < 129; ind++) controllers_to_delete [ind] = controllers [ind];
	bool ret = orbiter :: release ();
	if (ret) {
		if (triggers_to_delete != 0) triggers_to_delete -> release ();
		for (int ind = 0; ind < 129; ind++) {if (controllers_to_delete [ind] != 0) controllers_to_delete [ind] -> release ();}
	}
	return ret;
}
moonbase :: moonbase (orbiter_core * core) : CommandModule (core) {
	pthread_mutex_init (& critical, 0);
	choice = triggers = 0; mono_mode = false; signal = 1.0; base_key = 64; previous_key = -1; key_counter = 0;
	for (int ind = 0; ind < 129; ind++) {controllers [ind] = 0; ctrl_lsbs [ind] = 0; shifts [ind] = 0.0;}
	initialise ();
}

moonbase :: ~ moonbase (void) {pthread_mutex_destroy (& critical);}

void up1 (arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
	arp -> index++;
}

void up2 (arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 1) arp -> octave = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	arp -> index++;
}

void up3 (arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 2) arp -> octave = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	arp -> index++;
}

void up4 (arpeggiator * arp) {
	if (arp -> index < 0) arp -> index = 0;
	if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave++;}
	if (arp -> octave > 3) arp -> octave = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	arp -> index++;
}

void down1 (arpeggiator * arp) {
	if (--arp -> index < 0) arp -> index = arp -> active_key_pointer - 1;
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
}

void down2 (arpeggiator * arp) {
	if (--arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; if (--arp -> octave < 0) arp -> octave = 1;}
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
}

void down3 (arpeggiator * arp) {
	if (--arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; if (--arp -> octave < 0) arp -> octave = 2;}
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
}

void down4 (arpeggiator * arp) {
	if (--arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; if (--arp -> octave < 0) arp -> octave = 3;}
	if (arp -> index < 0) arp -> index = 0;
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
}

void updown1 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {arp -> index = 1; arp -> up_direction = true;}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> up_direction = false;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void updown2 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 1) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 0;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 1; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void updown3 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 2) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 1;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 1; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void updown4 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 3) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 2; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 2;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 1; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void updowndup1 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index], arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void updowndup2 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 1) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 0;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 0; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void updowndup3 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 2) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 1;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 0; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void updowndup4 (arpeggiator * arp) {
	if (arp -> up_direction) {
		if (arp -> index >= arp -> active_key_pointer) {
			if (arp -> octave < 3) {arp -> index = 0; arp -> octave++;}
			else {
				arp -> index = arp -> active_key_pointer - 1; arp -> up_direction = false;
				if (arp -> index < 0) {arp -> index = arp -> active_key_pointer - 1; arp -> octave = 2;}
			}
		}
		if (arp -> index < 0) {arp -> index = 0; arp -> up_direction = true;}
	} else {
		if (arp -> index < 0) {
			if (arp -> octave > 0) {
				arp -> index = arp -> active_key_pointer - 1;
				arp -> octave--;
				if (arp -> index < 0) arp -> index = 0;
			} else {arp -> index = 0; arp -> up_direction = true;}
		}
		if (arp -> index >= arp -> active_key_pointer) {arp -> index = 0; arp -> octave = 1;}
	}
	arp -> base -> keyon (arp -> active_keys [arp -> index] + 12 * arp -> octave, arp -> active_velocities [arp -> index]);
	if (arp -> up_direction) arp -> index++; else arp -> index--;
}

void randomdup1 (arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> base -> keyon (arp -> active_keys [key], arp -> active_velocities [key]);
}

void randomdup2 (arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int octave = (arp -> index * 2) >> 24;
	arp -> base -> keyon (arp -> active_keys [key] + 12 * octave, arp -> active_velocities [key]);
}

void randomdup3 (arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int octave = (arp -> index * 3) >> 24;
	arp -> base -> keyon (arp -> active_keys [key] + 12 * octave, arp -> active_velocities [key]);
}

void randomdup4 (arpeggiator * arp) {
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int key = (arp -> index * arp -> active_key_pointer) >> 24;
	arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
	int octave = (arp -> index * 4) >> 24;
	arp -> base -> keyon (arp -> active_keys [key] + 12 * octave, arp -> active_velocities [key]);
}

void random1 (arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index];
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}

void random2 (arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		int octave = (arp -> index * 2) >> 24;
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index] + 12 * octave;
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}

void random3 (arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		int octave = (arp -> index * 3) >> 24;
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index] + 12 * octave;
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}

void random4 (arpeggiator * arp) {
	int key = arp -> octave;
	int index = 0;
	while (key == arp -> octave) {
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		int octave = (arp -> index * 4) >> 24;
		arp -> index = (arp -> index * 0x5599d1 + 1) & 0xffffff;
		index = (arp -> index * arp -> active_key_pointer) >> 24;
		key = arp -> active_keys [index] + 12 * octave;
	}
	arp -> base -> keyon (key, arp -> active_velocities [index]);
	arp -> octave = key;
}

int arpeggiator :: numberOfInputs (void) {return 5;}
char * arpeggiator :: inputName (int ind) {
	switch (ind) {
	case 0: return "SPEED"; break;
	case 1: return "DIVISION"; break;
	case 2: return "ACTIVE"; break;
	case 3: return "ALGO"; break;
	case 4: return "HOLD"; break;
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
	case 4: return & hold; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int arpeggiator :: numberOfOutputs (void) {return 0;}

bool arpeggiator :: release (void) {
	CommandModule * base_to_delete = base;
	bool ret = orbiter :: release ();
	if (ret && base_to_delete != 0) base_to_delete -> release ();
	return ret;
}

void arpeggiator :: private_signal (void) {
	if (base == 0) return;
	if (number_of_keys < 1 && active_key_pointer > 0 && hold == 0.0) active_key_pointer = 0;
	if (active_key_pointer < 1 && ! should_keyoff) return;
	double div = division < 1.0 ? 1.0 : division;
	if (should_keyoff && tick >= div * 0.5) {base -> keyoff (); should_keyoff = false;}
	while (tick >= div) {
		tick -= div;
		if (algo != 0) algo (this);
		should_keyoff = true;
	}
	tick += 1.0;
}

void arpeggiator :: timing_clock (void) {
	pthread_mutex_lock (& critical);
	private_signal ();
	pthread_mutex_unlock (& critical);
}

void arpeggiator :: propagate_signals (void) {
	orbiter :: propagate_signals ();
	if (active == 0.0) {if (previous_activity != 0.0) {if (base != 0) base -> keyoff (); previous_activity = 0.0;} return;}
	if (active != previous_activity) previous_activity = active;
	if (current_algo != previous_algo) {
		switch ((int) current_algo) {
		case 0: algo = up1; break;
		case 1: algo = up2; break;
		case 2: algo = up3; break;
		case 3: algo = up4; break;
		case 4: algo = down1; break;
		case 5: algo = down2; break;
		case 6: algo = down3; break;
		case 7: algo = down4; break;
		case 8: algo = updown1; break;
		case 9: algo = updown2; break;
		case 10: algo = updown3; break;
		case 11: algo = updown4; break;
		case 12: algo = updowndup1; break;
		case 13: algo = updowndup2; break;
		case 14: algo = updowndup3; break;
		case 15: algo = updowndup4; break;
		case 16: algo = randomdup1; index = core -> noise24b; break;
		case 17: algo = randomdup2; index = core -> noise24b; break;
		case 18: algo = randomdup3; index = core -> noise24b; break;
		case 19: algo = randomdup4; index = core -> noise24b; break;
		case 20: algo = random1; index = core -> noise24b; break;
		case 21: algo = random2; index = core -> noise24b; break;
		case 22: algo = random3; index = core -> noise24b; break;
		case 23: algo = random4; index = core -> noise24b; break;
		default: algo = up1; break;
		}
		previous_algo = current_algo;
	}
	while (time >= 1.0) {time -= 1.0; pthread_mutex_lock (& critical); private_signal (); pthread_mutex_unlock (& critical);}
	time += core -> sample_duration * tempo * 0.4;
}

void arpeggiator :: insert_key (int key, int velocity) {
	if (number_of_keys < 1 && active_key_pointer > 0) number_of_keys = active_key_pointer = 0;
	int location = 0;
	if (active_key_pointer >= 128) return;
	while (location < active_key_pointer && active_keys [location] < key) location++;
	if (location >= 128) return;
	if (location >= active_key_pointer) {
		active_keys [location] = key; active_velocities [location] = velocity; active_key_pointer = location + 1;
		if (location == 0) {number_of_keys = 1; if (hold == 0.0) ground ();}
		else number_of_keys++;
		return;
	}
	if (active_keys [location] == key) {
		active_key_pointer--;
		for (int ind = location; ind < active_key_pointer; ind++) {
			active_keys [ind] = active_keys [ind + 1];
			active_velocities [ind] = active_velocities [ind + 1];
		}
		if (location <= index) index--;
		number_of_keys--;
		return;
	}
	for (int ind = active_key_pointer; ind > location; ind--) {
		active_keys [ind] = active_keys [ind - 1];
		active_velocities [ind] = active_velocities [ind - 1];
	}
	active_keys [location] = key; active_velocities [location] = velocity; active_key_pointer++; number_of_keys++;
	if (location < index) index++;
}

void arpeggiator :: remove_key (int key) {
	if (hold != 0.0) {if (number_of_keys > 0) number_of_keys--; return;}
	int location = 0;
	while (location < active_key_pointer && active_keys [location] != key) location++;
	if (location >= active_key_pointer) return;
	active_key_pointer--; number_of_keys--;
	for (int ind = location; ind < active_key_pointer; ind++) {
		active_keys [ind] = active_keys [ind + 1];
		active_velocities [ind] = active_velocities [ind + 1];
	}
	if (location <= index) index--;
}

bool arpeggiator :: insert_trigger (lunar_trigger * trigger) {
	if (base != 0) return base -> insert_trigger (trigger);
	return false;
}
bool arpeggiator :: insert_controller (orbiter * controller, int location, double shift) {
	if (base != 0) return base -> insert_controller (controller, location, shift);
	return false;
}
void arpeggiator :: keyon (int key) {
	if (active == 0.0 && base != 0) base -> keyon (key);
	pthread_mutex_lock (& critical);
	insert_key (key, 100);
	pthread_mutex_unlock (& critical);
}

void arpeggiator :: keyon (int key, int velocity) {
	if (velocity < 1) {keyoff (key, 0); return;}
	if (active == 0.0 && base != 0) base -> keyon (key, velocity);
	pthread_mutex_lock (& critical);
	insert_key (key, velocity);
	pthread_mutex_unlock (& critical);
}

void arpeggiator :: keyoff (void) {
	if (active == 0.0 && base != 0) base -> keyoff ();
	pthread_mutex_lock (& critical);
	number_of_keys = 0;
	if (hold == 0.0) active_key_pointer = 0;
	pthread_mutex_unlock (& critical);
}

void arpeggiator :: keyoff (int key, int velocity) {
	if (active == 0.0 && base != 0) base -> keyoff (key, velocity);
	pthread_mutex_lock (& critical);
	remove_key (key);
	pthread_mutex_unlock (& critical);
}

void arpeggiator :: mono (void) {if (base != 0) base -> mono ();}
void arpeggiator :: poly (void) {if (base != 0) base -> poly ();}

bool arpeggiator :: isMonoMode (void) {
	if (base != 0) return base -> isMonoMode ();
	return false;
}

void arpeggiator :: control (int ctrl, double value) {if (base != 0) base -> control (ctrl, value);}

double arpeggiator :: getControl (int ctrl) {
	if (base != 0) return base -> getControl (ctrl);
	return 0.0;
}

void arpeggiator :: ground (void) {
	time = 1.0;
	tick = division;
	should_keyoff = false;
	index = octave = 0;
	up_direction = true;
}

arpeggiator :: arpeggiator (orbiter_core * core, CommandModule * base) : CommandModule (core) {
	pthread_mutex_init (& critical, 0);
	active_key_pointer = number_of_keys = 0;
	tempo = 140.0; division = 24.0;
	algo = up1; current_algo = previous_algo = 0.0;
	ground ();
	active = previous_activity = 0.0;
	hold = 0.0;
	this -> base = base; if (base != 0) base -> hold ();
	initialise (); activate ();
}

arpeggiator :: ~ arpeggiator (void) {pthread_mutex_destroy (& critical);}

///////////////
// SEQUENCER //
///////////////

sequence_element :: sequence_element (int type, int key, double velocity) {
	this -> type = type;
	this -> key = key;
	this -> velocity = velocity;
	this -> next = 0;
}

sequence_element :: ~ sequence_element (void) {if (next != 0) delete next;}

int sequencer :: numberOfInputs (void) {return 2;}
char * sequencer :: inputName (int ind) {
	switch (ind) {
	case 0: return "SPEED"; break;
	case 1: return "TRIGGER"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * sequencer :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & tempo; break;
	case 1: return & trigger; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
char * sequencer :: outputName (int ind) {if (ind == 0) return "BUSY"; return orbiter :: outputName (ind);}
bool sequencer :: release (void) {
	CommandModule * base_to_delete = base;
	bool ret = orbiter :: release ();
	if (ret && base_to_delete != 0) base_to_delete -> release ();
	return ret;
}

void sequencer :: propagate_signals (void) {
	orbiter :: propagate_signals ();
	if (trigger < 1.0) {
		if (time < 0.0) return;
		time = -1.0;
		if (base != 0) base -> keyoff ();
		return;
	}
	if (time < 0.0) {time = 1.0; tick = 0; current_frame = elements;}
	while (time >= 1.0) {time -= 1.0; pthread_mutex_lock (& critical); private_signal (); pthread_mutex_unlock (& critical);}
	time += core -> sample_duration * tempo * 0.4;
}

void sequencer :: private_signal (void) {
	if (current_frame == 0 || base == 0) {impulse_level = busy_level = 0.0; return;}
	if (current_frame == elements) {impulse_level = 0.0; busy_level = 1.0;}
	while (tick < 1) {
		switch (current_frame -> type) {
		case 0: tick = current_frame -> key; break;
		case 1: base -> keyon (current_frame -> key); break;
		case 2: base -> keyon (current_frame -> key, (int) current_frame -> velocity); break;
		case 3: base -> keyoff (); break;
		case 4: base -> keyoff (current_frame -> key); break;
		case 5: base -> control (current_frame -> key, current_frame -> velocity); break;
		case 8: impulse_level = busy_level = current_frame -> velocity; break;
		case 9: impulse_level = current_frame -> velocity; break;
		default: break;
		}
		current_frame = current_frame -> next;
		if (current_frame == 0) {
			if (trigger > 1.0) current_frame = elements;
			else return;
		}
	}
	tick--;
}

void sequencer :: move (void) {signal = impulse_level; impulse_level = busy_level;}

bool sequencer :: insert_trigger (lunar_trigger * trigger) {
	if (base != 0) return base -> insert_trigger (trigger);
	return false;
}

bool sequencer :: insert_controller (orbiter * controller, int location, double shift) {
	if (base != 0) return base -> insert_controller (controller, location, shift);
	return false;
}

void sequencer :: keyon (int key) {}
void sequencer :: keyon (int key, int velocity) {}
void sequencer :: keyoff (void) {}
void sequencer :: keyoff (int key, int velocity) {}
void sequencer :: mono (void) {if (base != 0) base -> mono ();}
void sequencer :: poly (void) {if (base != 0) base -> poly ();}
bool sequencer :: isMonoMode (void) {
	if (base != 0) return base -> isMonoMode ();
	return false;
}
void sequencer :: control (int ctrl, double value) {if (base != 0) base -> control (ctrl, value);}
double sequencer :: getControl (int ctrl) {
	if (base != 0) return base -> getControl (ctrl);
	return 0.0;
}

void sequencer :: timing_clock (void) {
	pthread_mutex_lock (& critical);
	private_signal ();
	pthread_mutex_unlock (& critical);
}

sequencer :: sequencer (orbiter_core * core, CommandModule * base) : CommandModule (core) {
	impulse_level = busy_level = 0.0;
	pthread_mutex_init (& critical, 0);
	tempo = 140.0;
	trigger = 0.0; time = -1.0;
	tick = 0;
	this -> base = base; if (base != 0) base -> hold ();
	elements = current_frame = 0;
	initialise (); activate ();
}

sequencer :: ~ sequencer (void) {pthread_mutex_destroy (& critical); if (elements != 0) delete elements;}

///////////////////
// POLYSEQUENCER //
///////////////////

polysequence_element :: polysequence_element (int type, int channel, int key, double velocity) {
	this -> type = type;
	this -> channel = channel;
	this -> key = key;
	this -> velocity = velocity;
	this -> next = 0;
}

polysequence_element :: ~ polysequence_element (void) {if (next != 0) delete next;}

int polysequencer :: numberOfInputs (void) {return 2;}
char * polysequencer :: inputName (int ind) {
	switch (ind) {
	case 0: return "SPEED"; break;
	case 1: return "TRIGGER"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * polysequencer :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & tempo; break;
	case 1: return & trigger; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
char * polysequencer :: outputName (int ind) {if (ind == 0) return "BUSY"; return orbiter :: outputName (ind);}
bool polysequencer :: release (void) {
	CommandModulePointer * bases_to_delete = bases;
	int number_of_bases_to_delete = base_pointer;
	bool ret = orbiter :: release ();
	if (ret && bases_to_delete != 0) {
		for (int ind = 0; ind < number_of_bases_to_delete; ind++) bases_to_delete [ind] -> release ();
		delete [] bases_to_delete;
	}
	return ret;
}

void polysequencer :: propagate_signals (void) {
	orbiter :: propagate_signals ();
	if (trigger < 1.0) {
		if (time < 0.0) return;
		time = -1.0;
		for (int ind = 0; ind < base_pointer; ind++) bases [ind] -> keyoff ();
		return;
	}
	if (time < 0.0) {time = 1.0; tick = 0; current_frame = elements;}
	while (time >= 1.0) {time -= 1.0; pthread_mutex_lock (& critical); private_signal (); pthread_mutex_unlock (& critical);}
	time += core -> sample_duration * tempo * 0.4;
}

void polysequencer :: private_signal (void) {
	if (current_frame == 0) {impulse_level = busy_level = 0.0; return;}
	if (current_frame == elements) {impulse_level = 0.0; busy_level = 1.0;}
	while (tick < 1) {
		switch (current_frame -> type) {
		case 0: tick = current_frame -> key; break;
		case 1:
			if (current_frame -> channel >= 0 && current_frame -> channel < base_pointer)
				bases [current_frame -> channel] -> keyon (current_frame -> key);
			break;
		case 2:
			if (current_frame -> channel >= 0 && current_frame -> channel < base_pointer)
				bases [current_frame -> channel] -> keyon (current_frame -> key, (int) current_frame -> velocity);
			break;
		case 3: if (current_frame -> channel >= 0 && current_frame -> channel < base_pointer) bases [current_frame -> channel] -> keyoff (); break;
		case 4:
			if (current_frame -> channel >= 0 && current_frame -> channel < base_pointer) bases [current_frame -> channel] -> keyoff (current_frame -> key);
			break;
		case 5:
			if (current_frame -> channel >= 0 && current_frame -> channel < base_pointer)
				bases [current_frame -> channel] -> control (current_frame -> key, current_frame -> velocity);
			break;
		case 6: for (int ind = 0; ind < base_pointer; ind++) bases [ind] -> keyoff (); break;
		case 8: impulse_level = busy_level = current_frame -> velocity; break;
		case 9: impulse_level = current_frame -> velocity; break;
		default: break;
		}
		current_frame = current_frame -> next;
		if (current_frame == 0) {
			if (trigger > 1.0) current_frame = elements;
			else return;
		}
	}
	tick--;
}

void polysequencer :: move (void) {signal = impulse_level; impulse_level = busy_level;}

bool polysequencer :: insert_trigger (lunar_trigger * trigger) {return false;}
bool polysequencer :: insert_controller (orbiter * controller, int location, double shift) {return false;}

void polysequencer :: keyon (int key) {}
void polysequencer :: keyon (int key, int velocity) {}
void polysequencer :: keyoff (void) {}
void polysequencer :: keyoff (int key, int velocity) {}
void polysequencer :: mono (void) {}
void polysequencer :: poly (void) {}
bool polysequencer :: isMonoMode (void) {return false;}
void polysequencer :: control (int ctrl, double value) {}
double polysequencer :: getControl (int ctrl) {return 0.0;}

void polysequencer :: timing_clock (void) {
	pthread_mutex_lock (& critical);
	private_signal ();
	pthread_mutex_unlock (& critical);
}

void polysequencer :: add_base (CommandModule * module) {
	if (base_pointer >= number_of_bases) return;
	bases [base_pointer++] = module;
	module -> hold ();
}
int polysequencer :: numberOfBases (void) {return base_pointer;}

polysequencer :: polysequencer (orbiter_core * core, int number_of_bases) : CommandModule (core) {
	impulse_level = busy_level = 0.0;
	if (number_of_bases < 1) number_of_bases = 1;
	this -> number_of_bases = number_of_bases;
	base_pointer = 0;
	pthread_mutex_init (& critical, 0);
	tempo = 140.0;
	trigger = 0.0; time = -1.0;
	tick = 0;
	bases = new CommandModulePointer [number_of_bases];
	elements = current_frame = 0;
	initialise (); activate ();
}

polysequencer :: ~ polysequencer (void) {pthread_mutex_destroy (& critical); if (elements != 0) delete elements;}

