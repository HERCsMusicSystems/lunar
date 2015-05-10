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

//////////////////////////////////////////////////////////////////////
// This file was created on Wednesday, 7th May 2015 at 17:28:10 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class chorus_panel_action : public AudioModulePanel {
public:
	PrologAtom * level, * time, * speed, * amp;
	knob_active_graphics LEVEL, TIME, SPEED, AMP;
	void redraw (cairo_t * cr) {LEVEL . draw (cr); TIME . draw (cr); SPEED . draw (cr); AMP . draw (cr);}
	void MouseKeyon (point location, int button) {LEVEL . keyon (location); TIME . keyon (location); SPEED . keyon (location); AMP . keyon (location);}
	void MouseKeyoff (point location, int button) {LEVEL . keyoff (location); TIME . keyoff (location); SPEED . keyoff (location); AMP . keyoff (location);}
	void MouseMove (point delta) {
		bool redraw = false;
		if (LEVEL . move (delta)) {move (0); redraw = true;}
		if (TIME . move (delta)) {move (1); redraw = true;}
		if (SPEED . move (delta)) {move (2); redraw = true;}
		if (AMP . move (delta)) {move (3); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = level; v = LEVEL . value; break;
		case 1: a = time; v = TIME . value; break;
		case 2: a = speed; v = SPEED . value; break;
		case 3: a = amp; v = AMP . value; break;
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
									root -> pair (root -> var (3), root -> earth ()))));
		PrologElement * query = root -> earth ();
		query = root -> pair (root -> pair (root -> atom (amp), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (speed), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (time), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (level), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) LEVEL . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) TIME . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) SPEED . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP . setValue (sub -> getNumber ());
		}
		delete query;
	}
	chorus_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom,
											PrologAtom * level, PrologAtom * time, PrologAtom * speed, PrologAtom * amp, bool active) :
	LEVEL (point (18, 10), 0, resources, true, active, 0.0, 16384.0),
	TIME (point (88, 10), 0, resources, true, active, 0.0, 16384.0),
	SPEED (point (158, 10), 0, resources, true, active, -8192.0, 8192.0),
	AMP (point (228, 10), 0, resources, true, active, 0.0, 16384.0),
	AudioModulePanel (root, atom, resources != 0 ? resources -> chorus_panel_surface : 0) {
		this -> level = level; COLLECTOR_REFERENCE_INC (level);
		this -> time = time; COLLECTOR_REFERENCE_INC (time);
		this -> speed = speed; COLLECTOR_REFERENCE_INC (speed);
		this -> amp = amp; COLLECTOR_REFERENCE_INC (amp);
		feedback ();
	}
	~ chorus_panel_action (void) {
		level -> removeAtom ();
		time -> removeAtom ();
		speed -> removeAtom ();
		amp -> removeAtom ();
	}
};

bool chorus_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * level = 0, * time = 0, * speed = 0, * amp = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (level == 0) level = el;
			else if (time == 0) time = el;
			else if (speed == 0) speed = el;
			else if (amp == 0) amp = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || amp == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	chorus_panel_action * machine = new chorus_panel_action (resources, root, atom -> getAtom (), level -> getAtom (),
										time -> getAtom (), speed -> getAtom (), amp -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

chorus_panel_class :: chorus_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}

