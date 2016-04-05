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

//////////////////////////////////////////////////////////////////////
// This file was created on Friday, 30th March 2016 at 13:34:00 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar_prolog_landers.h"
#include "lunar_moonbase.h"
#include "lunar_integrated_components.h"

/*
Common Behaviour:
[module] => Closes the module. The module is also destroyed and grabage collected if it is not connected to any input of any other module.
[module : *io] => Reports names of all inputs and outputs.
[module module] => Used for test purposes to update input signals and push the model one sample forward.
[module []] => Disconnect all inputs.
[module source] => Connects the first output of source to the first input of module.
[module DESTINATION source] => Connects the first output of source to the DESTINATION input of module.
[module DESTINATION source SOURCE] => Connects the SOURCE port of source to the DESTINATION input of module.
[module DESTINATION source []] => Disconnects the first output of source from the DESTINATION input of module.
[module DESTINATION source SOURCE []] => Disconnects the SOURCE port of source from the DESTINATION input of module.
Note: both DESTINATION and SOURCE can be either text or number.
[module *x] => Reports the current value of the first output port of the module.
[module SOURCE *x] => Reports the current value of the SOURCE output port of the module.
[module value] => Sets the value of the first input port of the module.
[module DESTINATION value] => Sets the value of the DESTINATION input port of the module.
Note: value can be either integer or double.

Additional usage:
[module keyon key velocity] => MIDI keyon message. Velocity is optional.
[module keyon [note octave] velocity] => MIDI keyon message. Velocity is optional.
[module keyoff] => MIDI all keys off message.
[module keyoff key velocity] => MIDI keyoff message. Velocity is optional.
[module keyoff [note octave] velocity] => MIDI keyoff message. Velocity is optional.
[module control index value] => MIDI control message. Value can be double.
[module pitch lsb msb] => MIDI pitch message. Equivalent to [module control 128 (msb + lsb / 128)].
[module timingclock] => MIDI timingclock message. Important for arpeggiators and sequencers.
[module mono] => Changes mode to mono.
[module poly] => changes mode to poly.
[module control *x] => Returns either mono or poly depending on the current mode.
[module control index *x] => Returns the current value of the controlled parameter block.
[module pitch *x] => Equivalent to [module control 128 *x].
[module trigger] => Connects trigger to the base.
[module index controller] => Connects an audio module (most likely a parameter block) as a controller.
[module index []] => Removes controllable audio module.
Otherwise defaults to common behaviour.

*/


#define DIV_16384 0.00006103515625
/*
#define DIV_2097152 0.000000476837158203125
*/

class integrated_abakos;

class integrated_abakos_part {
private:
	integrated_trigger trigger;
	integrated_vco vco1, vco2;
	integrated_noise noise;
	integrated_filter filter;
	integrated_adsr adsr1;
	integrated_eg adsr2;
public:
	void move (integrated_abakos * ia);
	integrated_abakos_part (orbiter_core * core, integrated_abakos * ia);
};

typedef integrated_abakos_part * integrated_abakos_part_pointer;

