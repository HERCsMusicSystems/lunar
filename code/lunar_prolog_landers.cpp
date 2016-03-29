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
#include "chromatic.h"

#ifdef WIN32
#define strcasecmp _strcmpi
#endif

int toStyle (char * name) {
	if (name == 0) return 0;
	if (strcasecmp (name, "index") == 0) return 1;
	if (strcasecmp (name, "freq") == 0) return 2;
	if (strcasecmp (name, "amp") == 0) return 3;
	if (strcasecmp (name, "time") == 0) return 4;
	if (strcasecmp (name, "onoff") == 0) return 5;
	if (strcasecmp (name, "wave") == 0) return 6;
	if (strcasecmp (name, "ratio") == 0) return 7;
	if (strcasecmp (name, "fm4algo") == 0) return 8;
	return 0;
}

void formatToStyle (double signal, char * command, int style, orbiter_core * core) {
	switch (style) {
	case 1: sprintf (command, "%i", (int) signal); break;
	case 2: sprintf (command, "%i  (%.2f Hz)", (int) signal, core -> centre_frequency * pow (2.0, signal / 1536.0)); break;
	case 3:
		if (signal <= -16383.0) sprintf (command, "%i (ZERO)", (int) signal);
		else sprintf (command, "%i  (%.2f Db)", (int) signal, 10.0 * log10 (core -> Amplitude (signal)));
		break;
	case 4: sprintf (command, "%i  (%.2f sec.)", (int) signal, signal <= 0.0 ? 0.0 : pow (2.0, (signal - 8192.0) / 768.0)); break;
	case 5: if (signal == 0) sprintf (command, "off"); else sprintf (command, "on"); break;
	case 6:
		switch ((int) signal) {
		case 0: sprintf (command, "0 (sine)"); break;
		case 1: sprintf (command, "1 (triangle)"); break;
		case 2: sprintf (command, "2 (square)"); break;
		case 3: sprintf (command, "3 (random)"); break;
		case 4: sprintf (command, "4 (S/H)"); break;
		default: sprintf (command, "%i", (int) signal); break;
		}
		break;
	case 7: sprintf (command, "* %g", signal); break;
	case 8:
		switch ((int) signal) {
		case 0: sprintf (command, "0 (horizontal)"); break;
		case 1: sprintf (command, "1 (half-horizontal)"); break;
		case 2: sprintf (command, "2 (pyramid)"); break;
		case 3: sprintf (command, "3 (double)"); break;
		case 4: sprintf (command, "4 (low-tree)"); break;
		case 5: sprintf (command, "5 (tree)"); break;
		case 6: sprintf (command, "6 (high-tree)"); break;
		case 7: sprintf (command, "7 (vertical)"); break;
		default: sprintf (command, "%i (horizontal)", (int) signal); break;
		}
		break;
	default: sprintf (command, "??"); break;
	}
}


class pb_native_orbiter : public PrologNativeOrbiter {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		lunar_parameter_block * pb = (lunar_parameter_block *) module;
		if (pb -> style != 0 && parameters -> isVar ()) {
			char command [128];
			formatToStyle (pb -> signal, command, pb -> style, core);
			parameters -> setText (command);
			return true;
		}
		return PrologNativeOrbiter :: code (parameters, resolution);
	}
	pb_native_orbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {}
};

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

orbiter * morph_class :: create_orbiter (PrologElement * parameters) {
	int dimension = 2;
	double divisor = 1.0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isInteger ()) {int ind = el -> getInteger (); if (1 <= ind && ind <= 3) dimension = ind;}
		if (el -> isDouble ()) {double ind = el -> getDouble (); if (ind != 0.0) divisor = ind;}
		parameters = parameters -> getRight ();
	}
	if (divisor != 0.0) {
		switch (dimension) {
		case 1: return new lunar_morph_one_d (core, divisor); break;
		case 3: return new lunar_morph_three_d (core, divisor); break;
		default: break;
		}
		return new lunar_morph_two_d (core, divisor);
	}
	switch (dimension) {
	case 1: return new lunar_morph_one (core); break;
	case 3: return new lunar_morph_three (core); break;
	default: break;
	}
	return new lunar_morph_two (core);
}
morph_class :: morph_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

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
		bool clean = true;
		for (int ind = 0; ind < 128; ind++) {if (map -> map [ind] != (double) (map -> initial + ind) * 128.0) clean = false;}
		if (clean) {parameters -> setPair (); return;}
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
		PrologElement * reset = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isNumber ()) if (double_count < 128) doubles [double_count++] = el;
			if (el -> isVar ()) ret = el;
			if (el -> isEarth ()) reset = el;
			if (el -> isPair ()) {
				PrologElement * subel = el;
				while (subel -> isPair ()) {
					PrologElement * ell = subel -> getLeft ();
					if (ell -> isEarth ()) reset = ell;
					if (ell -> isNumber ()) if (double_count < 128) doubles [double_count++] = ell;
					subel = subel -> getRight ();
				}
			}
			parameters = parameters -> getRight ();
		}
		if (reset != 0) {map -> reset (); return true;}
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

