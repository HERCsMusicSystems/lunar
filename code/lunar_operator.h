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

#ifndef _LUNAR_OPERATOR_
#define _LUNAR_OPERATOR_

#include "lunar.h"
#include "lunar_wave.h"

class lunar_oscillator : public orbiter {
protected:
	double time;
	double freq;
	double ratio;
	double trigger;
	double amp, gain;
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	lunar_oscillator (orbiter_core * core);
};

class lunar_operator : public lunar_oscillator {
private:
	double shift;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_operator (orbiter_core * core);
};

class lunar_aliased_saw_operator : public lunar_oscillator {
public:
	virtual void move (void);
	lunar_aliased_saw_operator (orbiter_core * core);
};

class lunar_saw_operator : public lunar_oscillator {
protected:
	int blep_index;
public:
	virtual void move (void);
	static lunar_saw_operator * create (orbiter_core * core);
protected:
	lunar_saw_operator (orbiter_core * core);
};

class lunar_aliased_square_operator : public lunar_oscillator {
public:
	virtual void move (void);
	lunar_aliased_square_operator (orbiter_core * core);
};

class lunar_square_operator : public lunar_saw_operator {
private:
	bool stage;
public:
	virtual void move (void);
	static lunar_square_operator * create (orbiter_core * core);
protected:
	lunar_square_operator (orbiter_core * core);
};

class vco_operator : public lunar_square_operator {
private:
	double wave; // 0 = sine, 1 = saw, 2 = square, 3 = noise
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	vco_operator (orbiter_core * core);
};

class noise_operator : public orbiter {
private:
	double amp, gain;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	noise_operator (orbiter_core * core);
};

class lunar_sampler_operator : public orbiter {
private:
	double time;
	double freq;
	double ratio;
	double amp, gain;
	double index;
	double trigger;
	double signal_right;
	double busy;
	bool return_possible;
	lunar_wave * wave;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual char * outputName (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	virtual bool release (void);
	void install_wave (lunar_wave * wave);
	lunar_sampler_operator (orbiter_core * core);
};

class lunar_fm4_block : public orbiter {
private:
	double current_algo;
	double previous_algo;
	double trigger;
	double signal1, signal2, signal3, signal4;
	double time1, time2, time3, time4;
	double freq1, freq2, freq3, freq4;
	double amp1, amp2, amp3, amp4;
	double gain1, gain2, gain3, gain4;
	double ratio1, ratio2, ratio3, ratio4;
	double feedback1, feedback2, feedback3, feedback4;
	double (* algo) (lunar_fm4_block * block);
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	lunar_fm4_block (orbiter_core * core);
	friend double algo1 (lunar_fm4_block * block);
	friend double algo2 (lunar_fm4_block * block);
	friend double algo3 (lunar_fm4_block * block);
	friend double algo4 (lunar_fm4_block * block);
	friend double algo5 (lunar_fm4_block * block);
	friend double algo6 (lunar_fm4_block * block);
	friend double algo7 (lunar_fm4_block * block);
	friend double algo8 (lunar_fm4_block * block);
};

#endif

