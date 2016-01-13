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

/////////////////////////////////////////////////////////////////////
// This file was created on Monday, 21st July 2014 at 11:24:55 AM. //
/////////////////////////////////////////////////////////////////////

#include "lunar_operator.h"

//////////////////////
// LUNAR OSCILLATOR //
//////////////////////

int lunar_oscillator :: numberOfInputs (void) {return 5;}
char * lunar_oscillator :: inputName (int ind) {
	switch (ind) {
	case 0: return "FREQ"; break;
	case 1: return "AMP"; break;
	case 2: return "GAIN"; break;
	case 3: return "RATIO"; break;
	case 4: return "TRIGGER"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_oscillator :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & freq; break;
	case 1: return & amp; break;
	case 2: return & gain; break;
	case 3: return & ratio; break;
	case 4: return & trigger; break;
	default: break;
	}
	return 0;
}
lunar_oscillator :: lunar_oscillator (orbiter_core * core) : orbiter (core) {
	freq = amp = trigger = time = 0.0;
	ratio = 1.0; gain = 1.0;
}

///////////////////////
// LUNAR OPERATOR FM //
///////////////////////

int lunar_operator :: numberOfInputs (void) {return 6;}
char * lunar_operator :: inputName (int ind) {if (ind == 5) return "SHIFT"; return lunar_oscillator :: inputName (ind);}
double * lunar_operator :: inputAddress (int ind) {if (ind == 5) return & shift; return lunar_oscillator :: inputAddress (ind);}
void lunar_operator :: move (void) {
	if (trigger >= 16384.0) time = 0.0;
	this -> signal = gain * core -> Amplitude (amp) * core -> Sine (time + shift);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}
lunar_operator :: lunar_operator (orbiter_core * core) : lunar_oscillator (core) {shift = 0.0; initialise (); activate ();}

///////////////
// LUNAR SAW //
///////////////

void lunar_aliased_saw_operator :: move (void) {
	if (trigger >= 16384.0) time = 0.0;
	signal = 1.0 - 2.0 * time;
	signal *= gain * core -> Amplitude (amp);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}
lunar_aliased_saw_operator :: lunar_aliased_saw_operator (orbiter_core * core) : lunar_oscillator (core) {initialise (); activate ();}
void lunar_saw_operator :: move (void) {
	if (trigger >= 16384.0) time = 0.0;
	double delta = core -> TimeDelta (freq) * ratio;
	signal = core -> MinBlep (blep_index) - 1.0 - 2.0 * time;
	signal *= gain * core -> Amplitude (amp);
	time += delta;
	blep_index += 512;
	while (time >= 1.0) {time -= 1.0; blep_index = (int) (time * 512.0 / (delta > 0.0 ? delta : 1.0));}
}
lunar_saw_operator :: lunar_saw_operator (orbiter_core * core) : lunar_oscillator (core) {blep_index = 0;}
lunar_saw_operator * lunar_saw_operator :: create (orbiter_core * core) {
	lunar_saw_operator * op = new lunar_saw_operator (core);
	op -> initialise (); op -> activate ();
	return op;
}

//////////////////
// LUNAR SQUARE //
//////////////////

void lunar_aliased_square_operator :: move (void) {
	if (trigger >= 16384.0) time = 0.0;
	if (time < 0.5) signal = gain * core -> Amplitude (amp);
	else signal = - gain * core -> Amplitude (amp);
	time += core -> TimeDelta (freq) * ratio;
	while (time >= 1.0) time -= 1.0;
}
lunar_aliased_square_operator :: lunar_aliased_square_operator (orbiter_core * core) : lunar_oscillator (core) {initialise (); activate ();}
void lunar_square_operator :: move (void) {
	if (trigger >= 16384.0) time = 0.0;
	double delta = core -> TimeDelta (freq) * ratio;
	if (stage) signal = core -> MinBlep (blep_index) - 1.0;
	else signal = 1.0 - core -> MinBlep (blep_index);
	time += delta;
	blep_index += 512;
	if (stage && time > 0.5) {stage = false; blep_index = (int) ((time - 0.5) * 512.0 / (delta > 0.0 ? delta : 1.0));}
	while (time >= 1.0) {stage = true; time -= 1.0; blep_index = (int) (time * 512.0 / (delta > 0.0 ? delta : 1.0));}
	signal *= gain * core -> Amplitude (amp);
}
lunar_square_operator :: lunar_square_operator (orbiter_core * core) : lunar_saw_operator (core) {stage = true;}
lunar_square_operator * lunar_square_operator :: create (orbiter_core * core) {
	lunar_square_operator * op = new lunar_square_operator (core);
	op -> initialise (); op -> activate ();
	return op;
}

