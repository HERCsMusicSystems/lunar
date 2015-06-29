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
#include "graphics2d.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "gtk/gtk.h"

static gboolean RemoveOscilloscopeIdleCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}

class lunar_oscilloscope : public orbiter {
public:
	double previous;
	GtkWidget * viewport;
	bool no_redraw;
	double wave [1024];
	double fft [512];
	int frame_count;
	oscilloscope_class :: types type;
	int base, shift, refresh_samples;
	int current_shift, current_base;
	point location;
	virtual int numberOfInputs (void) {return numberOfOutputs ();}
	virtual char * inputName (int ind) {return outputName (ind);}
	virtual double * inputAddress (int ind) {return outputAddress (ind);}
	virtual void move (void);
	lunar_oscilloscope (orbiter_core * core, oscilloscope_class :: types type, int base, int shift, int refresh_samples) : orbiter (core) {
		previous = signal;
		location = point (0.0, 0.0);
		this -> type = type;
		if (base < 1) base = 1;
		this -> base = base; current_base = 1;
		if (shift < 0) shift = 0;
		this -> current_shift = this -> shift = shift;
		if (refresh_samples < 256) refresh_samples = 256;
		this -> refresh_samples = refresh_samples;
		viewport = 0;
		no_redraw = false;
		frame_count = 0;
		for (int ind = 0; ind < 512; ind++) fft [ind] = wave [ind] = 0;
		initialise (); activate ();
	}
};
static gboolean RepaintOscilloscopeIdleCode (lunar_oscilloscope * osc) {if (osc -> no_redraw) return FALSE; gtk_widget_queue_draw (osc -> viewport); return FALSE;}

static void FFT (double * f, double * rec, double * ims, int count, double * sine_wave, int step, bool half) {
	if (count == 4) {
		rec [0] = (f [0] + f [1] + f [2] + f [3]) * 0.25;
		rec [1] = (f [0] - f [2]) * 0.25;
		rec [2] = (f [0] - f [1] + f [2] - f [3]) * 0.25;
		rec [3] = rec [1];
		ims [0] = 0;
		ims [1] = (f [1] - f [3]) * 0.25;
		ims [2] = 0;
		ims [3] = - ims [1];
		return;
	}
	int spectrum_length = count >> 1;
	double * evens = new double [count + count + count];
	double * odds = evens + spectrum_length;
	double * evensrec = odds + spectrum_length;
	double * evensims = evensrec + spectrum_length;
	double * oddsrec = evensims + spectrum_length;
	double * oddsims = oddsrec + spectrum_length;
	int index = 0;
	for (int ind = 0; ind < spectrum_length; ind++) {
		evens [ind] = f [index++];
		odds [ind] = f [index++];
	}
	int two_step = step << 1;
	FFT (evens, evensrec, evensims, spectrum_length, sine_wave, two_step, false);
	FFT (odds, oddsrec, oddsims, spectrum_length, sine_wave, two_step, false);
	int omega = 0;
	if (half) {
		for (int ind = 0; ind < spectrum_length; ind++) {
			double cosine = sine_wave [0x3fff & (omega + 4096)];
			double sine = sine_wave [0x3fff & omega];
			double re = oddsrec [ind];
			double im = oddsims [ind];
			double oddre = cosine * re - sine * im;
			double oddim = cosine * im + sine * re;
			rec [ind] = evensrec [ind] + oddre;
			ims [ind] = evensims [ind] + oddim;
			omega += step;
		}
	} else {
		index = spectrum_length;
		for (int ind = 0; ind < spectrum_length; ind++) {
			double cosine = sine_wave [0x3fff & (omega + 4096)];
			double sine = sine_wave [0x3fff & omega];
			double re = oddsrec [ind];
			double im = oddsims [ind];
			double oddre = cosine * re - sine * im;
			double oddim = cosine * im + sine * re;
			rec [ind] = evensrec [ind] + oddre;
			ims [ind] = evensims [ind] + oddim;
			rec [index] = evensrec [ind] - oddre;
			ims [index] = evensims [ind] - oddim;
			omega += step;
			index++;
		}
	}
	delete [] evens;
}

