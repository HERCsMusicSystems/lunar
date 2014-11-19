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
// This file was created on ???????????????????????????????????????. //
///////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"
#include "graphic_resources.h"

class parameter_block_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * command;
	GtkWidget * viewport;
	point captured;
	point location;
	knob_active_graphics ctrl;
	cairo_surface_t * background_image;
	void move (double value) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> integer ((int) (value * 128.0) - 64),
								root -> earth ()));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void feedback (void) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> earth ()));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isNumber ()) ctrl . angle = el -> getNumber ();
		}
		delete query;
	}
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}

	bool code (PrologElement * paramters, PrologResolution * resolution);
	parameter_block_panel_action (GraphicResources * resources, PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command, bool active) :
		ctrl (point (0, 0), 0, resources, true)
	{
		background_image = resources != 0 ? resources -> vector_surface : 0;
		viewport = 0;
		this -> root = root;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		feedback ();
	}
	~ parameter_block_panel_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		command -> removeAtom ();
	}
};

static gboolean ParameterBlockPanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, parameter_block_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static gboolean RedrawParameterBlockPanel (GtkWidget * viewport, GdkEvent * event, parameter_block_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> background_image != 0) {cairo_set_source_surface (cr, action -> background_image, 0, 0); cairo_paint (cr);}
	action -> ctrl . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}
static gint ParameterBlockPanelKeyon (GtkWidget * viewport, GdkEventButton * event, parameter_block_panel_action * action) {
	point location (event -> x, event -> y);
	action -> captured = location;
	action -> ctrl . keyon (location);
	return TRUE;
}
static gint ParameterBlockPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, parameter_block_panel_action * action) {
	point location (event -> x, event -> y);
	action -> ctrl . keyoff (location);
	return TRUE;
}
static gint ParameterBlockPanelMove (GtkWidget * viewport, GdkEventButton * event, parameter_block_panel_action * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured;
	action -> captured = location;
	bool redraw = false;
	if (action -> ctrl . move (delta)) {action -> move (action -> ctrl . angle); redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gboolean CreateParameterBlockPanelIdleCode (parameter_block_panel_action * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> atom -> name ());
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (ParameterBlockPanelDeleteEvent), action);
	GtkWidget * area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (action -> viewport), area);
	g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (RedrawParameterBlockPanel), action);
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (ParameterBlockPanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (ParameterBlockPanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (ParameterBlockPanelMove), action);
	if (action -> background_image != 0) gtk_window_resize (GTK_WINDOW (action -> viewport),
											cairo_image_surface_get_width (action -> background_image),
											cairo_image_surface_get_height (action -> background_image));
	gtk_widget_show_all (action -> viewport);
	return FALSE;
}
static gboolean RepositionParameterBlockPanel (parameter_block_panel_action * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

bool parameter_block_panel_action :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) return remove ();
	PrologElement * x = 0;
	PrologElement * y = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) if (x == 0) x = el; else y = el;
		parameters = parameters -> getRight ();
	}
	if (x == 0 || y == 0) return true;
	location = point (x -> getNumber (), y -> getNumber ());
	g_idle_add ((GSourceFunc) RepositionParameterBlockPanel, this);
	return true;
}

bool parameter_block_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * command = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) {if (atom == 0) atom = el; else command = el;}
		if (el -> isVar ()) {if (atom != 0) command = atom; atom = el;}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || command == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	parameter_block_panel_action * machine = new parameter_block_panel_action (resources, root, directory, atom -> getAtom (), command -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateParameterBlockPanelIdleCode, machine);
	return true;
}

parameter_block_panel_class :: parameter_block_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> directory = servo -> directory;
	this -> resources = servo -> resources;
}
