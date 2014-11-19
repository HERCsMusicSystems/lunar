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

///////////////////////////////////////////////////////////////////////
// This file was created on Thursday, 17th July 2014 at 11:50:04 AM. //
///////////////////////////////////////////////////////////////////////

#include "lunar_moonbase.h"
#include "prolog_lunar.h"
#include "multiplatform_audio.h"

static int cores = 0;

extern void alpha_callback (int frames, AudioBuffers * data, void * source);
extern void beta_callback (int frames, AudioBuffers * data, void * source);

class lunar_core : public orbiter {
public:
	double mono, left, right;
	double input_mono, input_left, input_right;
	double line [16384];
	int line_write;
	int line_read;
	virtual int numberOfInputs (void) {return 3;}
	virtual char * inputName (int ind) {
		switch (ind) {
		case 0: return "MONO"; break;
		case 1: return "LEFT"; break;
		case 2: return "RIGHT"; break;
		default: break;
		}
		return orbiter :: inputName (ind);
	}
	virtual double * inputAddress (int ind) {
		switch (ind) {
		case 0: return & mono; break;
		case 1: return & left; break;
		case 2: return & right; break;
		default: break;
		}
		return orbiter :: inputAddress (ind);
	}
	virtual int numberOfOutputs (int ind) {return numberOfInputs ();}
	virtual char * outputName (int ind) {return inputName (ind);}
	virtual double * outputAddress (int ind) {
		switch (ind) {
		case 0: return & input_mono; break;
		case 1: return & input_left; break;
		case 2: return & input_right; break;
		default: break;
		}
		return orbiter :: outputAddress (ind);
	}
	virtual void move (void) {
		if (line_read == line_write) return;
		input_mono = line [line_read++];
		if (line_read >= 16384) line_read = 0;
	}
	lunar_core (orbiter_core * core) : orbiter (core) {
		mono = left = right = 0.0;
		input_mono = input_left = input_right = 0.0;
		for (int ind = 0; ind < 16384; ind++) line [ind] = 0.0;
		line_write = line_read = 0;
		initialise (); activate ();
	}
};

class core_action : public PrologNativeOrbiter {
public:
	MultiplatformAudio * audio;
	core_action (PrologAtom * atom, orbiter_core * core) : PrologNativeOrbiter (atom, core, new lunar_core (core)) {
		printf ("..... creating moonbase\n");
		cores++;
		audio = new MultiplatformAudio (2, (int) core -> sampling_frequency, core -> latency_block_size);
		int outputs = audio -> getNumberOfOutputDevices ();
		printf ("Number of outputs = %i\n", outputs);
		for (int ind = 0; ind < outputs; ind++) printf ("	device [%s]\n", audio -> getOutputDeviceName (ind));
		int inputs = audio -> getNumberOfInputDevices ();
		printf ("Number of inputs = %i\n", inputs);
		for (int ind = 0; ind < inputs; ind++) printf ("	device [%s]\n", audio -> getInputDeviceName (ind));
		if (outputs > 0) {
			audio -> installOutputCallback (alpha_callback, this);
			audio -> selectOutputDevice (0);
		}
		if (inputs > 0) {
			audio -> installInputCallback (beta_callback, this);
			audio -> selectInputDevice (0);
		}
		printf ("Moonbase prolog created.\n");
	}
	~ core_action (void) {
		printf (".... deleting moonbase\n");
		delete audio;
		cores--;
		printf ("Moonbase prolog destroyed.\n");
	}
};

void alpha_callback (int frames, AudioBuffers * data, void * source) {
	core_action * base = (core_action *) source;
	orbiter_core * core = base -> core;
	double * moon = base -> module -> inputAddress (0);
	double * left_moon = base -> module -> inputAddress (1);
	double * right_moon = base -> module -> inputAddress (2);
	pthread_mutex_lock (& core -> main_mutex);
	for (int ind = 0; ind < frames; ind++) {
		core -> move_modules ();
		core -> propagate_signals ();
		data -> insertStereo (((* moon) + (* left_moon)) * 0.2, ((* moon) + (* right_moon)) * 0.2);
	}
	pthread_mutex_unlock (& core -> main_mutex);
}

void beta_callback (int frames, AudioBuffers * data, void * source) {
	core_action * base = (core_action *) source;
	orbiter_core * core = base -> core;
	lunar_core * moon = (lunar_core *) base -> module;
	pthread_mutex_lock (& core -> main_mutex);
	for (int ind = 0; ind < frames; ind++) {
		moon -> line [moon -> line_write++] = data -> getMono ();
		if (moon -> line_write >= 16384) moon -> line_write = 0;
	}
	pthread_mutex_unlock (& core -> main_mutex);
}

bool core_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (cores > 0) return false;
	PrologElement * atom = 0;
	double centre_frequency = -1;
	double sampling_frequency = -1;
	int latency_block_size = -1;
	int requested_number_of_actives = -1;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isVar ()) atom = el;
		if (el -> isInteger ()) {
			if (centre_frequency < 0.0) centre_frequency = (double) el -> getInteger ();
			else if (sampling_frequency < 0.0) sampling_frequency = (double) el -> getInteger ();
			else if (latency_block_size < 0) latency_block_size = el -> getInteger ();
			else requested_number_of_actives = el -> getInteger ();
		}
		if (el -> isDouble ()) centre_frequency = el -> getDouble ();
		parameters = parameters -> getRight ();
	}
	if (centre_frequency < 0.0) centre_frequency = core -> centre_frequency;
	if (sampling_frequency < 0.0) sampling_frequency = core -> sampling_frequency;
	if (latency_block_size < 16) latency_block_size = core -> latency_block_size;
	if (requested_number_of_actives < 0) requested_number_of_actives = core -> requested_active_size;
	if (atom == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	core -> centre_frequency = centre_frequency;
	core -> sampling_frequency = (double) sampling_frequency;
	core -> latency_block_size = latency_block_size;
	core -> requested_active_size = requested_number_of_actives;
	core -> recalculate ();
	core_action * machine = new core_action (atom -> getAtom (), core);
	if (atom -> getAtom () -> setMachine (machine)) return true;
	delete machine;
	return false;
}

core_class :: core_class (orbiter_core * core) {this -> core = core;}

