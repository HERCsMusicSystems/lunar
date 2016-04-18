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

#include "lunar_prolog_panel_base.h"
#include <string.h>

#ifdef WIN32
#include <direct.h>
#define cwd _getcwd
#else
#include <unistd.h>
#define cwd getcwd
#endif

class control_panel_action : public AudioModulePanel {
public:
	PrologDirectory * directory;
	PrologAtom * command;
	knob_active_graphics ctrl_volume;
	knob_active_graphics ctrl_attack, ctrl_decay, ctrl_sustain, ctrl_release;
	knob_active_graphics ctrl_freq, ctrl_drywet, ctrl_pan, ctrl_porta, ctrl_speed, ctrl_vibrato, ctrl_resonance;
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
	button_active_graphics arp, arp_hold, sustain, legato, auto_on, auto_loop, auto_record;
	int programs [10];
	int current_program;
	int current_delta;
	AREA program_area;
	void feedback (void) {
		PrologElement * query = root -> earth ();
		query = root -> pair (root -> var (23), query);
		query = root -> pair (root -> var (22), query);
		query = root -> pair (root -> var (21), query);
		query = root -> pair (root -> var (20), query);
		query = root -> pair (root -> var (19), query);
		query = root -> pair (root -> var (18), query);
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
		if (el -> isDouble ()) {x = el -> getDouble () * 0.0078125; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; pitch . position = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () * 0.0078125; if (x < 0.0) x = 0.0; if (x > 1.0) x = 1.0; modulation . position = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () * 0.015625; x -= 1.0; if (x < -1.0) x = -1.0; if (x > 1.0) x = 1.0; vector . position . x = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {x = el -> getDouble () * 0.015625; x -= 1.0; if (x < -1.0) x = -1.0; if (x > 1.0) x = 1.0; vector . position . y = x;}
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_volume . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_attack . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_decay . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_sustain . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_release . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_freq . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_drywet . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_pan . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_porta . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_speed . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_vibrato . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) ctrl_resonance . setValue (el -> getDouble () * 128.0);
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) arp . engaged = el -> getDouble () != 0.0;
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) arp_hold . engaged = el -> getDouble () != 0.0;
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) sustain . engaged = el -> getDouble () != 0.0;
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) legato . engaged = el -> getDouble () != 0.0;
		var = var -> getRight (); if (! var -> isPair ()) {delete query; return;} el = var -> getLeft ();
		if (el -> isDouble ()) {auto_on . engaged = el -> getDouble () == 1.0; auto_loop . engaged = el -> getDouble () == 2.0; auto_record . engaged = el -> getDouble () == 16.0;}
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
					cwd (program_area, sizeof (program_area));
					char * cp = strstr (el -> getText (), "] ");
					if (cp != 0 && strlen (cp) > 3) area_cat (program_area, area_cat (program_area, "/"), cp + 2);
					cp = strstr (program_area, " = ");
					if (cp != 0) * cp = '\0';
				}
			}
		}
		delete query;
		if (current_program == 0) feedback ();
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
		reset_programs ();
		current_program += delta;
		if (current_program < 0) current_program = 0; if (current_program > 9) current_program = 9;
		select_selector ();
		select_program ();
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
								root -> pair (root -> Double (value),
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
								root -> pair (root -> Double (x * 64.0 + 64.0),
								root -> pair (root -> Double (y * 64.0 + 64.0),
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
		feedback ();
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
		PrologDirectory * dir = root -> searchDirectory ("lunar");
		if (dir == 0) return;
		PrologAtom * atom = dir -> searchAtom (action);
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
		GtkWidget * dialog = gtk_message_dialog_new (GTK_WINDOW (viewport), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, "Load default settings?");
		int res = gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		if (res != GTK_RESPONSE_OK) return false;
		AREA file_name;
		area_cat (file_name, area_cat (file_name, 0, program_area), "/default.txt");
		preset_action ("Restore", file_name);
		return true;
	}
	void stop_recording_action (void) {
		PrologDirectory * dir = root -> searchDirectory ("lunar");
		if (dir == 0) return;
		PrologAtom * STOP = dir -> searchAtom ("STOP");
		if (STOP == 0) return;
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> atom (STOP),
								root -> earth ()));
		query = root -> pair (root -> earth (), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
		area_cat (display . area, 0, "RECORDING COMPLETED.");
	}
	void f_action (int ind) {
		PrologDirectory * dir = root -> searchDirectory ("lunar");
		if (dir == 0) return;
		PrologAtom * F = dir -> searchAtom ("FUNCTION_KEY");
		if (F == 0) return;
		PrologElement * query = root -> pair (root -> atom (F), root -> pair (root -> integer (ind), root -> earth ()));
		query = root -> pair (root -> earth (), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void audio_hardware_action (void) {
		PrologDirectory * dir = root -> searchDirectory ("lunar");
		if (dir == 0) return;
		PrologAtom * action = dir -> searchPrivateAtom ("CommanderHardwareAction");
		if (action == 0) return;
		PrologElement * query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (action), root -> earth ()), root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void redraw (cairo_t * cr) {
		ctrl_volume . draw (cr);
		ctrl_attack . draw (cr);
		ctrl_decay . draw (cr);
		ctrl_sustain . draw (cr);
		ctrl_release . draw (cr);
		ctrl_freq . draw (cr);
		ctrl_drywet . draw (cr);
		ctrl_pan . draw (cr);
		ctrl_porta . draw (cr);
		ctrl_speed . draw (cr);
		ctrl_vibrato . draw (cr);
		ctrl_resonance . draw (cr);
		vector . draw (cr);
		keyboard . draw (cr);
		display . draw (cr);
		selector0 . draw (cr);
		selector1 . draw (cr);
		selector2 . draw (cr);
		selector3 . draw (cr);
		selector4 . draw (cr);
		selector5 . draw (cr);
		selector6 . draw (cr);
		selector7 . draw (cr);
		selector8 . draw (cr);
		selector9 . draw (cr);
		program0 . draw (cr);
		program1 . draw (cr);
		program2 . draw (cr);
		program3 . draw (cr);
		program4 . draw (cr);
		program5 . draw (cr);
		program6 . draw (cr);
		program7 . draw (cr);
		program8 . draw (cr);
		program9 . draw (cr);
		add_one . draw (cr);
		sub_one . draw (cr);
		delta_1 . draw (cr);
		delta_8 . draw (cr);
		delta_128 . draw (cr);
		encoder . draw (cr);
		pitch . draw (cr);
		modulation . draw (cr);
		poly_mono . draw (cr);
		porta_on_off . draw (cr);
		store . draw (cr);
		restore . draw (cr);
		voice_init . draw (cr);
		arp . draw (cr); arp_hold . draw (cr); sustain . draw (cr); legato . draw (cr); auto_on . draw (cr); auto_loop . draw (cr); auto_record . draw (cr);
	}
	void MouseKeyon (point location, int button) {
		if (keyboard . keyon (location)) {key_action (keyboard . key, button == 1 ? 100 : 0); return;}
		ctrl_volume . keyon (location);
		ctrl_attack . keyon (location);
		ctrl_decay . keyon (location);
		ctrl_sustain . keyon (location);
		ctrl_release . keyon (location);
		ctrl_freq . keyon (location);
		ctrl_drywet . keyon (location);
		ctrl_pan . keyon (location);
		ctrl_porta . keyon (location);
		ctrl_speed . keyon (location);
		ctrl_vibrato . keyon (location);
		ctrl_resonance . keyon (location);
		vector . keyon (location);
		encoder . keyon (location);
		pitch . keyon (location);
		modulation . keyon (location);
		bool redraw = false;
		if (poly_mono . keyon (location)) {poly_mono . engaged = ! poly_mono . engaged; action (poly_mono . engaged ? 127 : 126, 0.0); redraw = true;}
		if (porta_on_off . keyon (location)) {porta_on_off . engaged = ! porta_on_off . engaged; action (65, porta_on_off . engaged ? 1.0 : 0.0); redraw = true;}
		if (arp . keyon (location)) {arp . engaged = ! arp . engaged; action (80, arp . engaged ? 1.0 : 0.0); redraw = true;}
		if (arp_hold . keyon (location)) {arp_hold . engaged = ! arp_hold . engaged; action (66, arp_hold . engaged ? 1.0 : 0.0); redraw = true;}
		if (sustain . keyon (location)) {sustain . engaged = ! sustain . engaged; action (64, sustain . engaged ? 1.0 : 0.0); redraw = true;}
		if (legato . keyon (location)) {legato . engaged = ! legato . engaged; action (84, legato . engaged ? 1.0 : 0.0); redraw = true;}
		if (auto_on . keyon (location)) {
			auto_on . engaged = ! auto_on . engaged;
			auto_loop . engaged = auto_record . engaged = false;
			action (85, auto_on . engaged ? 1.0 : 0.0);
			redraw = true;
		}
		if (auto_loop . keyon (location)) {
			auto_loop . engaged = ! auto_loop . engaged;
			auto_on . engaged = auto_record . engaged = false;
			action (85, auto_loop . engaged ? 2.0 : 0.0);
			redraw = true;
		}
		if (auto_record . keyon (location)) {
			auto_record . engaged = ! auto_record . engaged;
			auto_on . engaged = auto_loop . engaged = false;
			action (85, auto_record . engaged ? 16.0 : 0.0);
			redraw = true;
		}
		if (rect (point (1240.0, 20.0), point (20.0, 40.0)) . overlap (rect (location, point(0.0, 0.0)))) {audio_hardware_action ();}
		if (selector0 . keyon (location)) {program_action (& selector0); redraw = true;}
		if (selector1 . keyon (location)) {program_action (& selector1); redraw = true;}
		if (selector2 . keyon (location)) {program_action (& selector2); redraw = true;}
		if (selector3 . keyon (location)) {program_action (& selector3); redraw = true;}
		if (selector4 . keyon (location)) {program_action (& selector4); redraw = true;}
		if (selector5 . keyon (location)) {program_action (& selector5); redraw = true;}
		if (selector6 . keyon (location)) {program_action (& selector6); redraw = true;}
		if (selector7 . keyon (location)) {program_action (& selector7); redraw = true;}
		if (selector8 . keyon (location)) {program_action (& selector8); redraw = true;}
		if (selector9 . keyon (location)) {program_action (& selector9); redraw = true;}
		if (program0 . keyon (location)) {program_action (& program0); redraw = true;}
		if (program1 . keyon (location)) {program_action (& program1); redraw = true;}
		if (program2 . keyon (location)) {program_action (& program2); redraw = true;}
		if (program3 . keyon (location)) {program_action (& program3); redraw = true;}
		if (program4 . keyon (location)) {program_action (& program4); redraw = true;}
		if (program5 . keyon (location)) {program_action (& program5); redraw = true;}
		if (program6 . keyon (location)) {program_action (& program6); redraw = true;}
		if (program7 . keyon (location)) {program_action (& program7); redraw = true;}
		if (program8 . keyon (location)) {program_action (& program8); redraw = true;}
		if (program9 . keyon (location)) {program_action (& program9); redraw = true;}
		if (delta_1 . keyon (location)) {
			delta_1 . engaged = true; delta_8 . engaged = delta_128 . engaged = false;
			current_delta = 1; redraw = true;
		}
		if (delta_8 . keyon (location)) {
			delta_8 . engaged = true; delta_1 . engaged = delta_128 . engaged = false;
			current_delta = 8; redraw = true;
		}
		if (delta_128 . keyon (location)) {
			delta_128 . engaged = true; delta_1 . engaged = delta_8 . engaged = false;
			current_delta = 128; redraw = true;
		}
		if (add_one . keyon (location)) {value_change_action (1); add_one . engaged = true; redraw = true;}
		if (sub_one . keyon (location)) {value_change_action (-1); sub_one . engaged = true; redraw = true;}
		if (store . keyon (location)) redraw = store . engaged = true;
		if (restore . keyon (location)) redraw = restore . engaged = true;
		if (voice_init . keyon (location)) redraw = voice_init . engaged = true;
		if (redraw) gtk_widget_queue_draw (viewport);
	}
	void MouseKeyoff (point location, int button);
	void MouseMove (point delta) {
		bool redraw = false;
		if (ctrl_volume . move (delta)) {action (ctrl_volume . id, ctrl_volume . value * 0.0078125); redraw = true;}
		if (ctrl_attack . move (delta)) {action (ctrl_attack . id, ctrl_attack . value * 0.0078125); redraw = true;}
		if (ctrl_decay . move (delta)) {action (ctrl_decay . id, ctrl_decay . value * 0.0078125); redraw = true;}
		if (ctrl_sustain . move (delta)) {action (ctrl_sustain . id, ctrl_sustain . value * 0.0078125); redraw = true;}
		if (ctrl_release . move (delta)) {action (ctrl_release . id, ctrl_release . value * 0.0078125); redraw = true;}
		if (ctrl_freq . move (delta)) {action (ctrl_freq . id, ctrl_freq . value * 0.0078125); redraw = true;}
		if (ctrl_drywet . move (delta)) {action (ctrl_drywet . id, ctrl_drywet . value * 0.0078125); redraw = true;}
		if (ctrl_pan . move (delta)) {action (ctrl_pan . id, ctrl_pan . value * 0.0078125); redraw = true;}
		if (ctrl_porta . move (delta)) {action (ctrl_porta . id, ctrl_porta . value * 0.0078125); redraw = true;}
		if (ctrl_speed . move (delta)) {action (ctrl_speed . id, ctrl_speed . value * 0.0078125); redraw = true;}
		if (ctrl_vibrato . move (delta)) {action (ctrl_vibrato . id, ctrl_vibrato . value * 0.0078125); redraw = true;}
		if (ctrl_resonance . move (delta)) {action (ctrl_resonance . id, ctrl_resonance . value * 0.0078125); redraw = true;}
		if (vector . move (delta)) {action (vector . id, vector . position . x, vector . position . y); redraw = true;}
		if (encoder . move (delta)) {value_change_action ((int) encoder . increment); redraw = true;}
		if (pitch . move (delta)) {action (pitch . id, pitch . position * 128.0); redraw = true;}
		if (modulation . move (delta)) {action (modulation . id, modulation . position * 128.0); redraw = true;}
		if (redraw) update ();
	}
	void FunctionKey (int key, int state);
	control_panel_action (GraphicResources * resources, PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command, bool active) :
	ctrl_volume (point (186.0, 5.0), 7, resources, false, active, 0.0, 16384.0),
	ctrl_attack (point (268.0, 5.0), 73, resources, false, active, 0.0, 16384.0),
	ctrl_decay (point (338.0, 5.0), 75, resources, false, active, 0.0, 16384.0),
	ctrl_sustain (point (408.0, 5.0), 76, resources, false, active, 0.0, 16384.0),
	ctrl_release (point (478.0, 5.0), 72, resources, false, active, 0.0, 16384.0),
	ctrl_pan (point (557.0, 5.0), 10, resources, false, active, 0.0, 16384.0),
	ctrl_freq (point (218.0, 90.0), 74, resources, false, active, 0.0, 16384.0),
	ctrl_resonance (point (288.0, 90.0), 71, resources, false, active, 0.0, 16384.0),
	ctrl_drywet (point (358.0, 90.0), 77, resources, false, active, 0.0, 16384.0),
	ctrl_porta (point (442.0, 90.0), 5, resources, false, active, 0.0, 16384.0),
	ctrl_speed (point (512.0, 90.0), 78, resources, false, active, 0.0, 16384.0),
	ctrl_vibrato (point (582.0, 90.0), 79, resources, false, active, 0.0, 16384.0),
	vector (point (12.0, -2.0), 16, resources, 0.25, active),
	keyboard (point (114.0, 194.0), 2, 6, resources, active),
	display (point (623.0, -7.0), 7, resources, active),
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
	program0 (point (667.0, 152.0), 100, resources, active),
	program1 (point (707.0, 152.0), 101, resources, active),
	program2 (point (747.0, 152.0), 102, resources, active),
	program3 (point (787.0, 152.0), 103, resources, active),
	program4 (point (827.0, 152.0), 104, resources, active),
	program5 (point (867.0, 152.0), 105, resources, active),
	program6 (point (907.0, 152.0), 106, resources, active),
	program7 (point (947.0, 152.0), 107, resources, active),
	program8 (point (987.0, 152.0), 108, resources, active),
	program9 (point (1027.0, 152.0), 109, resources, active),
	sub_one (point (1176.0, 130.0), 302, resources, active),
	add_one (point (1216.0, 130.0), 301, resources, active),
	delta_1 (point (1155.0, 150.0), 303, resources, active),
	delta_8 (point (1195.0, 150.0), 304, resources, active),
	delta_128 (point (1235.0, 150.0), 305, resources, active),
	encoder (point (1088.0, 97.0), 401, resources, active),
	pitch (point (42.0, 228.0), 128, true, resources, true),
	modulation (point (72.0, 228.0), 1, false, resources, true),
	poly_mono (point (33.0, 175.0), 503, resources, active),
	porta_on_off (point (73.0, 175.0), 65, resources, active),
	store (point (947.0, 71.0), 601, resources, active),
	restore (point (987.0, 71.0), 602, resources, active),
	voice_init (point (1027.0, 71.0), 603, resources, active),
	arp (point (300.0, 183.0), 701, resources, true),
	arp_hold (point (340.0, 183.0), 702, resources, true),
	sustain (point (420.0, 183.0), 703, resources, true),
	legato (point (460.0, 183.0), 704, resources, true),
	auto_on (point (540.0, 183.0), 705, resources, true),
	auto_loop (point (580.0, 183.0), 706, resources, true),
	auto_record (point (620.0, 183.0), 707, resources, true),
	AudioModulePanel (root, atom, resources != 0 ? resources -> command_centre : 0) {
		cwd (program_area, sizeof (AREA));
		pitch . position = 0.5;
		this -> directory = directory;
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		current_program = 0;
		for (int ind = 0; ind < 10; ind++) programs [ind] = 0;
		current_delta = 128;
		delta_128 . engaged = true;
		program_action (& selector0);
	}
	~ control_panel_action (void) {command -> removeAtom ();}
};

static bool file_action (char * command, char * title, GtkWidget * viewport, control_panel_action * action) {
	bool redraw = false;
	GtkWidget * dialog = gtk_file_chooser_dialog_new (title, GTK_WINDOW (viewport),
										GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), action -> program_area);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char * file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		area_cat (action -> program_area, 0, file_name);
		char * cp = action -> program_area + strlen (action -> program_area);
		while (cp > action -> program_area && * cp != '/' && * cp != '\\') * cp-- = '\0'; * cp = '\0';
		action -> preset_action (command, file_name);
		g_free (file_name);
		redraw = true;
	}
	gtk_widget_destroy (dialog);
	return redraw;
}

