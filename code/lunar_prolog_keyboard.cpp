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

/////////////////////////////////////////////////////////////////////
// This file was created on Monday, 14th July 2014 at 11:50:02 AM. //
/////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"
#include "graphics2d.h"
#include "graphic_resources.h"
#include "gtk/gtk.h"

class keyboard_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * keyon;
	PrologAtom * atom;
	PrologAtom * command;
	GtkWidget * viewport;
	keyboard_active_graphics keyboard;
	point location;
	void action (int velocity) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> atom (keyon),
								root -> pair (root -> integer (keyboard . key),
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
	bool code (PrologElement * parameters, PrologResolution * resolution);
	keyboard_action (GraphicResources * resources, PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command, int size) :
			keyboard (point (0, 0), size - 1, 0, resources, true) {
		this -> root = root;
		keyon = directory == 0 ? 0 : directory -> searchAtom ("keyon");
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		viewport = 0;
	}
	~ keyboard_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		command -> removeAtom ();
	}
};

static gboolean RepositionKeyboard (keyboard_action * keyboard) {
	gtk_window_move (GTK_WINDOW (keyboard -> viewport), (int) keyboard -> location . x, (int) keyboard -> location . y);
	return FALSE;
}
bool keyboard_action :: code (PrologElement * parameters, PrologResolution * resolution) {
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
	g_idle_add ((GSourceFunc) RepositionKeyboard, this);
	return true;
}

static gboolean ViewportDeleteEvent (GtkWidget * viewport, GdkEvent * event, keyboard_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}

static gboolean RedrawKeyboard (GtkWidget * viewport, GdkEvent * event, keyboard_action * machine) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	machine -> keyboard . draw (cr);
	cairo_destroy (cr);
	return FALSE;
}

static gint KeyboardKeyon (GtkWidget * viewport, GdkEventButton * event, keyboard_action * machine) {
	point location ((int) event -> x, (int) event -> y);
	if ((int) event -> button == 1) machine -> keyboard . keyon (location);
	else machine -> keyboard . keyoff (location);
	machine -> action ((int) event -> button == 1 ? 100 : 0);
	return TRUE;
}
static gint KeyboardKeyoff (GtkWidget * viewport, GdkEventButton * event, keyboard_action * machine) {
	machine -> keyboard . keyoff (point ((int) event -> x, (int) event -> y));
	machine -> action (0);
	return TRUE;
}

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
	gtk_window_resize (GTK_WINDOW (parameter -> viewport), (int) parameter -> keyboard . location . size . x, (int) parameter -> keyboard . location . size . y);
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
	keyboard_action * machine = new keyboard_action (resources, root, directory, atom -> getAtom (), command -> getAtom (), size);
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateKeyboardIdleCode, machine);
	return true;
}

keyboard_class :: keyboard_class (PrologLunarServiceClass * servo, int size) {
	this -> resources = servo -> resources;
	this -> root = servo -> root;
	this -> directory = servo -> directory;
	this -> size = size;
}

