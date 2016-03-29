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
// This file was created on Tuesday, 29th July 2014, 13:47:30 PM. //
////////////////////////////////////////////////////////////////////

#ifndef _LUNAR_LANDERS_
#define _LUNAR_LANDERS_

#include "lunar.h"

class lunar_parameter_block : public orbiter {
private:
	bool active;
	double enter;
	double maximum_change;
public:
	int style; // 0 = none, 1 = index, 2 = freq, 3 = amp, 4 = time, 5 = on/off, 6 = wave
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_parameter_block (orbiter_core * core, int style, bool active, double maximum_change);
};

class lunar_morph_one : public orbiter {
protected:
	double x;
	double a, b;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_morph_one (orbiter_core * core);
};

class lunar_morph_one_d : public lunar_morph_one {
private: double divisor;
public:
	void move (void);
	lunar_morph_one_d (orbiter_core * core, double divisor);
};

class lunar_morph_two : public orbiter {
protected:
	double x, y;
	double a, b, c, d;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_morph_two (orbiter_core * core);
};

class lunar_morph_two_d : public lunar_morph_two {
private: double divisor;
public:
	void move (void);
	lunar_morph_two_d (orbiter_core * core, double divisor);
};

class lunar_morph_three : public orbiter {
protected:
	double x, y, z;
	double a, b, c, d, e, f, g, h;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_morph_three (orbiter_core * core);
};

class lunar_morph_three_d : public lunar_morph_three {
private: double divisor;
public:
	void move (void);
	lunar_morph_three_d (orbiter_core * core, double divisor);
};

class auto_frame {
public:
	double value;
	double time;
	auto_frame * next;
	auto_frame * previous;
	auto_frame (double value, double time = 0.0, auto_frame * previous = 0);
	~ auto_frame (void);
};

class auto_data : public orbiter {
private:
	double trigger; // 0 = inactive, 1 = record
	double record;
	double control; // 0 = no playback, 1 = one way, 2 = repeat, 3 = forward and backward, 16 <= record
	double time;
	pthread_mutex_t critical;
	auto_frame * current_frame;
public:
	auto_frame * frames;
	void clear_frames (void);
	auto_frame * insert_frame (double value, double time);
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual void move (void);
	auto_data (orbiter_core * core);
	~ auto_data (void);
	friend class auto_player;
};

class auto_player : public orbiter {
private:
	double trigger;
	bool active_playback;
	double time;
	bool returning;
	auto_data * data;
	auto_frame * frames;
	auto_frame * current_frame;
	double maximum_change;
	void filter (double enter);
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	virtual bool release (void);
	auto_player (orbiter_core * core, auto_data * data, double maximum_change);
};

class lunar_mixer : public orbiter {
private:
	double enter;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_mixer (orbiter_core * core);
};

class lunar_stereo_mixer : public orbiter{
private:
	double signal_right;
	double enter;
	double enter_right;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_stereo_mixer (orbiter_core * core);
};

class lunar_gateway : public orbiter {
protected:
	double enter;
	double gateway;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_gateway (orbiter_core * core);
};

class lunar_stereo_gateway : public orbiter {
protected:
	double signal_right;
	double enter;
	double enter_right;
	double gateway;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_stereo_gateway (orbiter_core * core);
};

class lunar_amplifier : public lunar_gateway {
public:
	virtual void move (void);
	lunar_amplifier (orbiter_core * core);
};

class lunar_stereo_amplifier : public lunar_stereo_gateway {
public:
	virtual void move (void);
	lunar_stereo_amplifier (orbiter_core * core);
};

class lunar_mono_volume : public lunar_gateway {
public:
	virtual char * inputName (int ind);
	virtual void move (void);
	lunar_mono_volume (orbiter_core * core);
};

class lunar_volume : public lunar_stereo_gateway {
public:
	virtual char * inputName (int ind);
	virtual void move (void);
	lunar_volume (orbiter_core * core);
};

