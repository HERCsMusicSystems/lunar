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

class lunar_oscilloscope : public orbiter {
public:
	GtkWidget * viewport;
	double wave [256];
	int frame_count;
	virtual int numberOfInputs (void) {return numberOfOutputs ();}
	virtual char * inputName (int ind) {return outputName (ind);}
	virtual double * inputAddress (int ind) {return outputAddress (ind);}
	virtual void move (void) {
		if (viewport == 0) return;
		if (frame_count < 0) {frame_count++; return;}
		wave [frame_count++] = signal;
		if (frame_count < 256) return;
		frame_count = -10000;
		gtk_widget_queue_draw (viewport);
	}
	lunar_oscilloscope (orbiter_core * core) : orbiter (core) {
		viewport = 0;
		frame_count = 0;
		for (int ind = 0; ind < 256; ind++) wave [ind] = 0;
		initialise (); activate ();
	}
};

class oscilloscope_action : public PrologNativeOrbiter {
public:
	bool remove_gtk;
	void remove_by_gtk (void) {
		remove_gtk = false;
		atom -> setMachine (0);
		delete this;
	}
	oscilloscope_action (PrologAtom * atom, orbiter_core * core, orbiter * module) : PrologNativeOrbiter (atom, core, module) {
		remove_gtk = true;
		COLLECTOR_REFERENCE_INC (atom);
	}
	~ oscilloscope_action (void) {
		if (remove_gtk) g_idle_add ((GSourceFunc) RemoveOscilloscopeIdleCode, ((lunar_oscilloscope *) module) -> viewport);
		atom -> removeAtom ();
	}
};

static gboolean DeleteOscilloscopeEvent (GtkWidget * viewport, GdkEvent * event, oscilloscope_action * osc) {
	gtk_widget_destroy (((lunar_oscilloscope *) osc -> module) -> viewport);
	osc -> remove_by_gtk ();
	return FALSE;
}

static gboolean RedrawOscilloscope (GtkWidget * viewport, GdkEvent * event, oscilloscope_action * osc) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	lunar_oscilloscope * losc = (lunar_oscilloscope *) osc -> module;
	cairo_rectangle (cr, 0.0, 0.0, 256.0, 128.0);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_fill (cr);
	cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
	cairo_move_to (cr, 0.0, 64.0 - losc -> wave [0] * 64.0);
	for (int ind = 1; ind < 256; ind++) cairo_line_to (cr, 0.0 + (double) ind, 64.0 - losc -> wave [ind] * 64.0);
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
	GtkWidget * viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (viewport), osc -> atom -> name ());
	g_signal_connect (viewport, "delete-event", G_CALLBACK (DeleteOscilloscopeEvent), osc);
	GtkWidget * drawing_area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (viewport), drawing_area);
	g_signal_connect (G_OBJECT (drawing_area), "expose-event", G_CALLBACK (RedrawOscilloscope), osc);
	gtk_window_resize (GTK_WINDOW (viewport), 256, 128);
	gtk_widget_show_all (viewport);
	((lunar_oscilloscope *) (osc -> module)) -> viewport = viewport;
	return FALSE;
}

orbiter * oscilloscope_class :: create_orbiter (PrologElement * parameters) {return new lunar_oscilloscope (core);}
PrologNativeOrbiter * oscilloscope_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new oscilloscope_action (atom, core, module);}
void oscilloscope_class :: code_created (PrologNativeOrbiter * machine) {g_idle_add ((GSourceFunc) CreateOscilloscopeIdleCode, machine);}
oscilloscope_class :: oscilloscope_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}