void control_panel_action :: MouseKeyoff (point location, int button) {
	if (keyboard . keyoff (location)) {key_action (keyboard . key, 0);}
	ctrl_volume . keyoff (location);
	ctrl_attack . keyoff (location);
	ctrl_decay . keyoff (location);
	ctrl_sustain . keyoff (location);
	ctrl_release . keyoff (location);
	ctrl_freq . keyoff (location);
	ctrl_drywet . keyoff (location);
	ctrl_pan . keyoff (location);
	ctrl_porta . keyoff (location);
	ctrl_speed . keyoff (location);
	ctrl_vibrato . keyoff (location);
	ctrl_resonance . keyoff (location);
	vector . keyoff (location);
	encoder . keyoff (location);
	modulation . keyoff (location);
	poly_mono . keyoff (location);
	porta_on_off . keyoff (location);
	bool redraw = false;
	if (pitch . keyoff (location)) {action (pitch . id, pitch . position * 128.0); redraw = true;}
	if (add_one . keyoff (location)) {add_one . engaged = false; redraw = true;}
	if (sub_one . keyoff (location)) {sub_one . engaged = false; redraw = true;}
	if (store . keyoff (location)) {
		file_action ("Store", "Save File", viewport, this);
		store . engaged = false; redraw = true;
	}
	if (restore . keyoff (location)) {
		file_action ("Restore", "Load File", viewport, this);
		feedback ();
		restore . engaged = false; redraw = true;
	}
	if (voice_init . keyoff (location)) {
		if (voice_init_action ()) feedback ();
		voice_init . engaged = false; redraw = true;
	}
	if (redraw) update ();
}

