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
// This file was created on Tuesday, 29th July 2014, 13:56:00 PM. //
////////////////////////////////////////////////////////////////////

#include "lunar_prolog_landers.h"
#include "lunar_landers.h"
#include "lunar_moonbase.h"
#include "lunar_wave.h"
#include "string.h"
#include <math.h>

#ifdef WIN32
#define strcasecmp _strcmpi
#endif

class pb_native_orbiter : public PrologNativeOrbiter {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		lunar_parameter_block * pb = (lunar_parameter_block *) module;
		if (pb -> style != 0 && parameters -> isVar ()) {
			char command [128];
			switch (pb -> style) {
			case 1: sprintf (command, "%i", (int) pb -> signal); break;
			case 2: sprintf (command, "%i  (%.2f Hz)", (int) pb -> signal, core -> centre_frequency * pow (2.0, pb -> signal / 1536.0)); break;
			case 3:
				if (pb -> signal <= -16383.0) sprintf (command, "%i (ZERO)", (int) pb -> signal);
				else sprintf (command, "%i  (%.2f Db)", (int) pb -> signal, 10.0 * log10 (core -> Amplitude (pb -> signal)));
				break;
			case 4: sprintf (command, "%i  (%.2f sec.)", (int) pb -> signal, pb -> signal <= 0.0 ? 0.0 : pow (2.0, (pb -> signal - 8192.0) / 768.0)); break;
			case 5: if (pb -> signal == 0) sprintf (command, "off"); else sprintf (command, "on"); break;
			case 6:
				switch ((int) pb -> signal) {
				case 0: sprintf (command, "0 (sine)"); break;
				case 1: sprintf (command, "1 (triangle)"); break;
				case 2: sprintf (command, "2 (square)"); break;
				case 3: sprintf (command, "3 (random)"); break;
				case 4: sprintf (command, "4 (S/H)"); break;
				default: sprintf (command, "%i", (int) pb -> signal); break;
				}
				break;
			case 7: sprintf (command, "* %g", pb -> signal); break;
			default: sprintf (command, "??"); break;
			}
			parameters -> setText (command);
			return true;
		}
		return PrologNativeOrbiter :: code (parameters, resolution);
	}
	pb_native_orbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {}
};

static int toStyle (char * name) {
	if (name == 0) return 0;
	if (strcasecmp (name, "index") == 0) return 1;
	if (strcasecmp (name, "freq") == 0) return 2;
	if (strcasecmp (name, "amp") == 0) return 3;
	if (strcasecmp (name, "time") == 0) return 4;
	if (strcasecmp (name, "onoff") == 0) return 5;
	if (strcasecmp (name, "wave") == 0) return 6;
	if (strcasecmp (name, "ratio") == 0) return 7;
	return 0;
}

orbiter * parameter_block_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * activity = 0;
	PrologElement * style_selector = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) activity = el;
		if (el -> isAtom () || el -> isText ()) style_selector = el;
		parameters = parameters -> getRight ();
	}
	char * style_name = 0;
	if (style_selector != 0) {
		if (style_selector -> isText ()) style_name = style_selector -> getText ();
		if (style_selector -> isAtom ()) style_name = style_selector -> getAtom () -> name ();
	}
	return new lunar_parameter_block (core, toStyle (style_name), activity != 0, activity != 0 ? activity -> getNumber () : 0.0);
}
PrologNativeOrbiter * parameter_block_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new pb_native_orbiter (atom, core, module);}
parameter_block_class :: parameter_block_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

