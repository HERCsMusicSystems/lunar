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
	button_active_graphics selector0, selector1, selector2, selector3, selector4, selector5, selector6, selector7, selector8, selector9;
	button_active_graphics add_one, sub_one, delta_1, delta_8, delta_128;
	point captured;
	int programs [10];
	int current_program;
	int current_delta;
	void reset_buttons (int id) {
		if (selector0 . id <= id && id <= selector9 . id) {
			selector0 . engaged = false;
			selector1 . engaged = false;
			selector2 . engaged = false;
			selector3 . engaged = false;
			selector4 . engaged = false;
			selector5 . engaged = false;
			selector6 . engaged = false;
			selector7 . engaged = false;
			selector8 . engaged = false;
			selector9 . engaged = false;
		}
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
	}
	void program_action (button_active_graphics * button, char * area) {
		reset_buttons (button -> id);
		if (selector0 . id <= button -> id && button -> id <= selector9 . id) {
			current_program = button -> id - selector0 . id;
			switch (programs [current_program]) {
			case 0: program0 . engaged = true; break;
			case 1: program1 . engaged = true; break;
			case 2: program2 . engaged = true; break;
			case 3: program3 . engaged = true; break;
			case 4: program4 . engaged = true; break;
			case 5: program5 . engaged = true; break;
			case 6: program6 . engaged = true; break;
			case 7: program7 . engaged = true; break;
			case 8: program8 . engaged = true; break;
			case 9: program9 . engaged = true; break;
			default: break;
			}
		} else programs [current_program] = button -> id - program0 . id;
		button -> engaged = true;
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> integer (programs [0]),
								root -> pair (root -> integer (programs [1]),
								root -> pair (root -> integer (programs [2]),
								root -> pair (root -> integer (programs [3]),
								root -> pair (root -> integer (programs [4]),
								root -> pair (root -> integer (programs [5]),
								root -> pair (root -> integer (programs [6]),
								root -> pair (root -> integer (programs [7]),
								root -> pair (root -> integer (programs [8]),
								root -> pair (root -> integer (programs [9]),
								root -> earth ()))))))))))));
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
	: attack (point (10.0, 10.0), 1, resources, true), decay (point (110.0, 10.0), 2, resources, true),
	sustain (point (210.0, 10.0), 3, resources, true), release (point (310.0, 10.0), 4, resources, true),
	vector (point (10.0, 100.0), 5, resources, true), keyboard (point (100.0, 280.0), 2, 6, resources, true),
	display (point (410.0, 10.0), 7, resources, true),
	selector0 (point (400.0, 120.0), 200, resources, true),
	selector1 (point (440.0, 120.0), 201, resources, true),
	selector2 (point (480.0, 120.0), 202, resources, true),
	selector3 (point (520.0, 120.0), 203, resources, true),
	selector4 (point (560.0, 120.0), 204, resources, true),
	selector5 (point (600.0, 120.0), 205, resources, true),
	selector6 (point (640.0, 120.0), 206, resources, true),
	selector7 (point (680.0, 120.0), 207, resources, true),
	selector8 (point (720.0, 120.0), 208, resources, true),
	selector9 (point (760.0, 120.0), 209, resources, true),
	program0 (point (400.0, 160.0), 100, resources, true),
	program1 (point (440.0, 160.0), 101, resources, true),
	program2 (point (480.0, 160.0), 102, resources, true),
	program3 (point (520.0, 160.0), 103, resources, true),
	program4 (point (560.0, 160.0), 104, resources, true),
	program5 (point (600.0, 160.0), 105, resources, true),
	program6 (point (640.0, 160.0), 106, resources, true),
	program7 (point (680.0, 160.0), 107, resources, true),
	program8 (point (720.0, 160.0), 108, resources, true),
	program9 (point (760.0, 160.0), 109, resources, true),
	add_one (point (840.0, 220.0), 301, resources, true),
	sub_one (point (880.0, 220.0), 302, resources, true),
	delta_1 (point (820.0, 240.0), 303, resources, true),
	delta_8 (point (860.0, 240.0), 304, resources, true),
	delta_128 (point (900.0, 240.0), 305, resources, true)
	{
		this -> root = root;
		this -> directory = directory;
		this -> resources = resources;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		viewport = 0;
		current_program = 0;
		for (int ind = 0; ind < 10; ind++) programs [ind] = 0;
		current_delta = 128;
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
	action -> selector0 . draw (cr);
	action -> selector1 . draw (cr);
	action -> selector2 . draw (cr);
	action -> selector3 . draw (cr);
	action -> selector4 . draw (cr);
	action -> selector5 . draw (cr);
	action -> selector6 . draw (cr);
	action -> selector7 . draw (cr);
	action -> selector8 . draw (cr);
	action -> selector9 . draw (cr);
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
	action -> add_one . draw (cr);
	action -> sub_one . draw (cr);
	action -> delta_1 . draw (cr);
	action -> delta_8 . draw (cr);
	action -> delta_128 . draw (cr);
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
	if (action -> selector0 . keyon (location)) {action -> program_action (& action -> selector0, action -> display . area); redraw = true;}
	if (action -> selector1 . keyon (location)) {action -> program_action (& action -> selector1, action -> display . area); redraw = true;}
	if (action -> selector2 . keyon (location)) {action -> program_action (& action -> selector2, action -> display . area); redraw = true;}
	if (action -> selector3 . keyon (location)) {action -> program_action (& action -> selector3, action -> display . area); redraw = true;}
	if (action -> selector4 . keyon (location)) {action -> program_action (& action -> selector4, action -> display . area); redraw = true;}
	if (action -> selector5 . keyon (location)) {action -> program_action (& action -> selector5, action -> display . area); redraw = true;}
	if (action -> selector6 . keyon (location)) {action -> program_action (& action -> selector6, action -> display . area); redraw = true;}
	if (action -> selector7 . keyon (location)) {action -> program_action (& action -> selector7, action -> display . area); redraw = true;}
	if (action -> selector8 . keyon (location)) {action -> program_action (& action -> selector8, action -> display . area); redraw = true;}
	if (action -> selector9 . keyon (location)) {action -> program_action (& action -> selector9, action -> display . area); redraw = true;}
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
	if (action -> delta_1 . keyon (location)) {
		action -> delta_1 . engaged = true; action -> delta_8 . engaged = action -> delta_128 . engaged = false;
		action -> current_delta = 1; redraw = true;
	}
	if (action -> delta_8 . keyon (location)) {
		action -> delta_8 . engaged = true; action -> delta_1 . engaged = action -> delta_128 . engaged = false;
		action -> current_delta = 8; redraw = true;
	}
	if (action -> delta_128 . keyon (location)) {
		action -> delta_128 . engaged = true; action -> delta_1 . engaged = action -> delta_8 . engaged = false;
		action -> current_delta = 128; redraw = true;
	}
	if (action -> add_one . keyon (location)) {action -> add_one . engaged = true; redraw = true;}
	if (action -> sub_one . keyon (location)) {action -> sub_one . engaged = true; redraw = true;}
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
	bool redraw = false;
	if (action -> add_one . keyoff (location)) {action -> add_one . engaged = false; redraw = true;}
	if (action -> sub_one . keyoff (location)) {action -> sub_one . engaged = false; redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
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
	gtk_window_resize (GTK_WINDOW (action -> viewport), 1300, 420);
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

