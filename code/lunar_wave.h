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

////////////////////////////////////////////////////////////////////
// This file was created on Thursday, 21st August at 14:11:10 PM. //
////////////////////////////////////////////////////////////////////

#ifndef _LUNAR_WAVE_
#define _LUNAR_WAVE_

#include "lunar.h"

class wave_data {
public:
	double sampling_freq;
	double loop_point;
	double loop_size;
	double midi_note;
	int channels;
	int wave_size;
	double * * data;
	double get_data (int channel, double time);
	wave_data (int sampling_freq, int channels, int wave_size);
	~ wave_data (void);
};

class lunar_wave : public orbiter {
public:
	int capacity;
	wave_data * * waves;
	virtual int numberOfOutputs (void);
	lunar_wave (orbiter_core * core, int capacity);
	~ lunar_wave (void);
};

extern wave_data * create_lunar_wave_data (char * file_name, double gain);
extern bool loop_wave (char * source, char * destination, int start, int stop);

#endif

