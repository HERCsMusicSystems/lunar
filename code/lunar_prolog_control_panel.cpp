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
// This file was created on Friday, 29th August 2014 at 11:45:49 AM. //
///////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"
#include "graphics2d.h"
#include "graphic_resources.h"

class control_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologDirectory * directory;
	PrologAtom * atom;
	PrologAtom * command;
	GraphicResources * resources;
	GtkWidget * viewport;
	point location;
	knob_active_graphics attack;
	knob_active_graphics decay;
	knob_active_graphics sustain;
	knob_active_graphics release;
	vector_active_graphics vector;
	keyboard_active_graphics keyboard;
	display_active_graphics display;
	button_active_graphics program0, program1, program2, program3, program4, program5, program6, program7, program8, program9;
	point captured;
	void program_action (button_active_graphics * button, char * area) {
		program0 . engaged = false;
		program1 . engaged = false;
		program2 . engaged = false;
		program3 . engaged = false;
		program4 . engaged = false;
		program5 . engaged = false;
		program6 . engaged = false;
		program7 . engaged = false;
		program8 . engaged = false;
		program9 . engaged = false;
		button -> engaged = true;
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> integer (button -> id),
								root -> earth ())));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) area_cat (area, 0, el -> getText ());
		}
		delete query;
	}
	void action (int ind, double value, char * area) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> integer (ind),
								root -> pair (root -> Double (value),
								root -> earth ()))));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) area_cat (area, 0, el -> getText ());
		}
		delete query;
	}
	void action (int ind, double x, double y, char * area) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> integer (ind),
								root -> pair (root -> Double (x),
								root -> pair (root -> Double (y),
								root -> earth ())))));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) area_cat (area, 0, el -> getText ());
		}
		delete query;
	}
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	control_panel_action (GraphicResources * resources, PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command)
	: attack (point (10.0, 10.0), 1, resources), decay (point (110.0, 10.0), 2, resources),
	sustain (point (210.0, 10.0), 3, resources), release (point (310.0, 10.0), 4, resources, true),
	vector (point (10.0, 100.0), 5, resources), keyboard (point (100.0, 280.0), 2, 6, resources, true),
	display (point (410.0, 10.0), 7, resources, true),
	program0 (point (200.0, 260.0), 100, resources, true),
	program1 (point (240.0, 260.0), 101, resources, true),
	program2 (point (280.0, 260.0), 102, resources, true),
	program3 (point (320.0, 260.0), 103, resources, true),
	program4 (point (360.0, 260.0), 104, resources, true),
	program5 (point (400.0, 260.0), 105, resources, true),
	program6 (point (440.0, 260.0), 106, resources, true),
	program7 (point (480.0, 260.0), 107, resources, true),
	program8 (point (520.0, 260.0), 108, resources, true),
	program9 (point (560.0, 260.0), 109, resources, true)
	{
		this -> root = root;
		this -> directory = directory;
		this -> resources = resources;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		viewport = 0;
	}
	~ control_panel_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		command -> removeAtom ();
	}
};

static gboolean RedrawControlPanel (GtkWidget * viewport, GdkEvent * event, control_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	action -> attack . draw (cr);
	action -> decay . draw (cr);
	action -> sustain . draw (cr);
	action -> release . draw (cr);
	action -> vector . draw (cr);
	action -> keyboard . draw (cr);
	action -> display . draw (cr);
	action -> program0 . draw (cr);
	action -> program1 . draw (cr);
	action -> program2 . draw (cr);
	action -> program3 . draw (cr);
	action -> program4 . draw (cr);
	action -> program5 . draw (cr);
	action -> program6 . draw (cr);
	action -> program7 . draw (cr);
	action -> program8 . draw (cr);
	action -> program9 . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}

static gboolean RepositionControlPanel (control_panel_action * action) {
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	return FALSE;
}

