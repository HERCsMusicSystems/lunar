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

#ifdef WIN32
#include "lunar_resource.h"
#include <Windows.h>
static char * GetResource (int ind) {
	HMODULE hm = GetModuleHandle ("lunar.dll");
	HRSRC resource = FindResource (hm, MAKEINTRESOURCE (ind), RT_RCDATA);
	if (! resource) return 0;
	HGLOBAL loader = LoadResource (hm, resource);
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
extern char resource_display_start;
extern char resource_display_end;
extern char resource_button_on_start;
extern char resource_button_on_end;
extern char resource_button_off_start;
extern char resource_button_off_end;
extern char resource_encoder_surface_start;
extern char resource_encoder_surface_end;
extern char resource_encoder_handle_start;
extern char resource_encoder_handle_end;
extern char resource_slider_surface_start;
extern char resource_slider_surface_end;
extern char resource_slider_handle_start;
extern char resource_slider_handle_end;
extern char resource_command_centre_start;
extern char resource_command_centre_end;
extern char resource_adsr_panel_start;
extern char resource_adsr_panel_end;
extern char resource_eg_panel_start;
extern char resource_eg_panel_end;
extern char resource_fm4_panel_start;
extern char resource_fm4_panel_end;
extern char resource_core_panel_start;
extern char resource_core_panel_end;
extern char resource_lfo_panel_start;
extern char resource_lfo_panel_end;
extern char resource_lfo_vibrato_start;
extern char resource_lfo_vibrato_end;
extern char resource_lfo_tremolo_start;
extern char resource_lfo_tremolo_end;
extern char resource_lfo_wah_wah_start;
extern char resource_lfo_wah_wah_end;
extern char resource_lfo_pan_start;
extern char resource_lfo_pan_end;
extern char resource_filter_panel_start;
extern char resource_filter_panel_end;
extern char resource_formant_filter_panel_start;
extern char resource_formant_filter_panel_end;
extern char resource_delay_panel_start;
extern char resource_delay_panel_end;
extern char resource_chorus_panel_start;
extern char resource_chorus_panel_end;
extern char resource_stereo_chorus_panel_start;
extern char resource_stereo_chorus_panel_end;
extern char resource_freeverb_panel_start;
extern char resource_freeverb_panel_end;
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
	png_closure display_closure (GetResource (DISPLAY_PNG), DISPLAY_SIZE);
	png_closure button_on_closure (GetResource (BUTTON_ON_PNG), BUTTON_ON_SIZE);
	png_closure button_off_closure (GetResource (BUTTON_OFF_PNG), BUTTON_OFF_SIZE);
	png_closure encoder_surface_closure (GetResource (ENCODER_SURFACE_PNG), ENCODER_SURFACE_SIZE);
	png_closure encoder_handle_closure (GetResource (ENCODER_HANDLE_PNG), ENCODER_HANDLE_SIZE);
	png_closure slider_surface_closure (GetResource (SLIDER_SURFACE_PNG), SLIDER_SURFACE_SIZE);
	png_closure slider_handle_closure (GetResource (SLIDER_HANDLE_PNG), SLIDER_HANDLE_SIZE);
	png_closure command_centre_closure (GetResource (COMMAND_CENTRE_PNG), COMMAND_CENTRE_SIZE);
	png_closure adsr_panel_surface_closure (GetResource (ADSR_PANEL_PNG), ADSR_PANEL_SIZE);
	png_closure eg_panel_surface_closure (GetResource (EG_PANEL_PNG), EG_PANEL_SIZE);
	png_closure fm4_panel_surface_closure (GetResource (FM4_PANEL_PNG), FM4_PANEL_SIZE);
	png_closure core_panel_surface_closure (GetResource (CORE_PANEL_PNG), CORE_PANEL_SIZE);
	png_closure lfo_panel_surface_closure (GetResource (LFO_PANEL_PNG), LFO_PANEL_SIZE);
	png_closure lfo_vibrato_surface_closure (GetResource (LFO_VIBRATO_PNG), LFO_VIBRATO_SIZE);
	png_closure lfo_tremolo_surface_closure (GetResource (LFO_TREMOLO_PNG), LFO_TREMOLO_SIZE);
	png_closure lfo_wah_wah_surface_closure (GetResource (LFO_WAH_WAH_PNG), LFO_WAH_WAH_SIZE);
	png_closure lfo_pan_surface_closure (GetResource (LFO_PAN_PNG), LFO_PAN_SIZE);
	png_closure filter_panel_surface_closure (GetResource (FILTER_PANEL_PNG), FILTER_PANEL_SIZE);
	png_closure formant_filter_panel_surface_closure (GetResource (FORMANT_FILTER_PANEL_PNG), FORMANT_FILTER_PANEL_SIZE);
	png_closure delay_panel_surface_closure (GetResource (DELAY_PANEL_PNG), DELAY_PANEL_SIZE);
	png_closure chorus_panel_surface_closure (GetResource (CHORUS_PANEL_PNG), CHORUS_PANEL_SIZE);
	png_closure stereo_chorus_panel_surface_closure (GetResource (STEREO_CHORUS_PANEL_PNG), STEREO_CHORUS_PANEL_SIZE);
	png_closure freeverb_panel_surface_closure (GetResource (FREEVERB_PANEL_PNG), FREEVERB_PANEL_SIZE);
#else
	png_closure frame_closure (& resource_vector_frame_start, & resource_vector_frame_end);
	png_closure handle_closure (& resource_vector_handle_start, & resource_vector_handle_end);
	png_closure small_keyboard_closure (& resource_small_keyboard_start, & resource_small_keyboard_end);
	png_closure keyboard_closure (& resource_keyboard_start, & resource_keyboard_end);
	png_closure big_keyboard_closure (& resource_big_keyboard_start, & resource_big_keyboard_end);
	png_closure knob_closure (& resource_knob_start, & resource_knob_end);
	png_closure knob_surface_closure (& resource_knob_surface_start, & resource_knob_surface_end);
	png_closure knob_handle_closure (& resource_knob_handle_start, & resource_knob_handle_end);
	png_closure display_closure (& resource_display_start, & resource_display_end);
	png_closure button_on_closure (& resource_button_on_start, & resource_button_on_end);
	png_closure button_off_closure (& resource_button_off_start, & resource_button_off_end);
	png_closure encoder_surface_closure (& resource_encoder_surface_start, & resource_encoder_surface_end);
	png_closure encoder_handle_closure (& resource_encoder_handle_start, & resource_encoder_handle_end);
	png_closure slider_surface_closure (& resource_slider_surface_start, & resource_slider_surface_end);
	png_closure slider_handle_closure (& resource_slider_handle_start, & resource_slider_handle_end);
	png_closure command_centre_closure (& resource_command_centre_start, & resource_command_centre_end);
	png_closure adsr_panel_surface_closure (& resource_adsr_panel_start, & resource_adsr_panel_end);
	png_closure eg_panel_surface_closure (& resource_eg_panel_start, & resource_eg_panel_end);
	png_closure fm4_panel_surface_closure (& resource_fm4_panel_start, & resource_fm4_panel_end);
	png_closure core_panel_surface_closure (& resource_core_panel_start, & resource_core_panel_end);
	png_closure lfo_panel_surface_closure (& resource_lfo_panel_start, & resource_lfo_panel_end);
	png_closure lfo_vibrato_surface_closure (& resource_lfo_vibrato_start, & resource_lfo_vibrato_end);
	png_closure lfo_tremolo_surface_closure (& resource_lfo_tremolo_start, & resource_lfo_tremolo_end);
	png_closure lfo_wah_wah_surface_closure (& resource_lfo_wah_wah_start, & resource_lfo_wah_wah_end);
	png_closure lfo_pan_surface_closure (& resource_lfo_pan_start, & resource_lfo_pan_end);
	png_closure filter_panel_surface_closure (& resource_filter_panel_start, & resource_filter_panel_end);
	png_closure formant_filter_panel_surface_closure (& resource_formant_filter_panel_start, & resource_formant_filter_panel_end);
	png_closure delay_panel_surface_closure (& resource_delay_panel_start, & resource_delay_panel_end);
	png_closure chorus_panel_surface_closure (& resource_chorus_panel_start, & resource_chorus_panel_end);
	png_closure stereo_chorus_panel_surface_closure (& resource_stereo_chorus_panel_start, & resource_stereo_chorus_panel_end);
	png_closure freeverb_panel_surface_closure (& resource_freeverb_panel_start, & resource_freeverb_panel_end);
#endif
	vector_surface = cairo_image_surface_create_from_png_stream (png_reader, & frame_closure);
	vector_handle = cairo_image_surface_create_from_png_stream (png_reader, & handle_closure);
	small_keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & small_keyboard_closure);
	keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & keyboard_closure);
	big_keyboard_surface = cairo_image_surface_create_from_png_stream (png_reader, & big_keyboard_closure);
	knob = cairo_image_surface_create_from_png_stream (png_reader, & knob_closure);
	knob_surface = cairo_image_surface_create_from_png_stream (png_reader, & knob_surface_closure);
	knob_handle = cairo_image_surface_create_from_png_stream (png_reader, & knob_handle_closure);
	display_surface = cairo_image_surface_create_from_png_stream (png_reader, & display_closure);
	button_surface_on = cairo_image_surface_create_from_png_stream (png_reader, & button_on_closure);
	button_surface_off = cairo_image_surface_create_from_png_stream (png_reader, & button_off_closure);
	encoder_surface = cairo_image_surface_create_from_png_stream (png_reader, & encoder_surface_closure);
	encoder_handle = cairo_image_surface_create_from_png_stream (png_reader, & encoder_handle_closure);
	slider_surface = cairo_image_surface_create_from_png_stream (png_reader, & slider_surface_closure);
	slider_handle = cairo_image_surface_create_from_png_stream (png_reader, & slider_handle_closure);
	command_centre = cairo_image_surface_create_from_png_stream (png_reader, & command_centre_closure);
	adsr_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & adsr_panel_surface_closure);
	eg_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & eg_panel_surface_closure);
	fm4_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & fm4_panel_surface_closure);
	core_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & core_panel_surface_closure);
	lfo_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & lfo_panel_surface_closure);
	lfo_vibrato_surface = cairo_image_surface_create_from_png_stream (png_reader, & lfo_vibrato_surface_closure);
	lfo_tremolo_surface = cairo_image_surface_create_from_png_stream (png_reader, & lfo_tremolo_surface_closure);
	lfo_wah_wah_surface = cairo_image_surface_create_from_png_stream (png_reader, & lfo_wah_wah_surface_closure);
	lfo_pan_surface = cairo_image_surface_create_from_png_stream (png_reader, & lfo_pan_surface_closure);
	filter_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & filter_panel_surface_closure);
	formant_filter_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & formant_filter_panel_surface_closure);
	delay_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & delay_panel_surface_closure);
	chorus_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & chorus_panel_surface_closure);
	stereo_chorus_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & stereo_chorus_panel_surface_closure);
	freeverb_panel_surface = cairo_image_surface_create_from_png_stream (png_reader, & freeverb_panel_surface_closure);
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
	if (display_surface != 0) cairo_surface_destroy (display_surface);
	if (button_surface_on != 0) cairo_surface_destroy (button_surface_on);
	if (button_surface_off != 0) cairo_surface_destroy (button_surface_off);
	if (encoder_surface != 0) cairo_surface_destroy (encoder_surface);
	if (encoder_handle != 0) cairo_surface_destroy (encoder_handle);
	if (slider_surface != 0) cairo_surface_destroy (slider_surface);
	if (slider_handle != 0) cairo_surface_destroy (slider_handle);
	if (command_centre != 0) cairo_surface_destroy (command_centre);
	if (adsr_panel_surface != 0) cairo_surface_destroy (adsr_panel_surface);
	if (eg_panel_surface != 0) cairo_surface_destroy (eg_panel_surface);
	if (fm4_panel_surface != 0) cairo_surface_destroy (fm4_panel_surface);
	if (core_panel_surface != 0) cairo_surface_destroy (core_panel_surface);
	if (lfo_panel_surface != 0) cairo_surface_destroy (lfo_panel_surface);
	if (lfo_vibrato_surface != 0) cairo_surface_destroy (lfo_vibrato_surface);
	if (lfo_tremolo_surface != 0) cairo_surface_destroy (lfo_tremolo_surface);
	if (lfo_wah_wah_surface != 0) cairo_surface_destroy (lfo_wah_wah_surface);
	if (lfo_pan_surface != 0) cairo_surface_destroy (lfo_pan_surface);
	if (filter_panel_surface != 0) cairo_surface_destroy (filter_panel_surface);
	if (formant_filter_panel_surface != 0) cairo_surface_destroy (formant_filter_panel_surface);
	if (delay_panel_surface != 0) cairo_surface_destroy (delay_panel_surface);
	if (chorus_panel_surface != 0) cairo_surface_destroy (chorus_panel_surface);
	if (stereo_chorus_panel_surface != 0) cairo_surface_destroy (stereo_chorus_panel_surface);
	if (freeverb_panel_surface != 0) cairo_surface_destroy (freeverb_panel_surface);
}

