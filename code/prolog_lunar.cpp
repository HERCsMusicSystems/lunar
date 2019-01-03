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

//////////////////////////////////////////////////////////////////////
// This file was created on Tuesday, 15th July 2014 at 15:47:20 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_landers.h"
#include "lunar_linux_midi.h"

#include <stdio.h>
#include <string.h>

static char * orbiter_action_code = "Orbiter Action Code";
char * PrologNativeOrbiter :: name (void) {return orbiter_action_code;}
char * PrologNativeOrbiter :: codeName (void) {return orbiter_action_code;}
bool PrologNativeOrbiter :: isTypeOf (char * code_name) {return orbiter_action_code == code_name ? true : PrologNativeCode :: isTypeOf (code_name);}
bool PrologNativeOrbiter :: onEarth (void) {if (atom != 0) atom -> setMachine (0); delete this; return true;}
bool PrologNativeOrbiter :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) return onEarth ();
	if (parameters -> isVar ()) {
		parameters -> setPair ();
		PrologElement * inputs = parameters -> getLeft ();
		PrologElement * outputs = parameters -> getRight (); outputs -> setPair (); outputs = outputs -> getLeft ();
		for (int ind = 0; ind < module -> numberOfInputs (); ind++) {
			inputs -> setPair (); inputs -> getLeft () -> setText (module -> inputName (ind)); inputs = inputs -> getRight ();
		}
		for (int ind = 0; ind < module -> numberOfOutputs (); ind++) {
			outputs -> setPair (); outputs -> getLeft () -> setText (module -> outputName (ind)); outputs = outputs -> getRight ();
		}
		return true;
	}
	PrologElement * atom = 0;
	PrologElement * port = 0;
	PrologElement * value = 0;
	PrologElement * ret = 0;
	PrologElement * disconnector = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isInteger ()) if (port == 0) port = el; else value = el;
		if (el -> isText ()) if (port == 0) port = el; else value = el;
		if (el -> isDouble ()) value = el;
		if (el -> isVar ()) ret = el;
		if (el -> isEarth ()) disconnector = el;
		parameters = parameters -> getRight ();
	}
	if (parameters -> isVar ()) ret = parameters;
	int destination_port = 0;
	int source_port = 0;
	if (port != 0) {
		if (port -> isInteger ()) source_port = destination_port = port -> getInteger ();
		if (port -> isText ()) {destination_port = module -> inputIndex (port -> getText ()); source_port = module -> outputIndex (port -> getText ());}
	}
	if (atom != 0) {
		if (atom -> getAtom () == this -> atom) {module -> propagate_signals (); module -> move (); return true;}
		PrologNativeCode * machine = atom -> getAtom () -> getMachine ();
		if (machine == 0 || ! machine -> isTypeOf (PrologNativeOrbiter :: name ())) return false;
		source_port = 0;
		if (value != 0) {
			if (value -> isInteger ()) source_port = value -> getInteger ();
			if (value -> isText ()) source_port = ((PrologNativeOrbiter *) machine) -> module -> outputIndex (value -> getText ());
		}
		if (disconnector == 0) return module -> connect (destination_port, ((PrologNativeOrbiter *) machine) -> module, source_port);
		return module -> disconnect (destination_port, ((PrologNativeOrbiter *) machine) -> module, source_port);
	} else {
		if (disconnector != 0) {
			if (port != 0) return module -> disconnect (destination_port);
			module -> disconnect ();
			return true;
		}
	}
	if (ret != 0) {
		if (module -> numberOfOutputs () <= source_port) return false;
		double * adres = module -> outputAddress (source_port);
		if (adres == 0) return false; ret -> setDouble (* adres);
		return true;
	}
	if (value != 0) {
		double * adres = module -> inputAddress (destination_port);
		if (adres == 0) return false;
		if (value -> isNumber ()) {* adres = value -> getNumber (); return true;}
	}
	if (port != 0) {
		double * adres = module -> inputAddress (0);
		if (adres == 0) return false;
		if (port -> isNumber ()) {* adres = port -> getNumber (); return true;}
	}
	return false;
}

PrologNativeOrbiter :: PrologNativeOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module) {
	this -> atom = atom;
	this -> core = core;
	this -> module = module;
	if (module == 0) return;
	module -> hold ();
	//printf ("	NATIVE ORBITER CREATED\n");
}

