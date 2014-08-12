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

orbiter * parameter_block_class :: create_orbiter (PrologElement * parameters) {
	if (parameters -> isEarth ()) return new lunar_inactive_parameter_block (core);
	if (parameters -> isPair ()) parameters = parameters -> getLeft ();
	if (parameters -> isInteger ()) return new lunar_active_parameter_block (core, (double) parameters -> getInteger ());
	if (parameters -> isDouble ()) return new lunar_active_parameter_block (core, parameters -> getDouble ());
	return new lunar_inactive_parameter_block (core);
}
parameter_block_class :: parameter_block_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

static char * key_orbiter_action_code = "Lunar Map Action";
char * PrologNativeKeyOrbiter :: name (void) {return key_orbiter_action_code;}
bool PrologNativeKeyOrbiter :: isTypeOf (char * code_name) {return key_orbiter_action_code == code_name ? true : PrologNativeOrbiter :: isTypeOf (code_name);}
PrologNativeKeyOrbiter :: PrologNativeKeyOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {}

class key_map_native_orbiter : public PrologNativeKeyOrbiter {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return onEarth ();
		lunar_map * map = (lunar_map *) module;
		if (parameters -> isVar ()) {
			int ind = 0;
			while (ind < 128) {
				parameters -> setPair ();
				parameters -> getLeft () -> setDouble (map -> map [ind++]);
				parameters = parameters -> getRight ();
			}
			return true;
		}
		int double_count = 0;
		PrologElement * doubles [128];
		PrologElement * ret = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isNumber ()) if (double_count < 128) doubles [double_count++] = el;
			if (el -> isVar ()) ret = el;
			parameters = parameters -> getRight ();
		}
		if (parameters -> isVar ()) ret = parameters;
		if (ret != 0) {
			if (double_count < 1) return false;
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

orbiter * key_map_class :: create_orbiter (PrologElement * parameters) {return new lunar_map (core);}
PrologNativeOrbiter * key_map_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new key_map_native_orbiter (atom, core, module);}
key_map_class :: key_map_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * impulse_class :: create_orbiter (PrologElement * parameters) {return new lunar_impulse (core);}
impulse_class :: impulse_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

class trigger_native_orbiter : public PrologNativeOrbiter {
private:
	PrologAtom * keyon, * keyoff;
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * atom = 0;
		PrologElement * key = 0;
		PrologElement * velocity = 0;
		PrologElement * pp = parameters;
		while (pp -> isPair ()) {
			PrologElement * el = pp -> getLeft ();
			if (el -> isAtom ()) atom = el;
			if (el -> isEarth ()) atom = el;
			if (el -> isInteger ()) if (key == 0) key = el; else velocity = el;
			pp = pp -> getRight ();
		}
		lunar_trigger * trigger = (lunar_trigger *) module;
		if (atom != 0) {
			if (atom -> isEarth ()) {trigger -> set_map (0); return true;}
			if (atom -> isAtom ()) {
				PrologAtom * a = atom -> getAtom ();
				if (a == keyon) {
					if (key == 0) return false;
					if (velocity == 0) trigger -> keyon (key -> getInteger ());
					else trigger -> keyon (key -> getInteger (), velocity -> getInteger ());
					return true;
				}
				if (a == keyoff) {trigger -> keyoff (); return true;}
				PrologNativeCode * machine = a -> getMachine ();
				if (machine == 0) return false;
				if (machine -> isTypeOf (key_map_native_orbiter :: name ())) {trigger -> set_map ((lunar_map *) ((key_map_native_orbiter *) machine) -> module); return true;}
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
orbiter * trigger_class :: create_orbiter (PrologElement * parameters) {return new lunar_trigger (core);}
PrologNativeOrbiter * trigger_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new trigger_native_orbiter (dir, atom, core, module);}
trigger_class :: trigger_class (PrologDirectory * dir, orbiter_core * core) : PrologNativeOrbiterCreator (core) {this -> dir = dir;}

orbiter * mixer_class :: create_orbiter (PrologElement * parameters) {return new lunar_mixer (core);}
mixer_class :: mixer_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * stereo_mixer_class :: create_orbiter (PrologElement * parameters) {return new lunar_stereo_mixer (core);}
stereo_mixer_class :: stereo_mixer_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * gateway_class :: create_orbiter (PrologElement * parameters) {return new lunar_gateway (core);}
gateway_class :: gateway_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * lfo_class :: create_orbiter (PrologElement * parameters) {return new lunar_lfo (core);}
lfo_class :: lfo_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * adsr_class :: create_orbiter (PrologElement * parameters) {return new lunar_adsr (core);}
adsr_class :: adsr_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * eg_class :: create_orbiter (PrologElement * parameters) {return new lunar_eg (core);}
eg_class :: eg_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * moonbase_class :: create_orbiter (PrologElement * parmaeters) {return new moonbase (core);}
moonbase_class :: moonbase_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

