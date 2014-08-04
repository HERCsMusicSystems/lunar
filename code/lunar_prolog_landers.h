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

#ifndef _LUNAR_PROLOG_LANDERS_
#define _LUNAR_PROLOG_LANDERS_

#include "prolog_lunar.h"
#include "lunar_landers.h"

class parameter_block_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	parameter_block_class (orbiter_core * core);
};

class PrologNativeKeyOrbiter : public PrologNativeOrbiter {
public:
	static char * name (void);
	virtual bool isTypeOf (char * code_name);
	PrologNativeKeyOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module);
};

class key_map_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	key_map_class (orbiter_core * core);
};

class impulse_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	impulse_class (orbiter_core * core);
};

class trigger_class : public PrologNativeOrbiterCreator {
private:
	PrologDirectory * dir;
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	trigger_class (PrologDirectory * dir, orbiter_core * core);
};

class mixer_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	mixer_class (orbiter_core * core);
};

class gateway_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parmaeters);
	gateway_class (orbiter_core * core);
};

class lfo_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	lfo_class (orbiter_core * core);
};

#endif