PrologNativeOrbiter :: ~ PrologNativeOrbiter (void) {
	if (module != 0) module -> release (); module = 0;
	//printf ("	NATIVE ORBITER DESTROYED\n");
}

PrologNativeOrbiter * PrologNativeOrbiterCreator :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new PrologNativeOrbiter (atom, core, module);}
void PrologNativeOrbiterCreator :: code_created (PrologNativeOrbiter * machine) {}

bool PrologNativeOrbiterCreator :: code (PrologElement * parameters, PrologResolution * resolution) {
	PrologElement * atom = 0;
	if (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isVar ()) atom = el;
		parameters = parameters -> getRight ();
	}
	if (atom == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	orbiter * module = create_orbiter (parameters);
	if (module == 0) return false;
	PrologNativeOrbiter * machine = create_native_orbiter (atom -> getAtom (), module);
	if (machine == 0) {module -> release (); return false;}
	if (atom -> getAtom () -> setMachine (machine)) {code_created (machine); return true;}
	delete machine;
	return false;
}

PrologNativeOrbiterCreator :: PrologNativeOrbiterCreator (orbiter_core * core) {this -> core = core;}

class orbiter_statistics : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) {printf ("Number of Orbiters = [%i]\n", orbiter_count); return true;}
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		parameters -> setInteger (orbiter_count);
		return true;
	}
};

class midi_class : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologDirectory * directory;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (directory == 0) return false;
		PrologElement * atom = 0;
		PrologAtom * callback = 0;
		PrologElement * location = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) if (atom == 0) atom = el; else callback = el -> getAtom ();
			if (el -> isVar ()) atom = el;
			if (el -> isText ()) location = el;
			parameters = parameters -> getRight ();
		}
		if (atom == 0) return midi_code :: info ();
		if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
		if (callback == 0) callback = directory -> searchAtom ("income_midi");
		if (callback == 0) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
		char * midi_location;
		if (location != 0) midi_location = location -> getText ();
		else midi_location = "/dev/snd/midiC2D0";
		midi_code * mc = new midi_code (root, directory, atom -> getAtom (), callback, midi_location);
		if (mc -> inactive ()) {delete mc; return false;}
		if (atom -> getAtom () -> setMachine (mc)) return true;
		delete mc;
		return false;
	}
	midi_class (PrologRoot * root, PrologDirectory * directory) {this -> root = root; this -> directory = directory;}
};

#ifdef WIN32

class midi_com_code : public PrologNativeCode {
public:
	unsigned char command;
	unsigned char channel;
	int v1, v2, running_command;
	PrologAtom * keyoff, * keyon, * polyaftertouch, * control, * programchange, * aftertouch, * pitch;
	PrologAtom * sysex, * timingclock, * start, * cont, * stop, * activesensing;
	chromatograph graph;
	pthread_mutex_t locker;
public:
	PrologRoot * root;
	PrologAtom * atom;
	PrologAtom * callback;
	pthread_t thread;
	bool should_continue;
	HANDLE fd;
	void run (void);
	void one_parameter (void);
	void two_parameters (void);
	void send_one (int command);
	void send_two (int command, int key);
	void send_three (int command, int key, int velocity);
	bool code (PrologElement * parameters, PrologResolution * resolution);
	bool port_not_found (void);
	midi_com_code (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * callback, char * location, int rate, int size, int stop_bits, int parity, int dtr_control, int timeout);
	~ midi_com_code (void);
};

static int midi_com_tmread (HANDLE fd) {
	int ind = 0;
	DWORD read = 0;
	ReadFile (fd, & ind, 1, & read, NULL);
	if (read < 1) return -1;
	return ind;
}

static int midi_com_tmreadt (HANDLE fd) {int ind = midi_com_tmread (fd); while (ind < 0) ind = midi_com_tmread (fd); return ind;}

static void * midi_com_runner (void * parameters) {
	midi_com_code * code = (midi_com_code *) parameters;
	code -> run ();
	return 0;
}

