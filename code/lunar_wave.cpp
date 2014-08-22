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
// This file was created on Thursday, 21st August at 14:11:22 PM. //
////////////////////////////////////////////////////////////////////

#include "lunar_wave.h"
#include <stdio.h>
#include <string.h>

typedef double * double_pointer;
int lunar_wave :: numberOfOutputs (void) {return 0;}
lunar_wave :: lunar_wave (orbiter_core * core, int sampling_freq, int channels, int wave_size) : orbiter (core) {
	this -> sampling_freq = (double) sampling_freq;
	this -> channels = channels;
	this -> wave_size = wave_size;
	data = new double_pointer [channels];
	for (int ind = 0; ind < channels; ind++) data [ind] = new double [wave_size + 16];
}
lunar_wave :: ~ lunar_wave (void) {
	if (data != 0) {for (int ind = 0; ind < channels; ind++) {delete [] data [ind]; data [ind] = 0;}};
	delete [] data; data = 0;
}

static bool read_id (FILE * fr, char * id) {
	for (int ind = 0; ind < 4; ind++) {
		int c = fgetc (fr);
		if (c < 0) {id [ind] = '\0'; return false;}
		id [ind] = (char) c;
	} id [4] = '\0'; return true;
}
static bool read1 (FILE * fr, char * ind) {
	* ind = 0;
	int c = fgetc (fr); if (c < 0) return false; * ind = (char) c;
	return true;
}
static bool read2 (FILE * fr, short int * ind) {
	* ind = 0;
	int c = fgetc (fr); if (c < 0) return false; * ind |= c;
	c = fgetc (fr); if (c < 0) return false; * ind |= c << 8;
	return true;
}
static bool read4 (FILE * fr, long int * ind) {
	* ind = 0;
	int c = fgetc (fr); if (c < 0) return false; * ind |= c;
	c = fgetc (fr); if (c < 0) return false; * ind |= c << 8;
	c = fgetc (fr); if (c < 0) return false; * ind |= c << 16;
	c = fgetc (fr); if (c < 0) return false; * ind |= c << 24;
	return true;
}

#define fail {fclose (fr); return 0;}
lunar_wave * create_lunar_wave (orbiter_core * core, char * file_name) {
	if (file_name == 0) return 0;
	FILE * fr = fopen (file_name, "rb");
	if (! fr) return 0;
	char command [8];
	if (! read_id (fr, command)) fail;
	if (strcmp (command, "RIFF") != 0) fail;
	long int riff_size;
	if (! read4 (fr, & riff_size)) fail;
	if (riff_size < 4) fail;
	if (! read_id (fr, command)) fail; riff_size -= 4; if (riff_size < 4) fail;
	if (strcmp (command, "WAVE") != 0) fail;
	printf ("wave [%s %i]\n", command, riff_size);
	short int format_tag = 0;
	short int channels = 0;
	long int samples_per_second = 0;
	long int bytes_per_second = 0;
	short int block_align = 0;
	short int bits_per_sample = 0;
	lunar_wave * wave = 0;
	while (riff_size > 0) {
		if (! read_id (fr, command)) fail; riff_size -= 4; if (riff_size < 4) fail;
		long int chunk_size;
		if (! read4 (fr, & chunk_size)) fail; riff_size -= 4; if (riff_size < 4) fail;
		printf ("chunk [%s %i] at [%i]\n", command, chunk_size, riff_size);
		if (chunk_size < 0 || chunk_size > riff_size) fail;
		riff_size -= chunk_size;
		if (strcmp (command, "fmt ") == 0) {
			if (chunk_size != 16) fail;
			if (! read2 (fr, & format_tag)) fail;
			if (! read2 (fr, & channels)) fail;
			if (! read4 (fr, & samples_per_second)) fail;
			if (! read4 (fr, & bytes_per_second)) fail;
			if (! read2 (fr, & block_align)) fail;
			if (! read2 (fr, & bits_per_sample)) fail;
			printf ("format [%i %i %i %i %i %i]\n", format_tag, channels, samples_per_second, bytes_per_second, block_align, bits_per_sample);
		} else if (strcmp (command, "data") == 0) {
			if (channels < 1 || block_align < 1 || chunk_size < block_align) fail;
			int wave_size = chunk_size / block_align;
			wave = new lunar_wave (core, samples_per_second, channels, wave_size);
			int index = 0;
			while (chunk_size > 0) {
				for (int channel = 0; channel < channels; channel++) {
					short short_sample;
					char char_sample;
					switch (bits_per_sample) {
					case 8:
						if (! read1 (fr, & char_sample)) fail;
						wave -> data [channel] [index] = ((double) char_sample - 128.0) / 128.0;
						chunk_size--;
						break;
					case 16:
						if (! read2 (fr, & short_sample)) fail;
						wave -> data [channel] [index] = (double) short_sample / 32768.0;
						chunk_size -= 2;
						break;
					default: fail; break;
					}
				}
				index++;
			}
			index++;
		} else if (strcmp (command, "smpl") == 0) {
			while (chunk_size-- > 0) fgetc (fr);
		} else {while (chunk_size-- > 0) fgetc (fr);}
	}
	fclose (fr);
	return wave;
}