class lunar_control : public lunar_gateway {
public:
	virtual void move (void);
	lunar_control (orbiter_core * core);
};

class lunar_map : public orbiter {
public:
	double map [128];
	int initial;
	virtual int numberOfOutputs (void);
	void reset (void);
	lunar_map (orbiter_core * core, int initial = -64);
};

class lunar_trigger : public orbiter {
private:
	lunar_map * key_map;
	lunar_map * velocity_map;
	double velocity, trigger, index, delay1, delay2;
	double origin, delta, target, porta_switch, porta_control, porta_time, time, hold_ctrl;
	bool active;
	int keystack [16];
	int keystack_pointer;
	void add_stack (int key);
	void drop_stack (int key);
	void sub_keyon (int key);
	void sub_velocity (int velocity);
	pthread_mutex_t critical;
	int request_key, request_velocity, request_base, request_previous;
	void keyon_velocity_request (void);
	void ground_request (void);
	void keyoff_request (void);
	void keyoff_all_request (void);
public:
	int request; // 0 = none, 1 = keyoff (key), 2 = keyoff (), 3 = keyon (key), 4 = keyon (key, vel), 5 = ground (key, vel, base, prev);
	double busy;
	int key;
	lunar_trigger * next;
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	void set_key_map (lunar_map * map);
	void set_velocity_map (lunar_map * map);
	void keyon (int key);
	void keyon (int key, int velocity);
	void ground (int key, int velocity, int base, int previous);
	void keyoff (int key);
	void keyoff (void);
	virtual bool release (void);
	virtual void move (void);
	lunar_trigger (orbiter_core * core, bool active, lunar_trigger * next);
	~ lunar_trigger (void);
};

class lunar_impulse : public orbiter {
private:
	double enter, sync;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_impulse (orbiter_core * core);
};

class lunar_lfo : public orbiter {
private:
	bool stage_one;
	double origin;
	double target;
	double delta;
	double time;
	double speed, wave, pulse, phase, sync;
	double positive, negative;
	double trigger, previous_trigger;
	double vibrato, tremolo, wahwah, pan;
	double vibrato_signal, tremolo_signal, wahwah_signal, pan_signal;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_lfo (orbiter_core * core);
};

class lunar_adsr : public orbiter {
private:
	double attack, decay, sustain, release;
	double trigger, busy;
	int stage;
	double threshold;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_adsr (orbiter_core * core);
};

class lunar_adsr_linear : public orbiter {
private:
	double attack;
	double decay;
	double sustain;
	double release;
	double trigger;
	double busy;
	double time;
	int stage;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_adsr_linear (orbiter_core * core);
};

class lunar_eg : public orbiter {
private:
	double time1, time2, time3, time4;
	double level1, level2, level3, level4;
	double trigger;
	double busy;
	int stage;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_eg (orbiter_core * core);
};

class lunar_eg_linear_expo : public orbiter {
private:
	double time1, time2, time3, time4;
	double level1, level2, level3, level4;
	double trigger;
	double busy;
	int stage;
	double threshold;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_eg_linear_expo (orbiter_core * core);
};

class lunar_vca : public orbiter {
private:
	double enter, amp, gain;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_vca (orbiter_core * core);
};

class lunar_vca_adsr : public orbiter {
private:
	double enter, gateway, trigger;
	double attack, decay, sustain, release;
	double envelope, busy;
	int stage;
	double threshold;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_vca_adsr (orbiter_core * core);
};

class lunar_vca_eg : public orbiter {
private:
	double enter, gateway, trigger;
	double time1, time2, time3, time4;
	double level1, level2, level3, level4;
	double envelope, busy;
	int stage;
	double threshold;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_vca_eg (orbiter_core * core);
};

