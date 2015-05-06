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
// This file was created on Wednesday, 25th April 2015 at 18:37:13. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class lfo_panel_action : public AudioModulePanel {
public:
	PrologAtom * speed, * wave, * pulse, * phase, * sync, * vibrato, * tremolo, * wah_wah, * pan;
	knob_active_graphics SPEED, WAVE, PULSE, PHASE, VIBRATO, TREMOLO, WAH_WAH, PAN;
	button_active_graphics SYNC;
	cairo_surface_t * vibrato_image;
	cairo_surface_t * tremolo_image;
	cairo_surface_t * wah_wah_image;
	cairo_surface_t * pan_image;
	void redraw (cairo_t * cr) {
		SPEED . draw (cr);
		WAVE . draw (cr);
		PULSE . draw (cr);
		PHASE . draw (cr);
		if (vibrato != 0) {cairo_set_source_surface (cr, vibrato_image, 18, 98); cairo_paint (cr); VIBRATO . draw (cr);}
		if (tremolo != 0) {cairo_set_source_surface (cr, tremolo_image, 88, 98); cairo_paint (cr); TREMOLO . draw (cr);}
		if (wah_wah != 0) {cairo_set_source_surface (cr, wah_wah_image, 156, 98); cairo_paint (cr); WAH_WAH . draw (cr);}
		if (pan != 0) {cairo_set_source_surface (cr, pan_image, 226, 104); cairo_paint (cr); PAN . draw (cr);}
		SYNC . draw (cr);
	}
	void MouseKeyon (point location, int button) {
		SPEED . keyon (location);
		WAVE . keyon (location);
		PULSE . keyon (location);
		PHASE . keyon (location);
		if (vibrato != 0) VIBRATO . keyon (location);
		if (tremolo != 0) TREMOLO . keyon (location);
		if (wah_wah != 0) WAH_WAH . keyon (location);
		if (pan != 0) PAN . keyon (location);
		if (SYNC . keyon (location)) {
			SYNC . engaged = ! SYNC . engaged;
			sync_changed ();
			update ();
		}
	}
	void MouseKeyoff (point location, int button) {
		SPEED . keyoff (location);
		WAVE . keyoff (location);
		PULSE . keyoff (location);
		PHASE . keyoff (location);
		if (vibrato != 0) VIBRATO . keyoff (location);
		if (tremolo != 0) TREMOLO . keyoff (location);
		if (wah_wah != 0) WAH_WAH . keyoff (location);
		if (pan != 0) PAN . keyoff (location);
	}
	void MouseMove (point delta) {
		bool redraw = false;
		if (SPEED . move (delta)) {move (0); redraw = true;}
		if (WAVE . move (delta)) {move (1); redraw = true;}
		if (PULSE . move (delta)) {move (2); redraw = true;}
		if (PHASE . move (delta)) {move (3); redraw = true;}
		if (vibrato != 0 && VIBRATO . move (delta)) {move (4); redraw = true;}
		if (tremolo != 0 && TREMOLO . move (delta)) {move (5); redraw = true;}
		if (wah_wah != 0 && WAH_WAH . move (delta)) {move (6); redraw = true;}
		if (pan != 0 && PAN . move (delta)) {move (7); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = speed; v = SPEED . value; break;
		case 1: a = wave; v = WAVE . value; break;
		case 2: a = pulse; v = PULSE . value; break;
		case 3: a = phase; v = PHASE . value; break;
		case 4: a = vibrato; v = VIBRATO . value; break;
		case 5: a = tremolo; v = TREMOLO . value; break;
		case 6: a = wah_wah; v = WAH_WAH . value; break;
		case 7: a = pan; v = PAN . value; break;
		default: a = 0; break;
		}
		if (a == 0) return;
		PrologElement * query = root -> pair (root -> atom (a), root -> pair (root -> Double (v), root -> earth ()));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void sync_changed (void) {
		PrologElement * query = root -> pair (root -> atom (sync), root -> pair (root -> Double (SYNC . engaged ? 1.0 : 0.0), root -> earth ()));
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
									root -> pair (root -> var (8), root -> earth ())))))))));
		PrologElement * query = root -> earth ();
		if (pan != 0) query = root -> pair (root -> pair (root -> atom (pan), root -> pair (root -> var (8), root -> earth ())), query);
		if (wah_wah != 0) query = root -> pair (root -> pair (root -> atom (wah_wah), root -> pair (root -> var (7), root -> earth ())), query);
		if (tremolo != 0) query = root -> pair (root -> pair (root -> atom (tremolo), root -> pair (root -> var (6), root -> earth ())), query);
		if (vibrato != 0) query = root -> pair (root -> pair (root -> atom (vibrato), root -> pair (root -> var (5), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (sync), root -> pair (root -> var (4), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (phase), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (pulse), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (wave), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (speed), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) SPEED . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) WAVE . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) PULSE . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) PHASE . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();

			if (sub -> isNumber ()) SYNC . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) VIBRATO . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) TREMOLO . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) WAH_WAH . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) PAN . setValue (sub -> getNumber ());
		}
		delete query;
	}
	lfo_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom,
		PrologAtom * speed, PrologAtom * wave, PrologAtom * pulse, PrologAtom * phase, PrologAtom * sync,
		PrologAtom * vibrato, PrologAtom * tremolo, PrologAtom * wah_wah, PrologAtom * pan, bool active) :
	SPEED (point (18, 10), 0, resources, true, active, -8192.0, 8192.0),
	WAVE (point (88, 10), 0, resources, true, active, 0.0, 4.0),
	PULSE (point (158, 10), 0, resources, true, active, -8192.0, 8192.0),
	PHASE (point (228, 10), 0, resources, true, active, 0.0, 16384.0),
	VIBRATO (point (18, 98), 0, resources, true, active, 0.0, 16384.0),
	TREMOLO (point (88, 98), 0, resources, true, active, 0.0, 16384.0),
	WAH_WAH (point (158, 98), 0, resources, true, active, 0.0, 16384.0),
	PAN (point (228, 104), 0, resources, true, active, 0.0, 16384.0),
	SYNC (point (263, 94), 0, resources, true),
	AudioModulePanel (root, atom, resources != 0 ? resources -> lfo_panel_surface : 0) {
		vibrato_image = resources != 0 ? resources -> lfo_vibrato_surface : 0;
		tremolo_image = resources != 0 ? resources -> lfo_tremolo_surface : 0;
		wah_wah_image = resources != 0 ? resources -> lfo_wah_wah_surface : 0;
		pan_image = resources != 0 ? resources -> lfo_pan_surface : 0;
		this -> speed = speed; COLLECTOR_REFERENCE_INC (speed);
		this -> wave = wave; COLLECTOR_REFERENCE_INC (wave);
		this -> pulse = pulse; COLLECTOR_REFERENCE_INC (pulse);
		this -> phase = phase; COLLECTOR_REFERENCE_INC (phase);
		this -> sync = sync; COLLECTOR_REFERENCE_INC (sync);
		this -> vibrato = vibrato; if (vibrato != 0) {COLLECTOR_REFERENCE_INC (vibrato);}
		this -> tremolo = tremolo; if (tremolo != 0) {COLLECTOR_REFERENCE_INC (tremolo);}
		this -> wah_wah = wah_wah; if (wah_wah != 0) {COLLECTOR_REFERENCE_INC (wah_wah);}
		this -> pan = pan; if (pan != 0) {COLLECTOR_REFERENCE_INC (pan);}
		feedback ();
	}
	~ lfo_panel_action (void) {
		speed -> removeAtom (); wave -> removeAtom (); pulse -> removeAtom (); phase -> removeAtom (); sync -> removeAtom ();
		if (vibrato != 0) vibrato -> removeAtom ();
		if (tremolo != 0) tremolo -> removeAtom ();
		if (wah_wah != 0) wah_wah -> removeAtom ();
		if (pan != 0) pan -> removeAtom ();
	}
};

