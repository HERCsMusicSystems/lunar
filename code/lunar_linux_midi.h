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
// This file was created on Thursday, 23rd October 2014, 17:46:09 PM. //
////////////////////////////////////////////////////////////////////////

#ifndef LUNAR_MIDI
#define LUNAR_MIDI

#include "prolog.h"

class midi_code : public PrologNativeCode {
public:
	unsigned char command;
	unsigned char channel;
	unsigned char v1, v2;
	int fd;
	pthread_t thread;
	bool should_continue;
	PrologAtom * atom;
	PrologAtom * callback;
	PrologRoot * root;
	PrologAtom * keyoff, * keyon, * polyaftertouch, * control, * programchange, * aftertouch, * pitch;
	PrologAtom * sysex, * timingclock, * start, * cont, * stop, * activesensing;
	void move (void);
	void one_parameter (void);
	void two_parameters (void);
	void many_parameter (void);
	bool code (PrologElement * parameters, PrologResolution * resolution);
	midi_code (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * callback, char * location);
	~ midi_code (void);
};

#endif
