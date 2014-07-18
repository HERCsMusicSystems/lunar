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
#include <stdio.h>
#include "multiplatform_audio.h"


class moonbase_alpha;
extern void alpha_callback (int frames, AudioBuffers * data, void * source);

class moonbase_alpha : public moonbase {
public:
	MultiplatformAudio * audio;
	moonbase_alpha (orbiter_core * core) : moonbase (core) {
		audio = new MultiplatformAudio (2, core -> sampling_frequency, core -> latency_block_size);
		int outputs = audio -> getNumberOfOutputDevices ();
		printf ("Number of outputs = %i\n", outputs);
		for (int ind = 0; ind < outputs; ind++) printf ("	device [%s]\n", audio -> getOutputDeviceName (ind));
		audio -> installOutputCallback (alpha_callback, this);
		audio -> selectOutputDevice (0);
	}
	~ moonbase_alpha (void) {
		delete audio;
		printf ("Moonbase ALPHA destroyed.\n");
	}
};

void alpha_callback (int frames, AudioBuffers * data, void * source) {
	moonbase_alpha * alpha = (moonbase_alpha *) source;
	for (int ind = 0; ind < frames; ind++) data -> insertMono (0.0);
}

moonbase :: moonbase (orbiter_core * core) : orbiter (core) {}

moonbase * create_moonbase (orbiter_core * core) {return new moonbase_alpha (core);}