/////////
// VCO //
/////////

int vco_operator :: numberOfInputs (void) {return 6;}
char * vco_operator :: inputName (int ind) {if (ind == 5) return "WAVE"; return lunar_square_operator :: inputName (ind);}
double * vco_operator :: inputAddress (int ind) {if (ind == 5) return & wave; return lunar_square_operator :: inputAddress (ind);}
void vco_operator :: move (void) {
	switch ((int) wave) {
	case 1: lunar_saw_operator :: move (); break;
	case 2: lunar_square_operator :: move (); break;
	case 3: signal = gain * core -> Amplitude (amp) * (0.00000011920928955078125 * (double) core -> noise24b - 1.0); break;
	default:
		if (trigger >= 16384.0) time = 0.0;
		this -> signal = gain * core -> Amplitude (amp) * core -> Sine (time);
		time += core -> TimeDelta (freq) * ratio;
		while (time >= 1.0) time -= 1.0;
		break;
	}
}
vco_operator :: vco_operator (orbiter_core * core) : lunar_square_operator (core) {wave = 0.0; initialise (); activate ();}

///////////
// NOISE //
///////////

int noise_operator :: numberOfInputs (void) {return 2;}
char * noise_operator :: inputName (int ind) {
	switch (ind) {
	case 0: return "AMP"; break;
	case 1: return "GAIN"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * noise_operator :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & amp; break;
	case 1: return & gain; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
void noise_operator :: move (void) {signal = gain * core -> Amplitude (amp) * (0.00000011920928955078125 * (double) core -> noise24b - 1.0);}
noise_operator :: noise_operator (orbiter_core * core) : orbiter (core) {amp = 0.0; gain = 1.0; initialise (); activate ();}

///////////////////
// LUNAR SAMPLER //
///////////////////

int lunar_sampler_operator :: numberOfInputs (void) {return 6;}
char * lunar_sampler_operator :: inputName (int ind) {
	switch (ind) {
	case 0: return "FREQ"; break;
	case 1: return "AMP"; break;
	case 2: return "GAIN"; break;
	case 3: return "RATIO"; break;
	case 4: return "INDEX"; break;
	case 5: return "TRIGGER"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_sampler_operator :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & freq; break;
	case 1: return & amp; break;
	case 2: return & gain; break;
	case 3: return & ratio; break;
	case 4: return & index; break;
	case 5: return & trigger; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}
int lunar_sampler_operator :: numberOfOutputs (void) {return 3;}
char * lunar_sampler_operator :: outputName (int ind) {
	switch (ind) {
	case 0: return "LEFT"; break;
	case 1: return "RIGHT"; break;
	case 2: return "BUSY"; break;
	default: break;
	}
	return orbiter :: outputName (ind);
}
double * lunar_sampler_operator :: outputAddress (int ind) {
	switch (ind) {
	case 0: return orbiter :: outputAddress (0);
	case 1: return & signal_right; break;
	case 2: return & busy; break;
	default: break;
	}
	return orbiter :: outputAddress (ind);
}
void lunar_sampler_operator :: install_wave (lunar_wave * wave) {
	if (this -> wave != 0) this -> wave -> release ();
	wave -> hold ();
	this -> wave = wave;
}
bool lunar_sampler_operator :: release (void) {
	lunar_wave * wp = wave;
	bool ret = orbiter :: release ();
	if (ret && wp != 0) wp -> release ();
	return ret;
}
void lunar_sampler_operator :: move (void) {
	if (trigger >= 16384.0) {time = 0.0; busy = 1.0; return_possible = true;}
	if (time < -1.0) {if (trigger > 0.0) return; time = -1.0;}
	if (time < 0.0) {if (trigger <= 0.0) return; time = 0.0; busy = 1.0; return_possible = true;}
	if (wave == 0) {signal = signal_right = 0.0; time = -1.0; busy = 0.0; return;}
	int ind = (int) index;
	if (ind < 0 || ind >= wave -> capacity) {signal = signal_right = 0.0; time = -1.0; busy = 0.0; return;}
	wave_data * data = wave -> waves [ind];
	if (data == 0) {signal = signal_right = 0.0; time = -1.0; busy = 0.0; return;}
	if ((int) time >= data -> wave_size) {signal = signal_right = 0.0; time = -2.0; busy = 0.0; return;}
	int channels = data -> channels;
	if (channels < 1) {signal = signal_right = 0.0; time = -1.0; busy = 0.0; return;}
	double amplitude = gain * core -> Amplitude (amp);
	if (channels > 0) signal = data -> get_data (0, time) * amplitude;
	signal_right = channels > 1 ? data -> get_data (1, time) * amplitude : signal;
	time += core -> SamplerTimeDelta (freq) * ratio * data -> sampling_freq;
	// The start point and end point should be identical. The end point is NOT meant to be played.
	while (time >= data -> loop_point && return_possible) {time -= data -> loop_size; if (trigger <= 0.0) return_possible = false;}
}
lunar_sampler_operator :: lunar_sampler_operator (orbiter_core * core) : orbiter (core) {
	time = -1.0;
	freq = amp = index = trigger = 0.0;
	gain = 1.0;
	ratio = 1.0; signal_right = signal;
	busy = 0.0;
	wave = 0;
	return_possible = true;
	initialise (); activate ();
}

//////////
// FM 4 //
//////////

int lunar_fm4_block :: numberOfInputs (void) {return 22;}
char * lunar_fm4_block :: inputName (int ind) {
	switch (ind) {
	case 0: return "ALGO"; break;
	case 1: return "TRIGGER"; break;
	case 2: return "FREQ1"; break;
	case 3: return "AMP1"; break;
	case 4: return "GAIN1"; break;
	case 5: return "RATIO1"; break;
	case 6: return "FEEDBACK1"; break;
	case 7: return "FREQ2"; break;
	case 8: return "AMP2"; break;
	case 9: return "GAIN2"; break;
	case 10: return "RATIO2"; break;
	case 11: return "FEEDBACK2"; break;
	case 12: return "FREQ3"; break;
	case 13: return "AMP3"; break;
	case 14: return "GAIN3"; break;
	case 15: return "RATIO3"; break;
	case 16: return "FEEDBACK3"; break;
	case 17: return "FREQ4"; break;
	case 18: return "AMP4"; break;
	case 19: return "GAIN4"; break;
	case 20: return "RATIO4"; break;
	case 21: return "FEEDBACK4"; break;
	default: break;
	}
	return orbiter :: inputName (ind);
}
double * lunar_fm4_block :: inputAddress (int ind) {
	switch (ind) {
	case 0: return & current_algo; break;
	case 1: return & trigger; break;
	case 2: return & freq1; break;
	case 3: return & amp1; break;
	case 4: return & gain1; break;
	case 5: return & ratio1; break;
	case 6: return & feedback1; break;
	case 7: return & freq2; break;
	case 8: return & amp2; break;
	case 9: return & gain2; break;
	case 10: return & ratio2; break;
	case 11: return & feedback2; break;
	case 12: return & freq3; break;
	case 13: return & amp3; break;
	case 14: return & gain3; break;
	case 15: return & ratio3; break;
	case 16: return & feedback3; break;
	case 17: return & freq4; break;
	case 18: return & amp4; break;
	case 19: return & gain4; break;
	case 20: return & ratio4; break;
	case 21: return & feedback4; break;
	default: break;
	}
	return orbiter :: inputAddress (ind);
}

#define FRAC 0.00006103515625
double algo1 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> feedback3 * block -> signal3 * FRAC);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> feedback1 * block -> signal1 * FRAC);
	return block -> signal1 + block -> signal2 + block -> signal3 + block -> signal4;
}
double algo2 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> feedback1 * block -> signal1 * FRAC);
	return block -> signal1 + block -> signal2 + block -> signal3;
}
double algo3 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> signal4);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal4);
	return block -> signal1 + block -> signal2 + block -> signal3;
}
double algo4 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2);
	return block -> signal1 + block -> signal3;
}
double algo5 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> feedback3 * block -> signal3 * FRAC);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) *
														block -> core -> Sine (block -> time1 + block -> signal2 + block -> signal3 + block -> signal4);
}
double algo6 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> feedback2 * block -> signal2 * FRAC);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2 + block -> signal3);
}
double algo7 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> feedback3 * block -> signal3 * FRAC);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> signal3 + block -> signal4);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2);
}
double algo8 (lunar_fm4_block * block) {
	block -> signal4 = block -> gain4 * block -> core -> Amplitude (block -> amp4) * block -> core -> Sine (block -> time4 + block -> feedback4 * block -> signal4 * FRAC);
	block -> signal3 = block -> gain3 * block -> core -> Amplitude (block -> amp3) * block -> core -> Sine (block -> time3 + block -> signal4);
	block -> signal2 = block -> gain2 * block -> core -> Amplitude (block -> amp2) * block -> core -> Sine (block -> time2 + block -> signal3);
	return block -> signal1 = block -> gain1 * block -> core -> Amplitude (block -> amp1) * block -> core -> Sine (block -> time1 + block -> signal2);
}

