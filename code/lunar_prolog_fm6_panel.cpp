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
	PrologAtom * algo, * a21, * a31, * a32, * a41, * a42, * a43, * a51, * a52, * a53, * a54, * a61, * a62, * a63, * a64, * a65;
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
	button_active_graphics feed21, feed31, feed32, feed41, feed42, feed43, feed51, feed52, feed53, feed54, feed61, feed62, feed63, feed64, feed65;
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
		query = root -> pair (root -> pair (root -> atom (a65), root -> pair (root -> var (14), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a64), root -> pair (root -> var (13), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a63), root -> pair (root -> var (12), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a62), root -> pair (root -> var (11), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a61), root -> pair (root -> var (10), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a54), root -> pair (root -> var (9), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a53), root -> pair (root -> var (8), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a52), root -> pair (root -> var (7), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a51), root -> pair (root -> var (6), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a43), root -> pair (root -> var (5), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a42), root -> pair (root -> var (4), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a41), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a32), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a31), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (a21), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) feed21 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed31 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed32 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed41 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed42 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed43 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed51 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed52 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed53 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed54 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed61 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed62 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed63 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed64 . engaged = (sub -> getNumber () != 0.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) feed65 . engaged = (sub -> getNumber () != 0.0);
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
	void feedback (void) {if (dx) feedback_dx (); else feedback_fm ();}
	void redraw (cairo_t * cr) {
		if (dx) {cairo_set_source_surface (cr, algo_image, 598, 98); cairo_paint (cr); ALGO . draw (cr);}
		else {
			feed21 . draw (cr); feed31 . draw (cr); feed32 . draw (cr); feed41 . draw (cr); feed42 . draw (cr);
			feed43 . draw (cr); feed51 . draw (cr); feed52 . draw (cr); feed53 . draw (cr);
			feed54 . draw (cr); feed61 . draw (cr); feed62 . draw (cr); feed63 . draw (cr); feed64 . draw (cr); feed65 . draw (cr);
		}
		FREQ1 . draw (cr); AMP1 . draw (cr); RATIO1 . draw (cr); FEEDBACK1 . draw (cr);
		FREQ2 . draw (cr); AMP2 . draw (cr); RATIO2 . draw (cr); FEEDBACK2 . draw (cr);
		FREQ3 . draw (cr); AMP3 . draw (cr); RATIO3 . draw (cr); FEEDBACK3 . draw (cr);
		FREQ4 . draw (cr); AMP4 . draw (cr); RATIO4 . draw (cr); FEEDBACK4 . draw (cr);
		FREQ5 . draw (cr); AMP5 . draw (cr); RATIO5 . draw (cr); FEEDBACK5 . draw (cr);
		FREQ6 . draw (cr); AMP6 . draw (cr); RATIO6 . draw (cr); FEEDBACK6 . draw (cr);
	}
	void MouseKeyon (point location, int button) {
		if (dx) ALGO . keyon (location);
		FREQ1 . keyon (location); AMP1 . keyon (location); RATIO1 . keyon (location); FEEDBACK1 . keyon (location);
		FREQ2 . keyon (location); AMP2 . keyon (location); RATIO2 . keyon (location); FEEDBACK2 . keyon (location);
		FREQ3 . keyon (location); AMP3 . keyon (location); RATIO3 . keyon (location); FEEDBACK3 . keyon (location);
		FREQ4 . keyon (location); AMP4 . keyon (location); RATIO4 . keyon (location); FEEDBACK4 . keyon (location);
		FREQ5 . keyon (location); AMP5 . keyon (location); RATIO5 . keyon (location); FEEDBACK5 . keyon (location);
		FREQ6 . keyon (location); AMP6 . keyon (location); RATIO6 . keyon (location); FEEDBACK6 . keyon (location);
		if (feed21 . keyon (location)) {feed21 . engaged = ! feed21 . engaged; interconnection_changed (a21, feed21 . engaged); update ();}
		if (feed31 . keyon (location)) {feed31 . engaged = ! feed31 . engaged; interconnection_changed (a31, feed31 . engaged); update ();}
		if (feed32 . keyon (location)) {feed32 . engaged = ! feed32 . engaged; interconnection_changed (a32, feed32 . engaged); update ();}
		if (feed41 . keyon (location)) {feed41 . engaged = ! feed41 . engaged; interconnection_changed (a41, feed41 . engaged); update ();}
		if (feed42 . keyon (location)) {feed42 . engaged = ! feed42 . engaged; interconnection_changed (a42, feed42 . engaged); update ();}
		if (feed43 . keyon (location)) {feed43 . engaged = ! feed43 . engaged; interconnection_changed (a43, feed43 . engaged); update ();}
		if (feed51 . keyon (location)) {feed51 . engaged = ! feed51 . engaged; interconnection_changed (a51, feed51 . engaged); update ();}
		if (feed52 . keyon (location)) {feed52 . engaged = ! feed52 . engaged; interconnection_changed (a52, feed52 . engaged); update ();}
		if (feed53 . keyon (location)) {feed53 . engaged = ! feed53 . engaged; interconnection_changed (a53, feed53 . engaged); update ();}
		if (feed54 . keyon (location)) {feed54 . engaged = ! feed54 . engaged; interconnection_changed (a54, feed54 . engaged); update ();}
		if (feed61 . keyon (location)) {feed61 . engaged = ! feed61 . engaged; interconnection_changed (a61, feed61 . engaged); update ();}
		if (feed62 . keyon (location)) {feed62 . engaged = ! feed62 . engaged; interconnection_changed (a62, feed62 . engaged); update ();}
		if (feed63 . keyon (location)) {feed63 . engaged = ! feed63 . engaged; interconnection_changed (a63, feed63 . engaged); update ();}
		if (feed64 . keyon (location)) {feed64 . engaged = ! feed64 . engaged; interconnection_changed (a64, feed64 . engaged); update ();}
		if (feed65 . keyon (location)) {feed65 . engaged = ! feed65 . engaged; interconnection_changed (a65, feed65 . engaged); update ();}
	}
	void MouseKeyoff (point location, int button) {
		if (dx) ALGO . keyoff (location);
		FREQ1 . keyoff (location); AMP1 . keyoff (location); RATIO1 . keyoff (location); FEEDBACK1 . keyoff (location);
		FREQ2 . keyoff (location); AMP2 . keyoff (location); RATIO2 . keyoff (location); FEEDBACK2 . keyoff (location);
		FREQ3 . keyoff (location); AMP3 . keyoff (location); RATIO3 . keyoff (location); FEEDBACK3 . keyoff (location);
		FREQ4 . keyoff (location); AMP4 . keyoff (location); RATIO4 . keyoff (location); FEEDBACK4 . keyoff (location);
		FREQ5 . keyoff (location); AMP5 . keyoff (location); RATIO5 . keyoff (location); FEEDBACK5 . keyoff (location);
		FREQ6 . keyoff (location); AMP6 . keyoff (location); RATIO6 . keyoff (location); FEEDBACK6 . keyoff (location);
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
		if (FREQ5 . move (delta)) {move (freq5, FREQ5 . value); redraw = true;}
		if (AMP5 . move (delta)) {move (amp5, AMP5 . value); redraw = true;}
		if (RATIO5 . move (delta)) {move (ratio5, RATIO5 . value); redraw = true;}
		if (FEEDBACK5 . move (delta)) {move (feedback5, FEEDBACK5 . value); redraw = true;}
		if (FREQ6 . move (delta)) {move (freq6, FREQ6 . value); redraw = true;}
		if (AMP6 . move (delta)) {move (amp6, AMP6 . value); redraw = true;}
		if (RATIO6 . move (delta)) {move (ratio6, RATIO6 . value); redraw = true;}
		if (FEEDBACK6 . move (delta)) {move (feedback6, FEEDBACK6 . value); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state) {}
	fm6_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom, PrologAtom * algo,
		PrologAtom * a12, PrologAtom * a13, PrologAtom * a14, PrologAtom * a15, PrologAtom * a16,
		PrologAtom * a23, PrologAtom * a24, PrologAtom * a25, PrologAtom * a26,
		PrologAtom * a34, PrologAtom * a35, PrologAtom * a36, PrologAtom * a45, PrologAtom * a46, PrologAtom * a56,
		PrologAtom * freq1, PrologAtom * amp1, PrologAtom * ratio1, PrologAtom * feedback1,
		PrologAtom * freq2, PrologAtom * amp2, PrologAtom * ratio2, PrologAtom * feedback2,
		PrologAtom * freq3, PrologAtom * amp3, PrologAtom * ratio3, PrologAtom * feedback3,
		PrologAtom * freq4, PrologAtom * amp4, PrologAtom * ratio4, PrologAtom * feedback4,
		PrologAtom * freq5, PrologAtom * amp5, PrologAtom * ratio5, PrologAtom * feedback5,
		PrologAtom * freq6, PrologAtom * amp6, PrologAtom * ratio6, PrologAtom * feedback6,
		bool active, bool dx) :
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
	feed61 (point (600, 20), 0, resources, true),
	feed51 (point (600, 40), 0, resources, true),
	feed41 (point (600, 60), 0, resources, true),
	feed31 (point (600, 80), 0, resources, true),
	feed21 (point (600, 100), 0, resources, true),
	feed62 (point (640, 20), 0, resources, true),
	feed52 (point (640, 40), 0, resources, true),
	feed42 (point (640, 60), 0, resources, true),
	feed32 (point (640, 80), 0, resources, true),
	feed63 (point (600, 140), 0, resources, true),
	feed53 (point (600, 160), 0, resources, true),
	feed43 (point (600, 180), 0, resources, true),
	feed64 (point (640, 140), 0, resources, true),
	feed54 (point (640, 160), 0, resources, true),
	feed65 (point (640, 200), 0, resources, true),
	AudioModulePanel (root, atom, resources != 0 ? resources -> fm6_panel_surface : 0) {
		this -> dx = dx;
		algo_image = resources != 0 ? resources -> fm4_algo_surface : 0;
		this -> algo = algo; if (algo != 0) {COLLECTOR_REFERENCE_INC (algo);}
		this -> a21 = a21; if (a21 != 0) {COLLECTOR_REFERENCE_INC (a21);}
		this -> a31 = a31; if (a31 != 0) {COLLECTOR_REFERENCE_INC (a31);}
		this -> a32 = a32; if (a32 != 0) {COLLECTOR_REFERENCE_INC (a32);}
		this -> a41 = a41; if (a41 != 0) {COLLECTOR_REFERENCE_INC (a41);}
		this -> a42 = a42; if (a42 != 0) {COLLECTOR_REFERENCE_INC (a42);}
		this -> a43 = a43; if (a43 != 0) {COLLECTOR_REFERENCE_INC (a43);}
		this -> a51 = a51; if (a51 != 0) {COLLECTOR_REFERENCE_INC (a51);}
		this -> a52 = a52; if (a52 != 0) {COLLECTOR_REFERENCE_INC (a52);}
		this -> a53 = a53; if (a53 != 0) {COLLECTOR_REFERENCE_INC (a53);}
		this -> a54 = a54; if (a54 != 0) {COLLECTOR_REFERENCE_INC (a54);}
		this -> a61 = a61; if (a61 != 0) {COLLECTOR_REFERENCE_INC (a61);}
		this -> a62 = a62; if (a62 != 0) {COLLECTOR_REFERENCE_INC (a62);}
		this -> a63 = a63; if (a63 != 0) {COLLECTOR_REFERENCE_INC (a63);}
		this -> a64 = a64; if (a64 != 0) {COLLECTOR_REFERENCE_INC (a64);}
		this -> a65 = a65; if (a65 != 0) {COLLECTOR_REFERENCE_INC (a65);}
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
		this -> freq5 = freq5; COLLECTOR_REFERENCE_INC (freq5);
		this -> amp5 = amp5; COLLECTOR_REFERENCE_INC (amp5);
		this -> ratio5 = ratio5; COLLECTOR_REFERENCE_INC (ratio5);
		this -> feedback5 = feedback5; COLLECTOR_REFERENCE_INC (feedback5);
		this -> freq6 = freq6; COLLECTOR_REFERENCE_INC (freq6);
		this -> amp6 = amp6; COLLECTOR_REFERENCE_INC (amp6);
		this -> ratio6 = ratio6; COLLECTOR_REFERENCE_INC (ratio6);
		this -> feedback6 = feedback6; COLLECTOR_REFERENCE_INC (feedback6);
		feedback ();
	}
	~ fm6_panel_action (void) {
		if (algo != 0) algo -> removeAtom ();
		if (a21= 0) a21 -> removeAtom (); if (a31 != 0) a31 -> removeAtom (); if (a32 != 0) a32 -> removeAtom (); if (a41 != 0) a41 -> removeAtom (); if (a42 != 0) a42 -> removeAtom ();
		if (a43 != 0) a43 -> removeAtom (); if (a51 != 0) a51 -> removeAtom (); if (a52 != 0) a52 -> removeAtom (); if (a53 != 0) a53 -> removeAtom ();
		if (a54 != 0) a54 -> removeAtom (); if (a61 != 0) a61 -> removeAtom (); if (a62 != 0) a62 -> removeAtom ();
		if (a63 != 0) a63 -> removeAtom (); if (a64 != 0) a64 -> removeAtom (); if (a65 != 0) a65 -> removeAtom ();
		freq1 -> removeAtom (); amp1 -> removeAtom (); ratio1 -> removeAtom (); feedback1 -> removeAtom ();
		freq2 -> removeAtom (); amp2 -> removeAtom (); ratio2 -> removeAtom (); feedback2 -> removeAtom ();
		freq3 -> removeAtom (); amp3 -> removeAtom (); ratio3 -> removeAtom (); feedback3 -> removeAtom ();
		freq4 -> removeAtom (); amp4 -> removeAtom (); ratio4 -> removeAtom (); feedback4 -> removeAtom ();
		freq5 -> removeAtom (); amp5 -> removeAtom (); ratio5 -> removeAtom (); feedback5 -> removeAtom ();
		freq6 -> removeAtom (); amp6 -> removeAtom (); ratio6 -> removeAtom (); feedback6 -> removeAtom ();
	}
};

