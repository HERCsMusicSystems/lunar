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

#include "lunar_moonbase.h"
#include "prolog_lunar.h"
#include "multiplatform_audio.h"

static int moonbases = 0;

void alpha_callback (int frames, AudioBuffers * data, void * source) {
	orbiter_core * core = (orbiter_core *) source;
	pthread_mutex_lock (& core -> maintenance_mutex);
	pthread_mutex_lock (& core -> main_mutex);
	for (int ind = 0; ind < frames; ind++) {
		core -> move_modules ();
		core -> propagate_signals ();
		data -> insertMono (0.0);
	}
	pthread_mutex_unlock (& core -> main_mutex);
	pthread_mutex_unlock (& core -> maintenance_mutex);
}

class moonbase_action : public PrologNativeOrbiter {
public:
	MultiplatformAudio * audio;
	moonbase_action (PrologAtom * atom, orbiter_core * core) : PrologNativeOrbiter (atom, core, new moonbase (core)) {
		printf ("..... creating moonbase\n");
		moonbases++;
		audio = new MultiplatformAudio (2, (int) core -> sampling_frequency, core -> latency_block_size);
		int outputs = audio -> getNumberOfOutputDevices ();
		printf ("Number of outputs = %i\n", outputs);
		for (int ind = 0; ind < outputs; ind++) printf ("	device [%s]\n", audio -> getOutputDeviceName (ind));
		audio -> installOutputCallback (alpha_callback, core);
		audio -> selectOutputDevice (0);
		printf ("Moonbase prolog created.\n");
	}
	~ moonbase_action (void) {
		printf (".... deleting moonbase\n");
		delete audio;
		moonbases--;
		printf ("Moonbase prolog destroyed.\n");
	}
};

bool moonbase_class :: code (PrologElement * parameters, PrologResolution * resolution) {
	if (moonbases > 0) return false;
	PrologElement * atom = 0;
	double sampling_frequency = -1;
	int latency_block_size = -1;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isAtom ()) atom = el;
		if (el -> isVar ()) atom = el;
		if (el -> isInteger ()) {
			if (sampling_frequency < 0.0) sampling_frequency = (double) el -> getInteger ();
			else latency_block_size = el -> getInteger ();
		}
		if (el -> isDouble ()) sampling_frequency = el -> getInteger ();
		parameters = parameters -> getRight ();
	}
	if (sampling_frequency < 0.0) sampling_frequency = 44100;
	if (latency_block_size < 16) latency_block_size = 512;
	if (atom == 0) return false;
	if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
	if (! atom -> isAtom ()) return false;
	if (atom -> getAtom () -> getMachine () != 0) return false;
	core -> sampling_frequency = sampling_frequency;
	core -> latency_block_size = latency_block_size;
	moonbase_action * machine = new moonbase_action (atom -> getAtom (), core);
	if (atom -> getAtom () -> setMachine (machine)) return true;
	delete machine;
	return false;
}

moonbase_class :: moonbase_class (orbiter_core * core) {this -> core = core;}

