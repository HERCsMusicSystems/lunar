///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2015 Robert P. Wolf                       //
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

/////////////////////////////////////////////////////////////////////////
// This file was created on Monday, 14th December 2015 at 11:37:02 AM. //
/////////////////////////////////////////////////////////////////////////

#ifndef _LUNAR_INTEGRATED_COMPONENTS_
#define _LUNAR_INTEGRATED_COMPONENTS_

#include "lunar.h"
#include "prolog.h"

class integrated_vco {
private:
	int blep_index;
	bool stage;
	double time;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal;
	// ==== INPUT ====
	double freq;
	double amp;
	double shift;
	double ratio;
	double trigger;
	double wave; // 0 = sine, 1 = saw, 2 = square, 3 = noise
	// ==== PROCESSING ====
	void move_sine (void);
	void move_square (void);
	void move_sawtooth (void);
	void move_noise (void);
	void move (void);
	integrated_vco (orbiter_core * core);
};

class integrated_lfo {
private:
	bool stage_one;
	double origin, target, delta;
	double time;
	double trigger, previous_trigger;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, positive, negative;
	double vibrato_signal, tremolo_signal, wahwah_signal, pan_signal;
	// ==== INPUT ====
	double speed, wave, pulse, phase, sync;
	double vibrato, tremolo, wahwah, pan;
	// ==== PROCESSING ====
	void move (void);
	integrated_lfo (orbiter_core * core);
};

class integrated_map {
public:
	double map [128];
	int initial;
	void reset (void);
	bool return_content (PrologElement * parameters);
	bool read_content (PrologElement * parameters);
	integrated_map (int initial = -64);
};

class integrated_trigger {
private:
	int request; // 0 = none, 1 = keyoff (key), 2 = keyoff (), 3 = keyon (key), 4 = keyon (key, vel), 5 = ground (key, vel, base, prev);
	int request_key, request_velocity, request_base, request_previous;
	int keystack [16];
	int keystack_pointer;
	double target, origin, delta;
	bool active;
	int key;
	double time;
	pthread_mutex_t critical;
	orbiter_core * core;
	integrated_trigger * next;
	void add_stack (int key);
	void drop_stack (int key);
	void sub_keyon (int key);
	void sub_velocity (int velocity);
	void keyon_velocity_request (void);
	void ground_request (void);
	void keyoff_request (void);
	void keyoff_all_request (void);
public:
	// ==== OUTPUT ====
	double signal, velocity, trigger, index;
	// ==== INPUT ====
	integrated_map * key_map;
	integrated_map * velocity_map;
	double busy, hold, porta, porta_time, legato;
	// ==== PROCESSING ====
	void keyon (int key);
	void keyon (int key, int velocity);
	void ground (int key, int velocity, int base, int previous);
	void keyoff (int key);
	void keyoff (void);
	void move (void);
	integrated_trigger (orbiter_core * core, bool active, integrated_trigger * trigger);
	~ integrated_trigger (void);
};

class integrated_adsr {
private:
	int stage;
	double threshold;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, busy;
	// ==== INPUPT ====
	double trigger;
	double attack, decay, sustain, release;
	// ==== PROCESSING ====
	void move (void);
	integrated_adsr (orbiter_core * core);
};

class integrated_pan {
private:
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double left, right;
	// ==== INPUT ====
	double enter, pan;
	// ==== PROCESSING ====
	void move (void);
	void power_move (void);
	void linear_move (void);
	integrated_pan (orbiter_core * core);
};

class integrated_stereo_pan {
private:
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double left, right;
	// ==== INPUT ====
	double enter_left, enter_right, pan;
	// ==== PROCESSING ====
	void move (void);
	void power_move (void);
	void linear_move (void);
	integrated_stereo_pan (orbiter_core * core);
};

class integrated_stereo_amplifier {
private:
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double left, right;
	// ==== INPUT ====
	double enter_left, enter_right, gateway;
	// ==== PROCESSING ====
	void amplify_move (void);
	void control_move (void);
	void gateway_move (void);
	void volume_move (void);
	integrated_stereo_amplifier (orbiter_core * core, double gateway);
};

class integrated_mono_amplifier {
private:
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double enter;
	// ==== INPUT ====
	double signal, gateway;
	// ==== PROCESSING ====
	void amplify_move (void);
	void control_move (void);
	void gateway_move (void);
	void volume_move (void);
	integrated_mono_amplifier (orbiter_core * core, double gateway);
};

class integrated_delay {
private:
	double previous_high_damp;
	double A, B;
	double left_1, right_1;
	int index;
	double line [262144];
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, signal_right;
	// ==== INPUT ====
	double enter, enter_right;
	double feedback, time, high_damp;
	// ==== PROCESSING ====
	void move (void);
	integrated_delay (orbiter_core * core);
};

class integrated_drywet {
private:
public:
	// ==== OUTPUT ====
	double left, right;
	// ==== INPUT ====
	double dry_left, dry_right, wet_left, wet_right, balance;
	// ==== PROCESSING ====
	void move (void);
	integrated_drywet (void);
};

class integrated_mono_drywet {
private:
public:
	// ==== OUTPUT ====
	double signal;
	// ==== INPUT ====
	double dry, wet, balance;
	// ==== PROCESSING ====
	void move (void);
	integrated_mono_drywet (void);
};

#endif
