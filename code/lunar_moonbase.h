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

///////////////////////////////////////////////////////////////
// This file was created on Friday, 18th July at 8:45:45 AM. //
///////////////////////////////////////////////////////////////

#ifndef _LUNAR_MOONBASE_
#define _LUNAR_MOONBASE_

#include "lunar.h"
#include "lunar_landers.h"

class CommandModule : public orbiter {
public:
	virtual bool set_map (lunar_map * map) = 0;
	virtual bool insert_trigger (lunar_trigger * trigger) = 0;
	virtual bool insert_controller (orbiter * controller, int location) = 0;
	virtual void keyon (int key) = 0;
	virtual void keyon (int key, int velocity) = 0;
	virtual void keyoff (void) = 0;
	virtual void keyoff (int key, int velocity = 0) = 0;
	virtual void mono (void) = 0;
	virtual void poly (void) = 0;
	virtual bool isMonoMode (void) = 0;
	virtual void control (int ctrl, int value) = 0;
	virtual double getControl (int ctrl) = 0;
	CommandModule (orbiter_core * core);
};

class moonbase : public CommandModule {
private:
	lunar_map * map;
	lunar_trigger * triggers;
	lunar_trigger * choice;
	lunar_trigger * select (void);
	lunar_trigger * select (int key);
	orbiter * controllers [129];
	int ctrl_lsbs [129];
	bool mono_mode;
	int previous_key;
	int base_key;
	int key_counter;
	pthread_mutex_t critical;
public:
	virtual int numberOfOutputs (void);
	virtual bool release (void);
	bool set_map (lunar_map * map);
	bool insert_trigger (lunar_trigger * trigger);
	bool insert_controller (orbiter * controller, int location);
	void keyon (int key);
	void keyon (int key, int velocity);
	void keyoff (void);
	void keyoff (int key, int velocity = 0);
	void mono (void);
	void poly (void);
	bool isMonoMode (void);
	void control (int ctrl, int value);
	double getControl (int ctrl);
	moonbase (orbiter_core * core);
	~ moonbase (void);
};

class arpeggiator : public CommandModule {
private:
	double tempo;
	double time;
	double active;
	int active_keys [128];
	int active_key_pointer;
	moonbase * base;
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual bool release (void);
public:
	bool set_map (lunar_map * map);
	bool insert_trigger (lunar_trigger * trigger);
	bool insert_controller (orbiter * controller, int location);
	void keyon (int key);
	void keyon (int key, int velocity);
	void keyoff (void);
	void keyoff (int key, int velocity = 0);
	void mono (void);
	void poly (void);
	bool isMonoMode (void);
	void control (int ctrl, int value);
	double getControl (int ctrl);
	arpeggiator (orbiter_core * core, moonbase * base);
};

#endif