class integrated_abakos : public CommandModule {
private:
	int polyphony;
public:
	integrated_moonbase base;
	integrated_arpeggiator arp;
	integrated_abakos_part_pointer * parts;
	integrated_stereo_pan pan;
	integrated_stereo_chorus chorus;
	integrated_delay delay;
	integrated_drywet drywet;
	integrated_stereo_amplifier volume;
	integrated_map key_map;
	integrated_lfo lfo;
	double pitch, modulation, vibrato;
	double lsb;
	struct {double pitch, vibrato;} sens;
	struct {
		double freq, amp, ratio, wave;
		struct {double key, adsr;} sens;
	} vco [2];
	double noise, ringmod;
	struct {
		double freq, resonance, amp;
		struct {double key, adsr;} sens;
	} filter;
	struct {double attack, decay, sustain, release;} adsr1;
	struct {double time [4], level [4];} adsr2;
	struct {double porta, time, legato, hold;} portamento;
	double vco_freq_lemat [2], filter_freq_lemat;
public:
	bool insert_trigger (lunar_trigger * trigger) {return false;}
	bool insert_controller (orbiter * controller, int location, double shift) {return false;}
	void keyon (int key, int velocity) {arp . keyon (key, velocity);}
	void keyon (int key) {arp . keyon (key);}
	void keyoff (void) {arp . keyoff ();}
	void keyoff (int key, int velocity) {arp . keyoff (key, velocity);}
	void mono (void) {arp . mono ();}
	void poly (void) {arp . poly ();}
	bool isMonoMode (void) {return arp . isMonoMode ();}
	void control (int ctrl, double value) {
		switch (ctrl) {
		case 1: modulation = value * 128.0 + lsb; lsb = 0.0; break;
		case 7: volume . gateway = value * 128.0 + lsb; lsb = 0.0; break;
		case 10: pan . pan = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 5: portamento . time = value * 128.0 + lsb; lsb = 0.0; break;
		//case 16: X = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		//case 17: Y = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 65: portamento . porta = value * 128.0 + lsb; lsb = 0.0; break;
		case 74: filter . freq = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 71: filter . resonance = value * 128.0 + lsb; lsb = 0.0; break;
		case 77: drywet . balance = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 78: lfo . speed = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 73: adsr1 . attack = value * 128.0 + lsb; lsb = 0.0; break;
		case 75: adsr1 . decay = value * 128.0 + lsb; lsb = 0.0; break;
		case 76: adsr1 . sustain = value * 128.0 - 16384.0 + lsb; lsb = 0.0; break;
		case 72: adsr1 . release = value * 128.0 + lsb; lsb = 0.0; break;
		case 79: lfo . wahwah = value * 128.0 + lsb; lsb = 0.0; break;
		case 126: arp . mono (); lsb = 0.0; break;
		case 127: arp . poly (); lsb = 0.0; break;
		case 128: pitch = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
			// INVISIBLE
		case 64: portamento . hold = value * 128.0 + lsb; lsb = 0.0; break;
		case 84: portamento . legato = value * 128.0 + lsb; lsb = 0.0; break;
		//case 85: auto_ctrl = value * 128.0 + lsb; lsb = 0.0; break;
		case 80: arp . active = value * 128.0 + lsb; lsb = 0.0; break;
		case 66: arp . hold = value * 128.0 + lsb; lsb = 0.0; break;
		default: lsb = value; break;
		}
	}
	double getControl (int ctrl) {
		switch (ctrl) {
		case 1: return modulation * 0.0078125; break;
		case 7: return volume . gateway * 0.0078125; break;
		case 10: return pan . pan * 0.0078125 + 64.0; break;
		case 5: return portamento . time * 0.0078125; break;
		//case 16: return X * 0.0078125 + 64.0; break;
		//case 17: return Y * 0.0078125 + 64.0; break;
		case 65: return portamento . porta * 0.0078125; break;
		case 74: return filter . freq * 0.0078125 + 64.0; break;
		case 71: return filter . resonance * 0.0078125; break;
		case 77: return drywet . balance * 0.0078125 + 64.0; break;
		case 78: return lfo . speed * 0.0078125 + 64.0; break;
		case 73: return adsr1 . attack * 0.0078125; break;
		case 75: return adsr1 . decay * 0.0078125; break;
		case 76: return adsr1 . sustain * 0.0078125 + 128.0; break;
		case 72: return adsr1 . release * 0.0078125; break;
		case 79: return lfo . wahwah * 0.0078125; break;
		case 126: return base . isMonoMode () ? 1.0 : 0.0; break;
		case 127: return base . isMonoMode () ? 0.0 : 1.0; break;
		case 128: return pitch * 0.0078125 + 64.0; break;
			// INVISIBLE
		case 64: return portamento . hold * 0.0078125; break;
		case 84: return portamento . legato * 0.0078125; break;
		//case 85: return auto_ctrl * 0.0078125; break;
		case 80: return arp . active * 0.0078125; break;
		case 66: return arp . hold * 0.0078125; break;
		default: break;
		}
		return 64.0;
	}
	void timing_clock (void) {}
	int numberOfOutputs (void) {return 2;}
	char * outputName (int ind) {
		switch (ind) {
		case 0: return "LEFT"; break;
		case 1: return "RIGHT"; break;
		default: break;
		}
		return CommandModule :: outputName (ind);
	}
	double * outputAddress (int ind) {
		switch (ind) {
		case 0: return & volume . left; break;
		case 1: return & volume . right; break;
		default: break;
		}
		return CommandModule :: outputAddress (ind);
	}
	void move (void) {
		arp . move ();
		lfo . vibrato = vibrato + modulation * sens . vibrato * DIV_16384;
		lfo . move (); lfo . trigger = 0.0;
		chorus . mono = 0.0;
		vco_freq_lemat [0] = vco [0] . freq + pitch * sens . pitch * DIV_16384 + lfo . vibrato_signal;
		vco_freq_lemat [1] = vco [1] . freq + pitch * sens . pitch * DIV_16384 + lfo . vibrato_signal;
		filter_freq_lemat = filter . freq + lfo . wahwah_signal;
		for (int ind = 0; ind < polyphony; ind++) parts [ind] -> move (this);
		chorus . move ();
		pan . enter_left = chorus . signal;
		pan . enter_right = chorus . signal_right;
		pan . move ();
		delay . enter = drywet . dry_left = pan . left;
		delay . enter_right = drywet . dry_right = pan . right;
		delay . move ();
		drywet . wet_left = delay . signal;
		drywet . wet_right = delay . signal_right;
		drywet . move ();
		volume . enter_left = drywet . left;
		volume . enter_right = drywet . right;
		volume . volume_move ();
	}
	integrated_abakos (orbiter_core * core, int polyphony)
			: CommandModule (core), base (core), arp (core, & base), pan (core), chorus (core), delay (core), volume (core, 12800.0), lfo (core) {
		this -> polyphony = polyphony;
		parts = new integrated_abakos_part_pointer [polyphony];
		for (int ind = 0; ind < polyphony; ind++) parts [ind] = new integrated_abakos_part (core, this);
		drywet . balance = -8192.0;
		pitch = modulation = vibrato = 0.0;
		sens . pitch = 256.0;
		sens . vibrato = 128.0;
		vco [0] . freq = vco [1] . freq = 0.0;
		vco [0] . amp = 0.0; vco [1] . amp = -16384.0;
		vco [0] . ratio = vco [1] . ratio = 1.0;
		vco [0] . wave = vco [1] . wave = 1.0;
		vco [0] . sens . key = vco [1] . sens . key = 16384.0;
		vco [0] . sens . adsr = vco [1] . sens . adsr = 0.0;
		noise = ringmod = -16384.0;
		filter . freq = 5120.0;
		filter . resonance = filter . amp = 0.0;
		filter . sens . key = filter . sens . adsr = 0.0;
		adsr1 . attack = adsr1 . decay = adsr1 . sustain = adsr1 . release = 0.0;
		for (int ind = 0; ind < 4; ind++) adsr2 . time [ind] = adsr2 . level [ind] = 0.0;
		portamento . porta = 1.0; portamento . time = portamento . legato = portamento . hold = 0.0;
		lfo . speed = 1792.0;
		lsb = 0.0;
		initialise (); activate ();
	}
	~ integrated_abakos (void) {for (int ind = 0; ind < polyphony; ind++) delete parts [ind]; delete [] parts; parts = 0;}
};

