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

#include "graphic_resources.h"
#define _USE_MATH_DEFINES
#include <math.h>

gboolean RemoveViewportIdleCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}

#ifdef WIN32
#include "lunar_resource.h"
#include <Windows.h>
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
extern char resource_small_keyboard_start;
extern char resource_small_keyboard_end;
extern char resource_keyboard_start;
extern char resource_keyboard_end;
extern char resource_big_keyboard_start;
extern char resource_big_keyboard_end;
extern char resource_knob_start;
extern char resource_knob_end;
extern char resource_knob_surface_start;
extern char resource_knob_surface_end;
extern char resource_knob_handle_start;
extern char resource_knob_handle_end;
#endif

class png_closure {
public:
	char * from;
	char * to;
#ifdef WIN32
	png_closure (char * from, int size) {this -> from = from; this -> to = from == 0 ? from : from + size;}
#else
	png_closure (char * from, char * to) {this -> from = from; this -> to = from == 0 ? from : to;}
#endif
};

static cairo_status_t png_reader (void * closure, unsigned char * data, unsigned int length) {
	png_closure * png_data = (png_closure *) closure;
	if (png_data -> from >= png_data -> to) return CAIRO_STATUS_READ_ERROR;
	while (length-- > 0 && png_data -> from < png_data -> to) {
		* data++ = * png_data -> from++;
	}
	return CAIRO_STATUS_SUCCESS;
}

GraphicResources :: GraphicResources (void) {
#ifdef WIN32
	png_closure frame_closure (GetResource (VECTOR_FRAME_PNG), VECTOR_FRAME_SIZE);
	png_closure handle_closure (GetResource (VECTOR_HANDLE_PNG), VECTOR_HANDLE_SIZE);
	png_closure small_keyboard_closure (GetResource (SMALL_KEYBOARD_PNG), SMALL_KEYBOARD_SIZE);
	png_closure keyboard_closure (GetResource (KEYBOARD_PNG), KEYBOARD_SIZE);
	png_closure big_keyboard_closure (GetResource (BIG_KEYBOARD_PNG), BIG_KEYBOARD_SIZE);
	png_closure knob_closure (GetResource (KNOB_PNG), KNOB_SIZE);
	png_closure knob_surface_closure (GetResource (KNOB_SURFACE_PNG), KNOB_SURFACE_SIZE);
	png_closure knob_handle_closure (GetResource (KNOB_HANDLE_PNG), KNOB_HANDLE_SIZE);
#else
	png_closure frame_closure (& resource_vector_frame_start, & resource_vector_frame_end);
	png_closure handle_closure (& resource_vector_handle_start, & resource_vector_handle_end);
	png_closure small_keyboard_closure (& resource_small_keyboard_start, & resource_small_keyboard_end);
	png_closure keyboard_closure (& resource_keyboard_start, & resource_keyboard_end);
	png_closure big_keyboard_closure (& resource_big_keyboard_start, & resource_big_keyboard_end);
	png_closure knob_closure (& resource_knob_start, & resource_knob_end);
	png_closure knob_surface_closure (& resource_knob_surface_start, & resource_knob_surface_end);
	png_closure knob_handle_closure (& resource_knob_handle_start, & resource_knob_handle_end);
#endif
	vector_surface = cairo_image_surface_create_from_png_stream (png_reader, & frame_closure);
	vector_handle = cairo_image_surface_create_from_png_stream (png_reader, & handle_closure);
	small_keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & small_keyboard_closure);
	keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & keyboard_closure);
	big_keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & big_keyboard_closure);
	knob = cairo_image_surface_create_from_png_stream (png_reader, & knob_closure);
	knob_surface = cairo_image_surface_create_from_png_stream (png_reader, & knob_surface_closure);
	knob_handle = cairo_image_surface_create_from_png_stream (png_reader, & knob_handle_closure);
}

