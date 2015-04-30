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

/////////////////////////////////////////////////////////////////////////
// This file was created on Wednesday, 27th November 2014 at 14:57:20. //
/////////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class adsr_panel_action : public AudioModulePanel {
public:
	PrologAtom * a, * d, * s, * r;
	knob_active_graphics A, D, S, R;
	void redraw (cairo_t * cr) {A . draw (cr); D . draw (cr); S . draw (cr); R . draw (cr);}
	void move (int ind) {
		if (a == 0) return;
		PrologElement * query;
		if (d != 0 && s != 0 && r != 0) {
			switch (ind) {
			case 0: query = root -> pair (root -> atom (a), root -> pair (root -> Double (A . value), root -> earth ())); break;
			case 1: query = root -> pair (root -> atom (d), root -> pair (root -> Double (D . value), root -> earth ())); break;
			case 2: query = root -> pair (root -> atom (s), root -> pair (root -> Double (S . value), root -> earth ())); break;
			default: query = root -> pair (root -> atom (r), root -> pair (root -> Double (R . value), root -> earth ())); break;
			}
			query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		} else {
			query = root -> pair (root -> atom (a),
					root -> pair (root -> Double (A . value),
					root -> pair (root -> Double (D . value),
					root -> pair (root -> Double (S . value),
					root -> pair (root -> Double (R . value), root -> earth ())))));
			query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		}
		root -> resolution (query);
		delete query;
	}
	void feedback (void) {
		if (a == 0) return;
		PrologElement * variables = root -> pair (root -> var (0),
									root -> pair (root -> var (1),
									root -> pair (root -> var (2),
									root -> pair (root -> var (3), root -> earth ()))));
		PrologElement * query;
		if (d != 0 && s != 0 && r != 0) {
			query = root -> pair (root -> pair (root -> atom (r), root -> pair (root -> var (3), root -> earth ())), root -> earth ());
			query = root -> pair (root -> pair (root -> atom (s), root -> pair (root -> var (2), root -> earth ())), query);
			query = root -> pair (root -> pair (root -> atom (d), root -> pair (root -> var (1), root -> earth ())), query);
			query = root -> pair (root -> pair (root -> atom (a), root -> pair (root -> var (0), root -> earth ())), query);
			query = root -> pair (variables, query);
		} else {
			query = root -> pair (root -> atom (a),
					root -> pair (root -> var (0),
					root -> pair (root -> var (1),
					root -> pair (root -> var (2),
					root -> pair (root -> var (3), root -> earth ())))));
			query = root -> pair (variables, root -> pair (query, root -> earth ()));
		}
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) A . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;}
			sub = el -> getLeft ();
			if (sub -> isNumber ()) D . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;}
			sub = el -> getLeft ();
			if (sub -> isNumber ()) S . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;}
			sub = el -> getLeft ();
			if (sub -> isNumber ()) R . setValue (sub -> getNumber ());
		}
		delete query;
	}
	void MouseKeyon (point location, int button) {A . keyon (location); D . keyon (location); S . keyon (location); R . keyon (location);}
	void MouseKeyoff (point location, int button) {A . keyoff (location); D . keyoff (location); S . keyoff (location); R . keyoff (location);}
	void MouseMove (point delta) {
		bool redraw = false;
		if (A . move (delta)) {move (0); redraw = true;}
		if (D . move (delta)) {move (1); redraw = true;}
		if (S . move (delta)) {move (2); redraw = true;}
		if (R . move (delta)) {move (3); redraw = true;}
		if (redraw) gtk_widget_queue_draw (viewport);
	}
	void FunctionKey (int key, int state) {}
	adsr_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom,
											PrologAtom * a, PrologAtom * d, PrologAtom * s, PrologAtom * r, bool active) :
	A (point (18, 10), 0, resources, true, active, 0.0, 16384.0),
	D (point (88, 10), 0, resources, true, active, 0.0, 16384.0),
	S (point (158, 10), 0, resources, true, active, -16384.0, 0.0),
	R (point (228, 10), 0, resources, true, active, 0.0, 16384.0),
	AudioModulePanel (root, atom, resources != 0 ? resources -> adsr_panel_surface : 0) {
		this -> a = a; COLLECTOR_REFERENCE_INC (a);
		this -> d = d; if (d != 0) {COLLECTOR_REFERENCE_INC (d);}
		this -> s = s; if (s != 0) {COLLECTOR_REFERENCE_INC (s);}
		this -> r = r; if (r != 0) {COLLECTOR_REFERENCE_INC (r);}
		feedback ();
	}
	~ adsr_panel_action (void) {
		a -> removeAtom ();
		if (d != 0) d -> removeAtom ();
		if (s != 0) s -> removeAtom ();
		if (r != 0) r -> removeAtom ();
	}
};

bool adsr_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * a = 0, * d = 0, * s = 0, * r = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (a == 0) a = el;
			else if (d == 0) d = el;
			else if (s == 0) s = el;
			else if (r == 0) r = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || a == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	adsr_panel_action * machine = new adsr_panel_action (resources, root, atom -> getAtom (), a -> getAtom (),
										d != 0 ? d -> getAtom () : 0, s != 0 ? s -> getAtom () : 0, r != 0 ? r -> getAtom () : 0, false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

adsr_panel_class :: adsr_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