orbiter * wave_class :: create_orbiter (PrologElement * parameters) {
	int capacity = 0;
	PrologElement * counter = parameters;
	while (counter -> isPair ()) {
		if (counter -> getLeft () -> isText ()) capacity++;
		counter = counter -> getRight ();
	}
	lunar_wave * wave = new lunar_wave (core, capacity);
	capacity = 0;
	double gain = 4.0;
	double headroom_reciprocal = core -> headroom_fraction > 0.0 ? 1.0 / core -> headroom_fraction : 1.0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isDouble ()) gain = el -> getDouble ();
		if (el -> isText ()) wave -> waves [capacity++] = create_lunar_wave_data (el -> getText (), gain * headroom_reciprocal);
		parameters = parameters -> getRight ();
	}
	return wave;
}
PrologNativeOrbiter * wave_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new PrologNativeWaveOrbiter (atom, core, module);}
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

orbiter * adsr_class :: create_orbiter (PrologElement * parameters) {if (linear) return new lunar_adsr_linear (core); else return new lunar_adsr (core);}
adsr_class :: adsr_class (orbiter_core * core, bool linear) : PrologNativeOrbiterCreator (core) {this -> linear = linear;}

orbiter * eg_class :: create_orbiter (PrologElement * parameters) {if (linear_expo) return new lunar_eg_linear_expo (core); else return new lunar_eg (core);}
eg_class :: eg_class (orbiter_core * core, bool linear_expo) : PrologNativeOrbiterCreator (core) {this -> linear_expo = linear_expo;}

orbiter * vca_class :: create_orbiter (PrologElement * parameters) {return new lunar_vca (core);}
vca_class :: vca_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * vca_adsr_class :: create_orbiter (PrologElement * parameters) {return new lunar_vca_adsr (core);}
vca_adsr_class :: vca_adsr_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * vca_eg_class :: create_orbiter (PrologElement * parameters) {return new lunar_vca_eg (core);}
vca_eg_class :: vca_eg_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * delay_class :: create_orbiter (PrologElement * parameters) {return new lunar_delay (core);}
delay_class :: delay_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * freeverb_class :: create_orbiter (PrologElement * parameters) {return new lunar_freeverb (core);}
freeverb_class :: freeverb_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * chorus_class :: create_orbiter (PrologElement * parameters) {return new lunar_chorus (core);}
chorus_class :: chorus_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}
orbiter * stereo_chorus_class :: create_orbiter (PrologElement * parametes) {return new lunar_stereo_chorus (core);}
stereo_chorus_class :: stereo_chorus_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * pan_class :: create_orbiter (PrologElement * parameters) {
	switch (type) {
	case 0: return new lunar_pan (core); break;
	case 1: return new lunar_power_pan (core); break;
	case 2: return new lunar_linear_pan (core); break;
	case 3: return new lunar_stereo_pan (core); break;
	case 4: return new lunar_stereo_power_pan (core); break;
	case 5: return new lunar_stereo_linear_pan (core); break;
	default: break;
	}
	return new lunar_pan (core);
}
pan_class :: pan_class (orbiter_core * core, int type) : PrologNativeOrbiterCreator (core) {this -> type = type;}

orbiter * drywet_class :: create_orbiter (PrologElement * parameters) {return new lunar_drywet (core);}
drywet_class :: drywet_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}
orbiter * drywet_mono_class :: create_orbiter (PrologElement * parameters) {return new lunar_drywet_mono (core);}
drywet_mono_class :: drywet_mono_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * sensitivity_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * divisor = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) divisor = el;
		parameters = parameters -> getRight ();
	}
	return new lunar_sensitivity (core, divisor == 0 ? 128.0 : divisor -> getNumber ());
}
sensitivity_class :: sensitivity_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

