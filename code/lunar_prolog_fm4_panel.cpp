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

#include "prolog_lunar.h"
#include "graphic_resources.h"

static double prepare (double angle) {
	angle *= 16384.0;
	if (angle > 16384.0) angle = 16384.0;
	if (angle < -16384.0) angle = -16384.0;
	int ind = (int) angle;
	return (double) ind;
}
static double unprepare (double angle) {
	if (angle <= -16384.0) return -1.0;
	if (angle >= 16384.0) return 1.0;
	return angle / 16384.0;
}

class fm4_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * algo;
	PrologAtom * freq1, * amp1, * ratio1, * feedback1;
	PrologAtom * freq2, * amp2, * ratio2, * feedback2;
	PrologAtom * freq3, * amp3, * ratio3, * feedback3;
	PrologAtom * freq4, * amp4, * ratio4, * feedback4;
	GtkWidget * viewport;
	point captured;
	point location;
	int captured_button;
	knob_active_graphics ALGO;
	knob_active_graphics FREQ1, AMP1, RATIO1, FEEDBACK1;
	knob_active_graphics FREQ2, AMP2, RATIO2, FEEDBACK2;
	knob_active_graphics FREQ3, AMP3, RATIO3, FEEDBACK3;
	knob_active_graphics FREQ4, AMP4, RATIO4, FEEDBACK4;
	cairo_surface_t * background_image;
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	void move (int ind) {
		PrologAtom * a = 0;
		double v;
		switch (ind) {
		case 0: a = algo; v = (double) ((int) ALGO . value); break;
		case 1: a = freq1; v = prepare (FREQ1 . angle - 0.5); break;
		case 2: a = amp1; v = prepare (AMP1 . angle - 1.0); break;
		case 3: a = ratio1; v = RATIO1 . value; break;
		case 4: a = feedback1; v = prepare (FEEDBACK1 . angle); break;
		case 5: a = freq2; v = prepare (FREQ2 . angle - 0.5); break;
		case 6: a = amp2; v = prepare (AMP2 . angle - 1.0); break;
		case 7: a = ratio2; v = RATIO2 . value; break;
		case 8: a = feedback2; v = prepare (FEEDBACK2 . angle); break;
		case 9: a = freq3; v = prepare (FREQ3 . angle - 0.5); break;
		case 10: a = amp3; v = prepare (AMP3 . angle - 1.0); break;
		case 11: a = ratio3; v = RATIO3 . value; break;
		case 12: a = feedback3; v = prepare (FEEDBACK3 . angle); break;
		case 13: a = freq4; v = prepare (FREQ4 . angle - 0.5); break;
		case 14: a = amp4; v = prepare (AMP4 . angle - 1.0); break;
		case 15: a = ratio4; v = RATIO4 . value; break;
		case 16: a = feedback4; v = prepare (FEEDBACK4 . angle); break;
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
			if (sub -> isNumber ()) ALGO . angle = sub -> getNumber () / 7.0;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ1 . angle = unprepare (sub -> getNumber ()) + 0.5;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP1 . angle = unprepare (sub -> getNumber ()) + 1.0;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO1 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK1 . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ2 . angle = unprepare (sub -> getNumber ()) + 0.5;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP2 . angle = unprepare (sub -> getNumber ()) + 1.0;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO2 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK2 . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ3 . angle = unprepare (sub -> getNumber ()) + 0.5;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP3 . angle = unprepare (sub -> getNumber ()) + 1.0;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO3 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK3 . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FREQ4 . angle = unprepare (sub -> getNumber ()) + 0.5;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) AMP4 . angle = unprepare (sub -> getNumber ()) + 1.0;
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) RATIO4 . setValue (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) FEEDBACK4 . angle = unprepare (sub -> getNumber ());
		}
		delete query;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
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
	FEEDBACK4 (point (520, 98), 0, resources, true, active, 0.0, 16384.0) {
		captured_button = 0;
		background_image = resources != 0 ? resources -> fm4_panel_surface : 0;
		viewport = 0;
		this -> root = root;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
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
		atom -> setMachine (0);
		atom -> removeAtom ();
		algo -> removeAtom ();
		freq1 -> removeAtom (); amp1 -> removeAtom (); ratio1 -> removeAtom (); feedback1 -> removeAtom ();
		freq2 -> removeAtom (); amp2 -> removeAtom (); ratio2 -> removeAtom (); feedback2 -> removeAtom ();
		freq3 -> removeAtom (); amp3 -> removeAtom (); ratio3 -> removeAtom (); feedback3 -> removeAtom ();
		freq4 -> removeAtom (); amp4 -> removeAtom (); ratio4 -> removeAtom (); feedback4 -> removeAtom ();
	}
};

static gboolean EGPanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, fm4_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static gboolean RedrawEGPanel (GtkWidget * viewport, GdkEvent * event, fm4_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> background_image != 0) {cairo_set_source_surface (cr, action -> background_image, 0, 0); cairo_paint (cr);}
	action -> ALGO . draw (cr);
	action -> FREQ1 . draw (cr);
	action -> AMP1 . draw (cr);
	action -> RATIO1 . draw (cr);
	action -> FEEDBACK1 . draw (cr);
	action -> FREQ2 . draw (cr);
	action -> AMP2 . draw (cr);
	action -> RATIO2 . draw (cr);
	action -> FEEDBACK2 . draw (cr);
	action -> FREQ3 . draw (cr);
	action -> AMP3 . draw (cr);
	action -> RATIO3 . draw (cr);
	action -> FEEDBACK3 . draw (cr);
	action -> FREQ4 . draw (cr);
	action -> AMP4 . draw (cr);
	action -> RATIO4 . draw (cr);
	action -> FEEDBACK4 . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}
