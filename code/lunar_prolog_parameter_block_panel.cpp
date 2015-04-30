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

/////////////////////////////////////////////////////////////////////////
// This file was created on Wednesday, 19th November 2014 at 13:38:23. //
/////////////////////////////////////////////////////////////////////////

#include "lunar_prolog_panel_base.h"

class parameter_block_panel_action : public AudioModulePanel {
public:
	PrologAtom * command;
	knob_active_graphics ctrl;
	void move (void) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> Double (ctrl . value),
								root -> earth ()));
		query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
		root -> resolution (query);
		delete query;
	}
	void feedback (void) {
		PrologElement * query = root -> pair (root -> atom (command),
								root -> pair (root -> var (0),
								root -> earth ()));
		query = root -> pair (root -> var (0), root -> pair (query, root -> earth ()));
		if (root -> resolution (query) == 1) {
			PrologElement * el = query;
			if (el -> isPair ()) el = el -> getLeft ();
			if (el -> isNumber ()) ctrl . setValue (el -> getNumber ());
		}
		delete query;
	}
	void redraw (cairo_t * cr) {ctrl . draw (cr);}
	void MouseKeyon (point location, int button) {ctrl . keyon (location);}
	void MouseKeyoff (point location, int button) {ctrl . keyoff (location);}
	void MouseMove (point delta) {if (ctrl . move (delta)) {move (); update ();}}
	void FunctionKey (int key, int state) {}
	parameter_block_panel_action (GraphicResources * resources, PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * command, bool active) :
		ctrl (point (0, 0), 0, resources, true, true), AudioModulePanel (root, atom, resources != 0 ? resources -> knob_surface : 0)
	{this -> command = command; COLLECTOR_REFERENCE_INC (command);}
	~ parameter_block_panel_action (void) {command -> removeAtom ();}
};

bool parameter_block_panel_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	PrologElement * command = 0;
	PrologElement * from = 0;
	PrologElement * to = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) {if (atom == 0) atom = el; else command = el;}
		if (el -> isVar ()) {if (atom != 0) command = atom; atom = el;}
		if (el -> isNumber ()) {if (from == 0) from = el; else to = el;}
		parameters = parameters -> getRight ();
	}
	if (atom == 0 || command == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	parameter_block_panel_action * machine = new parameter_block_panel_action (resources, root, directory, atom -> getAtom (), command -> getAtom (), false);
	if (from != 0) {
		machine -> ctrl . start = from -> getNumber ();
		if (to != 0) machine -> ctrl . range = to -> getNumber () - machine -> ctrl . start;
	}
	if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
	machine -> feedback ();
	machine -> BuildPanel ();
	return true;
}

parameter_block_panel_class :: parameter_block_panel_class (PrologLunarServiceClass * servo) {
	this -> root = servo -> root;
	this -> directory = servo -> directory;
	this -> resources = servo -> resources;
}
