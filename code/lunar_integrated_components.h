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

class integrated_noise {
private:
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal;
	// ==== INPUT ====
	double amp, gain;
	// ==== PROCESSING ====
	void move (void);
	integrated_noise (orbiter_core * core);
};

double integrated_sensitivity (double breakpoint, double left, double right, double enter, double gain = 0.0078125);

double integrated_morph (double x, double * vector);
double integrated_morph (double x, double y, double * vector);
double integrated_morph (double x, double y, double z, double * vector);

class integrated_fm4_block {
private:
	double (* current_algo) (integrated_fm4_block * block);
	double previous_algo;
	double time1, time2, time3, time4;
	double signal1, signal2, signal3, signal4;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal;
	// ==== INPUT ====
	double trigger;
	double algo;
	double freq1, freq2, freq3, freq4;
	double amp1, amp2, amp3, amp4;
	double gain1, gain2, gain3, gain4;
	double ratio1, ratio2, ratio3, ratio4;
	double feedback1, feedback2, feedback3, feedback4;
	// ==== PROCESSING ====
	void move (void);
	integrated_fm4_block (orbiter_core * core);
	friend double algo1 (integrated_fm4_block * block);
	friend double algo2 (integrated_fm4_block * block);
	friend double algo3 (integrated_fm4_block * block);
	friend double algo4 (integrated_fm4_block * block);
	friend double algo5 (integrated_fm4_block * block);
	friend double algo6 (integrated_fm4_block * block);
	friend double algo7 (integrated_fm4_block * block);
	friend double algo8 (integrated_fm4_block * block);
};

class integrated_filter {
private:
	double running_high_pass_signal, running_band_pass_signal, running_signal;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, high_pass_signal, band_pass_signal, band_reject_signal;
	// ==== INPUT ====
	double enter, freq, resonance, amp;
	// ==== PROCESSING ====
	void move (void);
	integrated_filter (orbiter_core * core);
};

