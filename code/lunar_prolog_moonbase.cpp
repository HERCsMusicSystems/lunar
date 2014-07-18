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

#include "lunar_moonbase.h"
#include "prolog_lunar.h"
#include "multiplatform_audio.h"

class moonbase_action : public PrologNativeCode {
public:
	PrologAtom * atom;
	moonbase * moon_base;
	orbiter_core core;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {if (atom != 0) atom -> setMachine (0); delete this; return true;}
		return true;
	}
	moonbase_action (PrologAtom * atom, double sampling_frequency, int latency_block_size) {
		this -> atom = atom;
		core . sampling_frequency = sampling_frequency;
		core . latency_block_size = latency_block_size;
		moon_base = create_moonbase (& core);
		moon_base -> hold ();
		printf ("Moonbase prolog created.\n");
		FindWindow (0, "name");
	}
	~ moonbase_action (void) {
		if (moon_base != 0) moon_base -> release (); moon_base = 0;
		printf ("Moonbase prolog destroyed.\n");
	}
};

bool moonbase_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	double sampling_frequency = -1;
	int latency_block_size = -1;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isVar ()) atom = el;
		if (el -> isInteger ()) {
			if (sampling_frequency < 0.0) sampling_frequency = (double) el -> getInteger ();
			else latency_block_size = el -> getInteger ();
		}
		if (el -> isDouble ()) sampling_frequency = el -> getInteger ();
		parameters = parameters -> getRight ();
	}
	if (sampling_frequency < 0.0) sampling_frequency = 44100;
	if (latency_block_size < 16) latency_block_size = 512;
	if (atom == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	moonbase_action * machine = new moonbase_action (atom -> getAtom (), sampling_frequency, latency_block_size);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	return true;
}
