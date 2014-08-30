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
#include "gtk/gtk.h"

static gboolean RemoveViewportIdleCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}

#ifdef WIN32
#include "lunar_resource.h"
static char * GetResource (int ind) {
	HRSRC resource = FindResource (NULL, MAKEINTRESOURCE (ind), RT_RCDATA);
	if (! resource) return 0;
	HGLOBAL loader = LoadResource (0, resource);
	if (! loader) return 0;
	return (char *) LockResource (loader);
}
#else
extern char resource_vector_frame_start;
extern char resource_vector_frame_end;
extern char resource_vector_handle_start;
extern char resource_vector_handle_end;
#endif

class png_closure {
public:
	char * from;
	char * to;
	png_closure (char * from, char * to) {this -> from = from; this -> to = from == 0 ? from : to;}
};

static cairo_status_t png_reader (void * closure, unsigned char * data, unsigned int length) {
	png_closure * png_data = (png_closure *) closure;
	if (png_data -> from >= png_data -> to) return CAIRO_STATUS_READ_ERROR;
	while (length-- > 0 && png_data -> from < png_data -> to) {
		* data++ = * png_data -> from++;
	}
	return CAIRO_STATUS_SUCCESS;
}

class vector_action : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * command;
	bool on;
	GtkWidget * viewport;
	cairo_surface_t * surface;
	cairo_surface_t * handle;
	point position;
	point reference;
	void action (void) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> Double (1.0 - position . x / 64.0),
								root -> pair (root -> Double (-1.0 + position . y / 64.0),
								root -> earth ())));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		delete this;
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {if (parameters -> isEarth ()) return remove (); return true;}
	vector_action (PrologRoot * root, PrologAtom * atom, PrologAtom * command) : position (64.0, 64.0) {
		on = false;
		this -> root = root;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> command = command; COLLECTOR_REFERENCE_INC (command);
		viewport = 0;
#ifdef WIN32
		char * resource = GetResource (VECTOR_FRAME_PNG);
		png_closure frame_closure (resource, resource + VECTOR_FRAME_SIZE);
		resource = GetResource (VECTOR_HANDLE_PNG);
		png_closure handle_closure (resource, resource + VECTOR_HANDLE_SIZE);
#else
		png_closure frame_closure (& resource_vector_frame_start, & resource_vector_frame_end);
		png_closure handle_closure (& resource_vector_handle_start, & resource_vector_handle_end);
#endif
		surface = cairo_image_surface_create_from_png_stream (png_reader, & frame_closure);
		handle = cairo_image_surface_create_from_png_stream (png_reader, & handle_closure);
	}
	~ vector_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
		command -> removeAtom ();
		if (surface != 0) cairo_surface_destroy (surface);
		if (handle != 0) cairo_surface_destroy (handle);
	}
};

static gboolean ViewportDeleteEvent (GtkWidget * viewport, GdkEvent * event, vector_action * machine) {
	gtk_widget_destroy (machine -> viewport);
	machine -> remove (false);
	return FALSE;
}

static gboolean RedrawVector (GtkWidget * viewport, GdkEvent * event, vector_action * machine) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	if (machine -> surface == 0) return FALSE;
	cairo_set_source_surface (cr, machine -> surface, 0.0, 0.0);
	cairo_paint (cr);
	cairo_surface_t * sub = cairo_surface_create_for_rectangle (machine -> handle, machine -> position . x, machine -> position . y, 129.0, 129.0);
	cairo_set_source_surface (cr, sub, 23.0, 24.0);
	cairo_paint (cr);
	cairo_surface_destroy (sub);
	cairo_destroy (cr);
	return FALSE;
}

static gint VectorKeyon (GtkWidget * viewport, GdkEventButton * event, vector_action * machine) {
	machine -> on = true;
	machine -> reference = point (event -> x, event -> y);
	return TRUE;
}

static gint VectorKeyoff (GtkWidget * viewport, GdkEventButton * event, vector_action * machine) {
	machine -> on = false;
	return TRUE;
}

static gint VectorMove (GtkWidget * viewport, GdkEventButton * event, vector_action * machine) {
	if (machine -> on) {
		point p (event -> x, event -> y);
		machine -> position -= (p - machine -> reference) * 0.25;
		machine -> reference = p;
		if (machine -> position . x < 0.0) machine -> position . x = 0.0;
		if (machine -> position . y < 0.0) machine -> position . y = 0.0;
		if (machine -> position . x > 128.0) machine -> position . x = 128.0;
		if (machine -> position . y > 128.0) machine -> position . y = 128.0;
		gtk_widget_queue_draw (machine -> viewport);
		machine -> action ();
	}
	return TRUE;
}

static gboolean CreateVectorIdleCode (vector_action * parameter) {
	parameter -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (parameter -> viewport), parameter -> atom -> name ());
	g_signal_connect (parameter -> viewport, "delete-event", G_CALLBACK (ViewportDeleteEvent), parameter);
	GtkWidget * area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (parameter -> viewport), area);
	g_signal_connect (G_OBJECT (area), "expose-event", G_CALLBACK (RedrawVector), parameter);
	gtk_widget_add_events (parameter -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (parameter -> viewport), "button_press_event", G_CALLBACK (VectorKeyon), parameter);
	g_signal_connect (G_OBJECT (parameter -> viewport), "button_release_event", G_CALLBACK (VectorKeyoff), parameter);
	g_signal_connect (G_OBJECT (parameter -> viewport), "motion_notify_event", G_CALLBACK (VectorMove), parameter);
	gtk_window_resize (GTK_WINDOW (parameter -> viewport), cairo_image_surface_get_width (parameter -> surface), cairo_image_surface_get_height (parameter -> surface));
	gtk_widget_show_all (parameter -> viewport);
	return FALSE;
}

bool vector_class :: code (PrologElement * parameters, PrologResolution * resolution) {
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
	vector_action * machine = new vector_action (root, atom -> getAtom (), command -> getAtom ());
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateVectorIdleCode, machine);
	return true;
}

vector_class :: vector_class (PrologRoot * root) {this -> root = root;}

