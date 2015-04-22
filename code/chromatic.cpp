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

#include "chromatic.h"

bool chromatograph :: get_channel (PrologElement * * parameters, int * channel) {
	if (! (* parameters) -> isPair ()) return false;
	PrologElement * el = (* parameters) -> getLeft ();
	if (! el -> isInteger ()) return false;
	int ch = el -> getInteger ();
	if (ch < 0 || ch > 15) return false;
	* channel = ch;
	* parameters = (* parameters) -> getRight ();
	return true;
}

bool chromatograph :: get_key (PrologElement * * parameters, int * key) {
	if (! (* parameters) -> isPair ()) return false;
	PrologElement * el = (* parameters) -> getLeft ();
	if (el -> isInteger ()) {* key = el -> getInteger (); * parameters = (* parameters) -> getRight (); return true;}
	if (! el -> isPair ()) return false;
	PrologElement * note = el -> getLeft ();
	if (! note -> isAtom ()) return false;
	el = el -> getRight ();
	if (! el -> isPair ()) return false;
	PrologElement * octave = el -> getLeft ();
	if (! octave -> isInteger ()) return false;
	* key = chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
	* parameters = (* parameters) -> getRight ();
	return true;
}

bool chromatograph :: get_velocity (PrologElement * * parameters, double * velocity) {
	if (! (* parameters) -> isPair ()) return false;
	PrologElement * el = (* parameters) -> getLeft ();
	if (! el -> isNumber ()) return false;
	* velocity = el -> getNumber ();
	* parameters = (* parameters) -> getRight ();
	return true;
}

int chromatograph :: chromatic (PrologAtom * atom) {
	if (atom == cbb) return -2;
	if (atom == cb) return -1;
	if (atom == c || atom == dbb) return 0;
	if (atom == cx || atom == db) return 1;
	if (atom == cxx || atom == d || atom == ebb) return 2;
	if (atom == dx || atom == eb || atom == fbb) return 3;
	if (atom == dxx || atom == e || atom == fb) return 4;
	if (atom == ex || atom == f || atom == gbb) return 5;
	if (atom == exx || atom == fx || atom == gb) return 6;
	if (atom == fxx || atom == g || atom == abb) return 7;
	if (atom == gx || atom == ab) return 8;
	if (atom == gxx || atom == a || atom == bbb) return 9;
	if (atom == ax || atom == bb || atom == cbb) return 10;
	if (atom == axx || atom == b || atom == cb) return 11;
	if (atom == bx) return 12;
	if (atom == bxx) return 13;
	return 0;
}

chromatograph :: chromatograph (PrologDirectory * dir) {
	cbb = cb = c = cx = cxx = 0;
	dbb = db = d = dx = dxx = 0;
	ebb = eb = e = ex = exx = 0;
	fbb = fb = f = fx = fxx = 0;
	gbb = gb = g = gx = gxx = 0;
	abb = ab = a = ax = axx = 0;
	bbb = bb = b = bx = bxx = 0;
	if (dir == 0) return;
	c = dir -> searchAtom ("C"); cb = dir -> searchAtom ("Cb"); cbb = dir -> searchAtom ("Cbb"); cx = dir -> searchAtom ("C#"); cxx = dir -> searchAtom ("Cx");
	d = dir -> searchAtom ("D"); db = dir -> searchAtom ("Db"); dbb = dir -> searchAtom ("Dbb"); dx = dir -> searchAtom ("D#"); dxx = dir -> searchAtom ("Dx");
	e = dir -> searchAtom ("E"); eb = dir -> searchAtom ("Eb"); ebb = dir -> searchAtom ("Ebb"); ex = dir -> searchAtom ("E#"); exx = dir -> searchAtom ("Ex");
	f = dir -> searchAtom ("F"); fb = dir -> searchAtom ("Fb"); fbb = dir -> searchAtom ("Fbb"); fx = dir -> searchAtom ("F#"); fxx = dir -> searchAtom ("Fx");
	g = dir -> searchAtom ("G"); gb = dir -> searchAtom ("Gb"); gbb = dir -> searchAtom ("Gbb"); gx = dir -> searchAtom ("G#"); gxx = dir -> searchAtom ("Gx");
	a = dir -> searchAtom ("A"); ab = dir -> searchAtom ("Ab"); abb = dir -> searchAtom ("Abb"); ax = dir -> searchAtom ("A#"); axx = dir -> searchAtom ("Ax");
	b = dir -> searchAtom ("B"); bb = dir -> searchAtom ("Bb"); bbb = dir -> searchAtom ("Bbb"); bx = dir -> searchAtom ("B#"); bxx = dir -> searchAtom ("Bx");
}

