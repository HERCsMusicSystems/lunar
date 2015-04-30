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

///////////////////////////////////////////////////////////////////////
// This file was created on Friday, 29th August 2014 at 11:45:49 AM. //
///////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class vector_action : public AudioModulePanel {
public:
	PrologAtom * command;
	bool on;
	cairo_surface_t * handle;
	point position;
	void action (void) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> Double (1.0 - position . x / 64.0),
								root -> pair (root -> Double (-1.0 + position . y / 64.0),
								root -> earth ())));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void feedback (void) {}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return remove ();
		PrologElement * x = 0;
		PrologElement * y = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isNumber ()) if (x == 0) x = el; else y = el;
			parameters = parameters -> getRight ();
		}
		if (x == 0 || y == 0) return true;
		if (x -> isInteger () && y -> isInteger ()) {
			location = point (x -> getNumber (), y -> getNumber ());
			reposition ();
		}
		if (x -> isDouble () && y -> isDouble ()) {
			double xx = x -> getDouble (); if (xx < -1.0) xx = -1.0; if (xx > 1.0) xx = 1.0;
			double yy = y -> getDouble (); if (yy < -1.0) yy = -1.0; if (yy > 1.0) yy = 1.0;
			position = point (64.0 - xx * 64, 64.0 + yy * 64.0);
			update ();
		}
		return true;
	}
	void redraw (cairo_t * cr) {
		cairo_surface_t * sub = cairo_surface_create_for_rectangle (handle, position . x, position . y, 129.0, 129.0);
		cairo_set_source_surface (cr, sub, 23.0, 24.0);
		cairo_paint (cr);
		cairo_surface_destroy (sub);
	}
	void MouseKeyon (point location, int button) {on = true;}
	void MouseKeyoff (point location, int button) {on = false;}
	void MouseMove (point delta) {
		if (on) {
			position -= delta * 0.25;
			if (position . x < 0.0) position . x = 0.0; if (position . y < 0.0) position . y = 0.0;
			if (position . x > 128.0) position . x = 128.0; if (position . y > 128.0) position . y = 128.0;
			update ();
			action ();
		}
	}
	void FunctionKey (int key, int state) {}
	vector_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom, PrologAtom * command) :
		AudioModulePanel (root, atom, resources != 0 ? resources -> vector_surface : 0), position (64.0, 64.0) {
		on = false;
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		this -> handle = resources -> vector_handle;
	}
	~ vector_action (void) {command -> removeAtom ();}
};

bool vector_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * command = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) {if (atom == 0) atom = el; else command = el;}
		if (el -> isVar ()) {if (atom != 0) command = atom; atom = el;}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || command == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	vector_action * machine = new vector_action (resources, root, atom -> getAtom (), command -> getAtom ());
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

vector_class :: vector_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}