bool lfo_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * speed = 0, * wave = 0, * pulse = 0, * phase = 0, * sync = 0;
	PrologElement * vibrato = 0, * tremolo = 0, * wah_wah = 0, * pan = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (speed == 0) speed = el;
			else if (wave == 0) wave = el;
			else if (pulse == 0) pulse = el;
			else if (phase == 0) phase = el;
			else if (sync == 0) sync = el;
			else if (vibrato == 0) vibrato = el;
			else if (tremolo == 0) tremolo = el;
			else if (wah_wah == 0) wah_wah = el;
			else if (pan == 0) pan = el;
		}
		if (el -> isEarth ()) {
			if (vibrato == 0) vibrato = el;
			else if (tremolo == 0) tremolo = el;
			else if (wah_wah == 0) wah_wah = el;
			else if (pan == 0) pan = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || pan == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	lfo_panel_action * machine = new lfo_panel_action (resources, root, atom -> getAtom (),
		speed -> getAtom (), wave -> getAtom (), pulse -> getAtom (), phase -> getAtom (), sync -> getAtom (),
		vibrato -> isAtom () ? vibrato -> getAtom () : 0, tremolo -> isAtom () ? tremolo -> getAtom () : 0,
		wah_wah -> isAtom () ? wah_wah -> getAtom () : 0, pan -> isAtom () ? pan -> getAtom () : 0, false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

lfo_panel_class :: lfo_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}