static char * auto_data_action_code = "Lunar Auto Data Action";
class auto_data_native_orbiter : public PrologNativeOrbiter {
public:
	static char * name (void) {return auto_data_action_code;}
	bool isTypeOf (char * code_name) {return auto_data_action_code == code_name ? true : PrologNativeOrbiter :: isTypeOf (code_name);}
	char * codeName (void) {return name ();}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		auto_data * ad = (auto_data *) module;
		if (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isEarth ()) {ad -> clear_frames (); return true;}
			if (el -> isPair ()) {
				ad -> clear_frames ();
				while (el -> isPair ()) {
					PrologElement * value = el -> getLeft (); if (! value -> isNumber ()) return false;
					el = el -> getRight (); if (! el -> isPair ()) return false;
					PrologElement * time = el -> getLeft (); if (! time -> isNumber ()) return false;
					ad -> insert_frame (value -> getNumber (), time -> getNumber ());
					parameters = parameters -> getRight (); if (! parameters -> isPair ()) return true;
					el = parameters -> getLeft ();
				}
				return false;
			}
			if (el -> isVar ()) {
				el -> setEarth ();
				parameters = el;
				auto_frame * af = ad -> frames;
				while (af != 0) {
					parameters -> setPair ();
					PrologElement * el = parameters -> getLeft ();
					el -> setPair (); el -> getLeft () -> setDouble (af -> value); el = el -> getRight ();
					el -> setPair (); el -> getLeft () -> setDouble (af -> time);
					af = af -> next; parameters = parameters -> getRight ();
				}
				return true;
			}
		}
		return PrologNativeOrbiter :: code (parameters, resolution);
	}
	auto_data_native_orbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {}
};
orbiter * auto_data_class :: create_orbiter (PrologElement * parameters) {return new auto_data (core);}
PrologNativeOrbiter * auto_data_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new auto_data_native_orbiter (atom, core, module);}
auto_data_class :: auto_data_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * auto_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * activity = 0;
	PrologElement * data = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) activity = el;
		if (el -> isAtom ()) data = el;
		parameters = parameters -> getRight ();
	}
	if (data == 0) return 0;
	if (! data -> isAtom ()) return 0;
	PrologNativeCode * machine = data -> getAtom () -> getMachine ();
	if (machine == 0) return 0;
	if (! machine -> isTypeOf (auto_data_native_orbiter :: name ())) return 0;
	return new auto_player (core, ((auto_data *) ((auto_data_native_orbiter *) machine) -> module), activity != 0 ? activity -> getNumber () : 0.0);
}
auto_class :: auto_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

static char * key_orbiter_action_code = "Lunar Map Action";
char * PrologNativeKeyOrbiter :: name (void) {return key_orbiter_action_code;}
bool PrologNativeKeyOrbiter :: isTypeOf (char * code_name) {return key_orbiter_action_code == code_name ? true : PrologNativeOrbiter :: isTypeOf (code_name);}
PrologNativeKeyOrbiter :: PrologNativeKeyOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {}

class key_map_native_orbiter : public PrologNativeKeyOrbiter {
public:
	void return_content (PrologElement * parameters, lunar_map * map) {
		for (int ind = 0; ind < 128; ind++) {
			parameters -> setPair ();
			parameters -> getLeft () -> setDouble (map -> map [ind]);
			parameters = parameters -> getRight ();
		}
	}
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return onEarth ();
		lunar_map * map = (lunar_map *) module;
		if (parameters -> isVar ()) {return_content (parameters, map); return true;}
		int double_count = 0;
		PrologElement * doubles [128];
		PrologElement * ret = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isNumber ()) if (double_count < 128) doubles [double_count++] = el;
			if (el -> isVar ()) ret = el;
			if (el -> isPair ()) {
				PrologElement * subel = el;
				while (subel -> isPair ()) {
					PrologElement * ell = subel -> getLeft ();
					if (ell -> isNumber ()) if (double_count < 128) doubles [double_count++] = ell;
					subel = subel -> getRight ();
				}
			}
			parameters = parameters -> getRight ();
		}
		if (parameters -> isVar ()) ret = parameters;
		if (ret != 0) {
			if (double_count < 1) {return_content (ret, map); return true;}
			if (! doubles [0] -> isInteger ()) return false;
			int index = doubles [0] -> getInteger ();
			if (index < 0 || index > 127) return false;
			ret -> setDouble (map -> map [index]);
			return true;
		}
		if (double_count == 2) {
			if (! doubles [0] -> isInteger ()) return false;
			int index = doubles [0] -> getInteger ();
			if (index < 0 || index > 127) return false;
			map -> map [index] = doubles [1] -> getNumber ();
			return true;
		}
		if (double_count == 128) {for (int ind = 0; ind < 128; ind++) map -> map [ind] = doubles [ind] -> getNumber (); return true;}
		return false;
	}
	key_map_native_orbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeKeyOrbiter (atom, core, module) {}
};