void midi_com_code :: run (void) {
	should_continue = true;
	while (should_continue) {
		v1 = midi_com_tmread (fd);
		if (v1 < 0) continue;
		if (v1 < 128) {
			if (command >= 0x80) {
				if (command >= 0xc0 && command < 0xe0) one_parameter ();
				else {v2 = midi_com_tmreadt (fd); two_parameters ();}
			}
		} else {
			command = v1; channel = command & 0xf;
			if ((command >= 0x80 && command < 0xc0) || (command >= 0xe0 && command < 0xf0)) {
				command &= 0xf0; v1 = midi_com_tmreadt (fd); v2 = midi_com_tmreadt (fd); two_parameters ();
			} else if (command < 0xf0) {
					command &= 0xf0; v1 = midi_com_tmreadt (fd); one_parameter ();
			} else {
				PrologElement * query;
				PrologElement * el;
				int sub;
				switch (command) {
				case 0xf0:
					el = root -> earth ();
					query = root -> pair (root -> head (0), root -> pair (root -> pair (root -> atom (callback),
											root -> pair (root -> atom (sysex), el)), root -> earth ()));
					sub = midi_com_tmreadt (fd);
					while (sub >= 0 && sub < 128) {
						el -> setPair (root -> integer (sub), root -> earth ());
						el = el -> getRight ();
						sub = midi_com_tmreadt (fd);
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

void midi_com_code :: one_parameter (void) {
	PrologElement * query = root -> pair (root -> atom (callback),
								root -> pair (root -> atom (command < 0xd0 ? programchange : aftertouch),
								root -> pair (root -> integer (channel),
								root -> pair (root -> integer (v1),
								root -> earth ()))));
	query = root -> pair (root -> head (0), root -> pair (query, root -> earth ()));
	root -> resolution (query);
	delete query;
}

void midi_com_code :: two_parameters (void) {
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

bool midi_com_code :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
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
		DWORD written;
		data = 0xf0; WriteFile (fd, & data, 1, & written, NULL);
		while (parameters -> isPair ()) {
			if (graph . get_key (& parameters, & key)) {data = (unsigned char) key; WriteFile (fd, & data, 1, & written, NULL);}
			else if (parameters -> getLeft () -> isText ()) {
				char * cp = parameters -> getLeft () -> getText ();
				while (* cp != '\0') {data = (unsigned char) * cp++; WriteFile (fd, & data, 1, & written, NULL);}
				parameters = parameters -> getRight ();
			} else parameters = parameters -> getRight ();
		}
		data = 0xf7; WriteFile (fd, & data, 1, & written, NULL);
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

void midi_com_code :: send_one (int command) {DWORD written; WriteFile (fd, & command, 1, & written, NULL);}
void midi_com_code :: send_two (int command, int data) {DWORD written; WriteFile (fd, & command, 1, & written, NULL); WriteFile (fd, & data, 1, & written, NULL);}
void midi_com_code :: send_three (int command, int v1, int v2) {DWORD written; WriteFile (fd, & command, 1, & written, NULL); WriteFile (fd, & v1, 1, & written, NULL); WriteFile (fd, & v2, 1, & written, NULL);}

bool midi_com_code :: port_not_found (void) {return fd == INVALID_HANDLE_VALUE;}

midi_com_code :: midi_com_code (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * callback, char * location, int rate, int size, int stop_bits, int parity, int dtr_control, int timeout)
				: graph (directory) {
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
	this -> should_continue = false;
	this -> root = root;
	this -> atom = atom;
	this -> callback = callback;
	if (callback != 0) {COLLECTOR_REFERENCE_INC (callback);}
	fd = CreateFile (location, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	DCB dcb; dcb . DCBlength = sizeof (DCB);
	GetCommState (fd, & dcb);
	if (rate >= 0) dcb . BaudRate = rate;
	if (size >= 0) dcb . ByteSize = size;
	if (stop_bits >= 0) dcb . StopBits = stop_bits;
	if (parity >= 0) dcb . Parity = parity;
	if (dtr_control >= 0) dcb . fDtrControl = dtr_control;
	SetCommState (fd, & dcb);
	if (timeout >= 0) {
		COMMTIMEOUTS cmt;
		GetCommTimeouts (fd, & cmt);
		cmt . ReadIntervalTimeout = timeout;
		cmt . ReadTotalTimeoutMultiplier = 1;
		cmt . ReadTotalTimeoutConstant = 1;
		SetCommTimeouts (fd, & cmt);
	}
	if (callback == 0) return;
	if (port_not_found ()) return;
	pthread_create (& thread, 0, midi_com_runner, this);
	pthread_detach (thread);
}

midi_com_code :: ~ midi_com_code (void) {
	if (should_continue) {should_continue = false; while (! should_continue) Sleep (100);}
	if (callback != 0) callback -> removeAtom (); callback = 0;
	if (! port_not_found ()) {CloseHandle (fd);}
}

class midi_com_class : public PrologNativeCode {
public:
	PrologRoot * root;
	PrologDirectory * directory;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * path = 0;
		PrologElement * atom = 0;
		PrologElement * callback = 0;
		int rate = -1, size = -1, stop_bits = -1, parity = -1, dtr_control = -1, timeout = -1;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isText ()) path = el;
			if (el -> isVar ()) el -> setAtom (new PrologAtom ());
			if (el -> isAtom ()) {if (atom == 0) atom = el; else callback = el;}
			if (el -> isInteger ()) {
				if (rate < 0) rate = el -> getInteger ();
				else if (size < 0) size = el -> getInteger ();
				else if (stop_bits < 0) stop_bits = el -> getInteger ();
				else if (parity < 0) parity = el -> getInteger ();
				else if (dtr_control < 0) dtr_control = el -> getInteger ();
				else timeout = el -> getInteger ();
			}
			parameters = parameters -> getRight ();
		}
		if (timeout < 0) timeout = 10;
		if (atom == 0) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
		char * serial_location = 0;
		if (path != 0) serial_location = path -> getText ();
		else return false;
		midi_com_code * mc = new midi_com_code (root, directory, atom -> getAtom (), callback != 0 ? callback -> getAtom () : 0, serial_location, rate, size, stop_bits, parity, dtr_control, timeout);
		if (mc -> port_not_found ()) {delete mc; return false;}
		if (atom -> getAtom () -> setMachine (mc)) return true;
		delete mc;
		return false;
	}
	midi_com_class (PrologRoot * root, PrologDirectory * directory) {this -> root = root; this -> directory = directory;}
};

#else

class midi_com_class : public midi_class {
public: midi_com_class (PrologRoot * root, PrologDirectory * directory) : midi_class (root, directory) {}
};

#endif

class move_modules_class : public PrologNativeCode {
public:
	orbiter_core * core;
	bool code (PrologElement * parameters, PrologResolution * resolution) {if (core == 0) return false; core -> move_modules (); return true;}
	move_modules_class (orbiter_core * core) {this -> core = core;}
};

class propagate_signals_class : public PrologNativeCode {
public:
	orbiter_core * core;
	bool code (PrologElement * parameters, PrologResolution * resolution) {if (core == 0) return false; core -> propagate_signals (); return true;}
	propagate_signals_class (orbiter_core * core) {this -> core = core;}
};

class move_core_class : public PrologNativeCode {
public:
	orbiter_core * core;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (core == 0) return false;
		int cycles = 1;
		if (parameters -> isPair ()) {
			parameters = parameters -> getLeft ();
			if (parameters -> isInteger ()) cycles = parameters -> getInteger ();
		}
		while (cycles-- > 0) {core -> propagate_signals (); core -> move_modules ();}
		return true;
	}
	move_core_class (orbiter_core * core) {this -> core = core;}
};

class arranger_array : public PrologNativeCode {
public:
	PrologLunarServiceClass * service;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * index = 0;
		PrologElement * table = 0;
		PrologElement * reset = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isEarth ()) reset = el;
			if (el -> isInteger ()) index = el;
			if (el -> isPair ()) table = el;
			if (el -> isVar ()) {if (index == 0) index = el; else table = el;}
			parameters = parameters -> getRight ();
		}
		if (reset != 0) {service -> core . arranger_reset (); return true;}
		if (index == 0) {if (table != 0) return false; service -> core . arranger_tonal_reset (); return true;}
		if (index -> isVar ()) {index -> setInteger (service -> core . arranger_reference_note); return true;}
		if (index -> isInteger ()) {
			if (table == 0) {service -> core . arranger_reference_note = index -> getInteger (); return true;}
			int ind = index -> getInteger ();
			if (ind < 0 || ind > 127) return false;
			if (table -> isVar ()) {
				for (int sub = 0; sub < 12; sub++) {
					table -> setPair ();
					table -> getLeft () -> setInteger (service -> core . arranger_array [ind] [sub]);
					table = table -> getRight ();
				}
				return true;
			}
			int sub = 0;
			while (sub < 12 && table -> isPair ()) {
				PrologElement * el = table -> getLeft ();
				if (el -> isInteger ()) service -> core . arranger_array [ind] [sub++] = el -> getInteger ();
				table = table -> getRight ();
			}
			if (sub == 12 && table -> isEarth ()) return true;
		}
		return false;
	}
	arranger_array (PrologLunarServiceClass * service) {this -> service = service;}
};

class unicar_code : public PrologNativeCode {
public:
	PrologAtom * atom;
	PrologElement * container;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) {
			parameters = parameters -> getLeft ();
			PrologElement * el = container;
			while (el -> isPair ()) {
				PrologElement * sub = el -> getLeft ();
				if (sub -> isAtom () && parameters -> isAtom () && sub -> getAtom () == parameters -> getAtom ()) return true;
				if (sub -> isInteger () && parameters -> isInteger () && sub -> getInteger () == parameters -> getInteger ()) return true;
				el = el -> getRight ();
			}
			el -> setPair ();
			el -> getLeft () -> duplicate (parameters);
			return true;
		}
		if (parameters -> isVar ()) {parameters -> duplicate (container); return true;}
		if (parameters -> isEarth ()) {atom -> setMachine (0); delete this; return true;}
		return false;
	}
	unicar_code (PrologAtom * atom) {this -> atom = atom; container = new PrologElement ();}
	~ unicar_code (void) {delete container;}
};

