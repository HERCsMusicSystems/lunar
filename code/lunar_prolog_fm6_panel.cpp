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
// This file was created on Wednesday, 7th February2018 at 12:51:48. //
///////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class fm6_panel_action : public AudioModulePanel {
public:
	PrologAtom * algo, * a12, * a13, * a14, * a15, * a16, * a23, * a24, * a25, * a26, * a34, * a35, * a36, * a45, * a46, * a56;
	PrologAtom * freq1, * amp1, * ratio1, * feedback1;
	PrologAtom * freq2, * amp2, * ratio2, * feedback2;
	PrologAtom * freq3, * amp3, * ratio3, * feedback3;
	PrologAtom * freq4, * amp4, * ratio4, * feedback4;
	PrologAtom * freq5, * amp5, * ratio5, * feedback5;
	PrologAtom * freq6, * amp6, * ratio6, * feedback6;
	knob_active_graphics ALGO;
	knob_active_graphics FREQ1, AMP1, RATIO1, FEEDBACK1;
	knob_active_graphics FREQ2, AMP2, RATIO2, FEEDBACK2;
	knob_active_graphics FREQ3, AMP3, RATIO3, FEEDBACK3;
	knob_active_graphics FREQ4, AMP4, RATIO4, FEEDBACK4;
	knob_active_graphics FREQ5, AMP5, RATIO5, FEEDBACK5;
	knob_active_graphics FREQ6, AMP6, RATIO6, FEEDBACK6;
	button_active_graphics feed12, feed13, feed14, feed15, feed16, feed23, feed24, feed25, feed26, feed34, feed35, feed36, feed45, feed46, feed56;
	cairo_surface_t * algo_image;
	bool dx;
	void move (PrologAtom * a, double v) {
		if (a == 0) return;
		PrologElement * query = root -> pair (root -> atom (a), root -> pair (root -> Double (v), root -> earth ()));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void interconnection_changed (PrologAtom * atom, bool value) {
		if (atom == 0) return;
		PrologElement * query = root -> pair (root -> atom (atom), root -> pair (root -> Double (value ? 1.0 : 0.0), root -> earth ()));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void feedback_dx (void) {
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
									root -> pair (root -> var (16),
									root -> pair (root -> var (17),
									root -> pair (root -> var (18),
									root -> pair (root -> var (19),
									root -> pair (root -> var (20),
									root -> pair (root -> var (21),
									root -> pair (root -> var (22),
									root -> pair (root -> var (23),
									root -> pair (root -> var (24), root -> earth ())))))))))))))))))))))))));
		PrologElement * query;
		query = root -> pair (root -> pair (root -> atom (feedback6), root -> pair (root -> var (24), root -> earth ())), root -> earth ());
		query = root -> pair (root -> pair (root -> atom (ratio6), root -> pair (root -> var (23), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp6), root -> pair (root -> var (22), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq6), root -> pair (root -> var (21), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback5), root -> pair (root -> var (20), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio5), root -> pair (root -> var (19), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp5), root -> pair (root -> var (18), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq5), root -> pair (root -> var (17), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback4), root -> pair (root -> var (16), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio4), root -> pair (root -> var (15), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp4), root -> pair (root -> var (14), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq4), root -> pair (root -> var (13), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback3), root -> pair (root -> var (12), root -> earth ())), query);
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
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ5 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP5 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO5 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK5 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ6 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP6 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO6 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK6 . setValue (sub -> getNumber ());
		}
		delete query;
	}
	void feedback_fm (void) {
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
									root -> pair (root -> var (16),
									root -> pair (root -> var (17),
									root -> pair (root -> var (18),
									root -> pair (root -> var (19),
									root -> pair (root -> var (20),
									root -> pair (root -> var (21),
									root -> pair (root -> var (22),
									root -> pair (root -> var (23),
									root -> pair (root -> var (24),
									root -> pair (root -> var (25),
									root -> pair (root -> var (26),
									root -> pair (root -> var (27),
									root -> pair (root -> var (28),
									root -> pair (root -> var (29),
									root -> pair (root -> var (30),
									root -> pair (root -> var (31),
									root -> pair (root -> var (32),
									root -> pair (root -> var (33),
									root -> pair (root -> var (34),
									root -> pair (root -> var (35),
									root -> pair (root -> var (36),
									root -> pair (root -> var (37),
									root -> pair (root -> var (38), root -> earth ())))))))))))))))))))))))))))))))))))))));
		PrologElement * query;
		query = root -> pair (root -> pair (root -> atom (feedback6), root -> pair (root -> var (38), root -> earth ())), root -> earth ());
		query = root -> pair (root -> pair (root -> atom (ratio6), root -> pair (root -> var (37), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp6), root -> pair (root -> var (36), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq6), root -> pair (root -> var (35), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback5), root -> pair (root -> var (34), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio5), root -> pair (root -> var (33), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp5), root -> pair (root -> var (32), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq5), root -> pair (root -> var (31), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback4), root -> pair (root -> var (30), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio4), root -> pair (root -> var (29), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp4), root -> pair (root -> var (28), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq4), root -> pair (root -> var (27), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback3), root -> pair (root -> var (26), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio3), root -> pair (root -> var (25), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp3), root -> pair (root -> var (24), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq3), root -> pair (root -> var (23), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback2), root -> pair (root -> var (22), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio2), root -> pair (root -> var (21), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp2), root -> pair (root -> var (20), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq2), root -> pair (root -> var (19), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (feedback1), root -> pair (root -> var (18), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (ratio1), root -> pair (root -> var (17), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (amp1), root -> pair (root -> var (16), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (freq1), root -> pair (root -> var (15), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a56), root -> pair (root -> var (14), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a46), root -> pair (root -> var (13), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a45), root -> pair (root -> var (12), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a36), root -> pair (root -> var (11), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a35), root -> pair (root -> var (10), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a34), root -> pair (root -> var (9), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a26), root -> pair (root -> var (8), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a25), root -> pair (root -> var (7), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a24), root -> pair (root -> var (6), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a23), root -> pair (root -> var (5), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a16), root -> pair (root -> var (4), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a15), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a14), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a13), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a12), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) feed12 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed13 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed14 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed15 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed16 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed23 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed24 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed25 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed26 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed34 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed45 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed46 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed56 . engaged = (sub -> getNumber () != 0.0);
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
	void feedback (void) {if (dx) feedback_dx (); else feedback_fm ();}
	void redraw (cairo_t * cr) {
		if (dx) {cairo_set_source_surface (cr, algo_image, 598, 98); cairo_paint (cr); ALGO . draw (cr);}
		else {feed12 . draw (cr); feed13 . draw (cr); feed14 . draw (cr); feed23 . draw (cr); feed24 . draw (cr); feed34 . draw (cr);}
		FREQ1 . draw (cr); AMP1 . draw (cr); RATIO1 . draw (cr); FEEDBACK1 . draw (cr);
		FREQ2 . draw (cr); AMP2 . draw (cr); RATIO2 . draw (cr); FEEDBACK2 . draw (cr);
		FREQ3 . draw (cr); AMP3 . draw (cr); RATIO3 . draw (cr); FEEDBACK3 . draw (cr);
		FREQ4 . draw (cr); AMP4 . draw (cr); RATIO4 . draw (cr); FEEDBACK4 . draw (cr);
	}
	void MouseKeyon (point location, int button) {
		if (dx) ALGO . keyon (location);
		FREQ1 . keyon (location); AMP1 . keyon (location); RATIO1 . keyon (location); FEEDBACK1 . keyon (location);
		FREQ2 . keyon (location); AMP2 . keyon (location); RATIO2 . keyon (location); FEEDBACK2 . keyon (location);
		FREQ3 . keyon (location); AMP3 . keyon (location); RATIO3 . keyon (location); FEEDBACK3 . keyon (location);
		FREQ4 . keyon (location); AMP4 . keyon (location); RATIO4 . keyon (location); FEEDBACK4 . keyon (location);
		if (feed12 . keyon (location)) {feed12 . engaged = ! feed12 . engaged; interconnection_changed (a12, feed12 . engaged); update ();}
		if (feed13 . keyon (location)) {feed13 . engaged = ! feed13 . engaged; interconnection_changed (a13, feed13 . engaged); update ();}
		if (feed14 . keyon (location)) {feed14 . engaged = ! feed14 . engaged; interconnection_changed (a14, feed14 . engaged); update ();}
		if (feed23 . keyon (location)) {feed23 . engaged = ! feed23 . engaged; interconnection_changed (a23, feed23 . engaged); update ();}
		if (feed24 . keyon (location)) {feed24 . engaged = ! feed24 . engaged; interconnection_changed (a24, feed24 . engaged); update ();}
		if (feed34 . keyon (location)) {feed34 . engaged = ! feed34 . engaged; interconnection_changed (a34, feed34 . engaged); update ();}
	}
	void MouseKeyoff (point location, int button) {
		if (dx) ALGO . keyoff (location);
		FREQ1 . keyoff (location); AMP1 . keyoff (location); RATIO1 . keyoff (location); FEEDBACK1 . keyoff (location);
		FREQ2 . keyoff (location); AMP2 . keyoff (location); RATIO2 . keyoff (location); FEEDBACK2 . keyoff (location);
		FREQ3 . keyoff (location); AMP3 . keyoff (location); RATIO3 . keyoff (location); FEEDBACK3 . keyoff (location);
		FREQ4 . keyoff (location); AMP4 . keyoff (location); RATIO4 . keyoff (location); FEEDBACK4 . keyoff (location);
	}
	void MouseMove (point delta) {
		bool redraw = false;
		if (dx && ALGO . move (delta)) {move (algo, ALGO . value); redraw = true;}
		if (FREQ1 . move (delta)) {move (freq1, FREQ1 . value); redraw = true;}
		if (AMP1 . move (delta)) {move (amp1, AMP1 . value); redraw = true;}
		if (RATIO1 . move (delta)) {move (ratio1, RATIO1 . value); redraw = true;}
		if (FEEDBACK1 . move (delta)) {move (feedback1, FEEDBACK1 . value); redraw = true;}
		if (FREQ2 . move (delta)) {move (freq2, FREQ2 . value); redraw = true;}
		if (AMP2 . move (delta)) {move (amp2, AMP2 . value); redraw = true;}
		if (RATIO2 . move (delta)) {move (ratio2, RATIO2 . value); redraw = true;}
		if (FEEDBACK2 . move (delta)) {move (feedback2, FEEDBACK2 . value); redraw = true;}
		if (FREQ3 . move (delta)) {move (freq3, FREQ3 . value); redraw = true;}
		if (AMP3 . move (delta)) {move (amp3, AMP3 . value); redraw = true;}
		if (RATIO3 . move (delta)) {move (ratio3, RATIO3 . value); redraw = true;}
		if (FEEDBACK3 . move (delta)) {move (feedback3, FEEDBACK3 . value); redraw = true;}
		if (FREQ4 . move (delta)) {move (freq4, FREQ4 . value); redraw = true;}
		if (AMP4 . move (delta)) {move (amp4, AMP4 . value); redraw = true;}
		if (RATIO4 . move (delta)) {move (ratio4, RATIO4 . value); redraw = true;}
		if (FEEDBACK4 . move (delta)) {move (feedback4, FEEDBACK4 . value); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	fm6_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom, PrologAtom * algo,
		PrologAtom * a12, PrologAtom * a13, PrologAtom * a14,
		PrologAtom * a23, PrologAtom * a24, PrologAtom * a34,
		PrologAtom * freq1, PrologAtom * amp1, PrologAtom * ratio1, PrologAtom * feedback1,
		PrologAtom * freq2, PrologAtom * amp2, PrologAtom * ratio2, PrologAtom * feedback2,
		PrologAtom * freq3, PrologAtom * amp3, PrologAtom * ratio3, PrologAtom * feedback3,
		PrologAtom * freq4, PrologAtom * amp4, PrologAtom * ratio4, PrologAtom * feedback4, bool active, bool dx) :
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
	FREQ5 (point (18, 186), 0, resources, true, active, -8192.0, 8192.0),
	AMP5 (point (88, 186), 0, resources, true, active, -16384.0, 0.0),
	RATIO5 (point (158, 186), 0, resources, true, active, 1.0, 33.0),
	FEEDBACK5 (point (228, 186), 0, resources, true, active, 0.0, 16384.0),
	FREQ6 (point (310, 186), 0, resources, true, active, -8192.0, 8192.0),
	AMP6 (point (380, 186), 0, resources, true, active, -16384.0, 0.0),
	RATIO6 (point (450, 186), 0, resources, true, active, 1.0, 33.0),
	FEEDBACK6 (point (520, 186), 0, resources, true, active, 0.0, 16384.0),
	feed12 (point (584, 118), 0, resources, true),
	feed13 (point (610, 118), 0, resources, true),
	feed14 (point (636, 118), 0, resources, true),
	feed15 (point (662, 118), 0, resources, true),
	feed16 (point (698, 118), 0, resources, true),
	feed23 (point (610, 138), 0, resources, true),
	feed24 (point (636, 138), 0, resources, true),
	feed25 (point (662, 138), 0, resources, true),
	feed26 (point (698, 138), 0, resources, true),
	feed34 (point (636, 158), 0, resources, true),
	feed35 (point (662, 158), 0, resources, true),
	feed36 (point (698, 158), 0, resources, true),
	feed45 (point (662, 178), 0, resources, true),
	feed46 (point (698, 178), 0, resources, true),
	feed56 (point (698, 198), 0, resources, true),
	AudioModulePanel (root, atom, resources != 0 ? resources -> fm6_panel_surface : 0) {
		this -> dx = dx;
		algo_image = resources != 0 ? resources -> fm4_algo_surface : 0;
		this -> algo = algo; if (algo != 0) {COLLECTOR_REFERENCE_INC (algo);}
		this -> a12 = a12; if (a12 != 0) {COLLECTOR_REFERENCE_INC (a12);}
		this -> a13 = a13; if (a13 != 0) {COLLECTOR_REFERENCE_INC (a13);}
		this -> a14 = a14; if (a14 != 0) {COLLECTOR_REFERENCE_INC (a14);}
		this -> a23 = a23; if (a23 != 0) {COLLECTOR_REFERENCE_INC (a23);}
		this -> a24 = a24; if (a24 != 0) {COLLECTOR_REFERENCE_INC (a24);}
		this -> a34 = a34; if (a34 != 0) {COLLECTOR_REFERENCE_INC (a34);}
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
	~ fm6_panel_action (void) {
		if (algo != 0) algo -> removeAtom ();
		if (a12 != 0) a12 -> removeAtom (); if (a13 != 0) a13 -> removeAtom (); if (a14 != 0) a14 -> removeAtom ();
		if (a23 != 0) a23 -> removeAtom (); if (a24 != 0) a24 -> removeAtom (); if (a34 != 0) a34 -> removeAtom ();
		freq1 -> removeAtom (); amp1 -> removeAtom (); ratio1 -> removeAtom (); feedback1 -> removeAtom ();
		freq2 -> removeAtom (); amp2 -> removeAtom (); ratio2 -> removeAtom (); feedback2 -> removeAtom ();
		freq3 -> removeAtom (); amp3 -> removeAtom (); ratio3 -> removeAtom (); feedback3 -> removeAtom ();
		freq4 -> removeAtom (); amp4 -> removeAtom (); ratio4 -> removeAtom (); feedback4 -> removeAtom ();
	}
};

bool fm6_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * algo = 0;
	PrologElement * a12 = 0, * a13 = 0, * a14 = 0, * a23 = 0, * a24 = 0, * a34 = 0;
	PrologElement * freq1 = 0, * amp1 = 0, * ratio1 = 0, * feedback1 = 0;
	PrologElement * freq2 = 0, * amp2 = 0, * ratio2 = 0, * feedback2 = 0;
	PrologElement * freq3 = 0, * amp3 = 0, * ratio3 = 0, * feedback3 = 0;
	PrologElement * freq4 = 0, * amp4 = 0, * ratio4 = 0, * feedback4 = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (algo == 0 && a34 == 0) {
				if (dx) algo = el;
				else {
					if (a12 == 0) a12 = el;
					else if (a13 == 0) a13 = el;
					else if (a14 == 0) a14 = el;
					else if (a23 == 0) a23 = el;
					else if (a24 == 0) a24 = el;
					else if (a34 == 0) a34 = el;
				}
			} else if (freq1 == 0) freq1 = el;
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
	fm6_panel_action * machine = new fm6_panel_action (resources, root, atom -> getAtom (),
		algo ? algo -> getAtom () : 0,
		a12 ? a12 -> getAtom () : 0, a13 ? a13 -> getAtom () : 0, a14 ? a14 -> getAtom () : 0,
		a23 ? a23 -> getAtom () : 0, a24 ? a24 -> getAtom () : 0, a34 ? a34 -> getAtom () : 0,
		freq1 -> getAtom (), amp1 -> getAtom (), ratio1 -> getAtom (), feedback1 -> getAtom (),
		freq2 -> getAtom (), amp2 -> getAtom (), ratio2 -> getAtom (), feedback2 -> getAtom (),
		freq3 -> getAtom (), amp3 -> getAtom (), ratio3 -> getAtom (), feedback3 -> getAtom (),
		freq4 -> getAtom (), amp4 -> getAtom (), ratio4 -> getAtom (), feedback4 -> getAtom (), false, dx);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

fm6_panel_class :: fm6_panel_class (PrologLunarServiceClass * servo, bool dx) {
	this -> dx = dx;
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