orbiter * key_map_class :: create_orbiter (PrologElement * parameters) {return new lunar_map (core, initial);}
PrologNativeOrbiter * key_map_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new key_map_native_orbiter (atom, core, module);}
key_map_class :: key_map_class (orbiter_core * core, int initial) : PrologNativeOrbiterCreator (core) {this -> initial = initial;}

static char * wave_orbiter_action_code = "Lunar Wave Action";
char * PrologNativeWaveOrbiter :: name (void) {return wave_orbiter_action_code;}
bool PrologNativeWaveOrbiter :: isTypeOf (char * code_name) {return wave_orbiter_action_code == code_name ? true : PrologNativeOrbiter :: isTypeOf (code_name);}
PrologNativeWaveOrbiter :: PrologNativeWaveOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {}

class wave_native_orbiter : public PrologNativeWaveOrbiter {
public:
	wave_native_orbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeWaveOrbiter (atom, core, module) {}
};

orbiter * wave_class :: create_orbiter (PrologElement * parameters) {
	int capacity = 0;
	PrologElement * counter = parameters;
	while (counter -> isPair ()) {
		if (counter -> getLeft () -> isText ()) capacity++;
		counter = counter -> getRight ();
	}
	lunar_wave * wave = new lunar_wave (core, capacity);
	capacity = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isText ()) wave -> waves [capacity++] = create_lunar_wave_data (el -> getText ());
		parameters = parameters -> getRight ();
	}
	return wave;
}
PrologNativeOrbiter * wave_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new wave_native_orbiter (atom, core, module);}
wave_class :: wave_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

bool LoopWaveClass :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * source = 0;
	PrologElement * destination = 0;
	PrologElement * start = 0;
	PrologElement * stop = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isText ()) if (source == 0) source = el; else destination = el;
		if (el -> isInteger ()) if (start == 0) start = el; else stop = el;
		parameters = parameters -> getRight ();
	}
	if (source == 0 || destination == 0 || start == 0 || stop == 0) return false;
	return loop_wave (source -> getText (), destination -> getText (), start -> getInteger (), stop -> getInteger ());
}

orbiter * impulse_class :: create_orbiter (PrologElement * parameters) {return new lunar_impulse (core);}
impulse_class :: impulse_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

