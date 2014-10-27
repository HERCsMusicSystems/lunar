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

////////////////////////////////////////////////////////////////////////
// This file was created on Thursday, 23rd October 2014, 17:46:47 PM. //
////////////////////////////////////////////////////////////////////////

#include "lunar_linux_midi.h"
#ifndef WIN32
#include <unistd.h>
int tmread (int fd) {
	fd_set readset;
	FD_ZERO (& readset);
	FD_SET (fd, & readset);
	timeval timeout;
	timeout . tv_sec = 1;
	timeout . tv_usec = 0;
	if (select (fd + 1, & readset, 0, 0, & timeout) > 0) {int v; read (fd, & v, 1); return v;}
	return -1;
}
#else
#define tmread(fd) 0
#define usleep(time)
#define open(a, b) 0
#define close(a) 0
#endif
#include <fcntl.h>

static void * midi_runner (void * parameters) {
	midi_code * mc = (midi_code *) parameters;
	mc -> run ();
	return 0;
};

void midi_code :: run (void) {
	should_continue = true;
	while (should_continue) {
		v1 = tmread (fd);
		if (v1 < 0) continue;
		if (v1 < 128) {
			if ((command >= 0x80 && command < 0xc0) || (command >= 0xe0 && command < 0xf0)) {
				v2 = tmread (fd); two_parameters ();
			} else one_parameter ();
		} else {
			command = v1; channel = command & 0xf;
			if ((command >= 0x80 && command < 0xc0) || (command >= 0xe0 && command < 0xf0)) {
				command &= 0xf0; v1 = tmread (fd); v2 = tmread (fd); two_parameters ();
			} else {
				if (command < 0xf0) {
					command &= 0xf0; v1 = tmread (fd); one_parameter ();
				} else {
				}
			}
		}
	}
	should_continue = true;
}

void midi_code :: one_parameter (void) {
	PrologElement * query = root -> pair (root -> atom (callback),
								root -> pair (root -> atom (command < 0xd0 ? programchange : aftertouch),
								root -> pair (root -> integer (channel),
								root -> pair (root -> integer (v1),
								root -> earth ()))));
	query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
	root -> resolution (query);
	delete query;
}

void midi_code :: two_parameters (void) {
	PrologAtom * command_atom;
	switch (command) {
	case 0x80: command_atom = keyoff; break;
	case 0x90: command_atom = keyon; break;
	case 0xa0: command_atom = polyaftertouch; break;
	case 0xb0: command_atom = control; break;
	case 0xe0: command_atom = pitch; break;
	default: command_atom = sysex; break;
	}
	PrologElement * query = root -> pair (root -> atom (callback),
								root -> pair (root -> atom (command_atom),
								root -> pair (root -> integer (channel),
								root -> pair (root -> integer (v1),
								root -> pair (root -> integer (v2),
								root -> earth ())))));
	query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
	root -> resolution (query);
	delete query;
}

bool midi_code :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
	return true;
}

midi_code :: midi_code (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * callback, char * location) {
	command = channel = 0;
	keyoff = keyon = polyaftertouch = control = programchange = aftertouch = pitch = 0;
	sysex = timingclock = start = cont = stop = activesensing = 0;
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
	pthread_create (& thread, 0, midi_runner, this);
	pthread_detach (thread);
	fd = open (location, O_RDONLY | O_NONBLOCK);
}

midi_code :: ~ midi_code (void) {
	if (fd >= 0) close (fd);
	if (should_continue) {
		should_continue = false;
		while (! should_continue) usleep (100);
		should_continue = false;
	}
	if (callback != 0) callback -> removeAtom (); callback = 0;
}