static char * moonbase_action_code = "Moonbase Action";
char * native_moonbase :: name (void) {return moonbase_action_code;}
bool native_moonbase :: isTypeOf (char * code_name) {return moonbase_action_code == code_name ? true : PrologNativeOrbiter :: isTypeOf (code_name);}
char * native_moonbase :: codeName (void) {return name ();}
bool native_moonbase :: code (PrologElement * parameters, PrologResolution * resolution) {
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
		if (el -> isNumber ()) if (key == 0 && note == 0) key = el; else velocity = el;
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
			if (key != 0) return trigger -> insert_controller (0, (int) key -> getNumber (), 0);
		}
		if (atom -> isAtom ()) {
			PrologAtom * a = atom -> getAtom ();
			if (a == keyon) {
				int key_note;
				if (key != 0) key_note = (int) key -> getNumber ();
				else if (note != 0 && octave != 0) key_note = graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
				else return false;
				if (velocity == 0) trigger -> keyon (key_note);
				else trigger -> keyon (key_note, (int) velocity -> getNumber ());
				return true;
			}
			if (a == keyoff) {
				if (key != 0) trigger -> keyoff ((int) key -> getNumber (), velocity == 0 ? 0 : (int) velocity -> getNumber ());
				else if (note != 0 && octave != 0)
					trigger -> keyoff (graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48,
																			velocity == 0 ? 0 : (int) velocity -> getNumber ());
				else trigger -> keyoff ();
				return true;
			}
			if (a == control) {
				if (key != 0) {
					if (var != 0) {var -> setDouble (trigger -> getControl ((int) key -> getNumber ())); return true;}
					if (velocity != 0) {trigger -> control ((int) key -> getNumber (), velocity -> getNumber ()); return true;}
				}
				if (var != 0) {var -> setAtom (trigger -> isMonoMode () ? mono : poly); return true;}
				return false;
			}
			if (a == pitch) {
				if (key != 0 && velocity != 0) {
					int lsb = (int) key -> getNumber ();
					int msb = (int) velocity -> getNumber ();
					if (msb == 127 && lsb == 127) {msb = 128; lsb = 0;}
					trigger -> control (128, (double) msb + (double) lsb * 0.0078125);
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
			if (machine -> isTypeOf (trigger_native_orbiter :: name ())) {
				return trigger -> insert_trigger ((lunar_trigger *) ((trigger_native_orbiter *) machine) -> module);
			}
			if (machine -> isTypeOf (PrologNativeOrbiter :: name ()) && key != 0) {
				return trigger -> insert_controller ((orbiter *) ((PrologNativeOrbiter *) machine) -> module,
													(int) key -> getNumber (), velocity != 0 ? (int) velocity -> getNumber () : 0);
			}
		}
	}
	return PrologNativeOrbiter :: code (parameters, resolution);
}
native_moonbase :: native_moonbase (PrologDirectory * dir, PrologAtom * atom, orbiter_core * core, orbiter * module) : graph (dir), PrologNativeOrbiter (atom, core, module) {
	keyon = keyoff = pitch = control = mono = poly = timingclock = 0;
	if (dir == 0) return;
	keyon = dir -> searchAtom ("keyon");
	keyoff = dir -> searchAtom ("keyoff");
	pitch = dir -> searchAtom ("pitch");
	control = dir -> searchAtom ("control");
	mono = dir -> searchAtom ("mono");
	poly = dir -> searchAtom ("poly");
	timingclock = dir -> searchAtom ("timingclock");
}

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

