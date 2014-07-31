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

orbiter * parameter_block_class :: create_orbiter (PrologElement * parameters) {
	if (parameters -> isEarth ()) return new lunar_inactive_parameter_block (core);
	if (parameters -> isPair ()) parameters = parameters -> getLeft ();
	if (parameters -> isInteger ()) return new lunar_active_parameter_block (core, (double) parameters -> getInteger ());
	if (parameters -> isDouble ()) return new lunar_active_parameter_block (core, parameters -> getDouble ());
	return new lunar_inactive_parameter_block (core);
}
parameter_block_class :: parameter_block_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * impulse_class :: create_orbiter (PrologElement * parameters) {return new lunar_impulse (core);}
impulse_class :: impulse_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

class trigger_native_orbiter : public PrologNativeOrbiter {
public:
	virtual bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {if (atom != 0) atom -> setMachine (0); delete this; return true;}
		PrologElement * key = 0;
		PrologElement * velocity = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isInteger ()) if (key == 0) key = el; else velocity = el;
			parameters = parameters -> getRight ();
		}
		lunar_trigger * trigger = (lunar_trigger *) module;
		if (key == 0) {trigger -> keyoff (); return true;}
		if (velocity == 0) {trigger -> keyon (key -> getInteger (), 100); return true;}
		int v = velocity -> getInteger ();
		if (v == 0) {trigger -> keyoff (); return true;}
		trigger -> keyon (key -> getInteger (), v);
		return true;
	}
	trigger_native_orbiter (PrologAtom * atom, orbiter * module) : PrologNativeOrbiter (atom, core, module) {}
};
orbiter * trigger_class :: create_orbiter (PrologElement * parameters) {return new lunar_trigger (core);}
PrologNativeOrbiter * trigger_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new trigger_native_orbiter (atom, module);}
trigger_class :: trigger_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}
