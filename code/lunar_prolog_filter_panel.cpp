///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
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
// This file was created on Wednesday, 5th May 2015 at 21:20:38 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class filter_panel_action : public AudioModulePanel {
public:
	PrologAtom * freq, * resonance, * amp;
	knob_active_graphics FREQ, RESONANCE, AMP;
	void redraw (cairo_t * cr) {FREQ . draw (cr); RESONANCE . draw (cr); AMP . draw (cr);}
	void MouseKeyon (point location, int button) {FREQ . keyon (location); RESONANCE . keyon (location); AMP . keyon (location);}
	void MouseKeyoff (point location, int button) {FREQ . keyoff (location); RESONANCE . keyoff (location); AMP . keyoff (location);}
	void MouseMove (point delta) {
		bool redraw = false;
		if (FREQ . move (delta)) {move (0); redraw = true;}
		if (RESONANCE . move (delta)) {move (1); redraw = true;}
		if (AMP . move (delta)) {move (2); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = freq; v = FREQ . value; break;
		case 1: a = resonance; v = RESONANCE . value; break;
		case 2: a = amp; v = AMP . value; break;
		default: a = 0; break;
		}
		if (a == 0) return;
		PrologElement * query = root -> pair (root -> atom (a), root -> pair (root -> Double (v), root -> earth ()));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void feedback (void) {
		PrologElement * variables = root -> pair (root -> var (0),
									root -> pair (root -> var (1),
									root -> pair (root -> var (2), root -> earth ())));
		PrologElement * query = root -> earth ();
		query = root -> pair (root -> pair (root -> atom (amp), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (resonance), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RESONANCE . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP . setValue (sub -> getNumber ());
		}
		delete query;
	}
	filter_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom,
		PrologAtom * freq, PrologAtom * resonance, PrologAtom * amp, bool active) :
	FREQ (point (18, 10), 0, resources, true, active, -8192.0, 8192.0),
	RESONANCE (point (88, 10), 0, resources, true, active, 0.0, 16384.0),
	AMP (point (158, 10), 0, resources, true, active, -16384.0, 0.0),
	AudioModulePanel (root, atom, resources != 0 ? resources -> filter_panel_surface : 0) {
		this -> freq = freq; COLLECTOR_REFERENCE_INC (freq);
		this -> resonance = resonance; COLLECTOR_REFERENCE_INC (resonance);
		this -> amp = amp; COLLECTOR_REFERENCE_INC (amp);
		feedback ();
	}
	~ filter_panel_action (void) {freq -> removeAtom (); resonance -> removeAtom (); amp -> removeAtom ();}
};

bool filter_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * freq = 0, * resonance = 0, * amp = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (freq == 0) freq = el;
			else if (resonance == 0) resonance = el;
			else if (amp == 0) amp = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || amp == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	filter_panel_action * machine = new filter_panel_action (resources, root, atom -> getAtom (),
															freq -> getAtom (), resonance -> getAtom (), amp -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

filter_panel_class :: filter_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}