bool fm6_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * algo = 0;
	PrologElement * a12 = 0, * a13 = 0, * a14 = 0, * a15 = 0, * a16 = 0, * a23 = 0, * a24 = 0, * a25 = 0, * a26 = 0, * a34 = 0, * a35 = 0, * a36 = 0, * a45 = 0, * a46 = 0, * a56 = 0;
	PrologElement * freq1 = 0, * amp1 = 0, * ratio1 = 0, * feedback1 = 0;
	PrologElement * freq2 = 0, * amp2 = 0, * ratio2 = 0, * feedback2 = 0;
	PrologElement * freq3 = 0, * amp3 = 0, * ratio3 = 0, * feedback3 = 0;
	PrologElement * freq4 = 0, * amp4 = 0, * ratio4 = 0, * feedback4 = 0;
	PrologElement * freq5 = 0, * amp5 = 0, * ratio5 = 0, * feedback5 = 0;
	PrologElement * freq6 = 0, * amp6 = 0, * ratio6 = 0, * feedback6 = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (algo == 0 && a56 == 0) {
				if (dx) algo = el;
				else {
					if (a12 == 0) a12 = el;
					else if (a13 == 0) a13 = el;
					else if (a14 == 0) a14 = el;
					else if (a15 == 0) a15 = el;
					else if (a16 == 0) a16 = el;
					else if (a23 == 0) a23 = el;
					else if (a24 == 0) a24 = el;
					else if (a25 == 0) a25 = el;
					else if (a26 == 0) a26 = el;
					else if (a34 == 0) a34 = el;
					else if (a35 == 0) a35 = el;
					else if (a36 == 0) a36 = el;
					else if (a45 == 0) a45 = el;
					else if (a46 == 0) a46 = el;
					else if (a56 == 0) a56 = el;
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
			else if (freq5 == 0) freq5 = el;
			else if (amp5 == 0) amp5 = el;
			else if (ratio5 == 0) ratio5 = el;
			else if (feedback5 == 0) feedback5 = el;
			else if (freq6 == 0) freq6 = el;
			else if (amp6 == 0) amp6 = el;
			else if (ratio6 == 0) ratio6 = el;
			else if (feedback6 == 0) feedback6 = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || feedback4 == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	fm6_panel_action * machine = new fm6_panel_action (resources, root, atom -> getAtom (),
		algo ? algo -> getAtom () : 0,
		a12 ? a12 -> getAtom () : 0, a13 ? a13 -> getAtom () : 0, a14 ? a14 -> getAtom () : 0, a15 ? a15 -> getAtom () : 0, a16 ? a16 -> getAtom () : 0,
		a23 ? a23 -> getAtom () : 0, a24 ? a24 -> getAtom () : 0, a25 ? a25 -> getAtom () : 0, a26 ? a26 -> getAtom () : 0,
		a34 ? a34 -> getAtom () : 0, a35 ? a35 -> getAtom () : 0, a36 ? a36 -> getAtom () : 0, a45 ? a45 -> getAtom () : 0, a46 ? a46 -> getAtom () : 0, a56 ? a56 -> getAtom () : 0,
		freq1 -> getAtom (), amp1 -> getAtom (), ratio1 -> getAtom (), feedback1 -> getAtom (),
		freq2 -> getAtom (), amp2 -> getAtom (), ratio2 -> getAtom (), feedback2 -> getAtom (),
		freq3 -> getAtom (), amp3 -> getAtom (), ratio3 -> getAtom (), feedback3 -> getAtom (),
		freq4 -> getAtom (), amp4 -> getAtom (), ratio4 -> getAtom (), feedback4 -> getAtom (),
		freq5 -> getAtom (), amp5 -> getAtom (), ratio5 -> getAtom (), feedback5 -> getAtom (),
		freq6 -> getAtom (), amp6 -> getAtom (), ratio6 -> getAtom (), feedback6 -> getAtom (),
		false, dx);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

fm6_panel_class :: fm6_panel_class (PrologLunarServiceClass * servo, bool dx) {
	this -> dx = dx;
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
