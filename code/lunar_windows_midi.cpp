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

/////////////////////////////////////////////////////////////////////////
// This file was created on Friday, 11th November 2015 at 15:05:51 AM. //
/////////////////////////////////////////////////////////////////////////

#include "lunar_linux_midi.h"

bool midi_code :: code (PrologElement * parameters, PrologResolution * resolution) {
	return false;
}

midi_code :: midi_code (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * callback, char * location) : graph (directory) {
	command = channel = 0;
	keyoff = keyon = polyaftertouch = control = programchange = aftertouch = pitch = 0;
	sysex = timingclock = start = cont = stop = activesensing = 0;
	running_command = -1;
	if (directory != 0) {
		keyoff = directory -> searchAtom ("keyoff");
		keyon = directory -> searchAtom ("keyon");
		polyaftertouch = directory -> searchAtom ("polyaftertouch");
		control = directory -> searchAtom ("control");
		programchange = directory -> searchAtom ("programchange");
		aftertouch = directory -> searchAtom ("aftertouch");
		pitch = directory -> searchAtom ("pitch");
		sysex = directory -> searchAtom ("sysex");
		timingclock = directory -> searchAtom ("timingclock");
		start = directory -> searchAtom ("START");
		cont = directory -> searchAtom ("CONTINUE");
		stop = directory -> searchAtom ("STOP");
		activesensing = directory -> searchAtom ("activesensing");
	}
	this -> root = root;
	this -> atom = atom;
	this -> callback = callback;
	if (callback != 0) {COLLECTOR_REFERENCE_INC (callback);}
	int number_of_inputs = midiInGetNumDevs ();
	MIDIINCAPS in_info;
	for (int ind = 0; ind < number_of_inputs; ind++) {
		midiInGetDevCaps (ind, & in_info, sizeof (in_info));
		printf ("%i <= [%s]\n", ind, in_info . szPname);
	}
	int number_of_outputs = midiOutGetNumDevs ();
	MIDIOUTCAPS out_info;
	for (int ind = 0; ind < number_of_outputs; ind++) {
		midiOutGetDevCaps (ind, & out_info, sizeof (out_info));
		printf ("%i => [%s]\n", ind, out_info . szPname);
	}
}

midi_code :: ~ midi_code (void) {
	if (callback != 0) callback -> removeAtom (); callback = 0;
}
