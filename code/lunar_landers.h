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

class lunar_inactive_parameter_block : public orbiter {
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	lunar_inactive_parameter_block (orbiter_core * core);
};

class lunar_active_parameter_block : public orbiter {
private:
	double enter;
	double maximum_change;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_active_parameter_block (orbiter_core * core, double maximum_change);
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
private:
	double enter;
	double gateway;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_gateway (orbiter_core * core);
};

class lunar_map : public orbiter {
public:
	double map [128];
	virtual int numberOfOutputs (void);
	lunar_map (orbiter_core * core, int initial = -64);
};

class lunar_trigger : public orbiter {
private:
	lunar_map * key_map;
	lunar_map * velocity_map;
	double velocity, trigger;
	double origin, delta, target, porta_time, time;
	bool active;
	int keystack [16];
	int keystack_pointer;
	void add_stack (int key);
	void drop_stack (int key);
public:
	double busy;
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
	void keyoff (int key);
	void keyoff (void);
	virtual bool release (void);
	virtual void move (void);
	lunar_trigger (orbiter_core * core, bool active, lunar_trigger * next);
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
	lunar_adsr (orbiter_core * core);
};

class lunar_eg : public orbiter {
public:
	lunar_eg (orbiter_core * core);
};

#endif

