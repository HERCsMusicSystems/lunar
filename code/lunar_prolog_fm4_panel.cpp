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
// This file was created on Wednesday, 12th March 2015 at 13:25:55. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class fm4_panel_action : public AudioModulePanel {
public:
	PrologAtom * algo;
	PrologAtom * freq1, * amp1, * ratio1, * feedback1;
	PrologAtom * freq2, * amp2, * ratio2, * feedback2;
	PrologAtom * freq3, * amp3, * ratio3, * feedback3;
	PrologAtom * freq4, * amp4, * ratio4, * feedback4;
	knob_active_graphics ALGO;
	knob_active_graphics FREQ1, AMP1, RATIO1, FEEDBACK1;
	knob_active_graphics FREQ2, AMP2, RATIO2, FEEDBACK2;
	knob_active_graphics FREQ3, AMP3, RATIO3, FEEDBACK3;
	knob_active_graphics FREQ4, AMP4, RATIO4, FEEDBACK4;
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = algo; v = ALGO . value; break;
		case 1: a = freq1; v = FREQ1 . value; break;
		case 2: a = amp1; v = AMP1 . value; break;
		case 3: a = ratio1; v = RATIO1 . value; break;
		case 4: a = feedback1; v = FEEDBACK1 . value; break;
		case 5: a = freq2; v = FREQ2 . value; break;
		case 6: a = amp2; v = AMP2 . value; break;
		case 7: a = ratio2; v = RATIO2 . value; break;
		case 8: a = feedback2; v = FEEDBACK2 . value; break;
		case 9: a = freq3; v = FREQ3 . value; break;
		case 10: a = amp3; v = AMP3 . value; break;
		case 11: a = ratio3; v = RATIO3 . value; break;
		case 12: a = feedback3; v = FEEDBACK3 . value; break;
		case 13: a = freq4; v = FREQ4 . value; break;
		case 14: a = amp4; v = AMP4 . value; break;
		case 15: a = ratio4; v = RATIO4 . value; break;
		case 16: a = feedback4; v = FEEDBACK4 . value; break;
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
									root -> pair (root -> var (7),
									root -> pair (root -> var (8),
									root -> pair (root -> var (9),
									root -> pair (root -> var (10),
									root -> pair (root -> var (11),
									root -> pair (root -> var (12),
									root -> pair (root -> var (13),
									root -> pair (root -> var (14),
									root -> pair (root -> var (15),
									root -> pair (root -> var (16), root -> earth ())))))))))))))))));
		PrologElement * query;
		query = root -> pair (root -> pair (root -> atom (feedback4), root -> pair (root -> var (16), root -> earth ())), root -> earth ());
		query = root -> pair (root -> pair (root -> atom (ratio4), root -> pair (root -> var (15), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp4), root -> pair (root -> var (14), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq4), root -> pair (root -> var (13), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback4), root -> pair (root -> var (12), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio3), root -> pair (root -> var (11), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp3), root -> pair (root -> var (10), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq3), root -> pair (root -> var (9), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback2), root -> pair (root -> var (8), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio2), root -> pair (root -> var (7), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp2), root -> pair (root -> var (6), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq2), root -> pair (root -> var (5), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback1), root -> pair (root -> var (4), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio1), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp1), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq1), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (algo), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) ALGO . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ1 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP1 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO1 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK1 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ2 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP2 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO2 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK2 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ3 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP3 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO3 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK3 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ4 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP4 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO4 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK4 . setValue (sub -> getNumber ());
		}
		delete query;
	}
	void redraw (cairo_t * cr) {
		ALGO . draw (cr);
		FREQ1 . draw (cr); AMP1 . draw (cr); RATIO1 . draw (cr); FEEDBACK1 . draw (cr);
		FREQ2 . draw (cr); AMP2 . draw (cr); RATIO2 . draw (cr); FEEDBACK2 . draw (cr);
		FREQ3 . draw (cr); AMP3 . draw (cr); RATIO3 . draw (cr); FEEDBACK3 . draw (cr);
		FREQ4 . draw (cr); AMP4 . draw (cr); RATIO4 . draw (cr); FEEDBACK4 . draw (cr);
	}
	void MouseKeyon (point location, int button) {
		ALGO . keyon (location);
		FREQ1 . keyon (location); AMP1 . keyon (location); RATIO1 . keyon (location); FEEDBACK1 . keyon (location);
		FREQ2 . keyon (location); AMP2 . keyon (location); RATIO2 . keyon (location); FEEDBACK2 . keyon (location);
		FREQ3 . keyon (location); AMP3 . keyon (location); RATIO3 . keyon (location); FEEDBACK3 . keyon (location);
		FREQ4 . keyon (location); AMP4 . keyon (location); RATIO4 . keyon (location); FEEDBACK4 . keyon (location);
	}
	void MouseKeyoff (point location, int button) {
		ALGO . keyoff (location);
		FREQ1 . keyoff (location); AMP1 . keyoff (location); RATIO1 . keyoff (location); FEEDBACK1 . keyoff (location);
		FREQ2 . keyoff (location); AMP2 . keyoff (location); RATIO2 . keyoff (location); FEEDBACK2 . keyoff (location);
		FREQ3 . keyoff (location); AMP3 . keyoff (location); RATIO3 . keyoff (location); FEEDBACK3 . keyoff (location);
		FREQ4 . keyoff (location); AMP4 . keyoff (location); RATIO4 . keyoff (location); FEEDBACK4 . keyoff (location);
	}
	void MouseMove (point delta) {
		bool redraw = false;
		if (ALGO . move (delta)) {move (0); redraw = true;}
		if (FREQ1 . move (delta)) {move (1); redraw = true;}
		if (AMP1 . move (delta)) {move (2); redraw = true;}
		if (RATIO1 . move (delta)) {move (3); redraw = true;}
		if (FEEDBACK1 . move (delta)) {move (4); redraw = true;}
		if (FREQ2 . move (delta)) {move (5); redraw = true;}
		if (AMP2 . move (delta)) {move (6); redraw = true;}
		if (RATIO2 . move (delta)) {move (7); redraw = true;}
		if (FEEDBACK2 . move (delta)) {move (8); redraw = true;}
		if (FREQ3 . move (delta)) {move (9); redraw = true;}
		if (AMP3 . move (delta)) {move (10); redraw = true;}
		if (RATIO3 . move (delta)) {move (11); redraw = true;}
		if (FEEDBACK3 . move (delta)) {move (12); redraw = true;}
		if (FREQ4 . move (delta)) {move (13); redraw = true;}
		if (AMP4 . move (delta)) {move (14); redraw = true;}
		if (RATIO4 . move (delta)) {move (15); redraw = true;}
		if (FEEDBACK4 . move (delta)) {move (16); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	fm4_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom, PrologAtom * algo,
		PrologAtom * freq1, PrologAtom * amp1, PrologAtom * ratio1, PrologAtom * feedback1,
		PrologAtom * freq2, PrologAtom * amp2, PrologAtom * ratio2, PrologAtom * feedback2,
		PrologAtom * freq3, PrologAtom * amp3, PrologAtom * ratio3, PrologAtom * feedback3,
		PrologAtom * freq4, PrologAtom * amp4, PrologAtom * ratio4, PrologAtom * feedback4, bool active) :
	ALGO (point (598, 98), 0, resources, true, active, 0.0, 7.0),
	FREQ1 (point (18, 10), 0, resources, true, active, -8192.0, 8192.0),
	AMP1 (point (88, 10), 0, resources, true, active, -16384.0, 0.0),
	RATIO1 (point (158, 10), 0, resources, true, active, 1.0, 33.0),
	FEEDBACK1 (point (228, 10), 0, resources, true, active, 0.0, 16384.0),
	FREQ2 (point (310, 10), 0, resources, true, active, -8192.0, 8192.0),
	AMP2 (point (380, 10), 0, resources, true, active, -16384.0, 0.0),
	RATIO2 (point (450, 10), 0, resources, true, active, 1.0, 33.0),
	FEEDBACK2 (point (520, 10), 0, resources, true, active, 0.0, 16384.0),
	FREQ3 (point (18, 98), 0, resources, true, active, -8192.0, 8192.0),
	AMP3 (point (88, 98), 0, resources, true, active, -16384.0, 0.0),
	RATIO3 (point (158, 98), 0, resources, true, active, 1.0, 33.0),
	FEEDBACK3 (point (228, 98), 0, resources, true, active, 0.0, 16384.0),
	FREQ4 (point (310, 98), 0, resources, true, active, -8192.0, 8192.0),
	AMP4 (point (380, 98), 0, resources, true, active, -16384.0, 0.0),
	RATIO4 (point (450, 98), 0, resources, true, active, 1.0, 33.0),
	FEEDBACK4 (point (520, 98), 0, resources, true, active, 0.0, 16384.0),
	AudioModulePanel (root, atom, resources != 0 ? resources -> fm4_panel_surface : 0) {
		this -> algo = algo; COLLECTOR_REFERENCE_INC (algo);
		this -> freq1 = freq1; COLLECTOR_REFERENCE_INC (freq1);
		this -> amp1 = amp1; COLLECTOR_REFERENCE_INC (amp1);
		this -> ratio1 = ratio1; COLLECTOR_REFERENCE_INC (ratio1);
		this -> feedback1 = feedback1; COLLECTOR_REFERENCE_INC (feedback1);
		this -> freq2 = freq2; COLLECTOR_REFERENCE_INC (freq2);
		this -> amp2 = amp2; COLLECTOR_REFERENCE_INC (amp2);
		this -> ratio2 = ratio2; COLLECTOR_REFERENCE_INC (ratio2);
		this -> feedback2 = feedback2; COLLECTOR_REFERENCE_INC (feedback2);
		this -> freq3 = freq3; COLLECTOR_REFERENCE_INC (freq3);
		this -> amp3 = amp3; COLLECTOR_REFERENCE_INC (amp3);
		this -> ratio3 = ratio3; COLLECTOR_REFERENCE_INC (ratio3);
		this -> feedback3 = feedback3; COLLECTOR_REFERENCE_INC (feedback3);
		this -> freq4 = freq4; COLLECTOR_REFERENCE_INC (freq4);
		this -> amp4 = amp4; COLLECTOR_REFERENCE_INC (amp4);
		this -> ratio4 = ratio4; COLLECTOR_REFERENCE_INC (ratio4);
		this -> feedback4 = feedback4; COLLECTOR_REFERENCE_INC (feedback4);
		feedback ();
	}
	~ fm4_panel_action (void) {
		algo -> removeAtom ();
		freq1 -> removeAtom (); amp1 -> removeAtom (); ratio1 -> removeAtom (); feedback1 -> removeAtom ();
		freq2 -> removeAtom (); amp2 -> removeAtom (); ratio2 -> removeAtom (); feedback2 -> removeAtom ();
		freq3 -> removeAtom (); amp3 -> removeAtom (); ratio3 -> removeAtom (); feedback3 -> removeAtom ();
		freq4 -> removeAtom (); amp4 -> removeAtom (); ratio4 -> removeAtom (); feedback4 -> removeAtom ();
	}
};