class unicar : public PrologNativeCode {
public:
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) parameters = parameters -> getLeft ();
		if (parameters -> isVar ()) parameters -> setAtom (new PrologAtom ());
		if (! parameters -> isAtom ()) return false;
		PrologAtom * atom = parameters -> getAtom ();
		if (atom -> getMachine () != 0) return false;
		unicar_code * u = new unicar_code (atom);
		if (atom -> setMachine (u)) return true;
		delete u;
		return false;
	}
};

void PrologLunarServiceClass :: init (PrologRoot * root, PrologDirectory * directory) {
	this -> root = root;
	this -> directory = directory;
}

PrologNativeCode * PrologLunarServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "small_keyboard") == 0) return new keyboard_class (this, 1);
	if (strcmp (name, "keyboard") == 0) return new keyboard_class (this, 2);
	if (strcmp (name, "big_keyboard") == 0) return new keyboard_class (this, 3);
	if (strcmp (name, "small_keyboard_long") == 0) return new keyboard_class (this, 4);
	if (strcmp (name, "keyboard_long") == 0) return new keyboard_class (this, 5);
	if (strcmp (name, "big_keyboard_long") == 0) return new keyboard_class (this, 6);
	if (strcmp (name, "oscilloscope") == 0) return new oscilloscope_class (& core);
	if (strcmp (name, "spectroscope") == 0) return new oscilloscope_class (& core, oscilloscope_class :: SPECTROSCOPE);
	if (strcmp (name, "big_oscilloscope") == 0) return new oscilloscope_class (& core, oscilloscope_class :: BIG_OSCILLOSCOPE);
	if (strcmp (name, "big_spectroscope") == 0) return new oscilloscope_class (& core, oscilloscope_class :: BIG_SPECTROSCOPE);
	if (strcmp (name, "vector") == 0) return new vector_class (this);
	if (strcmp (name, "CommandCentre") == 0) return new control_panel_class (this);
	if (strcmp (name, "core") == 0) return new core_class (& core);
	if (strcmp (name, "jack") == 0) return new jack_class (root, directory, & core);
	if (strcmp (name, "jack_connect") == 0) return new jack_connect_class ();
	if (strcmp (name, "jack_disconnect") == 0) return new jack_connect_class (true);
	if (strcmp (name, "operator") == 0) return new operator_class (& core);
	if (strcmp (name, "square_operator") == 0) return new square_operator_class (& core);
	if (strcmp (name, "saw_operator") == 0) return new saw_operator_class (& core);
	if (strcmp (name, "noise_operator") == 0) return new noise_operator_class (& core);
	if (strcmp (name, "sampler_operator") == 0) return new sampler_operator_class (& core);
	if (strcmp (name, "dx") == 0) return new dx_operator_class (& core);
	if (strcmp (name, "fm") == 0) return new fm_operator_class (& core);
	if (strcmp (name, "vco") == 0) return new vco_operator_class (& core);
	if (strcmp (name, "filter") == 0) return new filter_class (& core);
	if (strcmp (name, "formant_filter") == 0) return new formant_filter_class (& core);
	if (strcmp (name, "DCOffsetFilter") == 0) return new DCOffsetFilter_class (& core, false);
	if (strcmp (name, "DCOffsetFilterMono") == 0) return new DCOffsetFilter_class (& core, true);
	if (strcmp (name, "parameter_block") == 0) return new parameter_block_class (& core);
	if (strcmp (name, "latch") == 0) return new latch_class (& core);
	if (strcmp (name, "morph") == 0) return new morph_class (& core);
	if (strcmp (name, "auto_data") == 0) return new auto_data_class (& core);
	if (strcmp (name, "auto") == 0) return new auto_class (& core);
	if (strcmp (name, "key_map") == 0) return new key_map_class (& core);
	if (strcmp (name, "wave") == 0) return new wave_class (& core);
	if (strcmp (name, "velocity_map") == 0) return new key_map_class (& core, 0);
	if (strcmp (name, "impulse") == 0) return new impulse_class (& core);
	if (strcmp (name, "trigger") == 0) return new trigger_class (directory, & core);
	if (strcmp (name, "mixer") == 0) return new mixer_class (& core);
	if (strcmp (name, "stereo_mixer") == 0) return new stereo_mixer_class (& core);
	if (strcmp (name, "control") == 0) return new control_class (& core);
	if (strcmp (name, "gateway") == 0) return new gateway_class (& core);
	if (strcmp (name, "stereo_gateway") == 0) return new stereo_gateway_class (& core);
	if (strcmp (name, "amplifier") == 0) return new amplifier_class (& core);
	if (strcmp (name, "stereo_amplifier") == 0) return new stereo_amplifier_class (& core);
	if (strcmp (name, "volume") == 0) return new volume_class (& core);
	if (strcmp (name, "mono_volume") == 0) return new mono_volume_class (& core);
	if (strcmp (name, "lfo") == 0) return new lfo_class (& core);
	if (strcmp (name, "adsr") == 0) return new adsr_class (& core);
	if (strcmp (name, "adsr-linear") == 0) return new adsr_class (& core, true);
	if (strcmp (name, "eg") == 0) return new eg_class (& core);
	if (strcmp (name, "eg-linear-expo") == 0) return new eg_class (& core, true);
	if (strcmp (name, "vca") == 0) return new vca_class (& core);
	if (strcmp (name, "vca-adsr") == 0) return new vca_adsr_class (& core);
	if (strcmp (name, "vca-eg") == 0) return new vca_eg_class (& core);
	if (strcmp (name, "delay") == 0) return new delay_class (& core);
	if (strcmp (name, "freeverb") == 0) return new freeverb_class (& core);
	if (strcmp (name, "chorus") == 0) return new chorus_class (& core);
	if (strcmp (name, "stereo_chorus") == 0) return new stereo_chorus_class (& core);
	if (strcmp (name, "pan") == 0) return new pan_class (& core, 0);
	if (strcmp (name, "power_pan") == 0) return new pan_class (& core, 1);
	if (strcmp (name, "linear_pan") == 0) return new pan_class (& core, 2);
	if (strcmp (name, "stereo_pan") == 0) return new pan_class (& core, 3);
	if (strcmp (name, "stereo_power_pan") == 0) return new pan_class (& core, 4);
	if (strcmp (name, "stereo_linear_pan") == 0) return new pan_class (& core, 5);
	if (strcmp (name, "drywet") == 0) return new drywet_class (& core);
	if (strcmp (name, "drywet_mono") == 0) return new drywet_mono_class (& core);
	if (strcmp (name, "sensitivity") == 0) return new sensitivity_class (& core);
	if (strcmp (name, "moonbase") == 0) return new moonbase_class (directory, & core);
	if (strcmp (name, "moonbase_monitor") == 0) return new moonbase_monitor_class (root, directory, & core);
	if (strcmp (name, "chord_detector") == 0) return new chord_detector_class (root, directory, & core);
	if (strcmp (name, "arpeggiator") == 0) return new arpeggiator_class (directory, & core);
	if (strcmp (name, "sequencer") == 0) return new sequencer_class (directory, & core);
	if (strcmp (name, "prolog_sequencer") == 0) return new prolog_sequencer_class (root, directory, & core);
	if (strcmp (name, "polysequencer") == 0) return new polysequencer_class (directory, & core);
	if (strcmp (name, "timingclock") == 0) return new timingclock_class (directory, & core);
	if (strcmp (name, "detector") == 0) return new detector_class (root, & core);
	if (strcmp (name, "orbiter") == 0) return new orbiter_statistics ();
	if (strcmp (name, "arranger_array") == 0) return new arranger_array (this);
	if (strcmp (name, "midi") == 0) return new midi_class (this -> root, directory);
	if (strcmp (name, "midicom") == 0) return new midi_com_class (this -> root, directory);
	if (strcmp (name, "ParameterBlockPanel") == 0) return new parameter_block_panel_class (this);
	if (strcmp (name, "AdsrPanel") == 0) return new adsr_panel_class (this);
	if (strcmp (name, "EGPanel") == 0) return new eg_panel_class (this);
	if (strcmp (name, "FEGPanel") == 0) return new eg_panel_class (this, true);
	if (strcmp (name, "DX9Panel") == 0) return new fm4_panel_class (this, true);
	if (strcmp (name, "FM4Panel") == 0) return new fm4_panel_class (this, false);
	if (strcmp (name, "DX7Panel") == 0) return new fm6_panel_class (this, true);
	if (strcmp (name, "FM6Panel") == 0) return new fm6_panel_class (this, false);
	if (strcmp (name, "CorePanel") == 0) return new core_panel_class (this);
	if (strcmp (name, "LfoPanel") == 0) return new lfo_panel_class (this);
	if (strcmp (name, "FilterPanel") == 0) return new filter_panel_class (this);
	if (strcmp (name, "FormantFilterPanel") == 0) return new formant_filter_panel_class (this);
	if (strcmp (name, "DelayPanel") == 0) return new delay_panel_class (this);
	if (strcmp (name, "ChorusPanel") == 0) return new chorus_panel_class (this);
	if (strcmp (name, "StereoChorusPanel") == 0) return new stereo_chorus_panel_class (this);
	if (strcmp (name, "FreeverbPanel") == 0) return new freeverb_panel_class (this);
	if (strcmp (name, "MoveModules") == 0) return new move_modules_class (& core);
	if (strcmp (name, "PropagateSignals") == 0) return new propagate_signals_class (& core);
	if (strcmp (name, "MoveCore") == 0) return new move_core_class (& core);
	if (strcmp (name, "LoopWave") == 0) return new LoopWaveClass ();
	if (strcmp (name, "unicar") == 0) return new unicar ();
	if (strcmp (name, "integrated_alarm") == 0) return new integrated_alarm_class (& core, directory);
	if (strcmp (name, "integrated_microdot") == 0) return new integrated_microdot_class (& core, directory);
	if (strcmp (name, "integrated_abakos") == 0) return new integrated_abakos_class (& core, directory);
	if (strcmp (name, "integrated_phobos") == 0) return new integrated_phobos_class (& core, directory);
	return 0;
}

