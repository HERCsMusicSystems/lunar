
#ifndef _LUNAR_GRAPHIC_RESOURCES_
#define _LUNAR_GRAPHIC_RESOURCES_

#include "gtk/gtk.h"

class GraphicResources {
public:
	cairo_surface_t * vector_surface;
	cairo_surface_t * vector_handle;
	cairo_surface_t * small_keyboard_surface;
	cairo_surface_t * keyboard_surface;
	cairo_surface_t * big_keyboard_surface;
	GraphicResources (void);
	~ GraphicResources (void);
};

#endif

