///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
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
// This file was created on Wednesday, 28th April 2015 at 18:37:13. //
//////////////////////////////////////////////////////////////////////

#ifndef _LUNAR_PROLOG_PANEL_BASE_
#define _LUNAR_PROLOG_PANEL_BASE_

#include "prolog_lunar.h"
#include "graphic_resources.h"

class AudioModulePanel : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologAtom * atom;
	point location;
	point captured_location;
	int captured_button;
	cairo_surface_t * background_image;
	GtkWidget * viewport;
	GtkWidget * area;
	int gtk_redrawer;
	bool not_ready_for_drop;
	bool remove (bool remove_gtk = true);
	void update (void);
	void reposition (void);
	bool code (PrologElement * parameter, PrologResolution * resolution);
	void BuildPanel (void);
	virtual void redraw (cairo_t * cr) = 0;
	virtual void MouseKeyon (point location, int button) = 0;
	virtual void MouseKeyoff (point location, int button) = 0;
	virtual void MouseMove (point delta) = 0;
	virtual void FunctionKey (int key, int state) = 0;
	virtual void feedback (void) = 0;
	AudioModulePanel (PrologRoot * root, PrologAtom * atom, cairo_surface_t * background_image);
	~ AudioModulePanel (void);
};

#endif
