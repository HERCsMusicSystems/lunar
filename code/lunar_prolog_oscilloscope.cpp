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
// This file was created on Wednesday, 16th July 2014, 10:53:04 AM. //
//////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "gtk/gtk.h"

static gboolean RemoveOscilloscopeIdleCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}

class oscilloscope_action : public PrologNativeCode {
public:
	GtkWidget * viewport;
	PrologAtom * atom;
	cairo_surface_t * surface;
	double wave [256];
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveOscilloscopeIdleCode, viewport);
		delete this;
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return remove ();
		return true;
	}
	oscilloscope_action (PrologAtom * atom) {
		viewport = 0;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		for (int ind = 0; ind < 256; ind++) wave [ind] = sin ((double) ind * M_PI * 2.0 / 256.0);
	}
	~ oscilloscope_action (void) {
		atom -> setMachine (0);
		atom -> removeAtom ();
	}
};

static gboolean DeleteOscilloscopeEvent (GtkWidget * viewport, GdkEvent * event, oscilloscope_action * osc) {
	gtk_widget_destroy (osc -> viewport);
	osc -> remove (false);
	return FALSE;
}

static gboolean RedrawOscilloscope (GtkWidget * viewport, GdkEvent * event, oscilloscope_action * osc) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	cairo_rectangle (cr, 0.0, 0.0, 256.0, 128.0);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_fill (cr);
	//cairo_set_line_width (cr, 0.5);
	cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
	cairo_move_to (cr, 0.0, 64.0 - osc -> wave [0] * 64.0);
	for (int ind = 1; ind < 256; ind++) cairo_line_to (cr, 0.0 + (double) ind, 64.0 - osc -> wave [ind] * 64.0);
	cairo_stroke (cr);
	cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
	cairo_move_to (cr, 0.0, 32.0);
	cairo_line_to (cr, 256.0, 32.0);
	cairo_move_to (cr, 0.0, 96.0);
	cairo_line_to (cr, 256.0, 96.0);
	cairo_move_to (cr, 32.0, 0.0);
	cairo_line_to (cr, 32.0, 128.0);
	cairo_move_to (cr, 64.0, 0.0);
	cairo_line_to (cr, 64.0, 128.0);
	cairo_move_to (cr, 96.0, 0.0);
	cairo_line_to (cr, 96.0, 128.0);
	cairo_move_to (cr, 160.0, 0.0);
	cairo_line_to (cr, 160.0, 128.0);
	cairo_move_to (cr, 192.0, 0.0);
	cairo_line_to (cr, 192.0, 128.0);
	cairo_move_to (cr, 224.0, 0.0);
	cairo_line_to (cr, 224.0, 128.0);
	cairo_stroke (cr);
	cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
	cairo_move_to (cr, 0.0, 64.0);
	cairo_line_to (cr, 256.0, 64.0);
	cairo_move_to (cr, 128.0, 0.0);
	cairo_line_to (cr, 128.0, 128.0);
	cairo_stroke (cr);
	cairo_destroy (cr);
	return FALSE;
}

static gboolean CreateOscilloscopeIdleCode (oscilloscope_action * osc) {
	osc -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (osc -> viewport), osc -> atom -> name ());
	g_signal_connect (osc -> viewport, "delete-event", G_CALLBACK (DeleteOscilloscopeEvent), osc);
	GtkWidget * drawing_area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (osc -> viewport), drawing_area);
	g_signal_connect (G_OBJECT (drawing_area), "expose-event", G_CALLBACK (RedrawOscilloscope), osc);
	gtk_window_resize (GTK_WINDOW (osc -> viewport), 256, 128);
	gtk_widget_show_all (osc -> viewport);
	return FALSE;
}

bool oscilloscope_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isVar ()) atom = el;
		parameters = parameters -> getRight ();
	}
	if (atom == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	oscilloscope_action * machine = new oscilloscope_action (atom -> getAtom ());
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	g_idle_add ((GSourceFunc) CreateOscilloscopeIdleCode, machine);
	return true;
}