bool fm4_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * algo = 0;
	PrologElement * freq1 = 0, * amp1 = 0, * ratio1 = 0, * feedback1 = 0;
	PrologElement * freq2 = 0, * amp2 = 0, * ratio2 = 0, * feedback2 = 0;
	PrologElement * freq3 = 0, * amp3 = 0, * ratio3 = 0, * feedback3 = 0;
	PrologElement * freq4 = 0, * amp4 = 0, * ratio4 = 0, * feedback4 = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (algo == 0) algo = el;
			else if (freq1 == 0) freq1 = el;
			else if (amp1 == 0) amp1 = el;
			else if (ratio1 == 0) ratio1 = el;
			else if (feedback1 == 0) feedback1 = el;
			else if (freq2 == 0) freq2 = el;
			else if (amp2 == 0) amp2 = el;
			else if (ratio2 == 0) ratio2 = el;
			else if (feedback2 == 0) feedback2 = el;
			else if (freq3 == 0) freq3 = el;
			else if (amp3 == 0) amp3 = el;
			else if (ratio3 == 0) ratio3 = el;
			else if (feedback3 == 0) feedback3 = el;
			else if (freq4 == 0) freq4 = el;
			else if (amp4 == 0) amp4 = el;
			else if (ratio4 == 0) ratio4 = el;
			else if (feedback4 == 0) feedback4 = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || feedback4 == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	fm4_panel_action * machine = new fm4_panel_action (resources, root, atom -> getAtom (), algo -> getAtom (),
		freq1 -> getAtom (), amp1 -> getAtom (), ratio1 -> getAtom (), feedback1 -> getAtom (),
		freq2 -> getAtom (), amp2 -> getAtom (), ratio2 -> getAtom (), feedback2 -> getAtom (),
		freq3 -> getAtom (), amp3 -> getAtom (), ratio3 -> getAtom (), feedback3 -> getAtom (),
		freq4 -> getAtom (), amp4 -> getAtom (), ratio4 -> getAtom (), feedback4 -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

fm4_panel_class :: fm4_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
