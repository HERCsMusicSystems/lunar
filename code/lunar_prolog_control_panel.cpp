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
	knob_active_graphics ctrl_volume;
	knob_active_graphics ctrl_attack, ctrl_decay, ctrl_sustain, ctrl_release;
	knob_active_graphics ctrl_freq, ctrl_drywet, ctrl_pan, ctrl_porta, ctrl_speed, ctrl_vibrato;
	vector_active_graphics vector;
	keyboard_active_graphics keyboard;
	display_active_graphics display;
	button_active_graphics program0, program1, program2, program3, program4, program5, program6, program7, program8, program9;
	button_active_graphics selector0, selector1, selector2, selector3, selector4, selector5, selector6, selector7, selector8, selector9;
	button_active_graphics add_one, sub_one, delta_1, delta_8, delta_128;
	button_active_graphics poly_mono, porta_on_off;
	button_active_graphics store, restore;
	encoder_active_graphics encoder;
	slider_active_graphics pitch, modulation;
	cairo_surface_t * command_centre_image;
	point captured;
	int programs [10];
	int current_program;
	int current_delta;
	void feedback_on_controllers (void) {
		PrologElement * query = root -> earth ();
		query = root -> pair (root -> var (12), query);
		query = root -> pair (root -> var (11), query);
		query = root -> pair (root -> var (10), query);
		query = root -> pair (root -> var (9), query);
		query = root -> pair (root -> var (8), query);
		query = root -> pair (root -> var (7), query);
		query = root -> pair (root -> var (6), query);
		query = root -> pair (root -> var (5), query);
		query = root -> pair (root -> var (4), query);
		query = root -> pair (root -> var (3), query);
		query = root -> pair (root -> var (2), query);
		query = root -> pair (root -> var (1), query);
		query = root -> pair (root -> var (0), query);
		query = root -> pair (query, root -> pair (root -> pair (root -> atom (command), root -> pair (root -> atom (command), query -> duplicate ())), root -> earth ()));
		root -> resolution (query);
		PrologElement * var = query;
		if (var -> isPair ()) var = var -> getLeft ();
		PrologElement * el;
		if (var -> isPair ()) {el = var -> getLeft (); if (el -> isDouble ()) poly_mono . engaged = el -> getDouble () != 0.0; var = var -> getRight ();}
		delete query;
	}
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
	void value_change_action (int delta) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> earth (),
								root -> pair (root -> integer (delta * current_delta),
								root -> earth ()))));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) area_cat (display . area, 0, el -> getText ());
		}
		delete query;
	}
	void key_action (int key, int velocity) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> integer (key),
								root -> pair (root -> integer (velocity),
								root -> earth ())));
		query = root -> pair (root -> earth (), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	control_panel_action (GraphicResources * resources, PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command, bool active)
	: ctrl_volume (point (186.0, 5.0), 11, resources, active),
	ctrl_attack (point (268.0, 5.0), 1, resources, active), ctrl_decay (point (338.0, 5.0), 2, resources, active),
	ctrl_sustain (point (408.0, 5.0), 3, resources, active), ctrl_release (point (478.0, 5.0), 4, resources, active),
	ctrl_freq (point (218.0, 90.0), 5, resources, active), ctrl_drywet (point (288.0, 90.0), 6, resources, active),
	ctrl_pan (point (358.0, 90.0), 7, resources, active), ctrl_porta (point (442.0, 90.0), 8, resources, active),
	ctrl_speed (point (512.0, 90.0), 9, resources, active), ctrl_vibrato (point (582.0, 90.0), 10, resources, active),
	vector (point (12.0, -2.0), 16, resources, active), keyboard (point (114.0, 194.0), 2, 6, resources, active),
	display (point (576.0, -7.0), 7, resources, active),
	selector0 (point (667.0, 112.0), 200, resources, active),
	selector1 (point (707.0, 112.0), 201, resources, active),
	selector2 (point (747.0, 112.0), 202, resources, active),
	selector3 (point (787.0, 112.0), 203, resources, active),
	selector4 (point (827.0, 112.0), 204, resources, active),
	selector5 (point (867.0, 112.0), 205, resources, active),
	selector6 (point (907.0, 112.0), 206, resources, active),
	selector7 (point (947.0, 112.0), 207, resources, active),
	selector8 (point (987.0, 112.0), 208, resources, active),
	selector9 (point (1027.0, 112.0), 209, resources, active),
	program0 (point (667.0, 156.0), 100, resources, active),
	program1 (point (707.0, 156.0), 101, resources, active),
	program2 (point (747.0, 156.0), 102, resources, active),
	program3 (point (787.0, 156.0), 103, resources, active),
	program4 (point (827.0, 156.0), 104, resources, active),
	program5 (point (867.0, 156.0), 105, resources, active),
	program6 (point (907.0, 156.0), 106, resources, active),
	program7 (point (947.0, 156.0), 107, resources, active),
	program8 (point (987.0, 156.0), 108, resources, active),
	program9 (point (1027.0, 156.0), 109, resources, active),
	sub_one (point (1175.0, 140.0), 302, resources, true),
	add_one (point (1215.0, 140.0), 301, resources, true),
	delta_1 (point (1155.0, 160.0), 303, resources, true),
	delta_8 (point (1195.0, 160.0), 304, resources, true),
	delta_128 (point (1235.0, 160.0), 305, resources, true),
	encoder (point (1100.0, 100.0), 401, resources, true),
	pitch (point (42.0, 228.0), 501, true, resources, false),
	modulation (point (72.0, 228.0), 502, false, resources, true),
	poly_mono (point (33.0, 175.0), 503, resources, active),
	porta_on_off (point (73.0, 175.0), 504, resources, active),
	store (point (1240.0, 110.0), 601, resources, true),
	restore (point (1280.0, 110.0), 602, resources, true)
	{
		command_centre_image = resources != 0 ? resources -> command_centre : 0;
		pitch . position = 0.5;
		this -> root = root;
		this -> directory = directory;
		this -> resources = resources;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		viewport = 0;
		current_program = 0;
		for (int ind = 0; ind < 10; ind++) programs [ind] = 0;
		current_delta = 128;
		program_action (& selector0, display . area);
		feedback_on_controllers ();
	}
	~ control_panel_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		command -> removeAtom ();
	}
};