void lunar_oscilloscope :: move (void) {
	if (viewport == 0) return;
	if (frame_count < 0) {frame_count++; previous = signal; return;}
	if (frame_count == 0 && current_shift == shift && (signal < 0.0 || previous >= 0.0)) {previous = signal; return;}
	if (current_shift-- > 0) return;
	if (current_base-- > 1) return;
	wave [frame_count++] = signal;
	current_base = base;
	int step = 0;
	double rec [512];
	double ims [512];
	switch (type) {
	case oscilloscope_class :: OSCILLOSCOPE: if (frame_count < 256) return; break;
	case oscilloscope_class :: SPECTROSCOPE:
		if (frame_count < 512) return;
		/*for (int ind = 0; ind < 256; ind++) {
			fft [ind] = 0.0;
			int alpha = 0;
			double re = 0.0, im = 0.0;
			for (int sub = 0; sub < 512; sub++) {
				re += core -> sine_wave [0x3fff & alpha] * wave [sub];
				im += core -> sine_wave [0x3fff & (alpha + 4096)] * wave [sub];
				alpha += step;
			}
			fft [ind] = 0.00390625 * sqrt (re * re + im * im);
			step += 32;
		}*/
		FFT (wave, rec, ims, 512, core -> sine_wave, 32, true);
		for (int ind = 0; ind < 256; ind++) fft [ind] = sqrt (rec [ind] * rec [ind] + ims [ind] * ims [ind]);
		break;
	case oscilloscope_class :: BIG_OSCILLOSCOPE: if (frame_count < 512) return; break;
	case oscilloscope_class :: BIG_SPECTROSCOPE:
		if (frame_count < 1024) return;
		FFT (wave, rec, ims, 1024, core -> sine_wave, 16, true);
		for (int ind = 0; ind < 512; ind++) fft [ind] = sqrt (rec [ind] * rec [ind] + ims [ind] * ims [ind]);
		break;
	default: break;
	}
	current_shift = shift;
	frame_count = - refresh_samples;
	g_idle_add ((GSourceFunc) RepaintOscilloscopeIdleCode, this);
}

class oscilloscope_action : public PrologNativeOrbiter {
public:
	oscilloscope_class :: types type;
	bool remove_gtk;
	GtkWidget * drawing_area;
	int gtk_redrawer;
	char markings [9] [16];
	int marking_locations [9];
	bool need_to_calculate_markings;
	void remove_by_gtk (void) {
		remove_gtk = false;
		atom -> setMachine (0);
		delete this;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution);
	oscilloscope_action (PrologAtom * atom, orbiter_core * core, orbiter * module, oscilloscope_class :: types type) : PrologNativeOrbiter (atom, core, module) {
		this -> type = type;
		remove_gtk = true;
		COLLECTOR_REFERENCE_INC (atom);
		need_to_calculate_markings = true;
	}
	~ oscilloscope_action (void) {
		if (remove_gtk) {
			g_signal_handler_disconnect (drawing_area, gtk_redrawer);
			g_idle_add ((GSourceFunc) RemoveOscilloscopeIdleCode, ((lunar_oscilloscope *) module) -> viewport);
		}
		atom -> removeAtom ();
	}
};

static gboolean RepositionOscilloscope (lunar_oscilloscope * osc) {
	gtk_window_move (GTK_WINDOW (osc -> viewport), (int) osc -> location . x, (int) osc -> location . y);
	return FALSE;
}
bool oscilloscope_action :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * x = 0;
	PrologElement * y = 0;
	PrologElement * p = parameters;
	while (p -> isPair ()) {
		PrologElement * el = p -> getLeft ();
		if (el -> isNumber ()) if (x == 0) x = el; else y = el;
		p = p -> getRight ();
	}
	if (x == 0 || y == 0) return PrologNativeOrbiter :: code (parameters, resolution);
	lunar_oscilloscope * osc = (lunar_oscilloscope *) module;
	osc -> location = point (x -> getNumber (), y -> getNumber ());
	g_idle_add ((GSourceFunc) RepositionOscilloscope, osc);
	return true;
}

static gboolean DeleteOscilloscopeEvent (GtkWidget * viewport, GdkEvent * event, oscilloscope_action * osc) {
	((lunar_oscilloscope *) osc -> module) -> no_redraw = true;
	gtk_widget_destroy (((lunar_oscilloscope *) osc -> module) -> viewport);
	osc -> remove_by_gtk ();
	return FALSE;
}

