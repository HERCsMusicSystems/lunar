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
double wave_data :: get_data (int channel, double time) {
	int ind = (int) time;
	time -= (double) ind;
	double * dp = data [channel] + ind;
	return * dp * (1.0 - time) + * (dp + 1) * time;
}
wave_data :: wave_data (int sampling_freq, int channels, int wave_size) {
	this -> sampling_freq = (double) sampling_freq;
	this -> loop_point = (double) wave_size + this -> sampling_freq;
	this -> midi_note = 64.0;
	this -> loop_size = 0.0;
	this -> channels = channels;
	this -> wave_size = wave_size;
	data = new double_pointer [channels];
	for (int ind = 0; ind < channels; ind++) data [ind] = new double [wave_size + 16];
}
wave_data :: ~ wave_data (void) {
	if (data != 0) {for (int ind = 0; ind < channels; ind++) {delete [] data [ind]; data [ind] = 0;}}
	delete [] data; data = 0;
}

typedef wave_data * wave_data_pointer;
int lunar_wave :: numberOfOutputs (void) {return 0;}
lunar_wave :: lunar_wave (orbiter_core * core, int capacity) : orbiter (core) {
	if (capacity < 0) capacity = 0;
	this -> capacity = capacity;
	if (capacity < 1) waves = 0;
	else {
		waves = new wave_data_pointer [capacity];
		for (int ind = 0; ind < capacity; ind++) waves [ind] = 0;
	}
}
lunar_wave :: ~ lunar_wave (void) {
	if (waves == 0) return;
	for (int ind = 0; ind < capacity; ind++) {if (waves [ind] != 0) delete waves [ind]; waves [ind] = 0;}
	delete [] waves; waves = 0;
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
wave_data * create_lunar_wave_data (char * file_name, double gain) {
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
	//printf ("wave [%s %li]\n", command, riff_size);
	short int format_tag = 0;
	short int channels = 0;
	long int samples_per_second = 0;
	long int bytes_per_second = 0;
	short int block_align = 0;
	short int bits_per_sample = 0;
	wave_data * wave = 0;
	while (riff_size > 0) {
		if (! read_id (fr, command)) fail; riff_size -= 4; if (riff_size < 4) fail;
		long int chunk_size;
		if (! read4 (fr, & chunk_size)) fail; riff_size -= 4; if (riff_size < 4) fail;
		//printf ("chunk [%s %li] at [%li]\n", command, chunk_size, riff_size);
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
			//printf ("format [%i %i %li %li %i %i]\n", format_tag, channels, samples_per_second, bytes_per_second, block_align, bits_per_sample);
		} else if (strcmp (command, "data") == 0) {
			if (channels < 1 || block_align < 1 || chunk_size < block_align) fail;
			int wave_size = chunk_size / block_align;
			wave = new wave_data (samples_per_second, channels, wave_size);
			int index = 0;
			double sample;
			while (chunk_size > 0) {
				for (int channel = 0; channel < channels; channel++) {
					short short_sample;
					char char_sample;
					switch (bits_per_sample) {
					case 8:
						if (! read1 (fr, & char_sample)) fail;
						wave -> data [channel] [index] = sample = gain * ((double) char_sample - 128.0) / 128.0;
						chunk_size--;
						break;
					case 16:
						if (! read2 (fr, & short_sample)) fail;
						wave -> data [channel] [index] = sample = gain * (double) short_sample / 32768.0;
						chunk_size -= 2;
						break;
					default: fail; break;
					}
				}
				index++;
			}
			for (int channel = 0; channel < channels; channel++) wave -> data [channel] [index] = sample;
			index++;
		} else if (strcmp (command, "smpl") == 0) {
			if (chunk_size < 60) fail;
			long int manufacturer; if (! read4 (fr, & manufacturer)) fail;
			long int product; if (! read4 (fr, & product)) fail;
			long int sample_period; if (! read4 (fr, & sample_period)) fail;
			long int unity_note; if (! read4 (fr, & unity_note)) fail;
			wave -> midi_note = (double) unity_note;
			long int pitch_fraction; if (! read4 (fr, & pitch_fraction)) fail;
			long int format; if (! read4 (fr, & format)) fail;
			long int offset; if (! read4 (fr, & offset)) fail;
			long int sample_loops; if (! read4 (fr, & sample_loops)) fail;
			long int sampler_data; if (! read4 (fr, & sampler_data)) fail;
			chunk_size -= 36;
			//printf ("........... got sampler [%li %li]\n", unity_note, sample_loops);
			while (chunk_size-- > 0) {
				long int dw_identifier; if (! read4 (fr, & dw_identifier)) fail;
				long int dw_type; if (! read4 (fr, & dw_type)) fail;
				long int dw_start; if (! read4 (fr, & dw_start)) fail;
				long int dw_end; if (! read4 (fr, & dw_end)) fail;
				long int dw_fraction; if (! read4 (fr, & dw_fraction)) fail;
				long int dw_play_count; if (! read4 (fr, & dw_play_count)) fail;
				chunk_size -= 24;
				//printf ("................... got loop [%li %li %li]\n", dw_type, dw_start, dw_end);
				if (dw_type == 0) {wave -> loop_point = (double) dw_end; wave -> loop_size = (double) (dw_end - dw_start);}
			}
		} else {while (chunk_size-- > 0) fgetc (fr);}
	}
	fclose (fr);
	return wave;
}

static void write_id (FILE * tc, char * command) {
	fputc (command [0], tc);
	fputc (command [1], tc);
	fputc (command [2], tc);
	fputc (command [3], tc);
}

static void write4 (FILE * tc, long int id) {
	fputc (id & 0xff, tc);
	fputc ((id >> 8) & 0xff, tc);
	fputc ((id >> 16) & 0xff, tc);
	fputc ((id >> 24) & 0xff, tc);
}

#define Fail {fclose (fr); fclose (tc); return false;}
bool loop_wave (char * source, char * destination, int start, int stop) {
	if (source == 0 && destination == 0) return false;
	if (start > stop) {int ind = start; start = stop; stop = ind;}
	if (start < 0) start = 0; if (stop < 0) stop = 0;
	FILE * fr = 0;
	FILE * tc = 0;
	fr = fopen (source, "rb");
	if (! fr) {
		fr = fopen (destination, "rb");
		if (! fr) return false;
		destination = source;
	}
	char command [8];
	if (! read_id (fr, command)) {fclose (fr); return false;}
	if (strcmp (command, "RIFF") != 0) {fclose (fr); return false;}
	long int riff_size;
	if (! read4 (fr, & riff_size)) {fclose (fr); return false;}
	if (riff_size < 4) {fclose (fr); return false;}
	tc = fopen (destination, "wb"); if (! tc) {fclose (fr); return false;}
	write_id (tc, command);
	write4 (tc, riff_size + 68);
	if (! read_id (fr, command)) Fail; riff_size -= 4; write_id (tc, command);
	while (riff_size-- > 0) fputc (fgetc (fr), tc);
	write_id (tc, "smpl<"); write4 (tc, 60);
	write4 (tc, 0); write4 (tc, 0); write4 (tc, 0); write4 (tc, 60);
	write4 (tc, 0); write4 (tc, 0); write4 (tc, 0); write4 (tc, 1); write4 (tc, 0);
	write4 (tc, 0); write4 (tc, 0); write4 (tc, start); write4 (tc, stop); write4 (tc, 0); write4 (tc, 0);
	fclose (fr);
	fclose (tc);
	return true;
}