GraphicResources * create_graphic_resources (void) {return new GraphicResources ();}
void destroy_graphic_resources (GraphicResources * resource) {if (resource != 0) delete resource;}

bool active_graphics :: keyon (point position) {
	if (! location . overlap (rect (position, point ()))) return false;
	on = true;
	return true;
}
bool active_graphics :: keyoff (point position) {on = false; return location . overlap (rect (position, point ()));}
bool active_graphics :: move (point delta) {return on;}
void active_graphics :: draw (cairo_t * cr) {}
active_graphics :: active_graphics (point location, int id) {on = false; this -> id = id; this -> location = rect (location, point (16.0, 16.0));}

void knob_active_graphics :: setValue (double v) {angle = (v - start) / (range != 0 ? range : 1.0);}

bool knob_active_graphics :: move (point delta) {
	if (! on) return false;
	angle -= 0.0078125 * delta . y;
	if (angle < 0.0) angle = 0.0; if (angle > 1.0) angle = 1.0;
	value = (double) ((int) (start + angle * range));
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
		point position = location . position + point (28, 29);
		double alpha = (angle * -1.5 - 0.25) * M_PI;
		position += point (sin (alpha), cos (alpha)) * 12.0;
		cairo_set_source_surface (cr, knob_handle_png, position . x, position . y);
		cairo_paint (cr);
	}
	if (no_display) return;
	char command [16];
	sprintf (command, "%i", (int) (angle * range + start));
	cairo_set_font_size (cr, 10);
	cairo_set_source_rgba (cr, 1.0, 1.0, 0.0, 1.0);
	cairo_text_extents_t extent;
	cairo_text_extents (cr, command, & extent);
	point position = location . position + point (32.0 - extent . width * 0.5, 62.0);
	cairo_move_to (cr, position . x, position . y);
	cairo_show_text (cr, command);
	cairo_identity_matrix (cr);
}


