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
	double sync;
	double trigger;
	double amp;
	double slope;
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	lunar_oscillator (orbiter_core * core);
};

#define RETRIGGER_OSCILLATOR if (slope != trigger) if (sync != 0.0 && trigger > 0.0) time = 0.0; slope = trigger;

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
	lunar_saw_operator (orbiter_core * core);
};

class lunar_square_operator : public lunar_saw_operator {
private:
	bool stage;
public:
	virtual void move (void);
	lunar_square_operator (orbiter_core * core);
};

class lunar_aliased_square_operator : public lunar_oscillator {
public:
	virtual void move (void);
	lunar_aliased_square_operator (orbiter_core * core);
};

class lunar_sampler_operator : public orbiter {
private:
	double time;
	double freq;
	double ratio;
	double amp;
	double index;
	double trigger;
	double slope;
	lunar_wave * wave;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual void move (void);
	virtual bool release (void);
	void install_wave (lunar_wave * wave);
	lunar_sampler_operator (orbiter_core * core);
};

#endif