void integrated_abakos_part :: move (integrated_abakos * ia) {
	trigger . porta = ia -> portamento . porta;
	trigger . porta_time = ia -> portamento . time;
	trigger . legato = ia -> portamento . legato;
	trigger . hold = ia -> portamento . hold;
	trigger . move ();
	adsr1 . trigger = trigger . trigger;
	adsr1 . attack = ia -> adsr1 . attack;
	adsr1 . decay = ia -> adsr1 . decay;
	adsr1 . sustain = ia -> adsr1 . sustain;
	adsr1 . release = ia -> adsr1 . release;
	adsr1 . move ();
	trigger . busy = adsr1 . busy;
	adsr2 . trigger = trigger . trigger;
	adsr2 . time1 = ia -> adsr2 . time [0];
	adsr2 . time2 = ia -> adsr2 . time [1];
	adsr2 . time3 = ia -> adsr2 . time [2];
	adsr2 . time4 = ia -> adsr2 . time [3];
	adsr2 . level1 = ia -> adsr2 . level [0];
	adsr2 . level2 = ia -> adsr2 . level [1];
	adsr2 . level3 = ia -> adsr2 . level [2];
	adsr2 . level4 = ia -> adsr2 . level [3];
	adsr2 . move ();
	vco1 . freq = ia -> vco_freq_lemat [0] + DIV_16384 * (adsr2 . signal * ia -> vco [0] . sens . adsr + trigger . signal * ia -> vco [0] . sens . key);
	vco1 . amp = ia -> vco [0] . amp;
	vco1 . ratio = ia -> vco [0] . ratio;
	vco1 . wave = ia -> vco [0] . wave;
	vco2 . freq = ia -> vco_freq_lemat [1] + DIV_16384 * (adsr2 . signal * ia -> vco [1] . sens . adsr + trigger . signal * ia -> vco [1] . sens . key);
	vco2 . amp = ia -> vco [1] . amp;
	vco2 . ratio = ia -> vco [1] . ratio;
	vco2 . wave = ia -> vco [1] . wave;
	vco1 . move (); vco2 . move ();
	noise . amp = ia -> noise;
	noise . move ();
	double ringmod = vco1 . signal * vco2 . signal * ia -> core -> Amplitude (ia -> ringmod);
	filter . enter = vco1 . signal + vco2 . signal + noise . signal + ringmod;
	filter . freq = ia -> filter_freq_lemat + DIV_16384 * (adsr2 . signal * ia -> filter . sens . adsr + trigger . signal * ia -> filter . sens . key);
	filter . resonance = ia -> filter . resonance;
	filter . amp = ia -> filter . amp;
	filter . move ();
	ia -> chorus . mono += filter . signal * adsr1 . signal;
}
integrated_abakos_part :: integrated_abakos_part (orbiter_core * core, integrated_abakos * ia)
		: trigger (core, true, 0), vco1 (core), vco2 (core), noise (core), filter (core), adsr1 (core), adsr2 (core) {
	trigger . key_map = & ia -> key_map;
	ia -> base . insert_trigger (& trigger);
}