knob_active_graphics :: knob_active_graphics (point location, int id, GraphicResources * resources,
												bool active_display, bool active_surface, double from, double to) : active_graphics (location, id) {
	knob_surface_png = knob_png = knob_handle_png = 0;
	no_display = ! active_display;
	if (resources != 0) {
		knob_surface_png = active_surface ? resources -> knob_surface : 0;
		knob_png = active_surface ? resources -> knob : 0;
		knob_handle_png = resources -> knob_handle;
	}
	angle = 0.0; this -> value = this -> start = from; this -> range = to - from;
	this -> location = rect (location, point (62, 83));
}

bool encoder_active_graphics :: move (point delta) {
	if (! on) return false;
	angle += 0.03125 * delta . y;
	increment = 0.0;
	if (delta . y < 0.0) increment = 1.0;
	if (delta . y > 0.0) increment = -1.0;
	while (angle < 0.0) angle += 1.0;
	while (angle > 1.0) angle -= 1.0;
	return true;
}

void encoder_active_graphics :: draw (cairo_t * cr) {
	if (encoder_surface_png != 0) {
		cairo_set_source_surface (cr, encoder_surface_png, location . position . x, location . position . y);
		cairo_paint (cr);
	}
	if (encoder_handle_png != 0) {
		point position = location . position + point (24.0, 24.0);
		double alpha = angle * 2.0 * M_PI;
		position += point (sin (alpha), cos (alpha)) * 14.0;
		cairo_set_source_surface (cr, encoder_handle_png, position . x, position . y);
		cairo_paint (cr);
	}
}

