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
#include <string.h>

#ifdef WIN32
#include <direct.h>
#define cwd _getcwd
#else
#include <unistd.h>
#define cwd getcwd
#endif

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
	button_active_graphics store, restore, voice_init;
	encoder_active_graphics encoder;
	slider_active_graphics pitch, modulation;
	cairo_surface_t * command_centre_image;
	point captured;
	int programs [10];
	int current_program;
	int current_delta;
	AREA area;
	void feedback_on_controllers (void) {
		PrologElement * query = root -> earth ();
		query = root -> pair (root -> var (17), query);
		query = root -> pair (root -> var (16), query);
		query = root -> pair (root -> var (15), query);
		query = root -> pair (root -> var (14), query);
		query = root -> pair (root -> var (13), query);
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
		double x;
		if (! var -> isPair ()) {delete query; return;}
		el = var -> getLeft (); if (el -> isDouble ()) poly_mono . engaged = el -> getDouble () != 0.0; var = var -> getRight ();
		if (! var -> isPair ()) {delete query; return;}
		el = var -> getLeft (); if (el -> isDouble ()) porta_on_off . engaged = el -> getDouble () != 0.0; var = var -> getRight ();
		if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; pitch . position = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; modulation . position = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 8192.0; x -= 1.0; if (x < -1.0) x = -1.0; if (x > 1.0) x = 1.0; vector . position . x = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 8192.0; x -= 1.0; if (x < -1.0) x = -1.0; if (x > 1.0) x = 1.0; vector . position . y = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < -1.0) x = -1.0; if (x > 1.0) x = 1.0; ctrl_volume . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_attack . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_decay . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_sustain . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_release . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_freq . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_drywet . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_pan . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_porta . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_speed . angle = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () / 16384.0; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; ctrl_vibrato . angle = x;}
		delete query;
	}
	void reset_selectors (void) {
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
	void reset_programs (void) {
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
	void select_program (void) {
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
	}
	void select_selector (void) {
		switch (current_program) {
		case 0: selector0 . engaged = true; break;
		case 1: selector1 . engaged = true; break;
		case 2: selector2 . engaged = true; break;
		case 3: selector3 . engaged = true; break;
		case 4: selector4 . engaged = true; break;
		case 5: selector5 . engaged = true; break;
		case 6: selector6 . engaged = true; break;
		case 7: selector7 . engaged = true; break;
		case 8: selector8 . engaged = true; break;
		case 9: selector9 . engaged = true; break;
		default: break;
		}
	}
	void program_action (void) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> integer (current_program),
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
								root -> earth ())))))))))))));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) {
				area_cat (display . area, 0, el -> getText ());
				if (current_program == 0) {
					cwd (area, sizeof (area)); area_cat (area, area_cat (area, "/"), el -> getText ());
					char * cp = strstr (area, " : ");
					if (cp != 0) * cp = '\0';
					printf ("setting instrument to: [%s]\n", area);
				}
			}
		}
		delete query;
		if (current_program == 0) feedback_on_controllers ();
	}
	void program_action (button_active_graphics * button) {
		reset_programs ();
		if (selector0 . id <= button -> id && button -> id <= selector9 . id) {
			reset_selectors ();
			current_program = button -> id - selector0 . id;
			select_program ();
		} else programs [current_program] = button -> id - program0 . id;
		button -> engaged = true;
		program_action ();
	}
	void change_selector (int delta) {
		reset_selectors ();
		current_program += delta;
		if (current_program < 0) current_program = 0; if (current_program > 9) current_program = 9;
		select_selector ();
		program_action ();
	}
	void change_program (int delta) {
		reset_programs ();
		programs [current_program] += delta;
		if (programs [current_program] < 0) programs [current_program] = 0; if (programs [current_program] > 9) programs [current_program] = 9;
		select_program ();
		program_action ();
	}
	void action (int ind, double value) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> integer (ind),
								root -> pair (root -> integer ((int) (value * 128.0)),
								root -> earth ()))));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) area_cat (display . area, 0, el -> getText ());
		}
		delete query;
	}
	void action (int ind, double x, double y) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> integer (ind),
								root -> pair (root -> integer ((int) (x * 64.0 + 64.0)),
								root -> pair (root -> integer ((int) (y * 64.0 + 64.0)),
								root -> earth ())))));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) area_cat (display . area, 0, el -> getText ());
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
		feedback_on_controllers ();
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
	void preset_action (char * action, char * file_name) {
		PrologAtom * atom = root -> search (action);
		if (atom == 0) return;
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> pair (root -> atom (atom),
								root -> pair (root -> text (file_name),
								root -> earth ()))));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isText ()) area_cat (display . area, 0, el -> getText ());
		}
		delete query;
	}
	bool voice_init_action (void) {
		GtkWidget * dialog = gtk_message_dialog_new (GTK_WINDOW (viewport), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, "Load default settings?", "INFO");
		int res = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		if (res != GTK_RESPONSE_OK) return false;
		AREA file_name;
		area_cat (file_name, area_cat (file_name, 0, area), "/default.txt");
		preset_action ("Restore", file_name);
		return true;
	}
	void stop_recording_action (void) {
		PrologAtom * STOP = root -> search ("STOP");
		if(atom == 0) return;
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> atom (STOP),
								root -> earth ()));
		query = root -> pair (root -> earth (), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
		area_cat (display . area, 0, "RECORDING COMPLETED.");
	}
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	control_panel_action (GraphicResources * resources, PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command, bool active) :
	ctrl_volume (point (186.0, 5.0), 7, resources, active),
	ctrl_attack (point (268.0, 5.0), 73, resources, active),
	ctrl_decay (point (338.0, 5.0), 93, resources, active),
	ctrl_sustain (point (408.0, 5.0), 94, resources, active),
	ctrl_release (point (478.0, 5.0), 72, resources, active),
	ctrl_freq (point (218.0, 90.0), 74, resources, active),
	ctrl_drywet (point (288.0, 90.0), 91, resources, active),
	ctrl_pan (point (358.0, 90.0), 10, resources, active),
	ctrl_porta (point (442.0, 90.0), 11, resources, active),
	ctrl_speed (point (512.0, 90.0), 95, resources, active),
	ctrl_vibrato (point (582.0, 90.0), 71, resources, active),
	vector (point (12.0, -2.0), 12, resources, active),
	keyboard (point (114.0, 194.0), 2, 6, resources, active),
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
	sub_one (point (1176.0, 130.0), 302, resources, true),
	add_one (point (1216.0, 130.0), 301, resources, true),
	delta_1 (point (1155.0, 150.0), 303, resources, true),
	delta_8 (point (1195.0, 150.0), 304, resources, true),
	delta_128 (point (1235.0, 150.0), 305, resources, true),
	encoder (point (1088.0, 97.0), 401, resources, true),
	pitch (point (42.0, 228.0), 128, true, resources, false),
	modulation (point (72.0, 228.0), 1, false, resources, true),
	poly_mono (point (33.0, 175.0), 503, resources, active),
	porta_on_off (point (73.0, 175.0), 65, resources, active),
	store (point (1200.0, 110.0), 601, resources, true),
	restore (point (1240.0, 110.0), 602, resources, true),
	voice_init (point (1280.0, 110.0), 603, resources, true)
	{
		cwd (area, sizeof (AREA));
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
		delta_128 . engaged = true;
		program_action (& selector0);
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
	action -> voice_init . draw (cr);
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
	location = point (x -> getNumber (), y -> getNumber ());
	g_idle_add ((GSourceFunc) RepositionControlPanel, this);
	return true;
}