extern GraphicResources * create_graphic_resources (void);
extern void destroy_graphic_resources (GraphicResources * resources);
PrologLunarServiceClass :: PrologLunarServiceClass (void) {resources = create_graphic_resources ();}
PrologLunarServiceClass :: ~ PrologLunarServiceClass (void) {
	destroy_graphic_resources (resources);
	if (orbiter_count != 0) printf ("orbiter counter [%i]\n", orbiter_count);
}

#ifdef WINDOWS_OPERATING_SYSTEM
#include "lunar_resource.h"
extern "C" {
	__declspec (dllexport) char * get_module_code (void) {
		HMODULE hm = GetModuleHandle ("lunar.dll");
		HRSRC resource = FindResource (hm, MAKEINTRESOURCE (LUNAR_PRC), RT_RCDATA);
		if (resource == 0) return 0;
		HGLOBAL loader = LoadResource (hm, resource);
		if (loader == 0) return 0;
		return (char *) LockResource (loader);
	}
	__declspec (dllexport) PrologServiceClass * create_service_class (void) {return new PrologLunarServiceClass ();}
}
#endif

#ifdef LINUX_OPERATING_SYSTEM
extern "C" {
	extern char * get_module_code (void);
	extern PrologServiceClass * create_service_class (void);
}
extern char resource_lunar;
char * get_module_code (void) {return & resource_lunar;}
PrologServiceClass * create_service_class (void) {return new PrologLunarServiceClass ();}
#endif

