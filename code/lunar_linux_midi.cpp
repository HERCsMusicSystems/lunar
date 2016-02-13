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
#include <sys/select.h>
int tmread (int fd) {
	fd_set readset;
	FD_ZERO (& readset);
	FD_SET (fd, & readset);
	timeval timeout;
	timeout . tv_sec = 1;
	timeout . tv_usec = 0;
	if (select (fd + 1, & readset, 0, 0, & timeout) > 0) {int v = 0; read (fd, & v, 1); return v;}
	return -1;
}
#else
#define tmread(fd) 0
#define usleep(time)
#define open(a, b) 0
#define close(a) 0
#define write(a, b, c)
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
			if (command >= 0x80) {
				if (command >= 0xc0 && command < 0xe0) one_parameter ();
				else {v2 = tmread (fd); two_parameters ();}
			}
		} else {
			command = v1; channel = command & 0xf;
			if ((command >= 0x80 && command < 0xc0) || (command >= 0xe0 && command < 0xf0)) {
				command &= 0xf0; v1 = tmread (fd); v2 = tmread (fd); two_parameters ();
			} else if (command < 0xf0) {
					command &= 0xf0; v1 = tmread (fd); one_parameter ();
			} else {
				PrologElement * query;
				PrologElement * el;
				int sub;
				switch (command) {
				case 0xf0:
					el = root -> earth ();
					query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (callback),
											root -> pair (root -> atom (sysex), el)), root -> earth ()));
					sub = tmread (fd);
					while (sub >= 0 && sub < 128) {
						el -> setPair (root -> integer (sub), root -> earth ());
						el = el -> getRight ();
						sub = tmread (fd);
					}
					root -> resolution (query); delete query;
					break;
				case 0xf8:
					query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (callback),
											root -> pair (root -> atom (timingclock), root -> earth ())), root -> earth ()));
					root -> resolution (query); delete query;
					break;
				case 0xfa:
					query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (callback),
											root -> pair (root -> atom (start), root -> earth ())), root -> earth ()));
					root -> resolution (query); delete query;
					break;
				case 0xfb:
					query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (callback),
											root -> pair (root -> atom (cont), root -> earth ())), root -> earth ()));
					root -> resolution (query); delete query;
					break;
				case 0xfc:
					query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (callback),
											root -> pair (root -> atom (stop), root -> earth ())), root -> earth ()));
					root -> resolution (query); delete query;
					break;
				case 0xfe:
					query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (callback),
											root -> pair (root -> atom (activesensing), root -> earth ())), root -> earth ()));
					root -> resolution (query); delete query;
					break;
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
	if (tc < 0) return false;
	if (! parameters -> isPair ()) return false;
	PrologElement * ae = parameters -> getLeft ();
	if (! ae -> isAtom ()) return false;
	PrologAtom * atom = ae -> getAtom ();
	parameters = parameters -> getRight ();
	int channel; int key; double velocity;
	unsigned char data;
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
	if (atom == sysex) {
		pthread_mutex_lock (& locker);
		running_command = 0xf0;
		data = 0xf0; write (tc, & data, 1);
		while (parameters -> isPair ()) {
			if (graph . get_key (& parameters, & key)) {data = (unsigned char) key; write (tc, & data, 1);}
			else if (parameters -> getLeft () -> isText ()) {
				char * cp = parameters -> getLeft () -> getText ();
				while (* cp != '\0') {data = (unsigned char) * cp++; write (tc, & data, 1);}
				parameters = parameters -> getRight ();
			} else parameters = parameters -> getRight ();
		}
		data = 0xf7; write (tc, & data, 1);
		pthread_mutex_unlock (& locker);
		return true;
	}
	if (atom == timingclock) {send_one (0xf8); return true;}
	if (atom == start) {send_one (0xfa); return true;}
	if (atom == cont) {send_one (0xfb); return true;}
	if (atom == stop) {send_one (0xfc); return true;}
	if (atom == activesensing) {send_one (0xfe); return true;}
	return false;
}

void midi_code :: send_one (int command) {
	unsigned char data = (unsigned char) data;
	pthread_mutex_lock (& locker);
	running_command = command;
	write (tc, & data, 1);
	pthread_mutex_unlock (& locker);
}

void midi_code :: send_two (int command, int key) {
	unsigned char data [2];
	pthread_mutex_lock (& locker);
	if (command == running_command) {
		data [0] = (unsigned char) key;
		write (tc, data, 1);
	} else {
		data [0] = (unsigned char) command;
		data [1] = (unsigned char) key;
		write (tc, data, 2);
	}
	pthread_mutex_unlock (& locker);
}

void midi_code :: send_three (int command, int key, int velocity) {
	unsigned char data [3];
	pthread_mutex_lock (& locker);
	if (command == running_command) {
		data [0] = (unsigned char) key;
		data [1] = (unsigned char) velocity;
		write (tc, data, 2);
	} else {
		data [0] = (unsigned char) command;
		data [1] = (unsigned char) key;
		data [2] = (unsigned char) velocity;
		running_command = command;
		write (tc, data, 3);
	}
	pthread_mutex_unlock (& locker);
}

bool midi_code :: inactive (void) {return fd < 0;}

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
	tc = -1;
	fd = open (location, O_RDONLY | O_NONBLOCK);
	if (fd < 0) return;
	tc = open (location, O_WRONLY);
	if (tc >= 0) pthread_mutex_init (& locker, 0);
	pthread_create (& thread, 0, midi_runner, this);
	pthread_detach (thread);
}

midi_code :: ~ midi_code (void) {
	if (fd >= 0) {
		close (fd);
		if (should_continue) {
			should_continue = false;
			while (! should_continue) usleep (100);
			should_continue = false;
		}
	}
	if (tc >= 0) {
		pthread_mutex_destroy (& locker);
		close (tc);
	}
	if (callback != 0) callback -> removeAtom (); callback = 0;
}

bool midi_code :: info (void) {return false;}

