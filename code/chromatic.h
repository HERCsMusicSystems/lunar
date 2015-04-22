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

#ifndef _PROLOG_LUNAR_CHROMATIC_
#define _PROLOG_LUNAR_CHROMATIC_

#include "prolog.h"

class chromatograph {
private:
	PrologAtom * cbb, * cb, * c, * cx, * cxx;
	PrologAtom * dbb, * db, * d, * dx, * dxx;
	PrologAtom * ebb, * eb, * e, * ex, * exx;
	PrologAtom * fbb, * fb, * f, * fx, * fxx;
	PrologAtom * gbb, * gb, * g, * gx, * gxx;
	PrologAtom * abb, * ab, * a, * ax, * axx;
	PrologAtom * bbb, * bb, * b, * bx, * bxx;
public:
	bool get_channel (PrologElement * * parameters, int * channel);
	bool get_key (PrologElement * * parameters, int * key);
	bool get_velocity (PrologElement * * parameters, double * velocity);
	int chromatic (PrologAtom * atom);
	chromatograph (PrologDirectory * dir);
};

#endif

