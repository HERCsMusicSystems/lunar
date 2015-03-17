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

class adsr_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * a, * d, * s, * r;
	GtkWidget * viewport;
	point captured;
	point location;
	int captured_button;
	knob_active_graphics A, D, S, R;
	cairo_surface_t * background_image;
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	void move (int ind) {
		if (a == 0) return;
		PrologElement * query;
		if (d != 0 && s != 0 && r != 0) {
			switch (ind) {
			case 0: query = root -> pair (root -> atom (a), root -> pair (root -> Double (prepare (A . angle)), root -> earth ())); break;
			case 1: query = root -> pair (root -> atom (d), root -> pair (root -> Double (prepare (D . angle)), root -> earth ())); break;
			case 2: query = root -> pair (root -> atom (s), root -> pair (root -> Double (prepare (-1.0 + S . angle)), root -> earth ())); break;
			default: query = root -> pair (root -> atom (r), root -> pair (root -> Double (prepare (R . angle)), root -> earth ())); break;
			}
			query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		} else {
			query = root -> pair (root -> atom (a),
					root -> pair (root -> Double (prepare (A . angle)),
					root -> pair (root -> Double (prepare (D . angle)),
					root -> pair (root -> Double (prepare (S . angle - 1.0)),
					root -> pair (root -> Double (prepare (R . angle)), root -> earth ())))));
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
			if (sub -> isNumber ()) A . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;}
			sub = el -> getLeft ();
			if (sub -> isNumber ()) D . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;}
			sub = el -> getLeft ();
			if (sub -> isNumber ()) S . angle = 1.0 + unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;}
			sub = el -> getLeft ();
			if (sub -> isNumber ()) R . angle = unprepare (sub -> getNumber ());
		}
		delete query;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	adsr_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom,
											PrologAtom * a, PrologAtom * d, PrologAtom * s, PrologAtom * r, bool active) :
	A (point (18, 10), 0, resources, true, active, 0.0, 16384.0),
	D (point (88, 10), 0, resources, true, active, 0.0, 16384.0),
	S (point (158, 10), 0, resources, true, active, -16384.0, 0.0),
	R (point (228, 10), 0, resources, true, active, 0.0, 16384.0) {
		captured_button = 0;
		background_image = resources != 0 ? resources -> adsr_panel_surface : 0;
		viewport = 0;
		this -> root = root;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> a = a; COLLECTOR_REFERENCE_INC (a);
		this -> d = d; if (d != 0) {COLLECTOR_REFERENCE_INC (d);}
		this -> s = s; if (s != 0) {COLLECTOR_REFERENCE_INC (s);}
		this -> r = r; if (r != 0) {COLLECTOR_REFERENCE_INC (r);}
		feedback ();
	}
	~ adsr_panel_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		a -> removeAtom ();
		if (d != 0) d -> removeAtom ();
		if (s != 0) s -> removeAtom ();
		if (r != 0) r -> removeAtom ();
	}
};

static gboolean AdsrPanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, adsr_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static gboolean RedrawAdsrPanel (GtkWidget * viewport, GdkEvent * event, adsr_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> background_image != 0) {cairo_set_source_surface (cr, action -> background_image, 0, 0); cairo_paint (cr);}
	action -> A . draw (cr);
	action -> D . draw (cr);
	action -> S . draw (cr);
	action -> R . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}
static gint AdsrPanelKeyon (GtkWidget * viewport, GdkEventButton * event, adsr_panel_action * action) {
	action -> captured_button = event -> button;
	point location (event -> x, event -> y);
	action -> captured = location;
	action -> A . keyon (location);
	action -> D . keyon (location);
	action -> S . keyon (location);
	action -> R . keyon (location);
	return TRUE;
}
static gint AdsrPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, adsr_panel_action * action) {
	point location (event -> x, event -> y);
	action -> A . keyoff (location);
	action -> D . keyoff (location);
	action -> S . keyoff (location);
	action -> R . keyoff (location);
	return TRUE;
}
static gint AdsrPanelMove (GtkWidget * viewport, GdkEventButton * event, adsr_panel_action * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured;
	if (action -> captured_button > 1) delta *= 0.0078125;
	action -> captured = location;
	bool redraw = false;
	if (action -> A . move (delta)) {action -> move (0); redraw = true;}
	if (action -> D . move (delta)) {action -> move (1); redraw = true;}
	if (action -> S . move (delta)) {action -> move (2); redraw = true;}
	if (action -> R . move (delta)) {action -> move (3); redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gboolean CreateAdsrPanelIdleCode (adsr_panel_action * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> atom -> name ());
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (AdsrPanelDeleteEvent), action);
	GtkWidget * area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (action -> viewport), area);
	g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (RedrawAdsrPanel), action);
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (AdsrPanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (AdsrPanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (AdsrPanelMove), action);
	if (action -> background_image != 0) gtk_window_resize (GTK_WINDOW (action -> viewport),
											cairo_image_surface_get_width (action -> background_image),
											cairo_image_surface_get_height (action -> background_image));
	gtk_widget_show_all (action -> viewport);
	return FALSE;
}

static gboolean RepositionAdsrPanel (adsr_panel_action * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

bool adsr_panel_action :: code (PrologElement * parameters, PrologResolution * resolution) {
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
	g_idle_add ((GSourceFunc) RepositionAdsrPanel, this);
	return true;
}

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
	g_idle_add ((GSourceFunc) CreateAdsrPanelIdleCode, machine);
	return true;
}

adsr_panel_class :: adsr_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
