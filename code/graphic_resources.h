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
	double angle;
	cairo_surface_t * knob_surface_png;
	cairo_surface_t * knob_png;
	cairo_surface_t * knob_handle_png;
	bool move (point delta);
	void draw (cairo_t * cr);
	knob_active_graphics (point location, int id, GraphicResources * resources, bool active_surface = false);
};

class vector_active_graphics : public active_graphics {
public:
	point position;
	cairo_surface_t * surface;
	cairo_surface_t * handle;
	bool move (point delta);
	void draw (cairo_t * cr);
	vector_active_graphics (point location, int id, GraphicResources * resources, bool active_surface = false);
};

class keyboard_active_graphics : public active_graphics {
public:
	point position;
	cairo_surface_t * surface;
	void draw (cairo_t * cr);
	keyboard_active_graphics (point location, int type, int id, GraphicResources * resources, bool active_surface = false);
};

class display_active_graphics : public active_graphics {
public:
	point position;
	cairo_surface_t * surface;
	char area [1024];
	void draw (cairo_t * cr);
	display_active_graphics (point location, int id, GraphicResources * resources, bool active_surface = false);
};

#endif