class lunar_delay : public orbiter {
private:
	double enter, enter_right;
	double feedback;
	double time;
	double high_damp;
	double previous_high_damp;
	double signal_right;
	double line [262144];
	int index;
	double A, B;
	double left_1, right_1;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_delay (orbiter_core * core);
};

class lunar_comb {
private:
	int * size_pointer;
	int index;
	friend class lunar_freeverb;
	double line [4096];
	double filter, feedback;
	double A, B;
public:
	double move (double enter);
	lunar_comb (void);
};

class lunar_allpass {
private:
	int * size_pointer;
	int index;
	friend class lunar_freeverb;
	double line [1536];
	double feedback;
public:
	double move (double enter);
	lunar_allpass (void);
};

class lunar_freeverb : public orbiter {
private:
	double mono, left, right;
	double signal_right;
	double feedback, diffusion, high_damp, dry, wet;
	double previous_feedback, previous_diffusion, previous_high_damp, previous_dry, previous_wet;
	double d1, w1, w2;
	lunar_comb left_combs [8];
	lunar_comb right_combs [8];
	lunar_allpass left_allpasses [4];
	lunar_allpass right_allpasses [4];
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_freeverb (orbiter_core * core);
};

class lunar_chorus : public orbiter {
private:
	double enter;
	double level;
	double time;
	double speed;
	double amp;
	double line [65536];
	double omega;
	int index;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_chorus (orbiter_core * core);
};

class lunar_stereo_chorus : public orbiter {
private:
	double mono, left, right;
	double signal_right;
	double level;
	double time, speed, amp;
	double bias, phase;
	double line [65536];
	double line_right [65536];
	double omega;
	int index;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_stereo_chorus (orbiter_core * core);
};

class lunar_drywet : public orbiter {
private:
	double dry_left, dry_right;
	double wet_left, wet_right;
	double signal_right;
	double balance;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_drywet (orbiter_core * core);
};

class lunar_drywet_mono : public orbiter {
private:
	double dry, wet;
	double balance;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_drywet_mono (orbiter_core * core);
};

class lunar_pan : public orbiter {
protected:
	double enter;
	double pan;
	double right;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_pan (orbiter_core * core);
};

class lunar_power_pan : public lunar_pan {
public:
	virtual void move (void);
	lunar_power_pan (orbiter_core * core);
};

class lunar_linear_pan : public lunar_pan {
public:
	virtual void move (void);
	lunar_linear_pan (orbiter_core * core);
};

class lunar_stereo_pan : public orbiter {
protected:
	double enter;
	double enter_right;
	double pan;
	double right;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_stereo_pan (orbiter_core * core);
};

class lunar_stereo_power_pan : public lunar_stereo_pan {
public:
	virtual void move (void);
	lunar_stereo_power_pan (orbiter_core * core);
};

class lunar_stereo_linear_pan : public lunar_stereo_pan {
public:
	virtual void move (void);
	lunar_stereo_linear_pan (orbiter_core * core);
};

class lunar_sensitivity : public orbiter {
private:
	double enter;
	double bp, lc, rc;
	double original_enter, original_bp, original_lc, original_rc;
	double gain;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_sensitivity (orbiter_core * core, double divisor = 128.0);
};

class lunar_filter : public orbiter {
private:
	double running_high_pass_signal;
	double running_band_pass_signal;
	double running_signal;
	double high_pass_signal;
	double band_pass_signal;
	double band_reject_signal;
	double enter;
	double freq;
	double resonance;
	double amp, gain;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_filter (orbiter_core * core);
};

class lunar_dc_offset_filter : public orbiter {
private:
	double enter, enter_right;
	double previous_enter, previous_enter_right;
	double signal_right;
	double fractor;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	lunar_dc_offset_filter (double fractor, orbiter_core * core);
};

class lunar_dc_offset_filter_mono : public orbiter {
private:
	double enter, previous, fractor;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_dc_offset_filter_mono (double fractor, orbiter_core * core);
};

#endif

