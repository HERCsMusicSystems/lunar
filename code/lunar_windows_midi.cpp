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

bool midi_code :: drop_system_exclusive (PrologElement * parameters) {
	unsigned char buffer [1024];
	buffer [0] = 0xf0;
	int buffer_pointer = 1;
	int key;
	while (parameters -> isPair ()) {
		if (graph . get_key (& parameters, & key)) buffer [buffer_pointer++] = (unsigned char) key;
		else parameters = parameters -> getRight ();
	}
	buffer [buffer_pointer++] = 0xf7;
	MIDIHDR midihdr;
	midihdr . lpData = (LPSTR) buffer;
	midihdr . dwBufferLength = buffer_pointer;
	midihdr . dwFlags = 0;
	if (midiOutPrepareHeader (out_port_handle, & midihdr, sizeof (MIDIHDR))) return false;
	if (midiOutLongMsg (out_port_handle, & midihdr, sizeof (MIDIHDR))) return false;
	while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader (out_port_handle, & midihdr, sizeof (MIDIHDR))) Sleep (10);
	return true;
}

void midi_code :: send_one (int command) {midiOutShortMsg (out_port_handle, command);}
void midi_code :: send_two (int command, int key) {midiOutShortMsg (out_port_handle, command | (key << 8));}
void midi_code :: send_three (int command, int key, int velocity) {midiOutShortMsg (out_port_handle, command | (key << 8) | (velocity << 16));}

bool midi_code :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
	if (! out_port_active) return false;
	if (! parameters -> isPair ()) return false;
	PrologElement * ae = parameters -> getLeft ();
	if (! ae -> isAtom ()) return false;
	PrologAtom * atom = ae -> getAtom ();
	parameters = parameters -> getRight ();
	int channel; int key; double velocity;
	if (atom == keyoff) {
		if (! graph . get_channel (& parameters, & channel)) return false;
		if (parameters -> isEarth ()) {send_three (0xb0 + channel, 123, 0); return true;}
		if (! graph . get_key (& parameters, & key)) return false;
		if (parameters -> isEarth ()) {send_three (0x80 + channel, key, 0); return true;}
		if (! graph . get_velocity (& parameters, & velocity)) return false;
		send_three (0x80 + channel, key, (int) velocity);
		return true;
	}
	if (atom == keyon) {
		if (! graph . get_channel (& parameters, & channel)) return false;
		if (! graph . get_key (& parameters, & key)) return false;
		if (parameters -> isEarth ()) {send_three (0x90 + channel, key, 100); return true;}
		if (! graph . get_velocity (& parameters, & velocity)) return false;
		send_three (0x90 + channel, key, (int) velocity);
		return true;
	}
	if (atom == polyaftertouch || atom == control) {
		if (! graph . get_channel (& parameters, & channel)) return false;
		if (! graph . get_key (& parameters, & key)) return false;
		if (! graph . get_velocity (& parameters, & velocity)) return false;
		send_three ((atom == control ? 0xb0 : 0xa0) + channel, key, (int) velocity);
		return true;
	}
	if (atom == programchange || atom == aftertouch) {
		if (! graph . get_channel (& parameters, & channel)) return false;
		if (! graph . get_key (& parameters, & key)) return false;
		send_two ((atom == programchange ? 0xc0 : 0xd0) + channel, key);
		return true;
	}
	if (atom == pitch) {
		if (! graph . get_channel (& parameters, & channel)) return false;
		if (! graph . get_key (& parameters, & key)) return false;
		if (parameters -> isEarth ()) {send_three (0xe0 + channel, 0, key); return true;}
		int msb; if (! graph . get_key (& parameters, & msb)) return false;
		send_three (0xe0 + channel, key, msb);
		return true;
	}
	if (atom == sysex) return drop_system_exclusive (parameters);
	if (atom == timingclock) {send_one (0xf8); return true;}
	if (atom == start) {send_one (0xfa); return true;}
	if (atom == cont) {send_one (0xfb); return true;}
	if (atom == stop) {send_one (0xfc); return true;}
	if (atom == activesensing) {send_one (0xfe); return true;}
	return false;
}

bool midi_code :: inactive (void) {return ! (out_port_active || in_port_active);}

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
	out_port_active = in_port_active = false;
	int number_of_outputs = midiOutGetNumDevs ();
	MIDIOUTCAPS out_info;
	for (int ind = 0; ind < number_of_outputs; ind++) {
		midiOutGetDevCaps (ind, & out_info, sizeof (out_info));
		if (strcmp (out_info . szPname, location) == 0) {out_port_active = midiOutOpen (& out_port_handle, ind, NULL, NULL, CALLBACK_NULL) == MMSYSERR_NOERROR; break;}
	}
}

midi_code :: ~ midi_code (void) {
	if (out_port_active) midiOutClose (out_port_handle);
	if (in_port_active) midiInClose (in_port_handle);
	if (callback != 0) callback -> removeAtom (); callback = 0;
}

bool midi_code :: info (void) {
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
	return true;
}
