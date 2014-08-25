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
// This file was created on Monday, 14th July 2014 at 11:50:27 AM. //
/////////////////////////////////////////////////////////////////////

#ifndef _PROLOG_LUNAR_SERVICE_CLASS_
#define _PROLOG_LUNAR_SERVICE_CLASS_

#include "prolog.h"
#include "lunar.h"

class PrologLunarServiceClass;

class PrologNativeOrbiter : public PrologNativeCode {
public:
	PrologAtom * atom;
	orbiter_core * core;
	orbiter * module;
	static char * name (void);
	virtual char * codeName (void);
	virtual bool isTypeOf (char * code_name);
	virtual bool code (PrologElement * parameters, PrologResolution * resolution);
	bool onEarth (void);
	PrologNativeOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module);
	~ PrologNativeOrbiter (void);
};

class PrologNativeOrbiterCreator : public PrologNativeCode {
public:
	orbiter_core * core;
	virtual orbiter * create_orbiter (PrologElement * parameters) = 0;
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	virtual void code_created (PrologNativeOrbiter * machine);
	virtual bool code (PrologElement * parameters, PrologResolution * resolution);
	PrologNativeOrbiterCreator (orbiter_core * core);
};

class keyboard_class : public PrologNativeCode {
public:
	int size;
	PrologRoot * root;
	PrologDirectory * directory;
	bool code (PrologElement * parameters, PrologResolution * resolution);
	keyboard_class (PrologRoot * root, PrologDirectory * directory, int size);
};

class oscilloscope_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	virtual void code_created (PrologNativeOrbiter * machine);
	oscilloscope_class (orbiter_core * core);
};

class core_class : public PrologNativeCode {
public:
	orbiter_core * core;
	bool code (PrologElement * parameters, PrologResolution * resolution);
	core_class (orbiter_core * core);
};

class operator_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	operator_class (orbiter_core * core);
};

class square_operator_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	square_operator_class (orbiter_core * core);
};

class saw_operator_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	saw_operator_class (orbiter_core * core);
};

class noise_operator_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	noise_operator_class (orbiter_core * core);
};

class sampler_operator_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameter);
	sampler_operator_class (orbiter_core * core);
};

class PrologLunarServiceClass : public PrologServiceClass {
public:
	PrologRoot * root;
	PrologDirectory * directory;
	orbiter_core core;
	void init (PrologRoot * root, PrologDirectory * directory);
	PrologNativeCode * getNativeCode (char * name);
};

#endif