class native_sequencer : public native_moonbase {
public:
	PrologAtom * keyon, * keyoff, * control, * busy, * impulse;
	chromatograph graph;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isVar ()) {
				sequencer * seq = (sequencer *) module;
				if (seq == 0) return false;
				sequence_element * sqep = seq -> elements;
				while (sqep != 0) {
					el -> setPair ();
					PrologElement * ell = el -> getLeft ();
					switch (sqep -> type) {
					case 0: ell -> setInteger (sqep -> key); break;
					case 1:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (keyon);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						break;
					case 2:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (keyon);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger ((int) sqep -> velocity);
						break;
					case 3: ell -> setAtom (keyoff); break;
					case 4:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (keyoff);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						break;
					case 5:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (control);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setDouble (sqep -> velocity);
						break;
					case 8:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (busy);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setDouble (sqep -> velocity);
						break;
					case 9:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (impulse);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setDouble (sqep -> velocity);
						break;
					default: break;
					}
					el = el -> getRight ();
					sqep = sqep -> next;
				}
				if (! el -> isEarth ()) el -> setPair ();
				return true;
			}
			if (el -> isPair ()) {
				sequencer * seq = (sequencer *) module;
				if (seq == 0) return false;
				if (seq -> elements != 0) delete seq -> elements; seq -> elements = 0;
				sequence_element * * sqep = & seq -> elements;
				while (el -> isPair ()) {
					PrologElement * eq = el -> getLeft ();
					if (eq -> isInteger ()) {* sqep = new sequence_element (0, eq -> getInteger ()); sqep = & (* sqep) -> next;}
					if (eq -> isAtom () && eq -> getAtom () == keyoff) {* sqep = new sequence_element (3); sqep = & (* sqep) -> next;}
					if (eq -> isPair ()) {
						PrologElement * atom = 0;
						PrologElement * key = 0;
						PrologElement * velocity = 0;
						PrologElement * note = 0;
						PrologElement * octave = 0;
						while (eq -> isPair ()) {
							PrologElement * eqq = eq -> getLeft ();
							if (eqq -> isAtom ()) atom = eqq;
							if (eqq -> isInteger ()) if (key == 0) key = eqq; else velocity = eqq;
							if (eqq -> isDouble ()) velocity = eqq;
							if (eqq -> isPair ()) {
								note = eqq -> getLeft ();
								octave = eqq -> getRight ();
								if (octave -> isPair ()) octave = octave -> getLeft ();
								if (! note -> isAtom ()) note = 0;
								if (! octave -> isInteger ()) octave = 0;
							}
							eq = eq -> getRight ();
						}
						if (atom != 0) {
							if (atom -> getAtom () == keyon) {
								if (note != 0 && octave != 0) {
									int sub = graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
									if (key != 0) * sqep = new sequence_element (2, sub, (double) key -> getInteger ());
									else if (velocity != 0) * sqep = new sequence_element (2, sub, velocity -> getNumber ());
									else * sqep = new sequence_element (1, sub);
									sqep = & (* sqep) -> next;
								} else if (key != 0) {
									if (velocity == 0) * sqep = new sequence_element (1, key -> getInteger ());
									else * sqep = new sequence_element (2, key -> getInteger (), velocity -> getNumber ());
									sqep = & (* sqep) -> next;
								}
							}
							if (atom -> getAtom () == keyoff) {
								if (note != 0 && octave != 0) * sqep = new sequence_element (4, graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48);
								else if (key != 0) * sqep = new sequence_element (4, key -> getInteger ());
								else * sqep = new sequence_element (3);
								sqep = & (* sqep) -> next;
							}
							if (atom -> getAtom () == control) {
								if (note != 0 && octave != 0) {
									int sub = graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
									if (key != 0) {
										* sqep = new sequence_element (5, sub, (double) key -> getInteger ());
										sqep = & (* sqep) -> next;
									} else if (velocity != 0) {
										* sqep = new sequence_element (5, sub, velocity -> getNumber ());
										sqep = & (* sqep) -> next;
									}
								} else if (key != 0 && velocity != 0) {
									* sqep = new sequence_element (5, key -> getInteger (), velocity -> getNumber ());
									sqep = & (* sqep) -> next;
								}
							}
							if (atom -> getAtom () == busy && (key != 0 || velocity != 0)) {
								* sqep = new sequence_element (8, 0, key != 0 ? key -> getNumber () : velocity -> getNumber ());
								sqep = & (* sqep) -> next;
							}
							if (atom -> getAtom () == impulse && (key != 0 || velocity != 0)) {
								* sqep = new sequence_element (9, 0, key != 0 ? key -> getNumber () : velocity -> getNumber ());
								sqep = & (* sqep) -> next;
							}
						} else if (key != 0) {* sqep = new sequence_element (0, key -> getInteger ()); sqep = & (* sqep) -> next;}
					}
					el = el -> getRight ();
				}
				return true;
			}
		}
		return native_moonbase :: code (parameters, resolution);
	}
	native_sequencer (PrologDirectory * dir, PrologAtom * atom, orbiter_core * core, orbiter * module) : graph (dir), native_moonbase (dir, atom, core, module) {
		keyon = keyoff = control = 0;
		if (dir == 0) return;
		keyon = dir -> searchAtom ("keyon");
		keyoff = dir -> searchAtom ("keyoff");
		control = dir -> searchAtom ("control");
		busy = dir -> searchAtom ("busy");
		impulse = dir -> searchAtom ("impulse");
	}
};