static gboolean RedrawControlPanel (GtkWidget * viewport, GdkEvent * event, control_panel_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (action -> command_centre_image != 0) {
		cairo_set_source_surface (cr, action -> command_centre_image, 0, 0);
		cairo_paint (cr);
	}
	action -> ctrl_volume . draw (cr);
	action -> ctrl_attack . draw (cr);
	action -> ctrl_decay . draw (cr);
	action -> ctrl_sustain . draw (cr);
	action -> ctrl_release . draw (cr);
	action -> ctrl_freq . draw (cr);
	action -> ctrl_drywet . draw (cr);
	action -> ctrl_pan . draw (cr);
	action -> ctrl_porta . draw (cr);
	action -> ctrl_speed . draw (cr);
	action -> ctrl_vibrato . draw (cr);
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
	action -> encoder . draw (cr);
	action -> pitch . draw (cr);
	action -> modulation . draw (cr);
	action -> poly_mono . draw (cr);
	action -> porta_on_off . draw (cr);
	action -> store . draw (cr);
	action -> restore . draw (cr);
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
	if (action -> keyboard . keyon (location)) {action -> key_action (action -> keyboard . key, 100); return TRUE;}
	action -> ctrl_volume . keyon (location);
	action -> ctrl_attack . keyon (location);
	action -> ctrl_decay . keyon (location);
	action -> ctrl_sustain . keyon (location);
	action -> ctrl_release . keyon (location);
	action -> ctrl_freq . keyon (location);
	action -> ctrl_drywet . keyon (location);
	action -> ctrl_pan . keyon (location);
	action -> ctrl_porta . keyon (location);
	action -> ctrl_speed . keyon (location);
	action -> ctrl_vibrato . keyon (location);
	action -> vector . keyon (location);
	action -> encoder . keyon (location);
	action -> pitch . keyon (location);
	action -> modulation . keyon (location);
	bool redraw = false;
	if (action -> poly_mono . keyon (location)) {action -> poly_mono . engaged = ! action -> poly_mono . engaged; redraw = true;}
	if (action -> porta_on_off . keyon (location)) {action -> porta_on_off . engaged = ! action -> porta_on_off . engaged; redraw = true;}
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
	if (action -> add_one . keyon (location)) {action -> value_change_action (1); action -> add_one . engaged = true; redraw = true;}
	if (action -> sub_one . keyon (location)) {action -> value_change_action (-1); action -> sub_one . engaged = true; redraw = true;}
	if (action -> store . keyon (location)) {
		GtkWidget * dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW (viewport),
											GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
			char * file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
			printf ("opening <%s>\n", file_name);
			g_free (file_name);
		}
		gtk_widget_destroy (dialog);
	}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gint ControlPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, control_panel_action * action) {
	point location (event -> x, event -> y);
	if (action -> keyboard . keyoff (location)) {action -> key_action (action -> keyboard . key, 0); return TRUE;}
	action -> ctrl_volume . keyoff (location);
	action -> ctrl_attack . keyoff (location);
	action -> ctrl_decay . keyoff (location);
	action -> ctrl_sustain . keyoff (location);
	action -> ctrl_release . keyoff (location);
	action -> ctrl_freq . keyoff (location);
	action -> ctrl_drywet . keyoff (location);
	action -> ctrl_pan . keyoff (location);
	action -> ctrl_porta . keyoff (location);
	action -> ctrl_speed . keyoff (location);
	action -> ctrl_vibrato . keyoff (location);
	action -> vector . keyoff (location);
	action -> encoder . keyoff (location);
	action -> modulation . keyoff (location);
	action -> poly_mono . keyoff (location);
	action -> porta_on_off . keyoff (location);
	bool redraw = false;
	if (action -> pitch . keyoff (location)) {redraw = true;}
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
	if (action -> ctrl_volume . move (delta)) {action -> action (action -> ctrl_vibrato . id, action -> ctrl_vibrato . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_attack . move (delta)) {action -> action (action -> ctrl_attack . id, action -> ctrl_attack . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_decay . move (delta)) {action -> action (action -> ctrl_decay . id, action -> ctrl_decay . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_sustain . move (delta)) {action -> action (action -> ctrl_sustain . id, action -> ctrl_sustain . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_release . move (delta)) {action -> action (action -> ctrl_release . id, action -> ctrl_release . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_freq . move (delta)) {action -> action (action -> ctrl_freq . id, action -> ctrl_freq . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_drywet . move (delta)) {action -> action (action -> ctrl_drywet . id, action -> ctrl_drywet . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_pan . move (delta)) {action -> action (action -> ctrl_pan . id, action -> ctrl_pan . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_porta . move (delta)) {action -> action (action -> ctrl_porta . id, action -> ctrl_porta . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_speed . move (delta)) {action -> action (action -> ctrl_speed . id, action -> ctrl_speed . angle, action -> display . area); redraw = true;}
	if (action -> ctrl_vibrato . move (delta)) {action -> action (action -> ctrl_vibrato . id, action -> ctrl_vibrato . angle, action -> display . area); redraw = true;}
	if (action -> vector . move (delta)) {
		action -> action (action -> vector . id, action -> vector . position . x, action -> vector . position . y, action -> display . area);
		redraw = true;
	}
	if (action -> encoder . move (delta)) {action -> value_change_action ((int) action -> encoder . increment); redraw = true;}
	if (action -> pitch . move (delta)) {redraw = true;}
	if (action -> modulation . move (delta)) {redraw = true;}
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
	if (action -> command_centre_image == 0) gtk_window_resize (GTK_WINDOW (action -> viewport), 1300, 420);
	else gtk_window_resize (GTK_WINDOW (action -> viewport),
								cairo_image_surface_get_width (action -> command_centre_image),
								cairo_image_surface_get_height (action -> command_centre_image));
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
	control_panel_action * machine = new control_panel_action (resources, root, directory, atom -> getAtom (), command -> getAtom (), false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateControlPanelIdleCode, machine);
	return true;
}

control_panel_class :: control_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> directory = servo -> directory;
	this -> resources = servo -> resources;
}