static gboolean RedrawOscilloscope (GtkWidget * viewport, GdkEvent * event, oscilloscope_action * osc) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	lunar_oscilloscope * losc = (lunar_oscilloscope *) osc -> module;
	if (osc -> type < oscilloscope_class :: BIG_OSCILLOSCOPE) cairo_rectangle (cr, 0.0, 0.0, 302.0, 174.0);
	else cairo_rectangle (cr, 0.0, 0.0, 552.0, 296.0);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_fill (cr);
	switch (osc -> type) {
	case oscilloscope_class :: OSCILLOSCOPE:
		// grid
		cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 276.0, 20.0);
		cairo_move_to (cr, 20.0, 52.0); cairo_line_to (cr, 276.0, 52.0);
		cairo_move_to (cr, 20.0, 116.0); cairo_line_to (cr, 276.0, 116.0);
		cairo_move_to (cr, 20.0, 148.0); cairo_line_to (cr, 276.0, 148.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 20.0, 148.0);
		cairo_move_to (cr, 52.0, 20.0); cairo_line_to (cr, 52.0, 148.0);
		cairo_move_to (cr, 84.0, 20.0); cairo_line_to (cr, 84.0, 148.0);
		cairo_move_to (cr, 116.0, 20.0); cairo_line_to (cr, 116.0, 148.0);
		cairo_move_to (cr, 180.0, 20.0); cairo_line_to (cr, 180.0, 148.0);
		cairo_move_to (cr, 212.0, 20.0); cairo_line_to (cr, 212.0, 148.0);
		cairo_move_to (cr, 244.0, 20.0); cairo_line_to (cr, 244.0, 148.0);
		cairo_move_to (cr, 276.0, 20.0); cairo_line_to (cr, 276.0, 148.0);
		cairo_stroke (cr);
		cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
		cairo_move_to (cr, 20.0, 84.0);
		cairo_line_to (cr, 276.0, 84.0);
		cairo_move_to (cr, 148.0, 20.0);
		cairo_line_to (cr, 148.0, 148.0);
		cairo_stroke (cr);
		// wave
		cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
		cairo_move_to (cr, 20.0, 84.0 - losc -> wave [0] * 64.0);
		for (int ind = 1; ind < 256; ind++) cairo_line_to (cr, 20.0 + (double) ind, 84.0 - losc -> wave [ind] * 64.0);
		cairo_stroke (cr);
		if (osc -> need_to_calculate_markings) {
			for (int ind = 1; ind < 5; ind++) {
				sprintf (osc -> markings [ind], "%iHz", (int) (4.0 * losc -> core -> sampling_frequency / (256.0 * (double) ind)));
				cairo_text_extents_t extent;
				cairo_text_extents (cr, osc -> markings [ind], & extent);
				osc -> marking_locations [ind] = (int) (20.0 + 64.0 * (double) ind - extent . width * 0.5);
			}
			osc -> need_to_calculate_markings = false;
		}
		cairo_set_font_size (cr, 10);
		cairo_set_source_rgb (cr, 1.0, 1.0, 0.0);
		for (int ind = 1; ind < 5; ind++) {
			cairo_move_to (cr, osc -> marking_locations [ind], 160.0);
			cairo_show_text (cr, osc -> markings [ind]);
		}
		break;
	case oscilloscope_class :: SPECTROSCOPE:
		// grid
		cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 276.0, 20.0);
		cairo_move_to (cr, 20.0, 52.0); cairo_line_to (cr, 276.0, 52.0);
		cairo_move_to (cr, 20.0, 116.0); cairo_line_to (cr, 276.0, 116.0);
		cairo_move_to (cr, 20.0, 148.0); cairo_line_to (cr, 276.0, 148.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 20.0, 148.0);
		cairo_move_to (cr, 52.0, 20.0); cairo_line_to (cr, 52.0, 148.0);
		cairo_move_to (cr, 84.0, 20.0); cairo_line_to (cr, 84.0, 148.0);
		cairo_move_to (cr, 116.0, 20.0); cairo_line_to (cr, 116.0, 148.0);
		cairo_move_to (cr, 180.0, 20.0); cairo_line_to (cr, 180.0, 148.0);
		cairo_move_to (cr, 212.0, 20.0); cairo_line_to (cr, 212.0, 148.0);
		cairo_move_to (cr, 244.0, 20.0); cairo_line_to (cr, 244.0, 148.0);
		cairo_move_to (cr, 276.0, 20.0); cairo_line_to (cr, 276.0, 148.0);
		cairo_stroke (cr);
		cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
		cairo_move_to (cr, 20.0, 84.0);
		cairo_line_to (cr, 276.0, 84.0);
		cairo_move_to (cr, 148.0, 20.0);
		cairo_line_to (cr, 148.0, 148.0);
		cairo_stroke (cr);
		cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
		for (int ind = 0; ind < 256; ind++) {
			cairo_move_to (cr, 20.0 + (double) ind, 148.0);
			cairo_line_to (cr, 20.0 + (double) ind, 148.0 - losc -> fft [ind] * 2.0);
		}
		cairo_stroke (cr);
		if (osc -> need_to_calculate_markings) {
			double cycle = losc -> core -> sampling_frequency / (512.0 * losc -> base);
			for (int ind = 0; ind < 5; ind++) {
				sprintf (osc -> markings [ind], "%iHz", (int) (ind == 0 ? cycle : (double) (ind * 64) * cycle));
				cairo_text_extents_t extent;
				cairo_text_extents (cr, osc -> markings [ind], & extent);
				osc -> marking_locations [ind] = (int) (20.0 + 64.0 * (double) ind - extent . width * 0.5);
			}
			osc -> need_to_calculate_markings = false;
		}
		cairo_set_font_size (cr, 10);
		cairo_set_source_rgb (cr, 1.0, 1.0, 0.0);
		for (int ind = 0; ind < 5; ind++) {
			cairo_move_to (cr, osc -> marking_locations [ind], 160.0);
			cairo_show_text (cr, osc -> markings [ind]);
		}
		break;
	case oscilloscope_class :: BIG_OSCILLOSCOPE:
		// grid
		cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 532.0, 20.0);
		cairo_move_to (cr, 20.0, 52.0); cairo_line_to (cr, 532.0, 52.0);
		cairo_move_to (cr, 20.0, 84.0); cairo_line_to (cr, 532.0, 84.0);
		cairo_move_to (cr, 20.0, 116.0); cairo_line_to (cr, 532.0, 116.0);
		cairo_move_to (cr, 20.0, 180.0); cairo_line_to (cr, 532.0, 180.0);
		cairo_move_to (cr, 20.0, 212.0); cairo_line_to (cr, 532.0, 212.0);
		cairo_move_to (cr, 20.0, 244.0); cairo_line_to (cr, 532.0, 244.0);
		cairo_move_to (cr, 20.0, 276.0); cairo_line_to (cr, 532.0, 276.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 20.0, 276.0);
		cairo_move_to (cr, 52.0, 20.0); cairo_line_to (cr, 52.0, 276.0);
		cairo_move_to (cr, 84.0, 20.0); cairo_line_to (cr, 84.0, 276.0);
		cairo_move_to (cr, 116.0, 20.0); cairo_line_to (cr, 116.0, 276.0);
		cairo_move_to (cr, 148.0, 20.0); cairo_line_to (cr, 148.0, 276.0);
		cairo_move_to (cr, 180.0, 20.0); cairo_line_to (cr, 180.0, 276.0);
		cairo_move_to (cr, 212.0, 20.0); cairo_line_to (cr, 212.0, 276.0);
		cairo_move_to (cr, 244.0, 20.0); cairo_line_to (cr, 244.0, 276.0);
		cairo_move_to (cr, 308.0, 20.0); cairo_line_to (cr, 308.0, 276.0);
		cairo_move_to (cr, 340.0, 20.0); cairo_line_to (cr, 340.0, 276.0);
		cairo_move_to (cr, 372.0, 20.0); cairo_line_to (cr, 372.0, 276.0);
		cairo_move_to (cr, 404.0, 20.0); cairo_line_to (cr, 404.0, 276.0);
		cairo_move_to (cr, 436.0, 20.0); cairo_line_to (cr, 436.0, 276.0);
		cairo_move_to (cr, 468.0, 20.0); cairo_line_to (cr, 468.0, 276.0);
		cairo_move_to (cr, 500.0, 20.0); cairo_line_to (cr, 500.0, 276.0);
		cairo_move_to (cr, 532.0, 20.0); cairo_line_to (cr, 532.0, 276.0);
		cairo_stroke (cr);
		cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
		cairo_move_to (cr, 20.0, 148.0); cairo_line_to (cr, 532.0, 148.0);
		cairo_move_to (cr, 276.0, 20.0); cairo_line_to (cr, 276.0, 276.0);
		cairo_stroke (cr);
		// wave
		cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
		cairo_move_to (cr, 20.0, 148.0 - losc -> wave [0] * 64.0);
		for (int ind = 1; ind < 512; ind++) cairo_line_to (cr, 20.0 + (double) ind, 148.0 - losc -> wave [ind] * 64.0);
		cairo_stroke (cr);
		if (osc -> need_to_calculate_markings) {
			for (int ind = 1; ind < 9; ind++) {
				sprintf (osc -> markings [ind], "%iHz", (int) (4.0 * losc -> core -> sampling_frequency / (256.0 * (double) ind)));
				cairo_text_extents_t extent;
				cairo_text_extents (cr, osc -> markings [ind], & extent);
				osc -> marking_locations [ind] = (int) (20.0 + 64.0 * (double) ind - extent . width * 0.5);
			}
			osc -> need_to_calculate_markings = false;
		}
		cairo_set_font_size (cr, 10);
		cairo_set_source_rgb (cr, 1.0, 1.0, 0.0);
		for (int ind = 1; ind < 9; ind++) {
			cairo_move_to (cr, osc -> marking_locations [ind], 288.0);
			cairo_show_text (cr, osc -> markings [ind]);
		}
		break;
	case oscilloscope_class :: BIG_SPECTROSCOPE:
		// grid
		cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 532.0, 20.0);
		cairo_move_to (cr, 20.0, 52.0); cairo_line_to (cr, 532.0, 52.0);
		cairo_move_to (cr, 20.0, 84.0); cairo_line_to (cr, 532.0, 84.0);
		cairo_move_to (cr, 20.0, 116.0); cairo_line_to (cr, 532.0, 116.0);
		cairo_move_to (cr, 20.0, 180.0); cairo_line_to (cr, 532.0, 180.0);
		cairo_move_to (cr, 20.0, 212.0); cairo_line_to (cr, 532.0, 212.0);
		cairo_move_to (cr, 20.0, 244.0); cairo_line_to (cr, 532.0, 244.0);
		cairo_move_to (cr, 20.0, 276.0); cairo_line_to (cr, 532.0, 276.0);
		cairo_move_to (cr, 20.0, 20.0); cairo_line_to (cr, 20.0, 276.0);
		cairo_move_to (cr, 52.0, 20.0); cairo_line_to (cr, 52.0, 276.0);
		cairo_move_to (cr, 84.0, 20.0); cairo_line_to (cr, 84.0, 276.0);
		cairo_move_to (cr, 116.0, 20.0); cairo_line_to (cr, 116.0, 276.0);
		cairo_move_to (cr, 148.0, 20.0); cairo_line_to (cr, 148.0, 276.0);
		cairo_move_to (cr, 180.0, 20.0); cairo_line_to (cr, 180.0, 276.0);
		cairo_move_to (cr, 212.0, 20.0); cairo_line_to (cr, 212.0, 276.0);
		cairo_move_to (cr, 244.0, 20.0); cairo_line_to (cr, 244.0, 276.0);
		cairo_move_to (cr, 308.0, 20.0); cairo_line_to (cr, 308.0, 276.0);
		cairo_move_to (cr, 340.0, 20.0); cairo_line_to (cr, 340.0, 276.0);
		cairo_move_to (cr, 372.0, 20.0); cairo_line_to (cr, 372.0, 276.0);
		cairo_move_to (cr, 404.0, 20.0); cairo_line_to (cr, 404.0, 276.0);
		cairo_move_to (cr, 436.0, 20.0); cairo_line_to (cr, 436.0, 276.0);
		cairo_move_to (cr, 468.0, 20.0); cairo_line_to (cr, 468.0, 276.0);
		cairo_move_to (cr, 500.0, 20.0); cairo_line_to (cr, 500.0, 276.0);
		cairo_move_to (cr, 532.0, 20.0); cairo_line_to (cr, 532.0, 276.0);
		cairo_stroke (cr);
		cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
		cairo_move_to (cr, 20.0, 148.0); cairo_line_to (cr, 532.0, 148.0);
		cairo_move_to (cr, 276.0, 20.0); cairo_line_to (cr, 276.0, 276.0);
		cairo_stroke (cr);
		cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
		for (int ind = 0; ind < 512; ind++) {
			cairo_move_to (cr, 20.0 + (double) ind, 276.0);
			cairo_line_to (cr, 20.0 + (double) ind, 276.0 - losc -> fft [ind] * 2.0);
		}
		cairo_stroke (cr);
		if (osc -> need_to_calculate_markings) {
			double cycle = losc -> core -> sampling_frequency / (512.0 * losc -> base);
			for (int ind = 0; ind < 9; ind++) {
				sprintf (osc -> markings [ind], "%iHz", (int) (ind == 0 ? cycle : (double) (ind * 32) * cycle));
				cairo_text_extents_t extent;
				cairo_text_extents (cr, osc -> markings [ind], & extent);
				osc -> marking_locations [ind] = (int) (20.0 + 64.0 * (double) ind - extent . width * 0.5);
			}
			osc -> need_to_calculate_markings = false;
		}
		cairo_set_font_size (cr, 10);
		cairo_set_source_rgb (cr, 1.0, 1.0, 0.0);
		for (int ind = 0; ind < 9; ind++) {
			cairo_move_to (cr, osc -> marking_locations [ind], 288.0);
			cairo_show_text (cr, osc -> markings [ind]);
		}
		break;
	default: break;
	}
	cairo_destroy (cr);
	return FALSE;
}

