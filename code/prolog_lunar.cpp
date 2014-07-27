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
#include <string.h>

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
	module -> activate ();
	module -> hold ();
	printf ("	NATIVE ORBITER CREATED\n");
}

PrologNativeOrbiter :: ~ PrologNativeOrbiter (void) {
	if (module != 0) module -> release (); module = 0;
	printf ("	NATIVE ORBITER DESTROYED\n");
}

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
	PrologNativeOrbiter * machine = new PrologNativeOrbiter (atom -> getAtom (), core, create_orbiter ());
	if (atom -> getAtom () -> setMachine (machine)) return true;
	delete machine;
	return false;
}

PrologNativeOrbiterCreator :: PrologNativeOrbiterCreator (orbiter_core * core) {this -> core = core;}

class dock_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * destination = 0;
		PrologElement * source = 0;
		PrologElement * input = 0;
		PrologElement * port = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) if (destination == 0) destination = el; else source = el;
			if (el -> isInteger ()) if (input == 0) input = el; else; port = el;
			if (el -> isText ()) if (input == 0) input = el; else port = el;
			parameters = parameters -> getRight ();
		}
		if (destination == 0 || source == 0 || input == 0 || port == 0) return false;
		PrologNativeCode * destination_machine = destination -> getAtom () -> getMachine ();
		if (! destination_machine -> isTypeOf (PrologNativeOrbiter :: name ())) return false;
		PrologNativeCode * source_machine = source -> getAtom () -> getMachine ();
		if (! source_machine -> isTypeOf (PrologNativeOrbiter :: name ())) return false;
		if (input -> isInteger () && port -> isInteger ())
			return ((PrologNativeOrbiter *) destination_machine) -> module -> connect (input -> getInteger (), ((PrologNativeOrbiter *) source_machine) -> module, port -> getInteger ());
		if (input -> isText () && port -> isText ())
			return ((PrologNativeOrbiter *) destination_machine) -> module -> connect (input -> getText (), ((PrologNativeOrbiter *) source_machine) -> module, port -> getText ());
		return false;
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
	if (strcmp (name, "oscilloscope") == 0) return new oscilloscope_class ();
	if (strcmp (name, "moonbase") == 0) return new moonbase_class (& core);
	if (strcmp (name, "operator") == 0) return new operator_class (& core);
	return 0;
}