encoder_active_graphics :: encoder_active_graphics (point location, int id, GraphicResources * resources, bool active_surface) :
		active_graphics (location, id) {
	encoder_surface_png = encoder_handle_png = 0;
	if (resources != 0) {
		encoder_surface_png = active_surface ? resources -> encoder_surface : 0;
		encoder_handle_png = resources -> encoder_handle;
	}
	angle = 0.0;
	increment = 0.0;
	this -> location = rect (location, point (62, 83));
}

bool slider_active_graphics :: keyoff (point location) {
	if (spring_loaded) {position = 0.5; bool oon = on; active_graphics :: keyoff (location); return oon;}
	return active_graphics :: keyoff (location);
}

bool slider_active_graphics :: move (point delta) {
	if (! on) return false;
	position -= delta . y / 128.0;
	if (position < 0.0) position = 0.0; if (position > 1.0) position = 1.0;
	return true;
}

void slider_active_graphics :: draw (cairo_t * cr) {
	if (slider_surface_png != 0) {
		cairo_set_source_surface (cr, slider_surface_png, location . position . x, location . position . y);
		cairo_paint (cr);
	}
	if (slider_handle_png != 0) {
		point position = location . position + point (0.0, 62.0) * (1.0 - this -> position);
		cairo_set_source_surface (cr, slider_handle_png, position . x, position . y);
		cairo_paint (cr);
	}
}

