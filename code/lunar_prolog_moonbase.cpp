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
	virtual int numberOfOutputs (void) {return numberOfInputs ();}
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
		input_left = line [line_read++];
		input_right = line [line_read++];
		input_mono = (input_left + input_right) * 0.5;
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

MultiplatformAudio audio (0);

class core_action : public PrologNativeOrbiter {
public:
	bool audio_failed;
	core_action (PrologAtom * atom, orbiter_core * core) : PrologNativeOrbiter (atom, core, new lunar_core (core)) {
		audio_failed = false;
		printf ("..... creating moonbase\n");
		cores++;
		//audio = new MultiplatformAudio ();
		audio . setChannels (2);
		audio . setSamplingFrequency ((int) core -> sampling_frequency);
		audio . setLatencyBufferSize (core -> latency_block_size);
		if (core -> output_device >= 0) {
			audio . installOutputCallback (alpha_callback, this);
			if (! audio . selectOutputDevice (core -> output_device)) audio_failed = true;
		}
		if (core -> input_device >= 0) {
			audio . installInputCallback (beta_callback, this);
			if (! audio . selectInputDevice (core -> input_device)) audio_failed = true;
		}
		printf ("Moonbase prolog created.\n");
	}
	~ core_action (void) {
		printf (".... deleting moonbase\n");
		audio . selectInputDevice (-1);
		audio . selectOutputDevice (-1);
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
	double headroom_fraction = core -> headroom_fraction;
	pthread_mutex_lock (& core -> main_mutex);
	for (int ind = 0; ind < frames; ind++) {
		core -> propagate_signals ();
		core -> move_modules ();
		data -> insertStereo (((* moon) + (* left_moon)) * headroom_fraction, ((* moon) + (* right_moon)) * headroom_fraction);
	}
	pthread_mutex_unlock (& core -> main_mutex);
}

void beta_callback (int frames, AudioBuffers * data, void * source) {
	core_action * base = (core_action *) source;
	orbiter_core * core = base -> core;
	lunar_core * moon = (lunar_core *) base -> module;
	pthread_mutex_lock (& core -> main_mutex);
	for (int ind = 0; ind < frames; ind++) {
		moon -> line [moon -> line_write++] = data -> getStereoLeftRight ();
		moon -> line [moon -> line_write++] = data -> getStereoLeftRight ();
		if (moon -> line_write >= 16384) moon -> line_write = 0;
	}
	pthread_mutex_unlock (& core -> main_mutex);
}

bool core_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (parameters -> isEarth ()) {
		if (audio . outputFileActive ()) {audio . stopRecording (); return true;}
		int outputs = audio . getNumberOfOutputDevices ();
		printf ("Number of outputs = %i\n", outputs);
		for (int ind = 0; ind < outputs; ind++) printf ("	device %i [%s]\n", ind, audio . getOutputDeviceName (ind));
		int inputs = audio . getNumberOfInputDevices ();
		printf ("Number of inputs = %i\n", inputs);
		for (int ind = 0; ind < inputs; ind++) printf ("	device %i [%s]\n", ind, audio . getInputDeviceName (ind));
		return true;
	}
	PrologElement * atom = 0;
	PrologElement * file_name = 0;
	double centre_frequency = -1.0;
	double sampling_frequency = -1.0;
	double headroom_fraction = -1.0;
	int latency_block_size = -1;
	PrologElement * output_device = 0;
	PrologElement * input_device = 0;
	int requested_number_of_actives = -1;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isText ()) file_name = el;
		if (el -> isVar ()) atom = el;
		if (el -> isInteger ()) {
			if (centre_frequency < 0.0) centre_frequency = (double) el -> getInteger ();
			else if (sampling_frequency < 0.0) sampling_frequency = (double) el -> getInteger ();
			else if (latency_block_size < 0) latency_block_size = el -> getInteger ();
			else if (output_device == 0) output_device = el;
			else if (input_device == 0) input_device = el;
			else requested_number_of_actives = el -> getInteger ();
		}
		if (el -> isDouble ()) {
			if (centre_frequency < 0.0) centre_frequency = el -> getDouble ();
			else headroom_fraction = el -> getDouble ();
		}
		parameters = parameters -> getRight ();
	}
	if (atom == 0) {
		if (centre_frequency > 0.0) {
			audio . selectOutputFile (centre_frequency, file_name != 0 ? file_name -> getText () : 0);
			return true;
		}
		if (file_name != 0) {audio . stopRecording (file_name -> getText ()); return true;}
		return false;
	}
	if (cores > 0) return false;
	if (centre_frequency < 0.0) centre_frequency = core -> centre_frequency;
	if (sampling_frequency < 0.0) sampling_frequency = core -> sampling_frequency;
	if (headroom_fraction < 0.0) headroom_fraction = core -> headroom_fraction;
	if (latency_block_size < 16) latency_block_size = core -> latency_block_size;
	if (requested_number_of_actives < 0) requested_number_of_actives = core -> requested_active_size;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	core -> centre_frequency = centre_frequency;
	core -> sampling_frequency = (double) sampling_frequency;
	core -> headroom_fraction = headroom_fraction;
	core -> latency_block_size = latency_block_size;
	core -> output_device = output_device != 0 ? output_device -> getInteger () : 0;
	core -> input_device = input_device != 0 ? input_device -> getInteger () : 0;
	if (output_device != 0) core -> output_device = output_device -> getInteger ();
	if (input_device != 0) core -> input_device = input_device -> getInteger ();
	core -> requested_active_size = requested_number_of_actives;
	core -> recalculate ();
	core_action * machine = new core_action (atom -> getAtom (), core);
	if (machine -> audio_failed) {delete machine; return false;}
	if (atom -> getAtom () -> setMachine (machine)) return true;
	delete machine;
	return false;
}

core_class :: core_class (orbiter_core * core) {this -> core = core;}