GraphicResources :: ~ GraphicResources (void) {
	if (vector_surface != 0) cairo_surface_destroy (vector_surface);
	if (vector_handle != 0) cairo_surface_destroy (vector_handle);
	if (small_keyboard_surface != 0) cairo_surface_destroy (small_keyboard_surface);
	if (keyboard_surface != 0) cairo_surface_destroy (keyboard_surface);
	if (big_keyboard_surface != 0) cairo_surface_destroy (big_keyboard_surface);
	if (knob != 0) cairo_surface_destroy (knob);
	if (knob_surface != 0) cairo_surface_destroy (knob_surface);
	if (knob_handle != 0) cairo_surface_destroy (knob_handle);
}

GraphicResources * create_graphic_resources (void) {return new GraphicResources ();}
void destroy_graphic_resources (GraphicResources * resource) {if (resource != 0) delete resource;}

bool active_graphics :: keyon (point position, GtkWidget * viewport) {
	if (! location . overlap (rect (position, point ()))) return false;
	on = true;
	return true;
}
bool active_graphics :: keyoff (point position, GtkWidget * viewport) {on = false; return location . overlap (rect (position, point ()));}
bool active_graphics :: move (point delta, GtkWidget * viewport) {return on;}
void active_graphics :: draw (cairo_t * cr) {}
active_graphics :: active_graphics (point location, int id) {on = false; this -> id = id; this -> location = rect (location, point (16.0, 16.0));}

bool knob_active_graphics :: move (point delta, GtkWidget * viewport) {
	if (! on) return false;
	angle += 0.1 * delta . y;
	gtk_widget_queue_draw (viewport);
	return true;
}

void knob_active_graphics :: draw (cairo_t * cr) {
		if (knob_surface_png != 0) {
			cairo_set_source_surface (cr, knob_surface_png, location . position . x, location . position . y);
			cairo_paint (cr);
		}
		if (knob_png != 0) {
			point position = location . position + point (9, 9);
			cairo_set_source_surface (cr, knob_png, position . x, position . y);
			cairo_paint (cr);
		}
		if (knob_handle_png != 0) {
			point position = location . position + point (30, 30);
			position += point (sin (angle), cos (angle)) * 20.0;
			cairo_set_source_surface (cr, knob_handle_png, position . x, position . y);
			cairo_paint (cr);
		}
}


knob_active_graphics :: knob_active_graphics (point location, int id, GraphicResources * resources) : active_graphics (location, id) {
	knob_surface_png = knob_png = knob_handle_png = 0;
	if (resources != 0) {
		knob_surface_png = resources -> knob_surface;
		knob_png = resources -> knob;
		knob_handle_png = resources -> knob_handle;
	}
	angle = 0.0;
	this -> location = rect (location, point (62, 83));
}

bool vector_active_graphics :: move (point delta, GtkWidget * viewport) {
	if (! on) return false;
	position += delta * point (-0.00390625, 0.00390625);
	if (position . x < -1.0) position . x = -1.0; if (position . x > 1.0) position . x = 1.0;
	if (position . y < -1.0) position . y = -1.0; if (position . y > 1.0) position . y = 1.0;
	gtk_widget_queue_draw (viewport);
	return true;
}

void vector_active_graphics :: draw (cairo_t * cr) {
	if (surface != 0) {
		cairo_set_source_surface (cr, surface, location . position . x, location . position . y);
		cairo_paint (cr);
	}
	if (handle != 0) {
		cairo_surface_t * sub = cairo_surface_create_for_rectangle (handle, position . x * -64.0 + 64.0, position . y * 64.0 + 64.0, 129.0, 129.0);
		point p = location . position + point (23.0, 24.0);
		cairo_set_source_surface (cr, sub, p . x, p . y);
		cairo_paint (cr);
		cairo_surface_destroy (sub);
	}
}

vector_active_graphics :: vector_active_graphics (point location, int id, GraphicResources * resources) : active_graphics (location, id) {
	surface = handle = 0;
	if (resources == 0) return;
	surface = resources -> vector_surface;
	handle = resources -> vector_handle;
	this -> location . size = point (cairo_image_surface_get_width (surface), cairo_image_surface_get_height (surface));
}