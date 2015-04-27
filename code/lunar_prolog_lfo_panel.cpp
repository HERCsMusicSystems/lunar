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
// This file was created on Wednesday, 25th April 2015 at 18:37:13. //
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

class lfo_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * speed, * wave, * pulse, * phase, * sync, * vibrato, * tremolo, * wah_wah, * pan;
	GtkWidget * viewport;
	point captured;
	point location;
	int captured_button;
	knob_active_graphics SPEED, WAVE, PULSE, PHASE, VIBRATO, TREMOLO, WAH_WAH, PAN;
	button_active_graphics SYNC;
	cairo_surface_t * background_image;
	cairo_surface_t * vibrato_image;
	cairo_surface_t * tremolo_image;
	cairo_surface_t * wah_wah_image;
	cairo_surface_t * pan_image;
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
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
	bool code (PrologElement * parameters, PrologResolution * resolution);
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
	SYNC (point (263, 94), 0, resources, true) {
		captured_button = 0;
		background_image = resources != 0 ? resources -> lfo_panel_surface : 0;
		vibrato_image = resources != 0 ? resources -> lfo_vibrato_surface : 0;
		tremolo_image = resources != 0 ? resources -> lfo_tremolo_surface : 0;
		wah_wah_image = resources != 0 ? resources -> lfo_wah_wah_surface : 0;
		pan_image = resources != 0 ? resources -> lfo_pan_surface : 0;
		viewport = 0;
		this -> root = root;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
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
		atom -> setMachine (0);
		atom -> removeAtom ();
		speed -> removeAtom (); wave -> removeAtom (); pulse -> removeAtom (); phase -> removeAtom (); sync -> removeAtom ();
		if (vibrato != 0) vibrato -> removeAtom ();
		if (tremolo != 0) tremolo -> removeAtom ();
		if (wah_wah != 0) wah_wah -> removeAtom ();
		if (pan != 0) pan -> removeAtom ();
	}
};

static gboolean LfoPanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, lfo_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static gboolean RedrawLfoPanel (GtkWidget * viewport, GdkEvent * event, lfo_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> background_image != 0) {cairo_set_source_surface (cr, action -> background_image, 0, 0); cairo_paint (cr);}
	action -> SPEED . draw (cr);
	action -> WAVE . draw (cr);
	action -> PULSE . draw (cr);
	action -> PHASE . draw (cr);
	if (action -> vibrato != 0) {cairo_set_source_surface (cr, action -> vibrato_image, 18, 98); cairo_paint (cr); action -> VIBRATO . draw (cr);}
	if (action -> tremolo != 0) {cairo_set_source_surface (cr, action -> tremolo_image, 88, 98); cairo_paint (cr); action -> TREMOLO . draw (cr);}
	if (action -> wah_wah != 0) {cairo_set_source_surface (cr, action -> wah_wah_image, 156, 98); cairo_paint (cr); action -> WAH_WAH . draw (cr);}
	if (action -> pan != 0) {cairo_set_source_surface (cr, action -> pan_image, 226, 104); cairo_paint (cr); action -> PAN . draw (cr);}
	action -> SYNC . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}
static gint LfoPanelKeyon (GtkWidget * viewport, GdkEventButton * event, lfo_panel_action * action) {
	action -> captured_button = event -> button;
	point location (event -> x, event -> y);
	action -> captured = location;
	action -> SPEED . keyon (location);
	action -> WAVE . keyon (location);
	action -> PULSE . keyon (location);
	action -> PHASE . keyon (location);
	if (action -> vibrato != 0) action -> VIBRATO . keyon (location);
	if (action -> tremolo != 0) action -> TREMOLO . keyon (location);
	if (action -> wah_wah != 0) action -> WAH_WAH . keyon (location);
	if (action -> pan != 0) action -> PAN . keyon (location);
	if (action -> SYNC . keyon (location)) {
		action -> SYNC . engaged = ! action -> SYNC . engaged;
		action -> sync_changed ();
		gtk_widget_queue_draw (viewport);
	}
	return TRUE;
}
static gint LfoPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, lfo_panel_action * action) {
	point location (event -> x, event -> y);
	action -> SPEED . keyoff (location);
	action -> WAVE . keyoff (location);
	action -> PULSE . keyoff (location);
	action -> PHASE . keyoff (location);
	if (action -> vibrato != 0) action -> VIBRATO . keyoff (location);
	if (action -> tremolo != 0) action -> TREMOLO . keyoff (location);
	if (action -> wah_wah != 0) action -> WAH_WAH . keyoff (location);
	if (action -> pan != 0) action -> PAN . keyoff (location);
	return TRUE;
}
static gint LfoPanelMove (GtkWidget * viewport, GdkEventButton * event, lfo_panel_action * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured;
	if (action -> captured_button > 1) delta *= 0.0078125;
	action -> captured = location;
	bool redraw = false;
	if (action -> SPEED . move (delta)) {action -> move (0); redraw = true;}
	if (action -> WAVE . move (delta)) {action -> move (1); redraw = true;}
	if (action -> PULSE . move (delta)) {action -> move (2); redraw = true;}
	if (action -> PHASE . move (delta)) {action -> move (3); redraw = true;}
	if (action -> vibrato != 0 && action -> VIBRATO . move (delta)) {action -> move (4); redraw = true;}
	if (action -> tremolo != 0 && action -> TREMOLO . move (delta)) {action -> move (5); redraw = true;}
	if (action -> wah_wah != 0 && action -> WAH_WAH . move (delta)) {action -> move (6); redraw = true;}
	if (action -> pan != 0 && action -> PAN . move (delta)) {action -> move (7); redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gboolean CreateLfoPanelIdleCode (lfo_panel_action * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> atom -> name ());
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (LfoPanelDeleteEvent), action);
	GtkWidget * area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (action -> viewport), area);
	g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (RedrawLfoPanel), action);
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (LfoPanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (LfoPanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (LfoPanelMove), action);
	if (action -> background_image != 0) gtk_window_resize (GTK_WINDOW (action -> viewport),
											cairo_image_surface_get_width (action -> background_image),
											cairo_image_surface_get_height (action -> background_image));
	gtk_widget_show_all (action -> viewport);
	return FALSE;
}

static gboolean RepositionLfoPanel (lfo_panel_action * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

bool lfo_panel_action :: code (PrologElement * parameters, PrologResolution * resolution) {
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
	g_idle_add ((GSourceFunc) RepositionLfoPanel, this);
	return true;
}

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
	g_idle_add ((GSourceFunc) CreateLfoPanelIdleCode, machine);
	return true;
}

lfo_panel_class :: lfo_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}

