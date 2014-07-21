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

#ifndef _PROLOG_LUNAR_SERVICE_CLASS_
#define _PROLOG_LUNAR_SERVICE_CLASS_

#include "prolog.h"
#include "lunar.h"

class PrologLunarServiceClass;

class keyboard_class : public PrologNativeCode {
public:
	int size;
	PrologRoot * root;
	bool code (PrologElement * parameters, PrologResolution * resolution);
	keyboard_class (PrologRoot * root, int size);
};

class oscilloscope_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution);
};

class moonbase_class : public PrologNativeCode {
public:
	orbiter_core * core;
	bool code (PrologElement * parameters, PrologResolution * resolution);
	moonbase_class (orbiter_core * core);
};

class operator_class : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution);
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
