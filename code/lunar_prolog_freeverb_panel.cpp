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
// This file was created on Wednesday, 8th May 2015 at 17:48:05 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class freeverb_panel_action : public AudioModulePanel {
public:
	PrologAtom * feed, * diffusion, * highdamp, * dry, * wet;
	knob_active_graphics FEEDBACK, DIFFUSION, HIGHDAMP, DRY, WET;
	void redraw (cairo_t * cr) {FEEDBACK . draw (cr); DIFFUSION . draw (cr); HIGHDAMP . draw (cr); DRY . draw (cr); WET . draw (cr);}
	void MouseKeyon (point location, int button) {
		FEEDBACK . keyon (location); DIFFUSION . keyon (location); HIGHDAMP . keyon (location); DRY . keyon (location); WET . keyon (location);
	}
	void MouseKeyoff (point location, int button) {
		FEEDBACK . keyoff (location); DIFFUSION . keyoff (location); HIGHDAMP . keyoff (location); DRY . keyoff (location); WET . keyoff (location);
	}
	void MouseMove (point delta) {
		bool redraw = false;
		if (FEEDBACK . move (delta)) {move (0); redraw = true;}
		if (DIFFUSION . move (delta)) {move (1); redraw = true;}
		if (HIGHDAMP . move (delta)) {move (2); redraw = true;}
		if (DRY . move (delta)) {move (3); redraw = true;}
		if (WET . move (delta)) {move (4); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = feed; v = FEEDBACK . value; break;
		case 1: a = diffusion; v = DIFFUSION . value; break;
		case 2: a = highdamp; v = HIGHDAMP . value; break;
		case 3: a = dry; v = DRY . value; break;
		case 4: a = wet; v = WET . value; break;
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
									root -> pair (root -> var (4), root -> earth ())))));
		PrologElement * query = root -> earth ();
		query = root -> pair (root -> pair (root -> atom (wet), root -> pair (root -> var (4), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (dry), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (highdamp), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (diffusion), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feed), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) DIFFUSION . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) HIGHDAMP . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) DRY . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) WET . setValue (sub -> getNumber ());
		}
		delete query;
	}
	freeverb_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom,
											PrologAtom * feed, PrologAtom * diffusion, PrologAtom * highdamp, PrologAtom * dry, PrologAtom * wet, bool active) :
	FEEDBACK (point (18, 10), 0, resources, true, active, 0.0, 16384.0),
	DIFFUSION (point (88, 10), 0, resources, true, active, 0.0, 8192.0),
	HIGHDAMP (point (158, 10), 0, resources, true, active, 0.0, 16384.0),
	DRY (point (228, 10), 0, resources, true, active, 0.0, 16384.0),
	WET (point (298, 10), 0, resources, true, active, 0.0, 16384.0),
	AudioModulePanel (root, atom, resources != 0 ? resources -> freeverb_panel_surface : 0) {
		this -> feed = feed; COLLECTOR_REFERENCE_INC (feed);
		this -> diffusion = diffusion; COLLECTOR_REFERENCE_INC (diffusion);
		this -> highdamp = highdamp; COLLECTOR_REFERENCE_INC (highdamp);
		this -> dry = dry; COLLECTOR_REFERENCE_INC (dry);
		this -> wet = wet; COLLECTOR_REFERENCE_INC (wet);
		feedback ();
	}
	~ freeverb_panel_action (void) {
		feed -> removeAtom ();
		diffusion -> removeAtom ();
		highdamp -> removeAtom ();
		dry -> removeAtom ();
		wet -> removeAtom ();
	}
};

bool freeverb_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * feedback = 0, * diffusion = 0, * highdamp = 0, * dry = 0, * wet = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (feedback == 0) feedback = el;
			else if (diffusion == 0) diffusion = el;
			else if (highdamp == 0) highdamp = el;
			else if (dry == 0) dry = el;
			else if (wet == 0) wet = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || wet == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	freeverb_panel_action * machine = new freeverb_panel_action (resources, root, atom -> getAtom (), feedback -> getAtom (),
										diffusion -> getAtom (), highdamp -> getAtom (), dry -> getAtom (), wet -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

freeverb_panel_class :: freeverb_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}