void lunar_fm4_block :: move (void) {
	if (current_algo != previous_algo) {
		switch ((int) current_algo) {
		case 0: algo = algo1; break;
		case 1: algo = algo2; break;
		case 2: algo = algo3; break;
		case 3: algo = algo4; break;
		case 4: algo = algo5; break;
		case 5: algo = algo6; break;
		case 6: algo = algo7; break;
		case 7: algo = algo8; break;
		default: algo = algo1; break;
		}
		previous_algo = current_algo;
	}
	if (trigger >= 16384.0) time1 = time2 = time3 = time4 = 0.0;
	signal = algo (this);
	time1 += core -> TimeDelta (freq1) * ratio1;
	time2 += core -> TimeDelta (freq2) * ratio2;
	time3 += core -> TimeDelta (freq3) * ratio3;
	time4 += core -> TimeDelta (freq4) * ratio4;
	while (time1 >= 1.0) time1 -= 1.0;
	while (time2 >= 1.0) time2 -= 1.0;
	while (time3 >= 1.0) time3 -= 1.0;
	while (time4 >= 1.0) time4 -= 1.0;
}

lunar_fm4_block :: lunar_fm4_block (orbiter_core * core) : orbiter (core) {
	algo = algo1;
	current_algo = previous_algo = 0.0;
	trigger = 0.0;
	signal1 = signal2 = signal3 = signal4 = 0.0;
	time1 = time2 = time3 = time4 = 0.0;
	freq1 = freq2 = freq3 = freq4 = 0.0;
	amp1 = 0.0; amp2 = amp3 = amp4 = -16384.0;
	gain1 = gain2 = gain3 = gain4 = 1.0;
	ratio1 = ratio2 = ratio3 = ratio4 = 1.0;
	feedback1 = feedback2 = feedback3 = feedback4 = 0.0;
	initialise (); activate ();
}