static gint EGPanelKeyon (GtkWidget * viewport, GdkEventButton * event, fm4_panel_action * action) {
	action -> captured_button = event -> button;
	point location (event -> x, event -> y);
	action -> captured = location;
	action -> ALGO . keyon (location);
	action -> FREQ1 . keyon (location);
	action -> AMP1 . keyon (location);
	action -> RATIO1 . keyon (location);
	action -> FEEDBACK1 . keyon (location);
	action -> FREQ2 . keyon (location);
	action -> AMP2 . keyon (location);
	action -> RATIO2 . keyon (location);
	action -> FEEDBACK2 . keyon (location);
	action -> FREQ3 . keyon (location);
	action -> AMP3 . keyon (location);
	action -> RATIO3 . keyon (location);
	action -> FEEDBACK3 . keyon (location);
	action -> FREQ4 . keyon (location);
	action -> AMP4 . keyon (location);
	action -> RATIO4 . keyon (location);
	action -> FEEDBACK4 . keyon (location);
	return TRUE;
}
static gint EGPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, fm4_panel_action * action) {
	point location (event -> x, event -> y);
	action -> ALGO . keyoff (location);
	action -> FREQ1 . keyoff (location);
	action -> AMP1 . keyoff (location);
	action -> RATIO1 . keyoff (location);
	action -> FEEDBACK1 . keyoff (location);
	action -> FREQ2 . keyoff (location);
	action -> AMP2 . keyoff (location);
	action -> RATIO2 . keyoff (location);
	action -> FEEDBACK2 . keyoff (location);
	action -> FREQ3 . keyoff (location);
	action -> AMP3 . keyoff (location);
	action -> RATIO3 . keyoff (location);
	action -> FEEDBACK3 . keyoff (location);
	action -> FREQ4 . keyoff (location);
	action -> AMP4 . keyoff (location);
	action -> RATIO4 . keyoff (location);
	action -> FEEDBACK4 . keyoff (location);
	return TRUE;
}
static gint EGPanelMove (GtkWidget * viewport, GdkEventButton * event, fm4_panel_action * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured;
	if (action -> captured_button > 1) delta *= 0.0078125;
	action -> captured = location;
	bool redraw = false;
	if (action -> ALGO . move (delta)) {action -> move (0); redraw = true;}
	if (action -> FREQ1 . move (delta)) {action -> move (1); redraw = true;}
	if (action -> AMP1 . move (delta)) {action -> move (2); redraw = true;}
	if (action -> RATIO1 . move (delta)) {action -> move (3); redraw = true;}
	if (action -> FEEDBACK1 . move (delta)) {action -> move (4); redraw = true;}
	if (action -> FREQ2 . move (delta)) {action -> move (5); redraw = true;}
	if (action -> AMP2 . move (delta)) {action -> move (6); redraw = true;}
	if (action -> RATIO2 . move (delta)) {action -> move (7); redraw = true;}
	if (action -> FEEDBACK2 . move (delta)) {action -> move (8); redraw = true;}
	if (action -> FREQ3 . move (delta)) {action -> move (9); redraw = true;}
	if (action -> AMP3 . move (delta)) {action -> move (10); redraw = true;}
	if (action -> RATIO3 . move (delta)) {action -> move (11); redraw = true;}
	if (action -> FEEDBACK3 . move (delta)) {action -> move (12); redraw = true;}
	if (action -> FREQ4 . move (delta)) {action -> move (13); redraw = true;}
	if (action -> AMP4 . move (delta)) {action -> move (14); redraw = true;}
	if (action -> RATIO4 . move (delta)) {action -> move (15); redraw = true;}
	if (action -> FEEDBACK4 . move (delta)) {action -> move (16); redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gboolean CreateEGPanelIdleCode (fm4_panel_action * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> atom -> name ());
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (EGPanelDeleteEvent), action);
	GtkWidget * area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (action -> viewport), area);
	g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (RedrawEGPanel), action);
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (EGPanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (EGPanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (EGPanelMove), action);
	if (action -> background_image != 0) gtk_window_resize (GTK_WINDOW (action -> viewport),
											cairo_image_surface_get_width (action -> background_image),
											cairo_image_surface_get_height (action -> background_image));
	gtk_widget_show_all (action -> viewport);
	return FALSE;
}

static gboolean RepositionFM4Panel (fm4_panel_action * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

bool fm4_panel_action :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) return remove ();
	PrologElement * x = 0, * y = 0;
	PrologElement * refresher = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) if (x == 0) x = el; else y = el;
		if (el -> isEarth ()) refresher = el;
		parameters = parameters -> getRight ();
	}
	if (refresher != 0) {feedback (); gtk_widget_queue_draw (viewport); return true;}
	if (x == 0 || y == 0) return true;
	location = point (x -> getNumber (), y -> getNumber ());
	g_idle_add ((GSourceFunc) RepositionFM4Panel, this);
	return true;
}

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
	g_idle_add ((GSourceFunc) CreateEGPanelIdleCode, machine);
	return true;
}

fm4_panel_class :: fm4_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