static char * trigger_orbiter_action_code = "Lunar Trigger Action";
class trigger_native_orbiter : public PrologNativeOrbiter {
private:
	PrologAtom * keyon, * keyoff;
public:
	static char * name (void) {return trigger_orbiter_action_code;}
	virtual bool isTypeOf (char * code_name) {return trigger_orbiter_action_code == code_name ? true : PrologNativeOrbiter :: isTypeOf (code_name);}
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * key_atom = 0;
		PrologElement * velocity_atom = 0;
		PrologElement * key = 0;
		PrologElement * velocity = 0;
		PrologElement * pp = parameters;
		while (pp -> isPair ()) {
			PrologElement * el = pp -> getLeft ();
			if (el -> isAtom ()) if (key_atom == 0) key_atom = el; else velocity_atom = el;
			if (el -> isEarth ()) if (key_atom == 0) key_atom = el; else velocity_atom = el;
			if (el -> isInteger ()) if (key == 0) key = el; else velocity = el;
			pp = pp -> getRight ();
		}
		lunar_trigger * trigger = (lunar_trigger *) module;
		if (velocity_atom != 0) {
			if (velocity_atom -> isEarth ()) trigger -> set_velocity_map (0);
			if (velocity_atom -> isAtom ()) {
				PrologAtom * a = velocity_atom -> getAtom ();
				PrologNativeCode * machine = a -> getMachine ();
				if (machine == 0) return false;
				if (! machine -> isTypeOf (key_map_native_orbiter :: name ())) return false;
				trigger -> set_velocity_map ((lunar_map *) ((key_map_native_orbiter *) machine) -> module);
			}
		}
		if (key_atom != 0) {
			if (key_atom -> isEarth ()) {trigger -> set_key_map (0); return true;}
			if (key_atom -> isAtom ()) {
				PrologAtom * a = key_atom -> getAtom ();
				if (a == keyon) {
					if (key == 0) return false;
					if (velocity == 0) trigger -> keyon (key -> getInteger ());
					else trigger -> keyon (key -> getInteger (), velocity -> getInteger ());
					return true;
				}
				if (a == keyoff) {
					if (key == 0) trigger -> keyoff ();
					else trigger -> keyoff (key -> getInteger ());
					return true;
				}
				PrologNativeCode * machine = a -> getMachine ();
				if (machine == 0) return false;
				if (machine -> isTypeOf (key_map_native_orbiter :: name ())) {
					trigger -> set_key_map ((lunar_map *) ((key_map_native_orbiter *) machine) -> module);
					return true;
				}
			}
		}
		return PrologNativeOrbiter :: code (parameters, resolution);
	}
	trigger_native_orbiter (PrologDirectory * dir, PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {
		keyon = keyoff = 0;
		if (dir == 0) return;
		keyon = dir -> searchAtom ("keyon");
		keyoff = dir -> searchAtom ("keyoff");
	}
};
orbiter * trigger_class :: create_orbiter (PrologElement * parameters) {return new lunar_trigger (core, active, 0);}
PrologNativeOrbiter * trigger_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new trigger_native_orbiter (dir, atom, core, module);}
trigger_class :: trigger_class (PrologDirectory * dir, bool active, orbiter_core * core) : PrologNativeOrbiterCreator (core) {this -> dir = dir; this -> active = active;}

orbiter * mixer_class :: create_orbiter (PrologElement * parameters) {return new lunar_mixer (core);}
mixer_class :: mixer_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * stereo_mixer_class :: create_orbiter (PrologElement * parameters) {return new lunar_stereo_mixer (core);}
stereo_mixer_class :: stereo_mixer_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * control_class :: create_orbiter (PrologElement * parameters) {return new lunar_control (core);}
control_class :: control_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * gateway_class :: create_orbiter (PrologElement * parameters) {return new lunar_gateway (core);}
gateway_class :: gateway_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * stereo_gateway_class :: create_orbiter (PrologElement * parameters) {return new lunar_stereo_gateway (core);}
stereo_gateway_class :: stereo_gateway_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * amplifier_class :: create_orbiter (PrologElement * parameters) {return new lunar_amplifier (core);}
amplifier_class :: amplifier_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * stereo_amplifier_class :: create_orbiter (PrologElement * parameters) {return new lunar_stereo_amplifier (core);}
stereo_amplifier_class :: stereo_amplifier_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * volume_class :: create_orbiter (PrologElement * parameters) {return new lunar_volume (core);}
volume_class :: volume_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * mono_volume_class :: create_orbiter (PrologElement * parameters) {return new lunar_mono_volume (core);}
mono_volume_class :: mono_volume_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * lfo_class :: create_orbiter (PrologElement * parameters) {return new lunar_lfo (core);}
lfo_class :: lfo_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * adsr_class :: create_orbiter (PrologElement * parameters) {return new lunar_adsr (core);}
adsr_class :: adsr_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * eg_class :: create_orbiter (PrologElement * parameters) {return new lunar_eg (core);}
eg_class :: eg_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * delay_class :: create_orbiter (PrologElement * parameters) {return new lunar_delay (core);}
delay_class :: delay_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * chorus_class :: create_orbiter (PrologElement * parameters) {return new lunar_chorus (core);}
chorus_class :: chorus_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * pan_class :: create_orbiter (PrologElement * parameters) {
	switch (type) {
	case 0: return new lunar_pan (core); break;
	case 1: return new lunar_power_pan (core); break;
	default: break;
	}
	return new lunar_linear_pan (core);
}
pan_class :: pan_class (orbiter_core * core, int type) : PrologNativeOrbiterCreator (core) {this -> type = type;}

