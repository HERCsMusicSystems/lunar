
#include "graphic_resources.h"

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
#else
	png_closure frame_closure (& resource_vector_frame_start, & resource_vector_frame_end);
	png_closure handle_closure (& resource_vector_handle_start, & resource_vector_handle_end);
	png_closure small_keyboard_closure (& resource_small_keyboard_start, & resource_small_keyboard_end);
	png_closure keyboard_closure (& resource_keyboard_start, & resource_keyboard_end);
	png_closure big_keyboard_closure (& resource_big_keyboard_start, & resource_big_keyboard_end);
#endif
	vector_surface = cairo_image_surface_create_from_png_stream (png_reader, & frame_closure);
	vector_handle = cairo_image_surface_create_from_png_stream (png_reader, & handle_closure);
	small_keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & small_keyboard_closure);
	keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & keyboard_closure);
	big_keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & big_keyboard_closure);
}

GraphicResources :: ~ GraphicResources (void) {
	if (vector_surface != 0) cairo_surface_destroy (vector_surface);
	if (vector_handle != 0) cairo_surface_destroy (vector_handle);
	if (small_keyboard_surface != 0) cairo_surface_destroy (small_keyboard_surface);
	if (keyboard_surface != 0) cairo_surface_destroy (keyboard_surface);
	if (big_keyboard_surface != 0) cairo_surface_destroy (big_keyboard_surface);
}

GraphicResources * create_graphic_resources (void) {return new GraphicResources ();}
void destroy_graphic_resources (GraphicResources * resource) {if (resource != 0) delete resource;}


