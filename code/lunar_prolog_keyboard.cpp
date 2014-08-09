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

#include "prolog_lunar.h"
#include "keyboard_calculator.h"
#include "gtk/gtk.h"

static gboolean RemoveViewportIdleCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}

extern char _binary_small_keyboard_png_start;
extern char _binary_small_keyboard_png_end;
extern char _binary_keyboard_png_start;
extern char _binary_keyboard_png_end;
extern char _binary_big_keyboard_png_start;
extern char _binary_big_keyboard_png_end;

class png_closure {
public:
	char * from;
	char * to;
	png_closure (char * from, char * to) {this -> from = from; this -> to = to;}
};

cairo_status_t png_reader (void * closure, unsigned char * data, unsigned int length) {
	png_closure * png_data = (png_closure *) closure;
	if (png_data -> from >= png_data -> to) return CAIRO_STATUS_READ_ERROR;
	while (length-- > 0 && png_data -> from < png_data -> to) {
		* data++ = * png_data -> from++;
	}
	return CAIRO_STATUS_SUCCESS;
}

class keyboard_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * keyon;
	PrologAtom * atom;
	PrologAtom * command;
	GtkWidget * viewport;
	cairo_surface_t * surface;
	int keyboard_width;
	int keyboard_height;
	keyboard_calculator kb;
	void action (int velocity, int x, int y) {
		int key = kb . get (x, y);
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> atom (keyon),
								root -> pair (root -> integer (key),
								root -> pair (root -> integer (velocity),
								root -> earth ()))));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return remove ();
		return true;
	}
	keyboard_action (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command, int size) : kb (0, 0) {
		this -> root = root;
		keyon = directory == 0 ? 0 : directory -> searchAtom ("keyon");
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		viewport = 0;
		keyboard_width = 200; keyboard_height = 100;
		png_closure small_keyboard_png_closure (& _binary_small_keyboard_png_start, & _binary_small_keyboard_png_end);
		png_closure keyboard_png_closure (& _binary_keyboard_png_start, & _binary_keyboard_png_end);
		png_closure big_keyboard_png_closure (& _binary_big_keyboard_png_start, & _binary_big_keyboard_png_end);
		switch (size) {
		case 1:
			surface = cairo_image_surface_create_from_png_stream (png_reader, & small_keyboard_png_closure);
			kb . set_keyboard_layout_y (66, 44);
			kb . set_keyboard_layout_x (11, 1, 2, 3, 4, 5);
			kb . set_ambitus (17, 54);
			break;
		case 3:
			surface = cairo_image_surface_create_from_png_stream (png_reader, & big_keyboard_png_closure);
			kb . set_keyboard_layout_y (132, 88);
			kb . set_keyboard_layout_x (22, 2, 4, 6, 8, 10);
			kb . set_ambitus (17, 54);
			break;
		default:
			surface = cairo_image_surface_create_from_png_stream (png_reader, & keyboard_png_closure);
			kb . set_keyboard_layout_y (99, 66);
			kb . set_keyboard_layout_x (16, 2, 3, 4, 5, 6);
			kb . set_ambitus (17, 54);
			break;
		}
		if (cairo_surface_status (surface) != CAIRO_STATUS_SUCCESS) {cairo_surface_destroy (surface); surface = 0; return;}
		keyboard_width = cairo_image_surface_get_width (surface);
		keyboard_height = cairo_image_surface_get_height (surface);
	}
	~ keyboard_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		command -> removeAtom ();
		if (surface != 0) cairo_surface_destroy (surface);
	}
};

static gboolean ViewportDeleteEvent (GtkWidget * viewport, GdkEvent * event, keyboard_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}

static gboolean RedrawKeyboard (GtkWidget * viewport, GdkEvent * event, keyboard_action * machine) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (machine -> surface == 0) return FALSE;
	cairo_set_source_surface (cr, machine -> surface, 0.0, 0.0);
	cairo_paint (cr);
	cairo_destroy (cr);
	return FALSE;
}

static gint KeyboardKeyon (GtkWidget * viewport, GdkEventButton * event, keyboard_action * machine) {
	machine -> action ((int) event -> button == 1 ? 100 : 0, (int) event -> x, (int) event -> y);
	return TRUE;
}
static gint KeyboardKeyoff (GtkWidget * viewport, GdkEventButton * event, keyboard_action * machine) {machine -> action (0, (int) event -> x, (int) event -> y); return TRUE;}

static gboolean CreateKeyboardIdleCode (keyboard_action * parameter) {
	parameter -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (parameter -> viewport), parameter -> atom -> name ());
	g_signal_connect (parameter -> viewport, "delete-event", G_CALLBACK (ViewportDeleteEvent), parameter);
	GtkWidget * drawing_area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (parameter -> viewport), drawing_area);
	g_signal_connect (G_OBJECT (drawing_area), "expose-event", G_CALLBACK (RedrawKeyboard), parameter);
	gtk_widget_add_events (parameter -> viewport, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (parameter -> viewport), "button_press_event", G_CALLBACK (KeyboardKeyon), parameter);
	g_signal_connect (G_OBJECT (parameter -> viewport), "button_release_event", G_CALLBACK (KeyboardKeyoff), parameter);
	gtk_window_resize (GTK_WINDOW (parameter -> viewport), parameter -> keyboard_width, parameter -> keyboard_height);
	gtk_widget_show_all (parameter -> viewport);
	return FALSE;
}

bool keyboard_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * command = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) {
			if (atom == 0) atom = el;
			else command = el;
		}
		if (el -> isVar ()) {if (atom != 0) command = atom; atom = el;}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || command == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	keyboard_action * machine = new keyboard_action (root, directory, atom -> getAtom (), command -> getAtom (), size);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateKeyboardIdleCode, machine);
	return true;
}

keyboard_class :: keyboard_class (PrologRoot * root, PrologDirectory * directory, int size) {
	this -> root = root;
	this -> directory = directory;
	this -> size = size;
}

