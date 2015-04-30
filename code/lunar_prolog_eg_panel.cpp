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
// This file was created on Wednesday, 6th March 2015 at 09:57:59. //
/////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class eg_panel_action : public AudioModulePanel {
public:
	PrologAtom * t1, * t2, * t3, * t4, * l1, * l2, * l3, * l4;
	bool feg;
	knob_active_graphics T1, T2, T3, T4, L1, L2, L3, L4;
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = t1; v = T1 . value; break;
		case 1: a = t2; v = T2 . value; break;
		case 2: a = t3; v = T3 . value; break;
		case 3: a = t4; v = T4 . value; break;
		case 4: a = l1; v = L1 . value; break;
		case 5: a = l2; v = L2 . value; break;
		case 6: a = l3; v = L3 . value; break;
		case 7: a = l4; v = L4 . value; break;
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
									root -> pair (root -> var (2),
									root -> pair (root -> var (3),
									root -> pair (root -> var (4),
									root -> pair (root -> var (5),
									root -> pair (root -> var (6),
									root -> pair (root -> var (7), root -> earth ()))))))));
		PrologElement * query;
		query = root -> pair (root -> pair (root -> atom (l4), root -> pair (root -> var (7), root -> earth ())), root -> earth ());
		query = root -> pair (root -> pair (root -> atom (l3), root -> pair (root -> var (6), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (l2), root -> pair (root -> var (5), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (l1), root -> pair (root -> var (4), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t4), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t3), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t2), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t1), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) T1 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) T2 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) T3 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) T4 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L1 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L2 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L3 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L4 . setValue (sub -> getNumber ());
		}
		delete query;
	}
	void redraw (cairo_t * cr) {
		T1 . draw (cr); T2 . draw (cr); T3 . draw (cr); T4 . draw (cr);
		L1 . draw (cr); L2 . draw (cr); L3 . draw (cr); L4 . draw (cr);
	}
	void MouseKeyon (point location, int button) {
		T1 . keyon (location); T2 . keyon (location); T3 . keyon (location); T4 . keyon (location);
		L1 . keyon (location); L2 . keyon (location); L3 . keyon (location); L4 . keyon (location);
	}
	void MouseKeyoff (point location, int button) {
		T1 . keyoff (location); T2 . keyoff (location); T3 . keyoff (location); T4 . keyoff (location);
		L1 . keyoff (location); L2 . keyoff (location); L3 . keyoff (location); L4 . keyoff (location);
	}
	void MouseMove (point delta) {
		bool redraw = false;
		if (T1 . move (delta)) {move (0); redraw = true;}
		if (T2 . move (delta)) {move (1); redraw = true;}
		if (T3 . move (delta)) {move (2); redraw = true;}
		if (T4 . move (delta)) {move (3); redraw = true;}
		if (L1 . move (delta)) {move (4); redraw = true;}
		if (L2 . move (delta)) {move (5); redraw = true;}
		if (L3 . move (delta)) {move (6); redraw = true;}
		if (L4 . move (delta)) {move (7); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	eg_panel_action (GraphicResources * resources, PrologRoot * root, bool feg, PrologAtom * atom,
		PrologAtom * t1, PrologAtom * t2, PrologAtom * t3, PrologAtom * t4,
		PrologAtom * l1, PrologAtom * l2, PrologAtom * l3, PrologAtom * l4, bool active) :
	T1 (point (18, 10), 0, resources, true, active, 0.0, 16384.0),
	T2 (point (88, 10), 0, resources, true, active, 0.0, 16384.0),
	T3 (point (158, 10), 0, resources, true, active, 0.0, 16384.0),
	T4 (point (228, 10), 0, resources, true, active, 0.0, 16384.0),
	L1 (point (18, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0),
	L2 (point (88, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0),
	L3 (point (158, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0),
	L4 (point (228, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0),
	AudioModulePanel (root, atom, resources != 0 ? resources -> eg_panel_surface : 0) {
		this -> feg = feg;
		this -> t1 = t1; COLLECTOR_REFERENCE_INC (t1);
		this -> t2 = t2; COLLECTOR_REFERENCE_INC (t2);
		this -> t3 = t3; COLLECTOR_REFERENCE_INC (t3);
		this -> t4 = t4; COLLECTOR_REFERENCE_INC (t4);
		this -> l1 = l1; COLLECTOR_REFERENCE_INC (l1);
		this -> l2 = l2; COLLECTOR_REFERENCE_INC (l2);
		this -> l3 = l3; COLLECTOR_REFERENCE_INC (l3);
		this -> l4 = l4; COLLECTOR_REFERENCE_INC (l4);
		feedback ();
	}
	~ eg_panel_action (void) {
		t1 -> removeAtom (); t2 -> removeAtom (); t3 -> removeAtom (); t4 -> removeAtom ();
		l1 -> removeAtom (); l2 -> removeAtom (); l3 -> removeAtom (); l4 -> removeAtom ();
	}
};

bool eg_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * t1 = 0, * t2 = 0, * t3 = 0, * t4 = 0;
	PrologElement * l1 = 0, * l2 = 0, * l3 = 0, * l4 = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (t1 == 0) t1 = el;
			else if (t2 == 0) t2 = el;
			else if (t3 == 0) t3 = el;
			else if (t4 == 0) t4 = el;
			else if (l1 == 0) l1 = el;
			else if (l2 == 0) l2 = el;
			else if (l3 == 0) l3 = el;
			else if (l4 == 0) l4 = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || l4 == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	eg_panel_action * machine = new eg_panel_action (resources, root, feg, atom -> getAtom (),
		t1 -> getAtom (), t2 -> getAtom (), t3 -> getAtom (), t4 -> getAtom (),
		l1 -> getAtom (), l2 -> getAtom (), l3 -> getAtom (), l4 -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

eg_panel_class :: eg_panel_class (PrologLunarServiceClass * servo, bool feg) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
	this -> feg = feg;
}