void control_panel_action :: FunctionKey (int key, int state) {
	bool redraw = false;
	if (key >= 65470 && key <= 65481) {f_action (key - 65469 + 12 * state); return;}
	switch (key) {
	case 33: program_action (& selector0); redraw = true; break;
	case 64: program_action (& selector1); redraw = true; break;
	case 35: program_action (& selector2); redraw = true; break;
	case 36: program_action (& selector3); redraw = true; break;
	case 37: program_action (& selector4); redraw = true; break;
	case 94: program_action (& selector5); redraw = true; break;
	case 38: program_action (& selector6); redraw = true; break;
	case 42: program_action (& selector7); redraw = true; break;
	case 40: program_action (& selector8); redraw = true; break;
	case 41: program_action (& selector9); redraw = true; break;
	case 49: program_action (& program0); redraw = true; break;
	case 50: program_action (& program1); redraw = true; break;
	case 51: program_action (& program2); redraw = true; break;
	case 52: program_action (& program3); redraw = true; break;
	case 53: program_action (& program4); redraw = true; break;
	case 54: program_action (& program5); redraw = true; break;
	case 55: program_action (& program6); redraw = true; break;
	case 56: program_action (& program7); redraw = true; break;
	case 57: program_action (& program8); redraw = true; break;
	case 48: program_action (& program9); redraw = true; break;
	case 122:
		delta_1 . engaged = true;
		delta_8 . engaged = delta_128 . engaged = false;
		current_delta = 1;
		redraw = true;
		break;
	case 120:
		delta_8 . engaged = true;
		delta_1 . engaged = delta_128 . engaged = false;
		current_delta = 8;
		redraw = true;
		break;
	case 99:
		delta_128 . engaged = true;
		delta_1 . engaged = delta_8 . engaged = false;
		current_delta = 128;
		redraw = true;
		break;
	case 65293: audio_hardware_action (); break;
	case 65361: value_change_action (-1); redraw = true; break;
	case 65362: value_change_action (4); redraw = true; break;
	case 65363: value_change_action (1); redraw = true; break;
	case 65364: value_change_action (-4); redraw = true; break;
	case 91: redraw = file_action ("Store", "Save File", viewport, this); break;
	case 92: redraw = voice_init_action (); feedback (); break;
	case 93: redraw = file_action ("Restore", "Load File", viewport, this); feedback (); break;
	case 44:
		poly_mono . engaged = ! poly_mono . engaged;
		action (poly_mono . engaged ? 127 : 126, 0.0);
		redraw = true;
		break;
	case 46:
		porta_on_off . engaged = ! porta_on_off . engaged;
		action (65, porta_on_off . engaged ? 1.0 : 0.0);
		redraw = true;
		break;
	case 47: redraw = file_action ("START", "Record File", viewport, this); break;
	case 32: stop_recording_action (); redraw = true; break;
	case 97: change_selector (-1); redraw = true; break;
	case 100: change_selector (1); redraw = true; break;
	case 115: change_program (1); redraw = true; break;
	case 119: change_program (-1); redraw = true; break;
	default: return;
	}
	if (redraw) update ();
}

bool control_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologAtom * command = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {if (atom == 0) atom = el; else command = el -> getAtom ();}
		parameters = parameters -> getRight ();
	}
	if (command == 0) {
		PrologDirectory * dir = root -> searchDirectory ("lunar");
		if (dir == 0) return false;
		command = dir -> searchAtom ("CCCB");
	}
	if (atom == 0 || command == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	control_panel_action * machine = new control_panel_action (resources, root, directory, atom -> getAtom (), command, false);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> BuildPanel ();
	return true;
}

control_panel_class :: control_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> directory = servo -> directory;
	this -> resources = servo -> resources;
}

