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
// This file was created on Wednesday, 28th April 2015 at 18:37:13. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"
#include <string.h>

gboolean RemoveViewportIdleCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}

static gboolean RepositionAudioModulePanel (AudioModulePanel * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

static gboolean AudioModulePanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, AudioModulePanel * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}

static gboolean RedrawAudioModulePanel (GtkWidget * viewport, GdkEvent * event, AudioModulePanel * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> background_image != 0) {cairo_set_source_surface (cr, action -> background_image, 0, 0); cairo_paint (cr);}
	action -> redraw (cr);
	cairo_destroy (cr);
	return FALSE;
}

static gint AudioModulePanelKeyon (GtkWidget * viewport, GdkEventButton * event, AudioModulePanel * action) {
	//if (event -> type == GDK_BUTTON_PRESS && event -> button == 1 && event -> y < 20.0) {
	//	gtk_window_begin_move_drag (GTK_WINDOW (gtk_widget_get_toplevel (viewport)), event -> button, event -> x_root, event -> y_root, event -> time);
	//	return FALSE;
	//}
	action -> captured_button = event -> button;
	point location (event -> x, event -> y);
	action -> captured_location = location;
	action -> MouseKeyon (location, (int) event -> button);
	return TRUE;
}

static gint AudioModulePanelKeyoff (GtkWidget * viewport, GdkEventButton * event, AudioModulePanel * action) {
	point location (event -> x, event -> y);
	action -> MouseKeyoff (location, (int) event -> button);
	return TRUE;
}

static gint AudioModulePanelMove (GtkWidget * viewport, GdkEventButton * event, AudioModulePanel * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured_location;
	if (action -> captured_button > 1) delta *= 0.0078125;
	action -> captured_location = location;
	action -> MouseMove (delta);
	return TRUE;
}

static gboolean AudioModulePanelFunctionKey (GtkWidget * widget, GdkEventKey * event, AudioModulePanel * action) {
	action -> FunctionKey ((int) event -> keyval, (int) event -> state);
	return FALSE;
}

static gboolean dnd_drop (GtkWidget * widget, GdkDragContext * context, gint x, gint y, guint time, AudioModulePanel * action) {
	GdkAtom target_type;
	action -> not_ready_for_drop = false;
	if (context -> targets) {
		target_type = GDK_POINTER_TO_ATOM (g_list_nth_data (context -> targets, 0));
		gtk_drag_get_data (widget, context, target_type, time);
	} else return FALSE;
	return TRUE;
}

static gboolean dnd_motion (GtkWidget * widget, GdkDragContext * context, gint x, gint y, GtkSelectionData * gsd, guint ttype, guint time, gpointer * ptr) {return TRUE;}

static void dnd_leave (GtkWidget * widget, GdkDragContext * context, guint time, gpointer * ptr) {}

static void dnd_receive (GtkWidget * widget, GdkDragContext * context, gint x, gint y, GtkSelectionData * data, guint ttype, guint time, AudioModulePanel * action) {
	if (action == 0) return;
	if (action -> not_ready_for_drop) return;
	action -> not_ready_for_drop = true;
	PrologRoot * root = action -> root;
	PrologDirectory * LunarDirectory = root -> searchDirectory ("lunar");
	if (LunarDirectory == 0) return;
	PrologAtom * LunarDrop = LunarDirectory -> searchAtom ("LunarDrop");
	if (LunarDrop == 0) return;
	gchar * ptr = (char *) data -> data;
	char command [4096];
	PrologElement * query = root -> earth ();
	while (strncmp (ptr, "file:///", 8) == 0) {
		ptr += 7;
		#ifdef WIN32
		ptr++;
		#endif
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

gboolean CreateAudioModulePanelIdleCode (AudioModulePanel * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> atom -> name ());
	//gtk_window_set_decorated (GTK_WINDOW (action -> viewport), FALSE);
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (AudioModulePanelDeleteEvent), action);
	action -> area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (action -> viewport), action -> area);
	action -> gtk_redrawer = g_signal_connect (G_OBJECT (action -> area), "expose-event", G_CALLBACK (RedrawAudioModulePanel), action);
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (AudioModulePanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (AudioModulePanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (AudioModulePanelMove), action);
	g_signal_connect (G_OBJECT (action -> viewport), "key-press-event", G_CALLBACK (AudioModulePanelFunctionKey), action);
	if (action -> background_image != 0) gtk_window_resize (GTK_WINDOW (action -> viewport),
											cairo_image_surface_get_width (action -> background_image),
											cairo_image_surface_get_height (action -> background_image));
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	//gtk_window_set_position (GTK_WINDOW (action -> viewport), GTK_WIN_POS_CENTER);
	const GtkTargetEntry targets [3] = {{"text/plain", 0, 0}, {"text/uri-list", 0, 0}, {"application/x-rootwindow-drop", 0, 0}};
	gtk_drag_dest_set (action -> area, GTK_DEST_DEFAULT_ALL, targets, 3, GDK_ACTION_COPY);
	g_signal_connect (action -> area, "drag-drop", G_CALLBACK (dnd_drop), action);
	g_signal_connect (action -> area, "drag-motion", G_CALLBACK (dnd_motion), 0);
	g_signal_connect (action -> area, "drag-data-received", G_CALLBACK (dnd_receive), action);
	g_signal_connect (action -> area, "drag-leave", G_CALLBACK (dnd_leave), 0);
	gtk_widget_show_all (action -> viewport);
	return FALSE;
}

bool AudioModulePanel :: remove (bool remove_gtk) {
	if (remove_gtk) {
		g_signal_handler_disconnect (area, gtk_redrawer);
		g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
	}
	delete this;
	return true;
}

void AudioModulePanel :: update (void) {gtk_widget_queue_draw (viewport);}

void AudioModulePanel :: reposition (void) {g_idle_add ((GSourceFunc) RepositionAudioModulePanel, this);}

bool AudioModulePanel :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) return remove ();
	PrologElement * x = 0, * y = 0;
	PrologElement * refresher = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) if (x == 0) x = el; else y = el;
		if (el -> isEarth ()) refresher = el;
		parameters = parameters -> getRight ();
	}
	if (refresher != 0) {feedback (); update (); return true;}
	if (x == 0 || y == 0) return true;
	location = point (x -> getNumber (), y -> getNumber ());
	reposition ();
	return true;
}

void AudioModulePanel :: BuildPanel (void) {
	g_idle_add ((GSourceFunc) CreateAudioModulePanelIdleCode, this);
}

AudioModulePanel :: AudioModulePanel (PrologRoot * root, PrologAtom * atom, cairo_surface_t * background_image) {
	captured_button = 0;
	location = captured_location = point (32.0, 32.0);
	viewport = 0; area = 0; gtk_redrawer = 0;
	not_ready_for_drop = true;
	this -> root = root;
	this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
	this -> background_image = background_image;
}

AudioModulePanel :: ~ AudioModulePanel (void) {
	atom -> setMachine (0);
	atom -> removeAtom ();
}
