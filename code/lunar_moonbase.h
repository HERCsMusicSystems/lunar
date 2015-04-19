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
	virtual bool insert_trigger (lunar_trigger * trigger) = 0;
	virtual bool insert_controller (orbiter * controller, int location, double shift) = 0;
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
	CommandModule (orbiter_core * core);
};

class moonbase : public CommandModule {
private:
	lunar_trigger * triggers;
	lunar_trigger * choice;
	lunar_trigger * select (void);
	lunar_trigger * select (int key);
	orbiter * controllers [129];
	double ctrl_lsbs [129];
	double shifts [129];
	bool mono_mode;
	int previous_key;
	int base_key;
	int key_counter;
	pthread_mutex_t critical;
public:
	virtual int numberOfOutputs (void);
	virtual bool release (void);
	bool insert_trigger (lunar_trigger * trigger);
	bool insert_controller (orbiter * controller, int location, double shift);
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
	moonbase (orbiter_core * core);
	~ moonbase (void);
};

class arpeggiator : public CommandModule {
private:
	double tempo;
	double division;
	double tick;
	double time;
	double active;
	double previous_activity;
	double hold;
	bool should_keyoff;
	int active_keys [128];
	int active_velocities [128];
	int active_key_pointer;
	int number_of_keys;
	int index;
	int octave;
	bool up_direction;
	double current_algo, previous_algo;
	void (* algo) (arpeggiator * arp);
	CommandModule * base;
	pthread_mutex_t critical;
	friend void up1 (arpeggiator * arp);
	friend void up2 (arpeggiator * arp);
	friend void up3 (arpeggiator * arp);
	friend void up4 (arpeggiator * arp);
	friend void down1 (arpeggiator * arp);
	friend void down2 (arpeggiator * arp);
	friend void down3 (arpeggiator * arp);
	friend void down4 (arpeggiator * arp);
	friend void updown1 (arpeggiator * arp);
	friend void updown2 (arpeggiator * arp);
	friend void updown3 (arpeggiator * arp);
	friend void updown4 (arpeggiator * arp);
	friend void updowndup1 (arpeggiator * arp);
	friend void updowndup2 (arpeggiator * arp);
	friend void updowndup3 (arpeggiator * arp);
	friend void updowndup4 (arpeggiator * arp);
	friend void randomdup1 (arpeggiator * arp);
	friend void randomdup2 (arpeggiator * arp);
	friend void randomdup3 (arpeggiator * arp);
	friend void randomdup4 (arpeggiator * arp);
	friend void random1 (arpeggiator * arp);
	friend void random2 (arpeggiator * arp);
	friend void random3 (arpeggiator * arp);
	friend void random4 (arpeggiator * arp);
private:
	void insert_key (int key, int velocity);
	void remove_key (int key);
	void ground (void);
	void private_signal (void);
public:
	bool insert_trigger (lunar_trigger * trigger);
	bool insert_controller (orbiter * controller, int location, double shift);
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
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual int numberOfOutputs (void);
	virtual bool release (void);
	virtual void propagate_signals (void);
	arpeggiator (orbiter_core * core, CommandModule * base);
	~ arpeggiator (void);
};

class sequence_element {
public:
	int type; // 0 = wait, 1 = keyon [key], 2 = keyon [key velocity] 3 = keyoff [], 4 = keyoff [key], 5 = control
	// 8 = busy [x], 9 = impulse [x]
	int key;
	double velocity;
	sequence_element * next;
	sequence_element (int type, int key = 0, double velocity = 0);
	~ sequence_element (void);
};

class sequencer : public CommandModule {
private:
	double tempo;
	double trigger;
	double time;
	int tick;
	double impulse_level, busy_level;
	sequence_element * current_frame;
	CommandModule * base;
	pthread_mutex_t critical;
	void private_signal (void);
public:
	sequence_element * elements;
public:
	bool insert_trigger (lunar_trigger * trigger);
	bool insert_controller (orbiter * controller, int location, double shift);
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
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual char * outputName (int ind);
	virtual bool release (void);
	virtual void propagate_signals (void);
	virtual void move (void);
public:
	sequencer (orbiter_core * core, CommandModule * base);
	~ sequencer (void);
};

class polysequence_element {
public:
	int type; // 0 = wait, 1 = keyon [channel key], 2 = keyon [channel key velocity] 3 = keyoff [channel], 4 = keyoff [channel key], 5 = control, 6 = keyoff []
	// 8 = busy [x], 9 = impules [x]
	int channel;
	int key;
	double velocity;
	polysequence_element * next;
	polysequence_element (int type, int channel = 0, int key = 0, double velocity = 0);
	~ polysequence_element (void);
};

typedef CommandModule * CommandModulePointer;
class polysequencer : public CommandModule {
private:
	double tempo;
	double trigger;
	double time;
	int tick;
	double impulse_level, busy_level;
	polysequence_element * current_frame;
	CommandModulePointer * bases;
	int base_pointer;
	int number_of_bases;
	pthread_mutex_t critical;
	void private_signal (void);
public:
	polysequence_element * elements;
public:
	bool insert_trigger (lunar_trigger * trigger);
	bool insert_controller (orbiter * controller, int location, double shift);
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
	void add_base (CommandModule * base);
	int numberOfBases (void);
public:
	virtual int numberOfInputs (void);
	virtual char * inputName (int ind);
	virtual double * inputAddress (int ind);
	virtual char * outputName (int ind);
	virtual bool release (void);
	virtual void propagate_signals (void);
	virtual void move (void);
public:
	polysequencer (orbiter_core * core, int number_of_bases);
	~ polysequencer (void);
};

#endif

