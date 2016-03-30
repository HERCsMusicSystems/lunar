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
// This file was created on Tuesday, 29th July 2014, 13:56:00 PM. //
////////////////////////////////////////////////////////////////////

#ifndef _LUNAR_PROLOG_LANDERS_
#define _LUNAR_PROLOG_LANDERS_

#include "prolog_lunar.h"
#include "lunar_landers.h"
#include "chromatic.h"

int toStyle (char * name);
void formatToStyle (double signal, char * command, int style, orbiter_core * core);

class parameter_block_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	parameter_block_class (orbiter_core * core);
};

class morph_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	morph_class (orbiter_core * core);
};

class auto_data_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	auto_data_class (orbiter_core * core);
};

class auto_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	auto_class (orbiter_core * core);
};

class PrologNativeKeyOrbiter : public PrologNativeOrbiter {
public:
	static char * name (void);
	virtual bool isTypeOf (char * code_name);
	PrologNativeKeyOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module);
};

class key_map_class : public PrologNativeOrbiterCreator {
private:
	int initial;
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	key_map_class (orbiter_core * core, int initial = -64);
};

class PrologNativeWaveOrbiter : public PrologNativeOrbiter {
public:
	static char * name (void);
	virtual bool isTypeOf (char * code_name);
	PrologNativeWaveOrbiter (PrologAtom * atom, orbiter_core * core, orbiter * module);
};

class wave_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	wave_class (orbiter_core * core);
};

class LoopWaveClass : public PrologNativeCode {public: bool code (PrologElement * parameters, PrologResolution * resolution);};

class impulse_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	impulse_class (orbiter_core * core);
};

class trigger_class : public PrologNativeOrbiterCreator {
private:
	PrologDirectory * dir;
	bool active;
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	trigger_class (PrologDirectory * dir, bool active, orbiter_core * core);
};

class mixer_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	mixer_class (orbiter_core * core);
};

class stereo_mixer_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	stereo_mixer_class (orbiter_core * core);
};

class control_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	control_class (orbiter_core * core);
};

class gateway_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parmaeters);
	gateway_class (orbiter_core * core);
};

class stereo_gateway_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	stereo_gateway_class (orbiter_core * core);
};

class amplifier_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	amplifier_class (orbiter_core * core);
};

class stereo_amplifier_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	stereo_amplifier_class (orbiter_core * core);
};

class volume_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	volume_class (orbiter_core * core);
};

class mono_volume_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	mono_volume_class (orbiter_core * core);
};

class lfo_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	lfo_class (orbiter_core * core);
};

class adsr_class : public PrologNativeOrbiterCreator {
public:
	bool linear;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	adsr_class (orbiter_core * core, bool linear = false);
};

class eg_class : public PrologNativeOrbiterCreator {
public:
	bool linear_expo;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	eg_class (orbiter_core * core, bool linear_expo = false);
};

class vca_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	vca_class (orbiter_core * core);
};

class vca_adsr_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	vca_adsr_class (orbiter_core * core);
};

class vca_eg_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	vca_eg_class (orbiter_core * core);
};

class delay_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	delay_class (orbiter_core * core);
};

class freeverb_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	freeverb_class (orbiter_core * core);
};

class chorus_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	chorus_class (orbiter_core * core);
};

class stereo_chorus_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	stereo_chorus_class (orbiter_core * core);
};

class pan_class : public PrologNativeOrbiterCreator {
public:
	int type;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	pan_class (orbiter_core * core, int type); // 0 = sine based recommended by MMA, 1 = power, 2 = linear
};

class drywet_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	drywet_class (orbiter_core * core);
};

class drywet_mono_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	drywet_mono_class (orbiter_core * core);
};

class sensitivity_class : public PrologNativeOrbiterCreator {
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	sensitivity_class (orbiter_core * core);
};

class moonbase_class : public PrologNativeOrbiterCreator {
private:
	PrologDirectory * dir;
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	moonbase_class (PrologDirectory * dir, orbiter_core * core);
};

class native_moonbase : public PrologNativeOrbiter {
private:
	PrologAtom * keyon, * keyoff, *pitch, * control, * mono, * poly, * timingclock;
	chromatograph graph;
public:
	static char * name (void);
	bool isTypeOf (char * code_name);
	char * codeName (void);
	virtual bool code (PrologElement * parameters, PrologResolution * resolution);
	native_moonbase (PrologDirectory * dir, PrologAtom * atom, orbiter_core * core, orbiter * module);
};
class arpeggiator_class : public PrologNativeOrbiterCreator {
private:
	PrologDirectory * dir;
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	arpeggiator_class (PrologDirectory * dir, orbiter_core * core);
};

class sequencer_class : public PrologNativeOrbiterCreator {
private:
	PrologDirectory * dir;
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	sequencer_class (PrologDirectory * dir, orbiter_core * core);
};

class polysequencer_class : public PrologNativeOrbiterCreator {
private:
	PrologDirectory * dir;
public:
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	polysequencer_class (PrologDirectory * dir, orbiter_core * core);
};

class detector_class : public PrologNativeOrbiterCreator {
public:
	PrologRoot * root;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	detector_class (PrologRoot * root, orbiter_core * core);
};

class DCOffsetFilter_class : public PrologNativeOrbiterCreator {
public:
	bool mono;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	DCOffsetFilter_class (orbiter_core * core, bool mono);
};

class integrated_alarm_class : public PrologNativeOrbiterCreator {
public:
	PrologDirectory * directory;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	integrated_alarm_class (orbiter_core * core, PrologDirectory * directory);
};

class integrated_microdot_class : public PrologNativeOrbiterCreator {
public:
	PrologDirectory * directory;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	integrated_microdot_class (orbiter_core * core, PrologDirectory * directory);
};

class integrated_abakos_class : public PrologNativeOrbiterCreator {
public:
	PrologDirectory * directory;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	integrated_abakos_class (orbiter_core * core, PrologDirectory * directory);
};

class integrated_phobos_class : public PrologNativeOrbiterCreator {
public:
	PrologDirectory * directory;
	virtual orbiter * create_orbiter (PrologElement * parameters);
	virtual PrologNativeOrbiter * create_native_orbiter (PrologAtom * atom, orbiter * module);
	integrated_phobos_class (orbiter_core * core, PrologDirectory * directory);
};

#endif