orbiter * drywet_class :: create_orbiter (PrologElement * parameters) {return new lunar_drywet (core);}
drywet_class :: drywet_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}
orbiter * drywet_mono_class :: create_orbiter (PrologElement * parameters) {return new lunar_drywet_mono (core);}
drywet_mono_class :: drywet_mono_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * sensitivity_class :: create_orbiter (PrologElement * parameters) {return new lunar_sensitivity (core);}
sensitivity_class :: sensitivity_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

static char * moonbase_action_code = "Moonbase Action";
class native_moonbase : public PrologNativeOrbiter {
private:
	PrologAtom * keyon, * keyoff, *pitch, * control, * mono, * poly, * timingclock;
	PrologAtom * cbb, * cb, * c, * cx, * cxx;
	PrologAtom * dbb, * db, * d, * dx, * dxx;
	PrologAtom * ebb, * eb, * e, * ex, * exx;
	PrologAtom * fbb, * fb, * f, * fx, * fxx;
	PrologAtom * gbb, * gb, * g, * gx, * gxx;
	PrologAtom * abb, * ab, * a, * ax, * axx;
	PrologAtom * bbb, * bb, * b, * bx, * bxx;
	int chromatic (PrologAtom * atom) {
		if (atom == cbb) return -2;
		if (atom == cb) return -1;
		if (atom == c || atom == dbb) return 0;
		if (atom == cx || atom == db) return 1;
		if (atom == cxx || atom == d || atom == ebb) return 2;
		if (atom == dx || atom == eb || atom == fbb) return 3;
		if (atom == dxx || atom == e || atom == fb) return 4;
		if (atom == ex || atom == f || atom == gbb) return 5;
		if (atom == exx || atom == fx || atom == gb) return 6;
		if (atom == fxx || atom == g || atom == abb) return 7;
		if (atom == gx || atom == ab) return 8;
		if (atom == gxx || atom == a || atom == bbb) return 9;
		if (atom == ax || atom == bb || atom == cbb) return 10;
		if (atom == axx || atom == b || atom == cb) return 11;
		if (atom == bx) return 12;
		if (atom == bxx) return 13;
		return 0;
	}
public:
	static char * name (void) {return moonbase_action_code;}
	bool isTypeOf (char * code_name) {return moonbase_action_code == code_name ? true : PrologNativeOrbiter :: isTypeOf (code_name);}
	char * codeName (void) {return name ();}
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * atom = 0;
		PrologElement * key = 0;
		PrologElement * velocity = 0;
		PrologElement * note = 0;
		PrologElement * octave = 0;
		PrologElement * var = 0;
		PrologElement * pp = parameters;
		while (pp -> isPair ()) {
			PrologElement * el = pp -> getLeft ();
			if (el -> isAtom ()) atom = el;
			if (el -> isEarth ()) atom = el;
			if (el -> isInteger ()) if (key == 0 && note == 0) key = el; else velocity = el;
			if (el -> isVar ()) var = el;
			if (el -> isPair ()) {
				note = el -> getLeft ();
				octave = el -> getRight ();
				if (octave -> isPair ()) octave = octave -> getLeft ();
				if (! note -> isAtom ()) note = 0;
				if (! octave -> isInteger ()) octave = 0;
			}
			pp = pp -> getRight ();
		}
		if (pp -> isVar ()) var = pp;
		CommandModule * trigger = (CommandModule *) module;
		if (atom != 0) {
			if (atom -> isEarth ()) {
				if (key != 0) return trigger -> insert_controller (0, key -> getInteger (), 0);
				return trigger -> set_map (0);
			}
			if (atom -> isAtom ()) {
				PrologAtom * a = atom -> getAtom ();
				if (a == keyon) {
					int key_note;
					if (key != 0) key_note = key -> getInteger ();
					else if (note != 0 && octave != 0) key_note = chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
					else return false;
					if (velocity == 0) trigger -> keyon (key_note);
					else trigger -> keyon (key_note, velocity -> getInteger ());
					return true;
				}
				if (a == keyoff) {
					if (key != 0) trigger -> keyoff (key -> getInteger (), velocity == 0 ? 0 : velocity -> getInteger ());
					else if (note != 0 && octave != 0)
						trigger -> keyoff (chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48, velocity == 0 ? 0 : velocity -> getInteger ());
					else trigger -> keyoff ();
					return true;
				}
				if (a == control) {
					if (key != 0) {
						if (var != 0) {var -> setDouble (trigger -> getControl (key -> getInteger ())); return true;}
						if (velocity != 0) {trigger -> control (key -> getInteger (), velocity -> getInteger ()); return true;}
					}
					if (var != 0) {var -> setAtom (trigger -> isMonoMode () ? mono : poly); return true;}
					return false;
				}
				if (a == pitch) {
					if (key != 0 && velocity != 0) {
						int lsb = key -> getInteger ();
						int msb = velocity -> getInteger ();
						if (msb == 127 && lsb == 127) msb = 128;
						trigger -> control (128, msb);
						return true;
					}
					if (var != 0) {var -> setDouble (trigger -> getControl (128)); return true;}
					return false;
				}
				if (a == mono) {trigger -> mono (); return true;}
				if (a == poly) {trigger -> poly (); return true;}
				if (a == timingclock) {trigger -> timing_clock (); return true;}
				PrologNativeCode * machine = a -> getMachine ();
				if (machine == 0) return false;
				if (machine -> isTypeOf (key_map_native_orbiter :: name ())) {
					return trigger -> set_map ((lunar_map *) ((key_map_native_orbiter *) machine) -> module);
				}
				if (machine -> isTypeOf (trigger_native_orbiter :: name ())) {
					return trigger -> insert_trigger ((lunar_trigger *) ((trigger_native_orbiter *) machine) -> module);
				}
				if (machine -> isTypeOf (PrologNativeOrbiter :: name ()) && key != 0) {
					return trigger -> insert_controller ((orbiter *) ((PrologNativeOrbiter *) machine) -> module,
														key -> getInteger (), velocity != 0 ? velocity -> getInteger () : 0);
				}
			}
		}
		return PrologNativeOrbiter :: code (parameters, resolution);
	}
	native_moonbase (PrologDirectory * dir, PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {
		keyon = keyoff = pitch = control = mono = poly = timingclock = 0;
		cbb = cb = c = cx = cxx = 0;
		dbb = db = d = dx = dxx = 0;
		ebb = eb = e = ex = exx = 0;
		fbb = fb = f = fx = fxx = 0;
		gbb = gb = g = gx = gxx = 0;
		abb = ab = a = ax = axx = 0;
		bbb = bb = b = bx = bxx = 0;
		if (dir == 0) return;
		keyon = dir -> searchAtom ("keyon");
		keyoff = dir -> searchAtom ("keyoff");
		pitch = dir -> searchAtom ("pitch");
		control = dir -> searchAtom ("control");
		mono = dir -> searchAtom ("mono");
		poly = dir -> searchAtom ("poly");
		timingclock = dir -> searchAtom ("timingclock");
		c = dir -> searchAtom ("C"); cb = dir -> searchAtom ("Cb"); cbb = dir -> searchAtom ("Cbb"); cx = dir -> searchAtom ("C#"); cxx = dir -> searchAtom ("Cx");
		d = dir -> searchAtom ("D"); db = dir -> searchAtom ("Db"); dbb = dir -> searchAtom ("Dbb"); dx = dir -> searchAtom ("D#"); dxx = dir -> searchAtom ("Dx");
		e = dir -> searchAtom ("E"); eb = dir -> searchAtom ("Eb"); ebb = dir -> searchAtom ("Ebb"); ex = dir -> searchAtom ("E#"); exx = dir -> searchAtom ("Ex");
		f = dir -> searchAtom ("F"); fb = dir -> searchAtom ("Fb"); fbb = dir -> searchAtom ("Fbb"); fx = dir -> searchAtom ("F#"); fxx = dir -> searchAtom ("Fx");
		g = dir -> searchAtom ("G"); gb = dir -> searchAtom ("Gb"); gbb = dir -> searchAtom ("Gbb"); gx = dir -> searchAtom ("G#"); gxx = dir -> searchAtom ("Gx");
		a = dir -> searchAtom ("A"); ab = dir -> searchAtom ("Ab"); abb = dir -> searchAtom ("Abb"); ax = dir -> searchAtom ("A#"); axx = dir -> searchAtom ("Ax");
		b = dir -> searchAtom ("B"); bb = dir -> searchAtom ("Bb"); bbb = dir -> searchAtom ("Bbb"); bx = dir -> searchAtom ("B#"); bxx = dir -> searchAtom ("Bx");
	}
};
orbiter * moonbase_class :: create_orbiter (PrologElement * parameters) {return new moonbase (core);}
PrologNativeOrbiter * moonbase_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new native_moonbase (dir, atom, core, module);}
moonbase_class :: moonbase_class (PrologDirectory * dir, orbiter_core * core) : PrologNativeOrbiterCreator (core) {this -> dir = dir;}