class native_polysequencer : public native_sequencer {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isVar ()) {
				polysequencer * seq = (polysequencer *) module;
				if (seq == 0) return false;
				polysequence_element * sqep = seq -> elements;
				while (sqep != 0) {
					el -> setPair ();
					PrologElement * ell = el -> getLeft ();
					switch (sqep -> type) {
					case 0: ell -> setInteger (sqep -> key); break;
					case 1:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (keyon);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> channel);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						break;
					case 2:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (keyon);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> channel);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger ((int) sqep -> velocity);
						break;
					case 3:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (keyoff);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> channel);
						break;
					case 4:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (keyoff);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> channel);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						break;
					case 5:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (control);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> channel);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setInteger (sqep -> key);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setDouble (sqep -> velocity);
						break;
					case 6: ell -> setAtom (keyoff); break;
					case 8:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (busy);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setDouble (sqep -> velocity);
						break;
					case 9:
						ell -> setPair ();
						ell -> getLeft () -> setAtom (impulse);
						ell = ell -> getRight (); ell -> setPair ();
						ell -> getLeft () -> setDouble (sqep -> velocity);
						break;
					default: break;
					}
					el = el -> getRight ();
					sqep = sqep -> next;
				}
				if (! el -> isEarth ()) el -> setPair ();
				return true;
			}
			if (el -> isPair ()) {
				polysequencer * seq = (polysequencer *) module;
				if (seq == 0) return false;
				if (seq -> elements != 0) delete seq -> elements; seq -> elements = 0;
				polysequence_element * * sqep = & seq -> elements;
				while (el -> isPair ()) {
					PrologElement * eq = el -> getLeft ();
					if (eq -> isInteger ()) {* sqep = new polysequence_element (0, 0, eq -> getInteger ()); sqep = & (* sqep) -> next;}
					if (eq -> isAtom () && eq -> getAtom () == keyoff) {* sqep = new polysequence_element (6); sqep = & (* sqep) -> next;}
					if (eq -> isPair ()) {
						PrologElement * atom = 0;
						PrologElement * channel = 0;
						PrologElement * key = 0;
						PrologElement * note = 0;
						PrologElement * octave = 0;
						PrologElement * velocity = 0;
						while (eq -> isPair ()) {
							PrologElement * eqq = eq -> getLeft ();
							if (eqq -> isAtom ()) atom = eqq;
							if (eqq -> isInteger ()) if (channel == 0) channel = eqq; else if (key == 0) key = eqq; else velocity = eqq;
							if (eqq -> isDouble ()) velocity = eqq;
							if (eqq -> isPair ()) {
								note = eqq -> getLeft ();
								octave = eqq -> getRight ();
								if (octave -> isPair ()) octave = octave -> getLeft ();
								if (! note -> isAtom ()) note = 0;
								if (! octave -> isInteger ()) octave = 0;
							}
							eq = eq -> getRight ();
						}
						if (atom != 0) {
							if (atom -> getAtom () == busy && (channel != 0 || velocity != 0)) {
								* sqep = new polysequence_element (8, 0, 0, channel != 0 ? channel -> getNumber () : velocity -> getNumber ());
								sqep = & (* sqep) -> next;
							} else if (atom -> getAtom () == impulse && (channel != 0 || velocity != 0)) {
								* sqep = new polysequence_element (9, 0, 0, channel != 0 ? channel -> getNumber () : velocity -> getNumber ());
								sqep = & (* sqep) -> next;
							} else if (channel != 0 && channel -> getInteger () >= 0 && channel -> getInteger () < seq -> numberOfBases ()) {
								if (atom -> getAtom () == keyon) {
									if (note != 0 && octave != 0) {
										int sub = graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
										if (key != 0) * sqep = new polysequence_element (2, channel -> getInteger (), sub, (double) key -> getInteger ());
										else if (velocity != 0) * sqep = new polysequence_element (2, channel -> getInteger (), sub, velocity -> getNumber ());
										else * sqep = new polysequence_element (1, channel -> getInteger (), sub);
										sqep = & (* sqep) -> next;
									} else if (key != 0) {
										if (velocity == 0) * sqep = new polysequence_element (1, channel -> getInteger (), key -> getInteger ());
										else * sqep = new polysequence_element (2, channel -> getInteger (), key -> getInteger (), velocity -> getNumber ());
										sqep = & (* sqep) -> next;
									}
								}
								if (atom -> getAtom () == keyoff) {
									if (note != 0 && octave != 0)
										* sqep = new polysequence_element (4, channel -> getInteger (),
																graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48);
									else if (key != 0) * sqep = new polysequence_element (4, channel -> getInteger (), key -> getInteger ());
									else * sqep = new polysequence_element (3, channel -> getInteger ());
									sqep = & (* sqep) -> next;
								}
								if (atom -> getAtom () == control) {
									if (note != 0 && octave != 0) {
										int sub = graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
										if (key != 0) {
											* sqep = new polysequence_element (5, channel -> getInteger (), sub, (double) key -> getInteger ());
											sqep = & (* sqep) -> next;
										} else if (velocity != 0) {
											* sqep = new polysequence_element (5, channel -> getInteger (), sub, velocity -> getNumber ());
											sqep = & (* sqep) -> next;
										}
									} else if (key != 0 && velocity != 0) {
										* sqep = new polysequence_element (5, channel -> getInteger (), key -> getInteger (), velocity -> getNumber ());
										sqep = & (* sqep) -> next;
									}
								}
							} else {
								if (atom -> getAtom () == keyoff) {
									* sqep = new polysequence_element (6);
									sqep = & (* sqep) -> next;
								}
							}
						} else if (channel != 0) {* sqep = new polysequence_element (0, 0, channel -> getInteger ()); sqep = & (* sqep) -> next;}
					}
					el = el -> getRight ();
				}
				return true;
			}
		}
		return native_moonbase :: code (parameters, resolution);
	}
	native_polysequencer (PrologDirectory * dir, PrologAtom * atom, orbiter_core * core, orbiter * module) : native_sequencer (dir, atom, core, module) {}
};

