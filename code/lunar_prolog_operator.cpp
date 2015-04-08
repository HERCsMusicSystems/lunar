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

/////////////////////////////////////////////////////////////////////
// This file was created on Monday, 21st July 2014 at 11:24:55 AM. //
/////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"
#include "lunar_operator.h"
#include "lunar_prolog_landers.h"

orbiter * operator_class :: create_orbiter (PrologElement * parameters) {return new lunar_operator (core);}
operator_class :: operator_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * fm4_operator_class :: create_orbiter (PrologElement * parameters) {return new lunar_fm4_block (core);}
fm4_operator_class :: fm4_operator_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * square_operator_class :: create_orbiter (PrologElement * parameters) {
	if (parameters -> isPair ()) {
		parameters = parameters -> getLeft ();
		if (parameters -> isEarth ()) return new lunar_aliased_square_operator (core);
	}
	return lunar_square_operator :: create (core);
}
square_operator_class :: square_operator_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * saw_operator_class :: create_orbiter (PrologElement * parameters) {
	if (parameters -> isPair ()) {
		parameters = parameters -> getLeft ();
		if (parameters -> isEarth ()) return new lunar_aliased_saw_operator (core);
	}
	return lunar_saw_operator :: create (core);
}
saw_operator_class :: saw_operator_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * noise_operator_class :: create_orbiter (PrologElement * parameters) {return new noise_operator (core);}
noise_operator_class :: noise_operator_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * vco_operator_class :: create_orbiter (PrologElement * parameters) {return new vco_operator (core);}
vco_operator_class :: vco_operator_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * sampler_operator_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * wave = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) wave = el;
		parameters = parameters -> getRight ();
	}
	if (wave == 0) return 0;
	PrologNativeCode * machine = wave -> getAtom () -> getMachine ();
	if (machine == 0) return 0;
	if (! machine -> isTypeOf (PrologNativeWaveOrbiter :: name ())) return 0;
	lunar_sampler_operator * sampler = new lunar_sampler_operator (core);
	sampler -> install_wave ((lunar_wave *) ((PrologNativeWaveOrbiter *) machine) -> module);
	return sampler;
}
sampler_operator_class :: sampler_operator_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}

orbiter * filter_class :: create_orbiter (PrologElement * parameters) {return new lunar_filter (core);}
filter_class :: filter_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}
