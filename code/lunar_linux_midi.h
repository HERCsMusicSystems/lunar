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
#include "chromatic.h"

class midi_code : public PrologNativeCode {
public:
	unsigned char command;
	unsigned char channel;
	unsigned char v1, v2;
#ifdef WIN32
	HMIDIOUT out_port_handle;
	HMIDIIN in_port_handle;
	bool out_port_active;
	bool in_port_active;
	bool drop_system_exclusive (PrologElement * parameters);
	MIDIHDR hdr1, hdr2;
	unsigned char sxb1 [1024], sxb2 [1024];
#else
	int fd;
	int tc;
#endif
	int running_command;
	pthread_t thread;
	bool should_continue;
	PrologAtom * atom;
	PrologAtom * callback;
	PrologRoot * root;
	PrologAtom * keyoff, * keyon, * polyaftertouch, * control, * programchange, * aftertouch, * pitch;
	PrologAtom * sysex, * timingclock, * start, * cont, * stop, * activesensing;
	pthread_mutex_t locker;
	chromatograph graph;
	void run (void);
	void no_parameters (void);
	void one_parameter (void);
	void two_parameters (void);
	void many_parameter (void);
	void sysex_parameters (unsigned char * data, int size);
	void send_one (int command);
	void send_two (int command, int key);
	void send_three (int command, int key, int velocity);
	bool code (PrologElement * parameters, PrologResolution * resolution);
	bool inactive (void);
	static bool info (void);
	midi_code (PrologRoot * root, PrologDirectory * directory, PrologAtom * atom, PrologAtom * callback, char * location);
	~ midi_code (void);
};

#endif