orbiter * sequencer_class :: create_orbiter (PrologElement * parameters) {
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
	return new sequencer (core, ((moonbase *) ((native_moonbase *) machine) -> module));
}
PrologNativeOrbiter * sequencer_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new native_sequencer (dir, atom, core, module);}
sequencer_class :: sequencer_class (PrologDirectory * dir, orbiter_core * core) : PrologNativeOrbiterCreator (core) {this -> dir = dir;}

orbiter * polysequencer_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * pb = parameters;
	int number_of_bases = 0;
	while (pb -> isPair ()) {
		PrologElement * el = pb -> getLeft ();
		if (el -> isAtom ()) {
			PrologNativeCode * machine = el -> getAtom () -> getMachine ();
			if (machine != 0 && machine -> isTypeOf (native_moonbase :: name ())) number_of_bases++;
		}
		pb = pb -> getRight ();
	}
	polysequencer * poly_seq = new polysequencer (core, number_of_bases);
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) {
			PrologNativeCode * machine = el -> getAtom () -> getMachine ();
			if (machine != 0 && machine -> isTypeOf (native_moonbase :: name ())) poly_seq -> add_base ((moonbase *) ((native_moonbase *) machine) -> module);
		}
		parameters = parameters -> getRight ();
	}
	return poly_seq;
}
PrologNativeOrbiter * polysequencer_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new native_polysequencer (dir, atom, core, module);}
polysequencer_class :: polysequencer_class (PrologDirectory * dir, orbiter_core * core) : PrologNativeOrbiterCreator (core) {this -> dir = dir;}

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

orbiter * DCOffsetFilter_class :: create_orbiter (PrologElement * parameters) {
	double fractor = 0.00390625;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isDouble ()) fractor = el -> getDouble ();
		parameters = parameters -> getRight ();
	}
	if (mono) return new lunar_dc_offset_filter_mono (fractor, core);
	return new lunar_dc_offset_filter (fractor, core);
}
DCOffsetFilter_class :: DCOffsetFilter_class (orbiter_core * core, bool mono) : PrologNativeOrbiterCreator (core) {this -> mono = mono;}