static gboolean CreateOscilloscopeIdleCode (oscilloscope_action * osc) {
	lunar_oscilloscope * lo = ((lunar_oscilloscope *) (osc -> module));
	lo -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (lo -> viewport), osc -> atom -> name ());
	g_signal_connect (lo -> viewport, "delete-event", G_CALLBACK (DeleteOscilloscopeEvent), osc);
	osc -> drawing_area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (lo -> viewport), osc -> drawing_area);
	osc -> gtk_redrawer = g_signal_connect (G_OBJECT (osc -> drawing_area), "expose-event", G_CALLBACK (RedrawOscilloscope), osc);
	gtk_window_move (GTK_WINDOW (lo -> viewport), (int) lo -> location . x, (int) lo -> location . y);
	if (osc -> type < oscilloscope_class :: BIG_OSCILLOSCOPE) gtk_window_resize (GTK_WINDOW (lo -> viewport), 302, 174);
	else gtk_window_resize (GTK_WINDOW (lo -> viewport), 552, 296);
	gtk_widget_show_all (lo -> viewport);
	return FALSE;
}

orbiter * oscilloscope_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * base = 0;
	PrologElement * shift = 0;
	PrologElement * refresh = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isInteger ()) {if (base == 0) base = el; else if (shift == 0) shift = el; else refresh = el;}
		parameters = parameters -> getRight ();
	}
	int refresh_rate = 2000;
	if (refresh != 0) refresh_rate = refresh -> getInteger ();
	else {
		switch (type) {
		case oscilloscope_class :: OSCILLOSCOPE: refresh_rate = 2000; break;
		case oscilloscope_class :: SPECTROSCOPE: refresh_rate = 20000; break;
		case oscilloscope_class :: BIG_OSCILLOSCOPE: refresh_rate = 2256; break;
		case oscilloscope_class :: BIG_SPECTROSCOPE: refresh_rate = 20512; break;
		default: break;
		}
	}
	return new lunar_oscilloscope (core, type, base != 0 ? base -> getInteger () : 1, shift != 0 ? shift -> getInteger () : 0, refresh_rate);
}
PrologNativeOrbiter * oscilloscope_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new oscilloscope_action (atom, core, module, type);}
void oscilloscope_class :: code_created (PrologNativeOrbiter * machine) {g_idle_add ((GSourceFunc) CreateOscilloscopeIdleCode, machine);}
oscilloscope_class :: oscilloscope_class (orbiter_core * core, types type) : PrologNativeOrbiterCreator (core) {this -> type = type;}

