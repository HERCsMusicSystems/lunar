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
// This file was created on Wednesday, 6th May 2015 at 21:49:37 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class delay_panel_action : public AudioModulePanel {
public:
	PrologAtom * feed, * time, * highdamp;
	knob_active_graphics FEEDBACK, TIME, HIGHDAMP;
	void redraw (cairo_t * cr) {FEEDBACK . draw (cr); TIME . draw (cr); HIGHDAMP . draw (cr);}
	void MouseKeyon (point location, int button) {FEEDBACK . keyon (location); TIME . keyon (location); HIGHDAMP . keyon (location);}
	void MouseKeyoff (point location, int button) {FEEDBACK . keyoff (location); TIME . keyoff (location); HIGHDAMP . keyoff (location);}
	void MouseMove (point delta) {
		bool redraw = false;
		if (FEEDBACK . move (delta)) {move (0); redraw = true;}
		if (TIME . move (delta)) {move (1); redraw = true;}
		if (HIGHDAMP . move (delta)) {move (2); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = feed; v = FEEDBACK . value; break;
		case 1: a = time; v = TIME . value; break;
		case 2: a = highdamp; v = HIGHDAMP . value; break;
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
		query = root -> pair (root -> pair (root -> atom (highdamp), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (time), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feed), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) TIME . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) HIGHDAMP . setValue (sub -> getNumber ());
		}
		delete query;
	}
	delay_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom,
		PrologAtom * feed, PrologAtom * time, PrologAtom * highdamp, bool active) :
	FEEDBACK (point (18, 10), 0, resources, true, active, 0.0, 16384.0),
	TIME (point (88, 10), 0, resources, true, active, 0.0, 16384.0),
	HIGHDAMP (point (158, 10), 0, resources, true, active, 0.0, 16384.0),
	AudioModulePanel (root, atom, resources != 0 ? resources -> delay_panel_surface : 0) {
		this -> feed = feed; COLLECTOR_REFERENCE_INC (feed);
		this -> time = time; COLLECTOR_REFERENCE_INC (time);
		this -> highdamp = highdamp; COLLECTOR_REFERENCE_INC (highdamp);
		feedback ();
	}
	~ delay_panel_action (void) {feed -> removeAtom (); time -> removeAtom (); highdamp -> removeAtom ();}
};

bool delay_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * freq = 0, * resonance = 0, * amp = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (freq == 0) freq = el;
			else if (resonance == 0) resonance = el;
			else amp = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || amp == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	delay_panel_action * machine = new delay_panel_action (resources, root, atom -> getAtom (),
															freq -> getAtom (), resonance -> getAtom (), amp -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

delay_panel_class :: delay_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}

