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

#include "lunar_prolog_panel_base.h"
#include "multiplatform_audio.h"
#include <math.h>

extern MultiplatformAudio audio;

static int sampling_rate_to_index (int freq) {
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
static int index_to_sampling_freq (int ind) {
	switch (ind) {
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
static char * index_to_sampling_freq_description (int ind) {
	switch (ind) {
	case 0: return "8,000 Hz [Telephone]";
	case 1: return "11,025 Hz [PCM MPEG]";
	case 2: return "16,000 Hz [VoIP]";
	case 3: return "22,050 Hz [PCM MPEG]";
	case 4: return "32,000 Hz [miniDV]";
	case 5: return "37,800 Hz [CD-XA]";
	case 6: return "44,056 Hz [NTSC]";
	case 7: return "44,100 Hz [Audio CD]";
	case 8: return "48,000 Hz [HD-SDI]";
	case 9: return "50,000 Hz [3M]";
	case 10: return "50,400 Hz [X-80]";
	case 11: return "88,200 Hz [CD]";
	case 12: return "96,000 Hz [DVD-Audio]";
	case 13: return "176,400 Hz [HDCD]";
	case 14: return "192,000 Hz [DVD-Audio]";
	case 15: return "362,800 Hz [eXtreme]";
	case 16: return "2,822,400 Hz [SACD]";
	case 17: return "5,644,800 Hz [DSD]";
	default: break;
	}
	return "unknown";
}
static char * index_to_latency_description (int ind) {
	switch (ind) {
	case 0: return "1 [no latency]";
	case 1: return "2 [almost no latency]";
	case 2: return "4 [ultra short]";
	case 3: return "8 [non detectable]";
	case 4: return "16 [neligable]";
	case 5: return "32 [excellent]";
	case 6: return "64 [very good]";
	case 7: return "128 [good]";
	case 8: return "256 [still good]";
	case 9: return "512 [audible]";
	case 10: return "1024 [not good]";
	case 11: return "2048 [bad]";
	case 12: return "4096 [very bad]";
	case 13: return "8192 [just make it work]";
	case 14: return "16384 [not very useful]";
	case 15: return "32768 [not useful]";
	case 16: return "65536 [never used]";
	default: break;
	}
	return "unknown";
}
static gboolean DeleterCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}
class core_panel_action : public AudioModulePanel {
public:
	PrologAtom * core;
	PrologAtom * reactor;
	PrologAtom * connect_all_moons;
	button_active_graphics START;
	button_active_graphics RECORD;
	button_active_graphics AUDIO_INPUT;
	button_active_graphics AUDIO_OUTPUT;
	button_active_graphics SAMPLING_RATE;
	button_active_graphics LATENCY_BLOCK_SIZE;
	int requested_sampling_rate;
	int requested_latency_buffer_size;
	int requested_output_device;
	int requested_input_device;
	GtkWidget * deleter;
	void drop_deleter (void) {if (deleter != 0) g_idle_add ((GSourceFunc) DeleterCode, deleter); deleter = 0;}
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
	void feedback (void) {
		START . engaged = reactor != 0 ? reactor -> getMachine () != 0 : false;
		RECORD . engaged = audio . outputFileActive ();
		requested_input_device = audio . getSelectedInputDevice ();
		requested_output_device = audio . getSelectedOutputDevice ();
		requested_sampling_rate = audio . getSamplingFrequency ();
		requested_latency_buffer_size = audio . getLatencyBufferSize ();
	}
	void redraw (cairo_t * cr) {
		START . draw (cr); RECORD . draw (cr); AUDIO_INPUT . draw (cr); AUDIO_OUTPUT . draw (cr);
		SAMPLING_RATE . draw (cr), LATENCY_BLOCK_SIZE . draw (cr);
		cairo_set_font_size (cr, 20.0);
		cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
		cairo_move_to (cr, 104.0, 32.0);
		cairo_show_text (cr, requested_input_device >= 0 ? audio . getInputDeviceName (requested_input_device) : "Inactive");
		cairo_move_to (cr, 104.0, 64.0);
		cairo_show_text (cr, requested_output_device >= 0 ? audio . getOutputDeviceName (requested_output_device) : "Inactive");
		cairo_move_to (cr, 104.0, 96.0);
		cairo_show_text (cr, index_to_sampling_freq_description (sampling_rate_to_index (requested_sampling_rate)));
		cairo_move_to (cr, 104.0, 128.0);
		int latency = (int) (log ((double) (requested_latency_buffer_size)) / log (2.0));
		cairo_show_text (cr, index_to_latency_description (latency));
	}
	void MouseKeyon (point location, int button);
	void MouseKeyoff (point location, int button) {
		bool redraw = false;
		if (START . keyoff (location)) {
			if (START . engaged = ! START . engaged) action_start_audio ();
			else action_stop_audio ();
			redraw = true;
		}
		if (RECORD . keyoff (location)) {
			if (RECORD . engaged) {action_stop_recording (); RECORD . engaged = false;}
			else RECORD . engaged = action_start_recording ();
			redraw = true;
		}
		if (redraw) update ();
	}
	void MouseMove (point delta) {}
	void FunctionKey (int key, int state) {}
	core_panel_action (GraphicResources * resources, PrologRoot * root, PrologAtom * atom, PrologAtom * core, PrologAtom * reactor, PrologAtom * connect_all_moons) :
		START (point (340.0, 88.0), 2, resources, true),
		RECORD (point (390.0, 88.0), 3, resources, true),
		AUDIO_INPUT (point (16.0, 32.0), 4, resources, true),
		AUDIO_OUTPUT (point (16.0, 64.0), 5, resources, true),
		SAMPLING_RATE (point (16.0, 96.0), 6, resources, true),
		LATENCY_BLOCK_SIZE (point (16.0, 128.0), 7, resources, true),
		AudioModulePanel (root, atom, resources != 0 ? resources -> core_panel_surface : 0)
	{
		deleter = 0;
		requested_sampling_rate = 48000;
		requested_latency_buffer_size = 128;
		requested_output_device = 0;
		requested_input_device = -1;
		this -> core = core; COLLECTOR_REFERENCE_INC (core);
		this -> reactor = reactor; COLLECTOR_REFERENCE_INC (reactor);
		this -> connect_all_moons = connect_all_moons; COLLECTOR_REFERENCE_INC (connect_all_moons);
		feedback ();
	}
	~ core_panel_action (void) {
		core -> removeAtom ();
		reactor -> removeAtom ();
		connect_all_moons -> removeAtom ();
	}
};

static void OnAudioInputSelected (GtkWidget * widget, core_panel_action * action) {
	GtkTreeIter iterator; GtkTreeModel * model;
	if (gtk_tree_selection_get_selected (GTK_TREE_SELECTION (widget), & model, & iterator)) {
		GtkTreePath * path = gtk_tree_model_get_path (model, & iterator);
		if (path != 0) {
			int * ip = gtk_tree_path_get_indices (path);
			if (ip != 0) {
				action -> requested_input_device = * ip - 1;
				action -> AUDIO_INPUT . engaged = false;
				action -> update ();
			}
			gtk_tree_path_free (path);
		}
	}
	action -> drop_deleter ();
}
static void OnAudioOutputSelected (GtkWidget * widget, core_panel_action * action) {
	GtkTreeIter iterator; GtkTreeModel * model;
	if (gtk_tree_selection_get_selected (GTK_TREE_SELECTION (widget), & model, & iterator)) {
		GtkTreePath * path = gtk_tree_model_get_path (model, & iterator);
		if (path != 0) {
			int * ip = gtk_tree_path_get_indices (path);
			if (ip != 0) {
				action -> requested_output_device = * ip - 1;
				action -> AUDIO_OUTPUT . engaged = false;
				action -> update ();
			}
			gtk_tree_path_free (path);
		}
	}
	action -> drop_deleter ();
}
static void OnSamplingRateSelected (GtkWidget * widget, core_panel_action * action) {
	GtkTreeIter iterator; GtkTreeModel * model;
	if (gtk_tree_selection_get_selected (GTK_TREE_SELECTION (widget), & model, & iterator)) {
		GtkTreePath * path = gtk_tree_model_get_path (model, & iterator);
		if (path != 0) {
			int * ip = gtk_tree_path_get_indices (path);
			if (ip != 0) {
				action -> requested_sampling_rate = index_to_sampling_freq (* ip);
				action -> SAMPLING_RATE . engaged = false;
				action -> update ();
			}
			gtk_tree_path_free (path);
		}
	}
	action -> drop_deleter ();
}
static void OnLatencyBlockSizeSelected (GtkWidget * widget, core_panel_action * action) {
	GtkTreeIter iterator; GtkTreeModel * model;
	if (gtk_tree_selection_get_selected (GTK_TREE_SELECTION (widget), & model, & iterator)) {
		GtkTreePath * path = gtk_tree_model_get_path (model, & iterator);
		if (path != 0) {
			int * ip = gtk_tree_path_get_indices (path);
			if (ip != 0) {
				action -> requested_latency_buffer_size = (int) pow (2.0, (double) (* ip));
				action -> LATENCY_BLOCK_SIZE . engaged = false;
				action -> update ();
			}
			gtk_tree_path_free (path);
		}
	}
	action -> drop_deleter ();
}
void core_panel_action :: MouseKeyon (point location, int button) {
	bool redraw = false;
	if (AUDIO_INPUT . keyon (location)) {
		AUDIO_INPUT . engaged = true;
		GtkWidget * list_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_move (GTK_WINDOW (list_window), (int) this -> location . x, (int) this -> location . y);
		gtk_window_set_modal (GTK_WINDOW (list_window), true);
		GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);
		GtkTreeIter iterator;
		gtk_list_store_append (store, & iterator); gtk_list_store_set (store, & iterator, 0, "Inactive", -1);
		for (int ind = 0; ind < audio . getNumberOfInputDevices (); ind++) {
			gtk_list_store_append (store, & iterator); gtk_list_store_set (store, & iterator, 0, audio . getInputDeviceName (ind), -1);
		}
		GtkWidget * list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
		g_object_unref (store);
		gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
		gtk_tree_view_append_column (GTK_TREE_VIEW (list), gtk_tree_view_column_new_with_attributes ("IO", gtk_cell_renderer_text_new (), "text", 0, 0));
		GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
		GtkTreePath * path = gtk_tree_path_new_from_indices (requested_input_device + 1, -1);
		gtk_tree_selection_select_path (selection, path);
		gtk_tree_path_free (path);
		gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
		g_signal_connect (selection, "changed", G_CALLBACK (OnAudioInputSelected), this);
		gtk_container_add (GTK_CONTAINER (list_window), list);
		deleter = list_window;
		gtk_widget_show_all (list_window);
		redraw = true;
	}
	if (AUDIO_OUTPUT . keyon (location)) {
		AUDIO_OUTPUT . engaged = true;
		GtkWidget * list_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_move (GTK_WINDOW (list_window), (int) this -> location . x, (int) this -> location . y);
		gtk_window_set_modal (GTK_WINDOW (list_window), true);
		GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);
		GtkTreeIter iterator;
		gtk_list_store_append (store, & iterator); gtk_list_store_set (store, & iterator, 0, "Inactive", -1);
		for (int ind = 0; ind < audio . getNumberOfOutputDevices (); ind++) {
			gtk_list_store_append (store, & iterator); gtk_list_store_set (store, & iterator, 0, audio . getOutputDeviceName (ind), -1);
		}
		GtkWidget * list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
		g_object_unref (store);
		gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
		gtk_tree_view_append_column (GTK_TREE_VIEW (list), gtk_tree_view_column_new_with_attributes ("IO", gtk_cell_renderer_text_new (), "text", 0, 0));
		GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
		GtkTreePath * path = gtk_tree_path_new_from_indices (requested_output_device + 1, -1);
		gtk_tree_selection_select_path (selection, path);
		gtk_tree_path_free (path);
		gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
		g_signal_connect (selection, "changed", G_CALLBACK (OnAudioOutputSelected), this);
		gtk_container_add (GTK_CONTAINER (list_window), list);
		deleter = list_window;
		gtk_widget_show_all (list_window);
		redraw = true;
	}
	if (SAMPLING_RATE . keyon (location)) {
		SAMPLING_RATE . engaged = true;
		GtkWidget * list_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_move (GTK_WINDOW (list_window), (int) this -> location . x, (int) this -> location . y);
		gtk_window_set_modal (GTK_WINDOW (list_window), true);
		GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);
		GtkTreeIter iterator;
		for (int ind = 0; ind < 18; ind++) {
			gtk_list_store_append (store, & iterator); gtk_list_store_set (store, & iterator, 0, index_to_sampling_freq_description (ind), -1);
		}
		GtkWidget * list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
		g_object_unref (store);
		gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
		gtk_tree_view_append_column (GTK_TREE_VIEW (list), gtk_tree_view_column_new_with_attributes ("IO", gtk_cell_renderer_text_new (), "text", 0, 0));
		GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
		GtkTreePath * path = gtk_tree_path_new_from_indices (requested_output_device + 1, -1);
		gtk_tree_selection_select_path (selection, path);
		gtk_tree_path_free (path);
		gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
		g_signal_connect (selection, "changed", G_CALLBACK (OnSamplingRateSelected), this);
		gtk_container_add (GTK_CONTAINER (list_window), list);
		deleter = list_window;
		gtk_widget_show_all (list_window);
		redraw = true;
	}
	if (LATENCY_BLOCK_SIZE . keyon (location)) {
		LATENCY_BLOCK_SIZE . engaged = true;
		GtkWidget * list_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		gtk_window_move (GTK_WINDOW (list_window), (int) this -> location . x, (int) this -> location . y);
		gtk_window_set_modal (GTK_WINDOW (list_window), true);
		GtkListStore * store = gtk_list_store_new (1, G_TYPE_STRING);
		GtkTreeIter iterator;
		for (int ind = 0; ind < 18; ind++) {
			gtk_list_store_append (store, & iterator); gtk_list_store_set (store, & iterator, 0, index_to_latency_description (ind), -1);
		}
		GtkWidget * list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
		g_object_unref (store);
		gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (list), FALSE);
		gtk_tree_view_append_column (GTK_TREE_VIEW (list), gtk_tree_view_column_new_with_attributes ("IO", gtk_cell_renderer_text_new (), "text", 0, 0));
		GtkTreeSelection * selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
		GtkTreePath * path = gtk_tree_path_new_from_indices (requested_output_device + 1, -1);
		gtk_tree_selection_select_path (selection, path);
		gtk_tree_path_free (path);
		gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);
		g_signal_connect (selection, "changed", G_CALLBACK (OnLatencyBlockSizeSelected), this);
		gtk_container_add (GTK_CONTAINER (list_window), list);
		deleter = list_window;
		gtk_widget_show_all (list_window);
		redraw = true;
	}
	if (redraw) update ();
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
	machine -> BuildPanel ();
	return true;
}

core_panel_class :: core_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> resources = servo -> resources;
}
