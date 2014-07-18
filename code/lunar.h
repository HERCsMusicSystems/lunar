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

#ifndef _LUNAR_
#define _LUNAR_

class orbiter_core {
public:
	double sampling_frequency;
	int latency_block_size;
};

class dock;

class orbiter {
public:
	int references;
	orbiter_core * core;
	orbiter * next, * previous;
public:
	virtual int numberOfInputs (void);
	virtual int numberOfOutputs (void);
	virtual char * inputName (int ind);
	virtual char * outputName (int ind);
	virtual int inputIndex (char * name);
	virtual int outputIndex (char * name);
	virtual double output (int ind = 0);
	virtual double output (char * name);
	virtual void input (double signal, int ind = 0);
	virtual void input (double signal, char * name);
	virtual void move (void);
public:
	void hold (void);
	void release (void);
	void connect (orbiter * source, int port);
	void connect (orbiter * source, char * port);
public:
	orbiter (orbiter_core * core);
	virtual ~ orbiter (void);
};

class dock {
public:
	orbiter * source;
	int port;
	dock * next;
};

#endif
