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
// This file was created on Wednesday, 17th April 2015 at 11:12:19. //
//////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"
#include "graphic_resources.h"
#include "multiplatform_audio.h"
#include <string.h>
#include <math.h>

extern MultiplatformAudio audio;

class core_panel_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * core;
	PrologAtom * reactor;
	PrologAtom * connect_all_moons;
	GtkWidget * viewport;
	cairo_surface_t * background_image;
	point location;
	point captured;
	int captured_button;
	GdkEventType captured_type;
	button_active_graphics START;
	button_active_graphics RECORD;
	button_active_graphics AUDIO_OUTPUT;
	int requested_sampling_rate;
	int requested_latency_buffer_size;
	int requested_output_device;
	int requested_input_device;
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	void action_start_audio (void) {
		if (reactor -> getMachine () != 0) return;
		PrologElement * query = root -> pair (root -> atom (core),
								root -> pair (root -> atom (reactor),
								root -> pair (root -> integer (330),
								root -> pair (root -> integer (requested_sampling_rate),
								root -> pair (root -> integer (requested_latency_buffer_size),
								root -> pair (root -> integer (requested_output_device),
								root -> pair (root -> integer (requested_input_device), root -> earth ())))))));
		PrologElement * q2 = root -> pair (root -> atom (connect_all_moons),
								root -> pair (root -> atom (reactor), root -> earth ()));
		query = root -> pair (query, root -> pair (q2, root -> earth ()));
		query = root -> pair (root -> earth (), query);
		root -> resolution (query);
		delete query;
	}
	void action_stop_audio (void) {
		if (reactor -> getMachine () == 0) return;
		PrologElement * query = root -> pair (root -> atom (reactor), root -> earth ());
		query = root -> pair (root -> earth (), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	bool action_start_recording (void) {
		GtkWidget * dialog = gtk_file_chooser_dialog_new ("Record audio.", GTK_WINDOW (viewport),
										GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
		bool ret = false;
		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
			char * file_name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
			audio . selectOutputFile (3600.0, file_name);
			g_free (file_name);
			ret = true;
		}
		gtk_widget_destroy (dialog);
		return ret;
	}
	void action_stop_recording (void) {audio . stopRecording ();}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	core_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom, PrologAtom * core, PrologAtom * reactor, PrologAtom * connect_all_moons) :
		START (point (340.0, 88.0), 2, resources, true),
		RECORD (point (390.0, 88.0), 3, resources, true),
		AUDIO_OUTPUT (point (390.0, 108.0), 4, resources, true)
	{
		this -> root = root;
		captured_button = 0;
		captured_type = (GdkEventType) 0;
		background_image = resources != 0 ? resources -> core_panel_surface : 0;
		requested_sampling_rate = 48000;
		requested_latency_buffer_size = 128;
		requested_output_device = 0;
		requested_input_device = -1;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> core = core; COLLECTOR_REFERENCE_INC (core);
		this -> reactor = reactor; COLLECTOR_REFERENCE_INC (reactor);
		this -> connect_all_moons = connect_all_moons; COLLECTOR_REFERENCE_INC (connect_all_moons);
		START . engaged = reactor == 0 ? 0 : reactor -> getMachine () != 0;
	}
	~ core_panel_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		core -> removeAtom ();
		reactor -> removeAtom ();
		connect_all_moons -> removeAtom ();
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
	action -> captured_type = event -> type;
	action -> captured_button = event -> button;
	point location (event -> x, event -> y);
	action -> captured = location;
	bool redraw = false;
	if (action -> AUDIO_OUTPUT . keyon (location)) {action -> AUDIO_OUTPUT . engaged = true; redraw = true;}
	if (redraw) gtk_widget_queue_draw (viewport);
	return TRUE;
}
static gint CorePanelKeyoff (GtkWidget * viewport, GdkEventButton * event, core_panel_action * action) {
	point location (event -> x, event -> y);
	bool redraw = false;
	if (action -> START . keyoff (location)) {
		if (action -> captured_type == GDK_BUTTON_PRESS) {
			if (action -> START . engaged = ! action -> START . engaged) action -> action_start_audio ();
			else action -> action_stop_audio ();
			redraw = true;
		}
	}
	if (action -> RECORD . keyoff (location)) {
		if (action -> captured_type == GDK_BUTTON_PRESS) {
			if (action -> RECORD . engaged) {action -> action_stop_recording (); action -> RECORD . engaged = false;}
			else action -> RECORD . engaged = action -> action_start_recording ();
			redraw = true;
		}
	}
	if (action -> AUDIO_OUTPUT . keyoff (location)) {action -> AUDIO_OUTPUT . engaged = false; redraw = true;}
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
	action -> START . draw (cr);
	action -> RECORD . draw (cr);
	action -> AUDIO_OUTPUT . draw (cr);
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
int sampling_rate_to_index (int freq) {
	switch (freq) {
	case 8000: return 0;
	case 11025: return 1;
	case 16000: return 2;
	case 22050: return 3;
	case 32000: return 4;
	case 37800: return 5;
	case 44056: return 6;
	case 44100: return 7;
	case 48000: return 8;
	case 50000: return 9;
	case 50400: return 10;
	case 88200: return 11;
	case 96000: return 12;
	case 176400: return 13;
	case 192000: return 14;
	case 362800: return 15;
	case 2822400: return 16;
	case 5644800: return 17;
	default: break;
	}
	return 8;
}
int index_to_sampling_freq (int freq) {
	switch (freq) {
	case 0: return 8000;
	case 1: return 11025;
	case 2: return 16000;
	case 3: return 22050;
	case 4: return 32000;
	case 5: return 37800;
	case 6: return 44056;
	case 7: return 44100;
	case 8: return 48000;
	case 9: return 50000;
	case 10: return 50400;
	case 11: return 88200;
	case 12: return 96000;
	case 13: return 176400;
	case 14: return 192000;
	case 15: return 362800;
	case 16: return 2822400;
	case 17: return 5644800;
	default: break;
	}
	return 48000;
}
static void SamplingRateChanged (GtkWidget * combo, core_panel_action * action) {
	int index = gtk_combo_box_get_active (GTK_COMBO_BOX (combo));
	action -> requested_sampling_rate = index_to_sampling_freq (index);
}
static void LatencyBufferSizeChanged (GtkWidget * combo, core_panel_action * action) {
	int index = gtk_combo_box_get_active (GTK_COMBO_BOX (combo));
	action -> requested_latency_buffer_size = (int) pow (2.0, index);
}
static void OutputDeviceChanged (GtkWidget * combo, core_panel_action * action) {
	action -> requested_output_device = gtk_combo_box_get_active (GTK_COMBO_BOX (combo));
}
static void InputDeviceChanged (GtkWidget * combo, core_panel_action * action) {
	action -> requested_input_device = gtk_combo_box_get_active (GTK_COMBO_BOX (combo));
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
	gtk_combo_box_set_active (GTK_COMBO_BOX (input_combo), 1 + (action -> requested_input_device = audio . getSelectedInputDevice ()));
	gtk_fixed_put (GTK_FIXED (area), input_combo, 20, 40);
	GtkWidget * output_combo = gtk_combo_box_new_text ();
	gtk_combo_box_append_text (GTK_COMBO_BOX (output_combo), "Inactive");
	for (int ind = 0; ind < audio . getNumberOfOutputDevices (); ind++) {
		gtk_combo_box_append_text (GTK_COMBO_BOX (output_combo), audio . getOutputDeviceName (ind));
	}
	gtk_combo_box_set_active (GTK_COMBO_BOX (output_combo), 1 + (action -> requested_output_device + audio . getSelectedOutputDevice ()));
	gtk_fixed_put (GTK_FIXED (area), output_combo, 20, 120);
	GtkWidget * latency_combo = gtk_combo_box_new_text ();
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "1 [no latency]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "2 [almost no latency]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "4 [ultra short]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "8 [non detectable]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "16 [neligable]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "32 [excellent]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "64 [very good]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "128 [good]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "256 [still good]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "512 [audible]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "1024 [not good]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "2048 [bad]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "4096 [very bad]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "8192 [just make it work]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "16384 [not very useful]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "32768 [not useful]");
	gtk_combo_box_append_text (GTK_COMBO_BOX (latency_combo), "65536 [never used]");
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
	if (action -> reactor != 0 && action -> reactor -> getMachine () != 0) {
		gtk_combo_box_set_active (GTK_COMBO_BOX (sampling_combo), sampling_rate_to_index (action -> requested_sampling_rate = audio . getSamplingFrequency ()));
		int latency = (int) (log ((double) (action -> requested_latency_buffer_size = audio . getLatencyBufferSize ())) / log (2.0));
		gtk_combo_box_set_active (GTK_COMBO_BOX (latency_combo), latency);
	} else {
		gtk_combo_box_set_active (GTK_COMBO_BOX (latency_combo), 7);
		gtk_combo_box_set_active (GTK_COMBO_BOX (sampling_combo), 8);
	}
	g_signal_connect (G_OBJECT (latency_combo), "changed", G_CALLBACK (LatencyBufferSizeChanged), action);
	gtk_fixed_put (GTK_FIXED (area), latency_combo, 440, 50);
	g_signal_connect (G_OBJECT (sampling_combo), "changed", G_CALLBACK (SamplingRateChanged), action);
	gtk_fixed_put (GTK_FIXED (area), sampling_combo, 440, 90);
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
	PrologAtom * connect_all_moons = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isVar ()) atom = el;
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else if (core == 0) core = el -> getAtom ();
			else if (reactor == 0) reactor = el -> getAtom ();
			else connect_all_moons = el -> getAtom ();
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0) return false;
	if (core == 0 || reactor == 0) {
		PrologDirectory * dir = root -> searchDirectory ("lunar");
		if (dir == 0) return false;
		if (core == 0) core = dir -> searchAtom ("core");
		if (reactor == 0) reactor = dir -> searchAtom ("reactor");
		if (connect_all_moons == 0) connect_all_moons = dir -> searchAtom ("ConnectAllMoons");
	}
	if (core == 0 || reactor == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (atom -> getAtom () -> getMachine () != 0) return false;
	core_panel_action * machine = new core_panel_action (resources, root, atom -> getAtom (), core, reactor, connect_all_moons);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateCorePanelIdleCode, machine);
	return true;
}

core_panel_class :: core_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
