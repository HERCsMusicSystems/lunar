
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
	GraphicResources (void);
	~ GraphicResources (void);
};

class knob {
public:
	int id;
	cairo_surface_t * knob_surface_png;
	cairo_surface_t * knob_png;
	cairo_surface_t * knob_handle_png;
	rect location;
	double angle;
	bool on;
	bool keyon (point position, GtkWidget * viewport);
	bool keyoff (point position, GtkWidget * viewport);
	bool move (point delta, GtkWidget * viewport);
	void draw (cairo_t * cr);
	knob (point location, GraphicResources * resources, int id);
};

#endif