class integrated_lfo {
private:
	bool stage_one;
	double origin, target, delta;
	double time;
	double previous_trigger;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, positive, negative;
	double vibrato_signal, tremolo_signal, wahwah_signal, pan_signal;
	// ==== INPUT ====
	double speed, wave, pulse, phase, sync;
	double vibrato, tremolo, wahwah, pan;
	double trigger;
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
public:
	int request; // 0 = none, 1 = keyoff (key), 2 = keyoff (), 3 = keyon (key), 4 = keyon (key, vel), 5 = ground (key, vel, base, prev);
private:
	int request_key, request_velocity, request_base, request_previous;
	int keystack [16];
	int keystack_pointer;
	double target, origin, delta;
	bool active;
	double time;
	pthread_mutex_t critical;
	orbiter_core * core;
	void add_stack (int key);
	void drop_stack (int key);
	void sub_keyon (int key);
	void sub_velocity (int velocity);
	void keyon_velocity_request (void);
	void ground_request (void);
	void keyoff_request (void);
	void keyoff_all_request (void);
public:
	int key;
	integrated_trigger * next;
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

class IntegratedCommandModule {
public:
	virtual bool insert_trigger (integrated_trigger * trigger) = 0;
	virtual bool insert_controller (double * controller, int location, double shift) = 0;
	virtual void keyon (int key) = 0;
	virtual void keyon (int key, int velocity) = 0;
	virtual void keyoff (void) = 0;
	virtual void keyoff (int key, int velocity = 0) = 0;
	virtual void mono (void) = 0;
	virtual void poly (void) = 0;
	virtual bool isMonoMode (void) = 0;
	virtual void control (int ctrl, double value) = 0;
	virtual double getControl (int ctrl) = 0;
	virtual void timing_clock (void) = 0;
};

class integrated_moonbase : public IntegratedCommandModule {
private:
	integrated_trigger * triggers;
	integrated_trigger * choice;
	integrated_trigger * select (void);
	integrated_trigger * select (int key);
	double * controllers [129];
	double ctrl_lsbs [129];
	double shifts [129];
	bool mono_mode;
	int previous_key;
	int base_key;
	int key_counter;
	pthread_mutex_t critical;
public:
	bool insert_trigger (integrated_trigger * trigger);
	bool insert_controller (double * controller, int location, double shift);
	void keyon (int key);
	void keyon (int key, int velocity);
	void keyoff (void);
	void keyoff (int key, int velocity = 0);
	void mono (void);
	void poly (void);
	bool isMonoMode (void);
	void control (int ctrl, double value);
	double getControl (int ctrl);
	void timing_clock (void);
	integrated_moonbase (orbiter_core * core);
};

class integrated_arpeggiator : public IntegratedCommandModule {
private:
	double tick, time, previous_activity;
	bool should_keyoff;
	int active_keys [128];
	int active_velocities [128];
	int active_key_pointer;
	int number_of_keys;
	int index, octave;
	bool up_direction;
	double previous_algo;
	void (* algo) (integrated_arpeggiator * arp);
	orbiter_core * core;
	IntegratedCommandModule * base;
	pthread_mutex_t critical;
	friend void up1 (integrated_arpeggiator * arp);
	friend void up2 (integrated_arpeggiator * arp);
	friend void up3 (integrated_arpeggiator * arp);
	friend void up4 (integrated_arpeggiator * arp);
	friend void down1 (integrated_arpeggiator * arp);
	friend void down2 (integrated_arpeggiator * arp);
	friend void down3 (integrated_arpeggiator * arp);
	friend void down4 (integrated_arpeggiator * arp);
	friend void updown1 (integrated_arpeggiator * arp);
	friend void updown2 (integrated_arpeggiator * arp);
	friend void updown3 (integrated_arpeggiator * arp);
	friend void updown4 (integrated_arpeggiator * arp);
	friend void updowndup1 (integrated_arpeggiator * arp);
	friend void updowndup2 (integrated_arpeggiator * arp);
	friend void updowndup3 (integrated_arpeggiator * arp);
	friend void updowndup4 (integrated_arpeggiator * arp);
	friend void randomdup1 (integrated_arpeggiator * arp);
	friend void randomdup2 (integrated_arpeggiator * arp);
	friend void randomdup3 (integrated_arpeggiator * arp);
	friend void randomdup4 (integrated_arpeggiator * arp);
	friend void random1 (integrated_arpeggiator * arp);
	friend void random2 (integrated_arpeggiator * arp);
	friend void random3 (integrated_arpeggiator * arp);
	friend void random4 (integrated_arpeggiator * arp);
private:
	void insert_key (int key, int velocity);
	void remove_key (int key);
	void ground (void);
	void private_signal (void);
public:
	double tempo, division, current_algo, active, hold;
public:
	bool insert_trigger (integrated_trigger * trigger);
	bool insert_controller (double * controller, int location, double shift);
	void keyon (int key);
	void keyon (int key, int velocity);
	void keyoff (void);
	void keyoff (int key, int velocity = 0);
	void mono (void);
	void poly (void);
	bool isMonoMode (void);
	void control (int ctrl, double value);
	double getControl (int ctrl);
	void timing_clock (void);
	void move (void);
	integrated_arpeggiator (orbiter_core * core, IntegratedCommandModule * base);
};

class integrated_adsr {
private:
	int stage;
	double threshold;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, busy;
	// ==== INUPT ====
	double trigger;
	double attack, decay, sustain, release;
	// ==== PROCESSING ====
	void move (void);
	integrated_adsr (orbiter_core * core);
};

class integrated_eg {
private:
	int stage;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, busy;
	// ==== INPUT ====
	double trigger;
	double time1, time2, time3, time4;
	double level1, level2, level3, level4;
	// ==== PROCESSING ====
	void move (void);
	integrated_eg (orbiter_core * core);
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

class integrated_chorus {
private:
	double omega;
	int index;
	double line [65536];
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal;
	// ==== INPUT ====
	double enter;
	double level, time, speed, amp;
	// ==== PROCESSING ====
	void move (void);
	integrated_chorus (orbiter_core * core);
};

class integrated_stereo_chorus {
private:
	double omega;
	int index;
	double line [65536];
	double line_right [65536];
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	double signal, signal_right;
	// ==== INPUT ====
	double mono, left, right;
	double level, time, bias, speed, phase, amp;
	// ==== PROCESSING ====
	void move (void);
	integrated_stereo_chorus (orbiter_core * core);
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

class integrated_auto_frame {
public:
	double value;
	double time;
	integrated_auto_frame * next;
	integrated_auto_frame * previous;
	integrated_auto_frame (double value, double time = 0.0, integrated_auto_frame * previous = 0);
	~ integrated_auto_frame (void);
};

class integrated_auto_data {
private:
	double record;
	double time;
	pthread_mutex_t critical;
	integrated_auto_frame * current_frame;
	orbiter_core * core;
public:
	// ==== OUTPUT ====
	// ==== INPUT ====
	double signal;
	double trigger; // 0 = inactive, 1 = record
	double control; // 0 = no playback, 1 = one way, 2 = repeat, 3 = forward and backward, 16 <= record
	// ==== PROCESSING ====
	integrated_auto_frame * frames;
	void clear_frames (void);
	integrated_auto_frame * insert_frame (double value, double time);
	virtual void move (void);
	bool return_content (PrologElement * parameters);
	bool read_content (PrologElement * parameters);
	integrated_auto_data (orbiter_core * core);
	~ integrated_auto_data (void);
	friend class integrated_auto_player;
};

class integrated_auto_player {
private:
	double time, maximum_change;
	bool active_playback, returning;
	integrated_auto_data * data;
	integrated_auto_frame * frames, * current_frame;
	orbiter_core * core;
	void filter (double enter);
public:
	// ==== OUTPUT ====
	double signal;
	// ==== INPUT ====
	double trigger;
	// ==== PROCESSING ====
	void move (void);
	integrated_auto_player (orbiter_core * core, integrated_auto_data * data, double maximum_change);
};

#endif
