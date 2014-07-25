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

//////////////////////////////////////////////////////////////////////
// This file was created on Tuesday, 15th July 2014 at 15:47:20 PM. //
//////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"

#include <stdio.h>

static char * orbiter_action_code = "Orbiter Action Code";
char * PrologNativeOrbiter :: name (void) {return orbiter_action_code;}
char * PrologNativeOrbiter :: codeName (void) {return orbiter_action_code;}
bool PrologNativeOrbiter :: isTypeOf (char * code_name) {return orbiter_action_code == code_name ? true : PrologNativeCode :: isTypeOf (code_name);}

bool PrologNativeOrbiter :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) {if (atom != 0) atom -> setMachine (0); delete this; return true;}
	return true;
}

PrologNativeOrbiter :: PrologNativeOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) {
	this -> atom = atom;
	this -> core = core;
	this -> module = module;
	if (module == 0) return;
	module -> activate ();
	module -> hold ();
	printf ("	NATIVE ORBITER CREATED\n");
}

PrologNativeOrbiter :: ~ PrologNativeOrbiter (void) {
	if (module != 0) module -> release (); module = 0;
	printf ("	NATIVE ORBITER DESTROYED\n");
}

PrologNativeOrbiter * PrologNativeOrbiterCreator :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new PrologNativeOrbiter (atom, core, module);}
void PrologNativeOrbiterCreator :: code_created (PrologNativeOrbiter * machine) {}

bool PrologNativeOrbiterCreator :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isVar ()) atom = el;
		parameters = parameters -> getRight ();
	}
	if (atom == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	PrologNativeOrbiter * machine = create_native_orbiter (atom -> getAtom (), create_orbiter ());
	if (machine == 0) return false;
	if (atom -> getAtom () -> setMachine (machine)) {code_created (machine); return true;}
	delete machine;
	return false;
}

PrologNativeOrbiterCreator :: PrologNativeOrbiterCreator (orbiter_core * core) {this -> core = core;}

class dock_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * source_module = 0;
		PrologElement * source_port = 0;
		PrologElement * destination_module = 0;
		PrologElement * destination_port = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) if (destination_module == 0) destination_module = el; else source_module = el;
			if (el -> isInteger ()) if (destination_port == 0) destination_port = el; else source_port = el;
			if (el -> isText ()) if (destination_port == 0) destination_port = el; else source_port = el;
			parameters = parameters -> getRight ();
		}
		if (destination_module == 0 || source_module == 0) return false;
		PrologNativeCode * destination_machine = destination_module -> getAtom () -> getMachine ();
		if (destination_machine == 0) return false;
		if (! destination_machine -> isTypeOf (PrologNativeOrbiter :: name ())) return false;
		PrologNativeCode * source_machine = source_module -> getAtom () -> getMachine ();
		if (source_machine == 0) return false;
		if (! source_machine -> isTypeOf (PrologNativeOrbiter :: name ())) return false;
		int destination_port_index = 0;
		if (destination_port != 0) {
			if (destination_port -> isInteger ()) destination_port_index = destination_port -> getInteger ();
			if (destination_port -> isText ()) destination_port_index = ((PrologNativeOrbiter *) destination_machine) -> module -> inputIndex (destination_port -> getText ());
		}
		int source_port_index = 0;
		if (source_port != 0) {
			if (source_port -> isInteger ()) source_port_index = source_port -> getInteger ();
			if (source_port -> isText ()) source_port_index = ((PrologNativeOrbiter *) source_machine) -> module -> outputIndex (source_port -> getText ());
		}
		return ((PrologNativeOrbiter *) destination_machine) -> module -> connect (destination_port_index, ((PrologNativeOrbiter *) source_machine) -> module, source_port_index);
		//if (input -> isInteger () && port -> isInteger ())
		//	return ((PrologNativeOrbiter *) destination_machine) -> module -> connect (input -> getInteger (), ((PrologNativeOrbiter *) source_machine) -> module, port -> getInteger ());
		//if (input -> isText () && port -> isText ())
		//	return ((PrologNativeOrbiter *) destination_machine) -> module -> connect (input -> getText (), ((PrologNativeOrbiter *) source_machine) -> module, port -> getText ());
	}
};

class undock_class : public PrologNativeCode {
public:
};

void PrologLunarServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
	this -> root = root;
	this -> directory = directory;
}

PrologNativeCode * PrologLunarServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "dock") == 0) return new dock_class ();
	if (strcmp (name, "undock") == 0) return new undock_class ();
	if (strcmp (name, "small_keyboard") == 0) return new keyboard_class (root, 1);
	if (strcmp (name, "keyboard") == 0) return new keyboard_class (root, 2);
	if (strcmp (name, "big_keyboard") == 0) return new keyboard_class (root, 3);
	if (strcmp (name, "oscilloscope") == 0) return new oscilloscope_class (& core);
	if (strcmp (name, "moonbase") == 0) return new moonbase_class (& core);
	if (strcmp (name, "operator") == 0) return new operator_class (& core);
	return 0;
}