static gboolean ControlPanelDeleteEvent (GtkWidget * viewport, GdkEvent * event, control_panel_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}
static bool file_action (char * command, char * title, GtkWidget * viewport, control_panel_action * action) {
	bool redraw = false;
	GtkWidget * dialog = gtk_file_chooser_dialog_new (title, GTK_WINDOW (viewport),
										GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), action -> area);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char * file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		area_cat (action -> area, 0, file_name);
		char * cp = action -> area + strlen (action -> area);
		while (cp > action -> area && * cp != '/' && * cp != '\\') * cp-- = '\0'; * cp = '\0';
		action -> preset_action (command, file_name);
		g_free (file_name);
		redraw = true;
	}
	gtk_widget_destroy (dialog);
	return redraw;
}
static gboolean ControlPanelButtonOn (GtkWidget * viewport, GdkEventKey * event, control_panel_action * action) {
	bool redraw = false;
	int key = (int) event -> keyval;
	switch (key) {
	case 65470: action -> program_action (& action -> selector0); redraw = true; break;
	case 65471: action -> program_action (& action -> selector1); redraw = true; break;
	case 65472: action -> program_action (& action -> selector2); redraw = true; break;
	case 65473: action -> program_action (& action -> selector3); redraw = true; break;
	case 65474: action -> program_action (& action -> selector4); redraw = true; break;
	case 65475: action -> program_action (& action -> selector5); redraw = true; break;
	case 65476: action -> program_action (& action -> selector6); redraw = true; break;
	case 65477: action -> program_action (& action -> selector7); redraw = true; break;
	case 65478: action -> program_action (& action -> selector8); redraw = true; break;
	case 65479: action -> program_action (& action -> selector9); redraw = true; break;
	case 49: action -> program_action (& action -> program0); redraw = true; break;
	case 50: action -> program_action (& action -> program1); redraw = true; break;
	case 51: action -> program_action (& action -> program2); redraw = true; break;
	case 52: action -> program_action (& action -> program3); redraw = true; break;
	case 53: action -> program_action (& action -> program4); redraw = true; break;
	case 54: action -> program_action (& action -> program5); redraw = true; break;
	case 55: action -> program_action (& action -> program6); redraw = true; break;
	case 56: action -> program_action (& action -> program7); redraw = true; break;
	case 57: action -> program_action (& action -> program8); redraw = true; break;
	case 48: action -> program_action (& action -> program9); redraw = true; break;
	case 122:
		action -> delta_1 . engaged = true;
		action -> delta_8 . engaged = action -> delta_128 . engaged = false;
		action -> current_delta = 1;
		redraw = true;
		break;
	case 120:
		action -> delta_8 . engaged = true;
		action -> delta_1 . engaged = action -> delta_128 . engaged = false;
		action -> current_delta = 8;
		redraw = true;
		break;
	case 99:
		action -> delta_128 . engaged = true;
		action -> delta_1 . engaged = action -> delta_8 . engaged = false;
		action -> current_delta = 128;
		redraw = true;
		break;
	case 65361: action -> value_change_action (-1); redraw = true; break;
	case 65362: action -> value_change_action (4); redraw = true; break;
	case 65363: action -> value_change_action (1); redraw = true; break;
	case 65364: action -> value_change_action (-4); redraw = true; break;
	case 91: redraw = file_action ("Store", "Save File", viewport, action); break;
	case 92: redraw = action -> voice_init_action (); action -> feedback_on_controllers (); break;
	case 93: redraw = file_action ("Restore", "Load File", viewport, action); action -> feedback_on_controllers (); break;
	case 44:
		action -> poly_mono . engaged = ! action -> poly_mono . engaged;
		action -> action (action -> poly_mono . engaged ? 127 : 126, 0.0);
		redraw = true;
		break;
	case 46:
		action -> porta_on_off . engaged = ! action -> porta_on_off . engaged;
		action -> action (65, action -> porta_on_off . engaged ? 1.0 : 0.0);
		redraw = true;
		break;
	case 47: redraw = file_action ("START", "Record File", viewport, action); break;
	case 32: action -> stop_recording_action (); redraw = true; break;
	case 97: action -> change_selector (-1); redraw = true; break;
	case 100: action -> change_selector (1); redraw = true; break;
	case 115: action -> change_program (1); redraw = true; break;
	case 119: action -> change_program (-1); redraw = true; break;
	default: return FALSE;
	}
	if (redraw) gtk_widget_queue_draw (viewport);
	return FALSE;
}
static gint ControlPanelKeyon (GtkWidget * viewport, GdkEventButton * event, control_panel_action * action) {
	point location (event -> x, event -> y);
	action -> captured = location;
	if (action -> keyboard . keyon (location)) {action -> key_action (action -> keyboard . key, event -> button == 1 ? 100 : 0); return TRUE;}
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
	if (action -> poly_mono . keyon (location)) {
		action -> poly_mono . engaged = ! action -> poly_mono . engaged;
		action -> action (action -> poly_mono . engaged ? 127 : 126, 0.0);
		redraw = true;
	}
	if (action -> porta_on_off . keyon (location)) {
		action -> porta_on_off . engaged = ! action -> porta_on_off . engaged;
		action -> action (65, action -> porta_on_off . engaged ? 1.0 : 0.0);
		redraw = true;
	}
	if (action -> selector0 . keyon (location)) {action -> program_action (& action -> selector0); redraw = true;}
	if (action -> selector1 . keyon (location)) {action -> program_action (& action -> selector1); redraw = true;}
	if (action -> selector2 . keyon (location)) {action -> program_action (& action -> selector2); redraw = true;}
	if (action -> selector3 . keyon (location)) {action -> program_action (& action -> selector3); redraw = true;}
	if (action -> selector4 . keyon (location)) {action -> program_action (& action -> selector4); redraw = true;}
	if (action -> selector5 . keyon (location)) {action -> program_action (& action -> selector5); redraw = true;}
	if (action -> selector6 . keyon (location)) {action -> program_action (& action -> selector6); redraw = true;}
	if (action -> selector7 . keyon (location)) {action -> program_action (& action -> selector7); redraw = true;}
	if (action -> selector8 . keyon (location)) {action -> program_action (& action -> selector8); redraw = true;}
	if (action -> selector9 . keyon (location)) {action -> program_action (& action -> selector9); redraw = true;}
	if (action -> program0 . keyon (location)) {action -> program_action (& action -> program0); redraw = true;}
	if (action -> program1 . keyon (location)) {action -> program_action (& action -> program1); redraw = true;}
	if (action -> program2 . keyon (location)) {action -> program_action (& action -> program2); redraw = true;}
	if (action -> program3 . keyon (location)) {action -> program_action (& action -> program3); redraw = true;}
	if (action -> program4 . keyon (location)) {action -> program_action (& action -> program4); redraw = true;}
	if (action -> program5 . keyon (location)) {action -> program_action (& action -> program5); redraw = true;}
	if (action -> program6 . keyon (location)) {action -> program_action (& action -> program6); redraw = true;}
	if (action -> program7 . keyon (location)) {action -> program_action (& action -> program7); redraw = true;}
	if (action -> program8 . keyon (location)) {action -> program_action (& action -> program8); redraw = true;}
	if (action -> program9 . keyon (location)) {action -> program_action (& action -> program9); redraw = true;}
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
	if (action -> store . keyon (location)) redraw = file_action ("Store", "Save File", viewport, action);
	if (action -> restore . keyon (location)) {if (redraw = file_action ("Restore", "Load File", viewport, action)) action -> feedback_on_controllers ();}
	if (action -> voice_init . keyon (location)) {if (redraw = action -> voice_init_action ()) action -> feedback_on_controllers ();}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gint ControlPanelKeyoff (GtkWidget * viewport, GdkEventButton * event, control_panel_action * action) {
	point location (event -> x, event -> y);
	if (action -> keyboard . keyoff (location)) {action -> key_action (action -> keyboard . key, 0);}
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
	if (action -> pitch . keyoff (location)) {action -> action (action -> pitch . id, action -> pitch . position); redraw = true;}
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
	if (action -> ctrl_volume . move (delta)) {action -> action (action -> ctrl_volume . id, action -> ctrl_volume . angle); redraw = true;}
	if (action -> ctrl_attack . move (delta)) {action -> action (action -> ctrl_attack . id, action -> ctrl_attack . angle); redraw = true;}
	if (action -> ctrl_decay . move (delta)) {action -> action (action -> ctrl_decay . id, action -> ctrl_decay . angle); redraw = true;}
	if (action -> ctrl_sustain . move (delta)) {action -> action (action -> ctrl_sustain . id, action -> ctrl_sustain . angle); redraw = true;}
	if (action -> ctrl_release . move (delta)) {action -> action (action -> ctrl_release . id, action -> ctrl_release . angle); redraw = true;}
	if (action -> ctrl_freq . move (delta)) {action -> action (action -> ctrl_freq . id, action -> ctrl_freq . angle); redraw = true;}
	if (action -> ctrl_drywet . move (delta)) {action -> action (action -> ctrl_drywet . id, action -> ctrl_drywet . angle); redraw = true;}
	if (action -> ctrl_pan . move (delta)) {action -> action (action -> ctrl_pan . id, action -> ctrl_pan . angle); redraw = true;}
	if (action -> ctrl_porta . move (delta)) {action -> action (action -> ctrl_porta . id, action -> ctrl_porta . angle); redraw = true;}
	if (action -> ctrl_speed . move (delta)) {action -> action (action -> ctrl_speed . id, action -> ctrl_speed . angle); redraw = true;}
	if (action -> ctrl_vibrato . move (delta)) {action -> action (action -> ctrl_vibrato . id, action -> ctrl_vibrato . angle); redraw = true;}
	if (action -> vector . move (delta)) {
		action -> action (action -> vector . id, action -> vector . position . x, action -> vector . position . y);
		redraw = true;
	}
	if (action -> encoder . move (delta)) {action -> value_change_action ((int) action -> encoder . increment); redraw = true;}
	if (action -> pitch . move (delta)) {action -> action (action -> pitch . id, action -> pitch . position); redraw = true;}
	if (action -> modulation . move (delta)) {action -> action (action -> modulation . id, action -> modulation . position); redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
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
	g_signal_connect (G_OBJECT (action -> viewport), "key-press-event", G_CALLBACK (ControlPanelButtonOn), action);
	if (action -> command_centre_image == 0) gtk_window_resize (GTK_WINDOW (action -> viewport), 1300, 420);
	else gtk_window_resize (GTK_WINDOW (action -> viewport),
								cairo_image_surface_get_width (action -> command_centre_image),
								cairo_image_surface_get_height (action -> command_centre_image));
	const GtkTargetEntry targets [2] = {{"text/plain", 0, 0}, {"application/x-rootwindow-drop", 0, 0}};
	gtk_drag_dest_set (area, GTK_DEST_DEFAULT_ALL, targets, 2, GDK_ACTION_COPY);
	g_signal_connect (area, "drag-drop", G_CALLBACK (dnd_drop), 0);
	g_signal_connect (area, "drag-motion", G_CALLBACK (dnd_motion), 0);
	g_signal_connect (area, "drag-data-received", G_CALLBACK (dnd_receive), action -> root);
	g_signal_connect (area, "drag-leave", G_CALLBACK (dnd_leave), 0);
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