bool control_panel_action :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) return remove ();
	PrologElement * x = 0;
	PrologElement * y = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isNumber ()) if (x == 0) x = el; else y = el;
		parameters = parameters -> getRight ();
	}
	if (x == 0 || y == 0) return true;
	if (x -> isInteger () && y -> isInteger ()) {
		location = point (x -> getNumber (), y -> getNumber ());
		g_idle_add ((GSourceFunc) RepositionControlPanel, this);
		return true;
	}
	return true;
}

static gboolean ControlPanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, control_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static gint ControlPanelKeyon (GtkWidget * viewport, GdkEventButton * event, control_panel_action * action) {
	point location (event -> x, event -> y);
	action -> captured = location;
	action -> attack . keyon (location);
	action -> decay . keyon (location);
	action -> sustain . keyon (location);
	action -> release . keyon (location);
	action -> vector . keyon (location);
	bool redraw = false;
	if (action -> program0 . keyon (location)) {action -> program_action (& action -> program0, action -> display . area); redraw = true;}
	if (action -> program1 . keyon (location)) {action -> program_action (& action -> program1, action -> display . area); redraw = true;}
	if (action -> program2 . keyon (location)) {action -> program_action (& action -> program2, action -> display . area); redraw = true;}
	if (action -> program3 . keyon (location)) {action -> program_action (& action -> program3, action -> display . area); redraw = true;}
	if (action -> program4 . keyon (location)) {action -> program_action (& action -> program4, action -> display . area); redraw = true;}
	if (action -> program5 . keyon (location)) {action -> program_action (& action -> program5, action -> display . area); redraw = true;}
	if (action -> program6 . keyon (location)) {action -> program_action (& action -> program6, action -> display . area); redraw = true;}
	if (action -> program7 . keyon (location)) {action -> program_action (& action -> program7, action -> display . area); redraw = true;}
	if (action -> program8 . keyon (location)) {action -> program_action (& action -> program8, action -> display . area); redraw = true;}
	if (action -> program9 . keyon (location)) {action -> program_action (& action -> program9, action -> display . area); redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gint ControlPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, control_panel_action * action) {
	point location (event -> x, event -> y);
	action -> attack . keyoff (location);
	action -> decay . keyoff (location);
	action -> sustain . keyoff (location);
	action -> release . keyoff (location);
	action -> vector . keyoff (location);
	return TRUE;
}
static gint ControlPanelMove (GtkWidget * viewport, GdkEventButton * event, control_panel_action * action) {
	point location (event -> x, event -> y);
	point delta = location - action -> captured;
	action -> captured = location;
	bool redraw = false;
	if (action -> attack . move (delta)) {action -> action (action -> attack . id, action -> attack . angle, action -> display . area); redraw = true;}
	if (action -> decay . move (delta))  {action -> action (action -> decay . id, action -> decay . angle, action -> display . area); redraw = true;}
	if (action -> sustain . move (delta)) {action -> action (action -> sustain . id, action -> sustain . angle, action -> display . area); redraw = true;}
	if (action -> release . move (delta)) {action -> action (action -> release . id, action -> release . angle, action -> display . area); redraw = true;}
	if (action -> vector . move (delta)) {
		action -> action (action -> vector . id, action -> vector . position . x, action -> vector . position . y, action -> display . area);
		redraw = true;
	}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gboolean CreateControlPanelIdleCode (control_panel_action * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> atom -> name ());
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (ControlPanelDeleteEvent), action);
	GtkWidget * area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (action -> viewport), area);
	g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (RedrawControlPanel), action);
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (ControlPanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (ControlPanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (ControlPanelMove), action);
	gtk_window_resize (GTK_WINDOW (action -> viewport), 900, 460);
	gtk_widget_show_all (action -> viewport);
	return FALSE;
}

bool control_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
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
	control_panel_action * machine = new control_panel_action (resources, root, directory, atom -> getAtom (), command -> getAtom ());
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateControlPanelIdleCode, machine);
	return true;
}

control_panel_class :: control_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> directory = servo -> directory;
	this -> resources = servo -> resources;
}
