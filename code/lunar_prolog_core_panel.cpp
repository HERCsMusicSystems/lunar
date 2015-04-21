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
// This file was created on Wednesday, 17th March 2015 at 11:12:59. //
/////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"
#include "graphic_resources.h"
#include <string.h>
#include "multiplatform_audio.h"

extern MultiplatformAudio audio;

class core_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * core;
	PrologAtom * reactor;
	GtkWidget * viewport;
	cairo_surface_t * background_image;
	point location;
	point captured;
	int captured_button;
	GdkEventType captured_type;
	button_active_graphics START;
	button_active_graphics STOP;
	button_active_graphics RECORD;
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	core_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom, PrologAtom * core, PrologAtom * reactor) :
		STOP (point (10.0, 92.0), 1, resources, true),
		START (point (40.0, 92.0), 2, resources, true),
		RECORD (point (90.0, 92.0), 3, resources, true)
	{
		this -> root = root;
		captured_button = 0;
		captured_type = (GdkEventType) 0;
		background_image = resources != 0 ? resources -> core_panel_surface : 0;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> core = core; COLLECTOR_REFERENCE_INC (core);
		this -> reactor = reactor; COLLECTOR_REFERENCE_INC (reactor);
	}
	~ core_panel_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		core -> removeAtom ();
		reactor -> removeAtom ();
	}
};

/*
class eg_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * t1, * t2, * t3, * t4, * l1, * l2, * l3, * l4;
	bool feg;
	GtkWidget * viewport;
	point captured;
	point location;
	int captured_button;
	knob_active_graphics T1, T2, T3, T4, L1, L2, L3, L4;
	cairo_surface_t * background_image;
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
*/

