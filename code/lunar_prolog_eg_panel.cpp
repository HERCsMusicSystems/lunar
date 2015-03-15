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

/////////////////////////////////////////////////////////////////////
// This file was created on Wednesday, 6th March 2015 at 09:57:59. //
/////////////////////////////////////////////////////////////////////

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

class eg_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * t1, * t2, * t3, * t4, * l1, * l2, * l3, * l4;
	bool feg;
	GtkWidget * viewport;
	point captured;
	point location;
	knob_active_graphics T1, T2, T3, T4, L1, L2, L3, L4;
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
		case 0: a = t1; v = prepare (T1 . angle); break;
		case 1: a = t2; v = prepare (T2 . angle); break;
		case 2: a = t3; v = prepare (T3 . angle); break;
		case 3: a = t4; v = prepare (T4 . angle); break;
		case 4: a = l1; v = prepare (L1 . angle - (feg ? 0.5 : 1.0)); break;
		case 5: a = l2; v = prepare (L2 . angle - (feg ? 0.5 : 1.0)); break;
		case 6: a = l3; v = prepare (L3 . angle - (feg ? 0.5 : 1.0)); break;
		case 7: a = l4; v = prepare (L4 . angle - (feg ? 0.5 : 1.0)); break;
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
									root -> pair (root -> var (7), root -> earth ()))))))));
		PrologElement * query;
		query = root -> pair (root -> pair (root -> atom (l4), root -> pair (root -> var (7), root -> earth ())), root -> earth ());
		query = root -> pair (root -> pair (root -> atom (l3), root -> pair (root -> var (6), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (l2), root -> pair (root -> var (5), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (l1), root -> pair (root -> var (4), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t4), root -> pair (root -> var (3), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t3), root -> pair (root -> var (2), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t2), root -> pair (root -> var (1), root -> earth ())), query);
		query = root -> pair (root -> pair (root -> atom (t1), root -> pair (root -> var (0), root -> earth ())), query);
		query = root -> pair (variables, query);
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (! el -> isPair ()) {delete query; return;}
			el = el -> getLeft (); if (! el -> isPair ()) {delete query; return;}
			PrologElement * sub = el -> getLeft ();
			if (sub -> isNumber ()) T1 . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) T2 . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) T3 . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) T4 . angle = unprepare (sub -> getNumber ());
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L1 . angle = unprepare (sub -> getNumber ()) + (feg ? 0.5 : 1.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L2 . angle = unprepare (sub -> getNumber ()) + (feg ? 0.5 : 1.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L3 . angle = unprepare (sub -> getNumber ()) + (feg ? 0.5 : 1.0);
			el = el -> getRight (); if (! el -> isPair ()) {delete query; return;} sub = el -> getLeft ();
			if (sub -> isNumber ()) L4 . angle = unprepare (sub -> getNumber ()) + (feg ? 0.5 : 1.0);
		}
		delete query;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	eg_panel_action (GraphicResources * resources, PrologRoot * root, bool feg, PrologAtom * atom,
		PrologAtom * t1, PrologAtom * t2, PrologAtom * t3, PrologAtom * t4,
		PrologAtom * l1, PrologAtom * l2, PrologAtom * l3, PrologAtom * l4, bool active) :
	T1 (point (18, 10), 0, resources, true, active, 0.0, 16384.0),
	T2 (point (88, 10), 0, resources, true, active, 0.0, 16384.0),
	T3 (point (158, 10), 0, resources, true, active, 0.0, 16384.0),
	T4 (point (228, 10), 0, resources, true, active, 0.0, 16384.0),
	L1 (point (18, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0),
	L2 (point (88, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0),
	L3 (point (158, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0),
	L4 (point (228, 98), 0, resources, true, active, feg ? -8192.0 : -16384.0, feg ? 8192.0 : 0.0) {
		background_image = resources != 0 ? resources -> eg_panel_surface : 0;
		viewport = 0;
		this -> feg = feg;
		this -> root = root;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> t1 = t1; COLLECTOR_REFERENCE_INC (t1);
		this -> t2 = t2; COLLECTOR_REFERENCE_INC (t2);
		this -> t3 = t3; COLLECTOR_REFERENCE_INC (t3);
		this -> t4 = t4; COLLECTOR_REFERENCE_INC (t4);
		this -> l1 = l1; COLLECTOR_REFERENCE_INC (l1);
		this -> l2 = l2; COLLECTOR_REFERENCE_INC (l2);
		this -> l3 = l3; COLLECTOR_REFERENCE_INC (l3);
		this -> l4 = l4; COLLECTOR_REFERENCE_INC (l4);
		feedback ();
	}
	~ eg_panel_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		t1 -> removeAtom (); t2 -> removeAtom (); t3 -> removeAtom (); t4 -> removeAtom ();
		l1 -> removeAtom (); l2 -> removeAtom (); l3 -> removeAtom (); l4 -> removeAtom ();
	}
};

static gboolean EGPanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, eg_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static gboolean RedrawEGPanel (GtkWidget * viewport, GdkEvent * event, eg_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> background_image != 0) {cairo_set_source_surface (cr, action -> background_image, 0, 0); cairo_paint (cr);}
	action -> T1 . draw (cr);
	action -> T2 . draw (cr);
	action -> T3 . draw (cr);
	action -> T4 . draw (cr);
	action -> L1 . draw (cr);
	action -> L2 . draw (cr);
	action -> L3 . draw (cr);
	action -> L4 . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}
static gint EGPanelKeyon (GtkWidget * viewport, GdkEventButton * event, eg_panel_action * action) {
	point location (event -> x, event -> y);
	action -> captured = location;
	action -> T1 . keyon (location);
	action -> T2 . keyon (location);
	action -> T3 . keyon (location);
	action -> T4 . keyon (location);
	action -> L1 . keyon (location);
	action -> L2 . keyon (location);
	action -> L3 . keyon (location);
	action -> L4 . keyon (location);
	return TRUE;
}
static gint EGPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, eg_panel_action * action) {
	point location (event -> x, event -> y);
	action -> T1 . keyoff (location);
	action -> T2 . keyoff (location);
	action -> T3 . keyoff (location);
	action -> T4 . keyoff (location);
	action -> L1 . keyoff (location);
	action -> L2 . keyoff (location);
	action -> L3 . keyoff (location);
	action -> L4 . keyoff (location);
	return TRUE;
}
static gint EGPanelMove (GtkWidget * viewport, GdkEventButton * event, eg_panel_action * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured;
	action -> captured = location;
	bool redraw = false;
	if (action -> T1 . move (delta)) {action -> move (0); redraw = true;}
	if (action -> T2 . move (delta)) {action -> move (1); redraw = true;}
	if (action -> T3 . move (delta)) {action -> move (2); redraw = true;}
	if (action -> T4 . move (delta)) {action -> move (3); redraw = true;}
	if (action -> L1 . move (delta)) {action -> move (4); redraw = true;}
	if (action -> L2 . move (delta)) {action -> move (5); redraw = true;}
	if (action -> L3 . move (delta)) {action -> move (6); redraw = true;}
	if (action -> L4 . move (delta)) {action -> move (7); redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gboolean CreateEGPanelIdleCode (eg_panel_action * action) {
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

static gboolean RepositionEGPanel (eg_panel_action * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

bool eg_panel_action :: code (PrologElement * parameters, PrologResolution * resolution) {
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
	g_idle_add ((GSourceFunc) RepositionEGPanel, this);
	return true;
}

bool eg_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * t1 = 0, * t2 = 0, * t3 = 0, * t4 = 0;
	PrologElement * l1 = 0, * l2 = 0, * l3 = 0, * l4 = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (t1 == 0) t1 = el;
			else if (t2 == 0) t2 = el;
			else if (t3 == 0) t3 = el;
			else if (t4 == 0) t4 = el;
			else if (l1 == 0) l1 = el;
			else if (l2 == 0) l2 = el;
			else if (l3 == 0) l3 = el;
			else if (l4 == 0) l4 = el;
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || l4 == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	eg_panel_action * machine = new eg_panel_action (resources, root, feg, atom -> getAtom (),
		t1 -> getAtom (), t2 -> getAtom (), t3 -> getAtom (), t4 -> getAtom (),
		l1 -> getAtom (), l2 -> getAtom (), l3 -> getAtom (), l4 -> getAtom (), true);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateEGPanelIdleCode, machine);
	return true;
}

eg_panel_class :: eg_panel_class (PrologLunarServiceClass * servo, bool feg) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
	this -> feg = feg;
}