orbiter * arpeggiator_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * base = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) base = el;
		parameters = parameters -> getRight ();
	}
	if (base == 0) return 0;
	PrologNativeCode * machine = base -> getAtom () -> getMachine ();
	if (machine == 0) return 0;
	if (! machine -> isTypeOf (native_moonbase :: name ())) return 0;
	return new arpeggiator (core, ((moonbase *) ((native_moonbase *) machine) -> module));
}
PrologNativeOrbiter * arpeggiator_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new native_moonbase (dir, atom, core, module);}
arpeggiator_class :: arpeggiator_class (PrologDirectory * dir, orbiter_core * core) : PrologNativeOrbiterCreator (core) {this -> dir = dir;}

class lunar_detector : public orbiter {
private:
	double trigger;
	bool trigger_active;
	PrologRoot * root;
	PrologElement * query;
public:
	int numberOfInputs (void) {return orbiter :: numberOfOutputs ();}
	char * inputName (int ind) {return orbiter :: outputName (ind);}
	double * inputAddress (int ind) {return orbiter :: outputAddress (ind);}
	int numberOfOuptuts (void) {return 0;}
	void move (void) {
		if (trigger_active) {
			if (trigger != signal) return;
			trigger_active = false;
			if (query != 0) {
				PrologElement * sub = query -> duplicate ();
				AREA area;
				root -> getValue (sub, area, 0);
				root -> resolution (sub);
				delete sub;
			}
			return;
		}
		if (trigger != signal) trigger_active = true;
	}
	lunar_detector (PrologRoot * root, double trigger, PrologElement * query, orbiter_core * core) : orbiter (core) {
		this -> root = root;
		this -> query = 0;
		if (query != 0) {
			PrologDirectory * studio = root -> searchDirectory ("studio");
			if (studio != 0) {
				PrologAtom * crack = studio -> searchAtom ("crack");
				if (crack != 0) {
					this -> query = root -> pair (root -> earth (),
						root -> pair (root -> pair (root -> atom (crack), query -> duplicate ()), root -> earth ()));
				}
			}
		}
		trigger_active = false; this -> trigger = trigger;
		initialise (); activate ();
	}
	~ lunar_detector (void) {if (query != 0) delete query;}
};

orbiter * detector_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * detector = 0;
	PrologElement * query = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isPair () && query == 0) query = parameters;
		if (el -> isNumber ()) detector = el;
		parameters = parameters -> getRight ();
	}
	return new lunar_detector (root, detector != 0 ? detector -> getNumber () : 0.0, query, core);
}
detector_class :: detector_class (PrologRoot * root, orbiter_core * core) : PrologNativeOrbiterCreator (core) {this -> root = root;}