class native_integrated_abakos : public native_moonbase {
public:
	PrologAtom * core_atom;
	PrologAtom * volume, * pan, * pitch, * modulation, * sens, * vibrato;
	PrologAtom * chorus, * level, * time, * bias, * speed, * phase, * amp;
	PrologAtom * delay, * balance, * feedback, * highdamp;
	PrologAtom * arpeggiator, * subdivision, * active, * algo, * hold;
	PrologAtom * lfo, * wave, * pulse, * sync, * tremolo, * wahwah;
	PrologAtom * vco, * freq, * ratio, * key, * ringmod, * noise;
	PrologAtom * adsr, * attack, * decay, * sustain, * release;
	PrologAtom * time1, * time2, * time3, *time4, * level1, * level2, * level3, * level4;
	PrologAtom * filter, * resonance;
	PrologAtom * portamento, * porta, * legato;
	PrologAtom * key_map;
	bool update_value (double * location, PrologElement * el, PrologElement * text, int style) {
		if (el -> isNumber ()) * location = el -> getNumber ();
		else {
			el -> setDouble (* location);
			if (text != 0) {
				char command [128];
				formatToStyle (* location, command, style, core);
				text -> setText (command);
			}
		}
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isPair ()) {
			PrologElement * v = parameters -> getLeft ();
			PrologElement * o = 0;
			if (v -> isNumber () || v -> isPair () || v -> isEarth () || v -> isVar ()) {
				integrated_abakos * ab = (integrated_abakos *) module;
				PrologElement * path = parameters -> getRight ();
				if (path -> isPair () && path -> getLeft () -> isVar ()) {o = path -> getLeft (); path = path -> getRight ();}
				PrologElement * se;
				PrologAtom * sa;
				if (path -> isPair ()) {
					se = path -> getLeft ();
					if (se -> isAtom ()) {
						sa = se -> getAtom ();
						if (sa == core_atom) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == volume) return update_value (& ab -> volume . gateway, v, o, 1);
									if (sa == pan) return update_value (& ab -> pan . pan, v, o, 1);
									if (sa == pitch) return update_value (& ab -> pitch, v, o, 1);
									if (sa == modulation) return update_value (& ab -> modulation, v, o, 1);
									if (sa == sens) {
										path = path -> getRight ();
										if (path -> isPair ()) {
											se = path -> getLeft ();
											if (se -> isAtom ()) {
												sa = se -> getAtom ();
												if (sa == pitch) return update_value (& ab -> sens . pitch, v, o, 1);
												if (sa == vibrato) return update_value (& ab -> sens . vibrato, v, o, 1);
											}
										}
										return false;
									}
									if (sa == chorus) {
										path = path -> getRight ();
										if (path -> isPair ()) {
											se = path -> getLeft ();
											if (se -> isAtom ()) {
												sa = se -> getAtom ();
												if (sa == level) return update_value (& ab -> chorus . level, v, o, 1);
												if (sa == time) return update_value (& ab -> chorus . time, v, o, 1);
												if (sa == bias) return update_value (& ab -> chorus . bias, v, o, 1);
												if (sa == speed) return update_value (& ab -> chorus . speed, v, o, 1);
												if (sa == phase) return update_value (& ab -> chorus . phase, v, o, 1);
												if (sa == amp) return update_value (& ab -> chorus . amp, v, o, 1);
											}
										}
										return false;
									}
									if (sa == delay) {
										path = path -> getRight ();
										if (path -> isPair ()) {
											se = path -> getLeft ();
											if (se -> isAtom ()) {
												sa = se -> getAtom ();
												if (sa == balance) return update_value (& ab -> drywet . balance, v, o, 1);
												if (sa == feedback) return update_value (& ab -> delay . feedback, v, o, 1);
												if (sa == time) return update_value (& ab -> delay . time, v, o, 4);
												if (sa == highdamp) return update_value (& ab -> delay . high_damp, v, o, 1);
											}
										}
										return false;
									}
								}
							}
							return false;
						}
						if (sa == arpeggiator) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == speed) return update_value (& ab -> arp . tempo, v, o, 1);
									if (sa == subdivision) return update_value (& ab -> arp . division, v, o, 1);
									if (sa == active) return update_value (& ab -> arp . active, v, o, 5);
									if (sa == algo) return update_value (& ab -> arp . current_algo, v, o, 1);
									if (sa == hold) return update_value (& ab -> arp . hold, v, o, 5);
								}
							}
							return false;
						}
						if (sa == lfo) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == speed) return update_value (& ab -> lfo . speed, v, o, 1);
									if (sa == wave) return update_value (& ab -> lfo .wave, v, o, 6);
									if (sa == pulse) return update_value (& ab -> lfo . pulse, v, o, 1);
									if (sa == phase) return update_value (& ab -> lfo . phase, v, o, 1);
									if (sa == sync) return update_value (& ab -> lfo . sync, v, o, 5);
									if (sa == vibrato) return update_value (& ab -> vibrato, v, o, 1);
									if (sa == tremolo) return update_value (& ab -> lfo . tremolo, v, o, 1);
									if (sa == wahwah) return update_value (& ab -> lfo . wahwah, v, o, 1);
								}
							}
							return false;
						}
						if (sa == vco) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isInteger ()) {
									int ind = se -> getInteger () - 1;
									if (ind < 0 || ind > 1) return false;
									path = path -> getRight ();
									if (path -> isPair ()) {
										se = path -> getLeft ();
										if (se -> isAtom ()) {
											sa = se -> getAtom ();
											if (sa == freq) return update_value (& ab -> vco [ind] . freq, v, o, 2);
											if (sa == amp) return update_value (& ab -> vco [ind] . amp, v, o, 3);
											if (sa == ratio) return update_value (& ab -> vco [ind] . ratio, v, o, 7);
											if (sa == wave) return update_value (& ab -> vco [ind] . wave, v, o, 6);
											if (sa == sens) {
												path = path -> getRight ();
												if (path -> isPair ()) {
													se = path -> getLeft ();
													if (se -> isAtom ()) {
														sa = se -> getAtom ();
														if (sa == key) return update_value (& ab -> vco [ind] . sens . key, v, o, 1);
														if (sa == adsr) return update_value(& ab -> vco [ind] . sens . adsr, v, o, 1);
													}
												}
												return false;
											}
										}
									}
									return false;
								}
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == ringmod) return update_value (& ab -> ringmod, v, o, 1);
								}
							}
							return false;
						}
						if (sa == noise) return update_value (& ab -> noise, v, o, 1);
						if (sa == filter) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == freq) return update_value (& ab -> filter . freq, v, o, 2);
									if (sa == resonance) return update_value (& ab -> filter . resonance, v, o, 1);
									if (sa == amp) return update_value (& ab -> filter . amp, v, o, 3);
									if (sa == sens) {
										path = path -> getRight ();
										if (path -> isPair ()) {
											se = path -> getLeft ();
											if (se -> isAtom ()) {
												sa = se -> getAtom ();
												if (sa == key) return update_value (& ab -> filter . sens . key, v, o, 1);
												if (sa == adsr) return update_value (& ab -> filter . sens . adsr, v, o, 1);
											}
										}
										return false;
									}
								}
							}
							return false;
						}
						if (sa == adsr) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isInteger ()) {
									int ind = se -> getInteger () - 1;
									if (ind == 0) {
										path = path -> getRight ();
										if (path -> isPair ()) {
											se = path -> getLeft ();
											if (se -> isAtom ()) {
												sa = se -> getAtom ();
												if (sa == attack) return update_value (& ab -> adsr1 . attack, v, o, 4);
												if (sa == decay) return update_value (& ab -> adsr1 . decay, v, o, 4);
												if (sa == sustain) return update_value (& ab -> adsr1 . sustain, v, o, 3);
												if (sa == release) return update_value (& ab -> adsr1 . release, v, o, 4);
											}
										}
										return false;
									}
									if (ind == 1) {
										path = path -> getRight ();
										if (path -> isPair ()) {
											se = path -> getLeft ();
											if (se -> isAtom ()) {
												sa = se -> getAtom ();
												if (sa == time1) return update_value (& ab -> adsr2 . time [0], v, o, 4);
												if (sa == time2) return update_value (& ab -> adsr2 . time [1], v, o, 4);
												if (sa == time3) return update_value (& ab -> adsr2 . time [2], v, o, 4);
												if (sa == time4) return update_value (& ab -> adsr2 . time [3], v, o, 4);
												if (sa == level1) return update_value (& ab -> adsr2 . level [0], v, o, 1);
												if (sa == level2) return update_value (& ab -> adsr2 . level [1], v, o, 1);
												if (sa == level3) return update_value (& ab -> adsr2 . level [2], v, o, 1);
												if (sa == level4) return update_value (& ab -> adsr2 . level [3], v, o, 1);
											}
										}
										return false;
									}
									return false;
								}
							}
							return false;
						}
						if (sa == portamento) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == porta) return update_value (& ab -> portamento . porta, v, o, 5);
									if (sa == time) return update_value (& ab -> portamento . time, v, o, 4);
									if (sa == legato) return update_value (& ab -> portamento . legato, v, o, 5);
									if (sa == hold) return update_value (& ab -> portamento . hold, v, o, 5);
								}
							}
							return false;
						}
						if (sa == key_map) {
							if (v -> isVar ()) return ab -> key_map . return_content (v);
							if (v -> isPair ()) return ab -> key_map . read_content (v);
							return false;
						}
					}
					return false;
				}
			}
		}
		return native_moonbase :: code (parameters, resolution);
	}
	native_integrated_abakos (PrologAtom * atom, orbiter_core * core, orbiter * module, PrologDirectory * directory) : native_moonbase (directory, atom, core, module) {
		core_atom = 0;
		volume = pan = pitch = modulation = sens = vibrato = 0;
		chorus = level = time = bias = speed = phase = amp = 0;
		delay = balance = feedback = highdamp = 0;
		arpeggiator = subdivision = active = algo = hold = 0;
		lfo = wave = pulse = sync = tremolo = wahwah = 0;
		vco = freq = ratio = key = ringmod = noise = 0;
		adsr = attack = decay = sustain = release = 0;
		time1 = time2 = time3 = time4 = level1 = level2 = level3 = level4 = 0;
		filter = resonance = 0;
		portamento = porta = legato;
		key_map = 0;
		if (directory == 0) return;
		core_atom = directory -> searchAtom ("core");
		volume = directory -> searchAtom ("volume");
		pan = directory -> searchAtom ("pan");
		pitch = directory -> searchAtom ("pitch");
		modulation = directory -> searchAtom ("modulation");
		sens = directory -> searchAtom ("sens");
		vibrato = directory -> searchAtom ("vibrato");
		chorus = directory -> searchAtom ("chorus");
		level = directory -> searchAtom ("level");
		time = directory -> searchAtom ("time");
		bias = directory -> searchAtom ("bias");
		speed = directory -> searchAtom ("speed");
		phase = directory -> searchAtom ("phase");
		amp = directory -> searchAtom ("amp");
		delay = directory -> searchAtom ("delay");
		balance = directory -> searchAtom ("balance");
		feedback = directory -> searchAtom ("feedback");
		highdamp = directory -> searchAtom ("highdamp");
		arpeggiator = directory -> searchAtom ("arpeggiator");
		subdivision = directory -> searchAtom ("subdivision");
		active = directory -> searchAtom ("active");
		algo = directory -> searchAtom ("algo");
		hold = directory -> searchAtom ("hold");
		lfo = directory -> searchAtom ("lfo");
		wave = directory -> searchAtom ("wave");
		pulse = directory -> searchAtom ("pulse");
		sync = directory -> searchAtom ("sync");
		tremolo = directory -> searchAtom ("tremolo");
		wahwah = directory -> searchAtom ("wahwah");
		vco = directory -> searchAtom ("vco");
		freq = directory -> searchAtom ("freq");
		ratio = directory -> searchAtom ("ratio");
		key = directory -> searchAtom ("key");
		ringmod = directory -> searchAtom ("ringmod");
		noise = directory -> searchAtom ("noise");
		adsr = directory -> searchAtom ("adsr");
		attack = directory -> searchAtom ("attack");
		decay = directory -> searchAtom ("decay");
		sustain = directory -> searchAtom ("sustain");
		release = directory -> searchAtom ("release");
		time1 = directory -> searchAtom ("time1");
		time2 = directory -> searchAtom ("time2");
		time3 = directory -> searchAtom ("time3");
		time4 = directory -> searchAtom ("time4");
		level1 = directory -> searchAtom ("level1");
		level2 = directory -> searchAtom ("level2");
		level3 = directory -> searchAtom ("level3");
		level4 = directory -> searchAtom ("level4");
		filter = directory -> searchAtom ("filter");
		resonance = directory -> searchAtom ("resonance");
		portamento = directory -> searchAtom ("portamento");
		porta = directory -> searchAtom ("porta");
		legato = directory -> searchAtom ("legato");
		key_map = directory -> searchAtom ("key_map");
	}
};

orbiter * integrated_abakos_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * polyphony = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isInteger ()) polyphony = el;
		parameters = parameters -> getRight ();
	}
	return new integrated_abakos (core, polyphony != 0 ? polyphony -> getInteger () : 8);
}
PrologNativeOrbiter * integrated_abakos_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new native_integrated_abakos (atom, core, module, directory);}
integrated_abakos_class :: integrated_abakos_class (orbiter_core * core, PrologDirectory * directory) : PrologNativeOrbiterCreator (core) {this -> directory = directory;}
