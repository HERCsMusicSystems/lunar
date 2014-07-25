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
// This file was created on Tuesday, 15th July 2014, 15:55:28 PM. //
////////////////////////////////////////////////////////////////////

#ifndef _LUNAR_
#define _LUNAR_

#include "pthread.h"

class orbiter;
class dock;
typedef dock * dock_pointer;
typedef double * connection_address;
class orbiter_core;

class orbiter_core {
public:
	double centre_frequency;
	double sampling_frequency;
	int latency_block_size;
	double time_deltas [32768];
	double * time_delta;
	pthread_mutex_t main_mutex;
	pthread_mutex_t maintenance_mutex;
	orbiter * root;
	void recalculate (void);
	void move_modules (void);
	void propagate_signals (void);
	orbiter_core (void);
	~ orbiter_core (void);
};

class orbiter {
public:
	double signal;
	int references;
	orbiter_core * core;
	dock_pointer * connectors;
	int number_of_connections;
	connection_address * connection_addresses;
	orbiter * next, * previous;
public:
	virtual int numberOfInputs (void);
	virtual int numberOfOutputs (void);
	virtual char * inputName (int ind);
	virtual char * outputName (int ind);
	virtual int inputIndex (char * name);
	virtual int outputIndex (char * name);
	virtual double output (int ind);
	virtual double output (char * name);
	virtual void input (double signal, int ind);
	virtual void input (double signal, char * name);
	virtual double * inputAddress (int ind);
	virtual double * outputAddress (int ind);
	virtual void move (void);
	virtual void propagate_signals (void);
public:
	void hold (void);
	void release (void);
	bool connect (int destination_port, orbiter * source, int source_port);
	bool disconnect (int destination_port, orbiter * source, int source_port);
public:
	void activate (void);
	orbiter (orbiter_core * core);
	virtual ~ orbiter (void);
};

class dock {
public:
	orbiter * source;
	int source_port;
	double * source_address;
	dock * next;
	dock (orbiter * source, int source_port, double * source_address, dock * next);
	~ dock (void);
};

#endif
