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

#ifndef _LUNAR_GRAPHIC_RESOURCES_
#define _LUNAR_GRAPHIC_RESOURCES_

#include "graphics2d.h"
#include "keyboard_calculator.h"
#include "gtk/gtk.h"

extern gboolean RemoveViewportIdleCode (GtkWidget * viewport);

class GraphicResources {
public:
	cairo_surface_t * vector_surface;
	cairo_surface_t * vector_handle;
	cairo_surface_t * small_keyboard_surface;
	cairo_surface_t * keyboard_surface;
	cairo_surface_t * big_keyboard_surface;
	cairo_surface_t * knob;
	cairo_surface_t * knob_surface;
	cairo_surface_t * knob_handle;
	cairo_surface_t * display_surface;
	cairo_surface_t * button_surface_off;
	cairo_surface_t * button_surface_on;
	cairo_surface_t * encoder_surface;
	cairo_surface_t * encoder_handle;
	cairo_surface_t * slider_surface;
	cairo_surface_t * slider_handle;
	cairo_surface_t * command_centre;
	cairo_surface_t * adsr_panel_surface;
	cairo_surface_t * eg_panel_surface;
	cairo_surface_t * fm4_panel_surface;
	cairo_surface_t * core_panel_surface;
	cairo_surface_t * lfo_panel_surface;
	cairo_surface_t * lfo_vibrato_surface;
	cairo_surface_t * lfo_tremolo_surface;
	cairo_surface_t * lfo_wah_wah_surface;
	cairo_surface_t * lfo_pan_surface;
	cairo_surface_t * filter_panel_surface;
	cairo_surface_t * formant_filter_panel_surface;
	cairo_surface_t * delay_panel_surface;
	cairo_surface_t * chorus_panel_surface;
	cairo_surface_t * stereo_chorus_panel_surface;
	cairo_surface_t * freeverb_panel_surface;
	GraphicResources (void);
	~ GraphicResources (void);
};

class active_graphics {
public:
	int id;
	rect location;
	bool on;
	virtual bool keyon (point position);
	virtual bool keyoff (point position);
	virtual bool move (point delta);
	virtual void draw (cairo_t * cr);
	active_graphics (point location, int id);
};

class knob_active_graphics : public active_graphics {
public:
	double angle, value, range, start;
	cairo_surface_t * knob_surface_png;
	cairo_surface_t * knob_png;
	cairo_surface_t * knob_handle_png;
	bool no_display;
	void setValue (double v);
	bool move (point delta);
	void draw (cairo_t * cr);
	knob_active_graphics (point location, int id, GraphicResources * resources,
							bool active_display = false, bool active_surface = false, double from = -64.0, double to = 64.0);
};

class encoder_active_graphics : public active_graphics {
public:
	double angle;
	double increment;
	cairo_surface_t * encoder_surface_png;
	cairo_surface_t * encoder_handle_png;
	bool move (point delta);
	void draw (cairo_t * cr);
	encoder_active_graphics (point location, int id, GraphicResources * resource, bool active_surface = false);
};

class slider_active_graphics : public active_graphics {
public:
	double position;
	bool spring_loaded;
	cairo_surface_t * slider_surface_png;
	cairo_surface_t * slider_handle_png;
	bool keyoff (point position);
	bool move (point delta);
	void draw (cairo_t * cr);
	slider_active_graphics (point location, int id, bool spring_loaded, GraphicResources * resource, bool active_surface = false);
};

class vector_active_graphics : public active_graphics {
public:
	point position;
	point precision;
	cairo_surface_t * surface;
	cairo_surface_t * handle;
	bool move (point delta);
	void draw (cairo_t * cr);
	vector_active_graphics (point location, int id, GraphicResources * resources, double precision = 1.0, bool active_surface = false);
};

class keyboard_active_graphics : public active_graphics {
public:
	cairo_surface_t * surface;
	keyboard_calculator calculator;
	int key;
	bool keyon (point position);
	bool keyoff (point position);
	void draw (cairo_t * cr);
	keyboard_active_graphics (point location, int type, int id, GraphicResources * resources, bool active_surface = false);
};

class display_active_graphics : public active_graphics {
public:
	cairo_surface_t * surface;
	char area [1024];
	void draw (cairo_t * cr);
	display_active_graphics (point location, int id, GraphicResources * resources, bool active_surface = false);
};

class button_active_graphics : public active_graphics {
public:
	cairo_surface_t * surface_off;
	cairo_surface_t * surface_on;
	bool engaged;
	void draw (cairo_t *cr);
	button_active_graphics (point location, int id, GraphicResources * resource, bool active_surface = false);
};

#endif