static gint CorePanelKeyon (GtkWidget * viewport, GdkEventButton * event, core_panel_action * action) {
	printf ("clicked....[%i]\n", (int) event -> type);
	action -> captured_type = event -> type;
	action -> captured_button = event -> button;
	point location (event -> x, event -> y);
	action -> captured = location;
	bool redraw = false;
	if (action -> STOP . keyon (location)) {
		action -> STOP . engaged = true;
		action -> START . engaged = false;
		redraw = true;
	}
	if (action -> START . keyon (location)) {
		action -> START . engaged = true;
		redraw = true;
	}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gint CorePanelKeyoff (GtkWidget * viewport, GdkEventButton * event, core_panel_action * action) {
	printf (".... [%i]\n", (int) event -> type);
	point location (event -> x, event -> y);
	bool redraw = false;
	if (action -> STOP . keyoff (location)) {action -> STOP . engaged = false; redraw = true;}
	if (action -> RECORD . keyoff (location)) {
		if (action -> captured_type == GDK_BUTTON_PRESS) {
			printf ("change record....\n");
			action -> RECORD . engaged = ! action -> RECORD . engaged;
			redraw = true;
		}
	}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gint CorePanelMove (GtkWidget * viewport, GdkEventButton * event, core_panel_action * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured;
	if (action -> captured_button > 1) delta *= 0.0078125;
	action -> captured = location;
	bool redraw = false;
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gboolean RedrawCorePanel (GtkWidget * viewport, GdkEvent * event, core_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> background_image != 0) {cairo_set_source_surface (cr, action -> background_image, 0, 0); cairo_paint (cr);}
	action -> STOP . draw (cr);
	action -> START . draw (cr);
	action -> RECORD . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}
static gboolean CorePanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, core_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static gboolean dnd_drop (GtkWidget * widget, GdkDragContext * context, gint x, gint y, guint time, gpointer * ptr) {
	GdkAtom target_type;
	if (context -> targets) {
		target_type = GDK_POINTER_TO_ATOM (g_list_nth_data (context -> targets, 0));
		gtk_drag_get_data (widget, context, target_type, time);
	} else return FALSE;
	return TRUE;
}
static gboolean dnd_motion (GtkWidget * widget, GdkDragContext * context, gint x, gint y, GtkSelectionData * gsd, guint ttype, guint time, gpointer * ptr) {return TRUE;}
static void dnd_leave (GtkWidget * widget, GdkDragContext * context, guint time, gpointer * ptr) {}
static void dnd_receive (GtkWidget * widget, GdkDragContext * context, gint x, gint y, GtkSelectionData * data, guint ttype, guint time, PrologRoot * root) {
	PrologDirectory * LunarDirectory = root -> searchDirectory ("lunar");
	if (LunarDirectory == 0) return;
	PrologAtom * LunarDrop = LunarDirectory -> searchAtom ("LunarDrop");
	if (LunarDrop == 0) return;
	gchar * ptr = (char *) data -> data;
	char command [4096];
	PrologElement * query = root -> earth ();
	while (strncmp (ptr, "file:///", 8) == 0) {
		ptr += 7;
		char * cp = command;
		while (* ptr >= ' ') * cp++ = * ptr++; * cp = '\0';
		query = root -> pair (root -> text (command), query);
		while (* ptr > '\0' && * ptr <= ' ') ptr++;
	}
	query = root -> pair (root -> integer ((int) y), query);
	query = root -> pair (root -> integer ((int) x), query);
	query = root -> pair (root -> atom (LunarDrop), query);
	query = root -> pair (root -> earth (), root -> pair (query, root -> earth ()));
	root -> resolution (query);
	delete query;
}
static void SamplingRateChanged (GtkWidget * combo, gpointer data) {
	int index = gtk_combo_box_get_active (GTK_COMBO_BOX (combo));
	printf ("selected [%i]\n", index);
}
static gboolean CreateCorePanelIdleCode (core_panel_action * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> atom -> name ());
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (CorePanelDeleteEvent), action);
	GtkWidget * area = gtk_fixed_new ();
	GtkWidget * input_combo = gtk_combo_box_new_text ();
	gtk_combo_box_append_text (GTK_COMBO_BOX (input_combo), "Inactive");
	for (int ind = 0; ind < audio . getNumberOfInputDevices (); ind++) {
		gtk_combo_box_append_text (GTK_COMBO_BOX (input_combo), audio . getInputDeviceName (ind));
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX (input_combo), audio . getSelectedInputDevice () + 1);
	gtk_fixed_put (GTK_FIXED (area), input_combo, 10, 10);
	GtkWidget * output_combo = gtk_combo_box_new_text ();
	gtk_combo_box_append_text (GTK_COMBO_BOX (output_combo), "Inactive");
	for (int ind = 0; ind < audio . getNumberOfOutputDevices (); ind++) {
		gtk_combo_box_append_text (GTK_COMBO_BOX (output_combo), audio . getOutputDeviceName (ind));
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX (output_combo), audio . getSelectedOutputDevice () + 1);
	gtk_fixed_put (GTK_FIXED (area), output_combo, 10, 30);
	GtkWidget * sampling_combo = gtk_combo_box_new_text();
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "8,000 Hz [Telephone]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "11,025 Hz [PCM MPEG]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "16,000 Hz [VoIP]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "22,050 Hz [PCM MPEG]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "32,000 Hz [miniDV]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "37,800 Hz [CD-XA]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "44,056 Hz [NTSC]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "44,100 Hz [Audio CD]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "48,000 Hz [HD-SDI]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "50,000 Hz [3M]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "50,400 Hz [X-80]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "88,200 Hz [CD]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "96,000 Hz [DVD-Audio]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "176,400 Hz [HDCD]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "192,000 Hz [DVD-Audio]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "362,800 Hz [eXtreme]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "2,822,400 Hz [SACD]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (sampling_combo), "5,644,800 Hz [DSD]");
	gtk_combo_box_set_active (GTK_COMBO_BOX (sampling_combo), 8);
	g_signal_connect (G_OBJECT (sampling_combo), "changed", G_CALLBACK (SamplingRateChanged), 0);
	gtk_fixed_put (GTK_FIXED (area), sampling_combo, 140, 10);
	gtk_container_add (GTK_CONTAINER (action -> viewport), area);

	g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (RedrawCorePanel), action);
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (CorePanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (CorePanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (CorePanelMove), action);
	if (action -> background_image != 0) gtk_window_resize (GTK_WINDOW (action -> viewport),
											cairo_image_surface_get_width (action -> background_image),
											cairo_image_surface_get_height (action -> background_image));

	const GtkTargetEntry targets [2] = {{"text/plain", 0, 0}, {"application/x-rootwindow-drop", 0, 0}};
	gtk_drag_dest_set (area, GTK_DEST_DEFAULT_ALL, targets, 2, GDK_ACTION_COPY);
	g_signal_connect (area, "drag-drop", G_CALLBACK (dnd_drop), 0);
	g_signal_connect (area, "drag-motion", G_CALLBACK (dnd_motion), 0);
	g_signal_connect (area, "drag-data-received", G_CALLBACK (dnd_receive), action -> root);
	g_signal_connect (area, "drag-leave", G_CALLBACK (dnd_leave), 0);

	gtk_widget_show_all (action -> viewport);
	return FALSE;
}

static gboolean RepositionCorePanel (core_panel_action * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

bool core_panel_action :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) return remove ();
	PrologElement * x = 0, * y = 0;
	PrologElement * refresher = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) if (x == 0) x = el; else y = el;
		if (el -> isEarth ()) refresher = el;
		parameters = parameters -> getRight ();
	}
	// if (refresher != 0) {feedback (); gtk_widget_queue_draw (viewport); return true;}
	if (x == 0 || y == 0) return true;
	location = point (x -> getNumber (), y -> getNumber ());
	g_idle_add ((GSourceFunc) RepositionCorePanel, this);
	return true;
}

bool core_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologAtom * core = 0;
	PrologAtom * reactor = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (core == 0) core = el -> getAtom ();
			else reactor = el -> getAtom ();
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0) return false;
	if (core == 0 || reactor == 0) {
		PrologDirectory * dir = root -> searchDirectory ("lunar");
		if (dir == 0) return false;
		if (core == 0) core = dir -> searchAtom ("core");
		if (reactor == 0) reactor = dir -> searchAtom ("reactor");
	}
	if (core == 0 || reactor == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	core_panel_action * machine = new core_panel_action (resources, root, atom -> getAtom (), core, reactor);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateCorePanelIdleCode, machine);
	return true;
}

core_panel_class :: core_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