slider_active_graphics :: slider_active_graphics (point location, int id, bool spring_loaded, GraphicResources * resources, bool active_surface) :
		active_graphics (location, id) {
	slider_surface_png = slider_handle_png = 0;
	if (resources != 0) {
		slider_surface_png = active_surface ? resources -> slider_surface : 0;
		slider_handle_png = resources -> slider_handle;
	}
	this -> position = 0.0;
	this -> spring_loaded = spring_loaded;
	this -> location = rect (location, point (8, 70));
}

bool vector_active_graphics :: move (point delta) {
	if (! on) return false;
	position += delta * precision;
	if (position . x < -1.0) position . x = -1.0; if (position . x > 1.0) position . x = 1.0;
	if (position . y < -1.0) position . y = -1.0; if (position . y > 1.0) position . y = 1.0;
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

vector_active_graphics :: vector_active_graphics (point location, int id, GraphicResources * resources, double precision, bool active_surface) :
	active_graphics (location, id) {
	precision *= 0.015625;
	this -> precision = point (precision, - precision);
	surface = handle = 0;
	if (resources == 0) return;
	surface = active_surface ? resources -> vector_surface : 0;
	handle = resources -> vector_handle;
	this -> location . size = point (cairo_image_surface_get_width (resources -> vector_surface), cairo_image_surface_get_height (resources -> vector_surface));
}

bool keyboard_active_graphics :: keyon (point position) {
	key = calculator . get ((int) position . x, (int) position . y);
	return active_graphics :: keyon (position);
}

bool keyboard_active_graphics :: keyoff (point position) {
	key = calculator . get ((int) position . x, (int) position . y);
	return active_graphics :: keyoff (position);
}

void keyboard_active_graphics :: draw (cairo_t * cr) {
	if (surface != 0) {
		cairo_set_source_surface (cr, surface, location . position . x, location . position . y);
		cairo_paint (cr);
	}
}

keyboard_active_graphics :: keyboard_active_graphics (point location, int type, int id, GraphicResources * resources, bool active_surface) :
		active_graphics (location, id), calculator ((int) location . x, (int) location . y) {
	surface = 0;
	key = 0;
	switch (type) {
	case 1:
		if (resources != 0 && active_surface) surface = resources -> keyboard_surface;
		this -> location . size = point (865, 100);
		calculator . set_keyboard_layout_y (99, 66);
		calculator . set_keyboard_layout_x (16, 2, 3, 4, 5, 6);
		calculator . set_ambitus (17, 54);
		break;
	case 2:
		if (resources != 0 && active_surface) surface = resources -> big_keyboard_surface;
		this -> location . size = point (1189, 133);
		calculator . set_keyboard_layout_y (132, 88);
		calculator . set_keyboard_layout_x (22, 2, 4, 6, 8, 10);
		calculator . set_ambitus (17, 54);
		break;
	default:
		if (resources != 0 && active_surface) surface = resources -> small_keyboard_surface;
		this -> location . size = point (595, 67);
		calculator . set_keyboard_layout_y (66, 44);
		calculator . set_keyboard_layout_x (11, 1, 2, 3, 4, 5);
		calculator . set_ambitus (17, 54);
		break;
	}
}

void display_active_graphics :: draw (cairo_t * cr) {
	if (surface != 0) {
		cairo_set_source_surface (cr, surface, location . position . x, location . position . y);
		cairo_paint (cr);
	}
	point text_position = location . position + point (26.0, 47.0);
	cairo_move_to (cr, text_position . x, text_position . y);
	cairo_set_font_size (cr, 22);
	cairo_set_source_rgba (cr, 1.0, 1.0, 0.0, 1.0);
	cairo_show_text (cr, area);
	cairo_identity_matrix (cr);
}

display_active_graphics :: display_active_graphics (point location, int id, GraphicResources * resource, bool active_surface) : active_graphics (location, id) {
	surface = 0;
	* area = '\0';
	if (resource == 0) return;
	surface = active_surface ? resource -> display_surface : 0;
}

void button_active_graphics :: draw (cairo_t * cr) {
	if (engaged && surface_on != 0) {
		cairo_set_source_surface (cr, surface_on, location . position . x, location . position . y);
		cairo_paint (cr);
		return;
	}
	if (surface_off != 0) {
		cairo_set_source_surface (cr, surface_off, location . position . x, location . position . y);
		cairo_paint (cr);
	}
}
button_active_graphics :: button_active_graphics (point location, int id, GraphicResources * resource, bool active_surface) : active_graphics (location, id) {
	engaged = false;
	surface_on = surface_off = 0;
	if (resource == 0) return;
	surface_on = resource -> button_surface_on;
	surface_off = active_surface ? resource -> button_surface_off : 0;
}

