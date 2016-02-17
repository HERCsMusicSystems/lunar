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

////////////////////////////////////////////////////////////////////////
// This file was created on Friday, 29th January 2016 at 15:47:00 PM. //
////////////////////////////////////////////////////////////////////////

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

class integrated_phobos;

struct pb_struct {double BP, left, right;};

struct operator_structure {
	double freq, amp, ratio, feedback;
	struct {
		double time [4], level [4];
		pb_struct egscal;
	} eg;
	struct {
		struct {
			pb_struct key;
			double eg, pitch, lfo [2];
		} freq;
		struct {
			pb_struct key, X, Y, velocity;
			double lfo;
		} amp;
	} sens;
};

class integrated_phobos_part {
public:
	integrated_trigger trigger;
	integrated_fm4_block fm;
	void move (integrated_phobos * phobos);
	integrated_phobos_part (orbiter_core * core) : trigger (core, true, 0), fm (core) {}
};

typedef integrated_phobos_part * integrated_phobos_part_pointer;

class integrated_phobos : public CommandModule {
private:
	int polyphony;
public:
	integrated_moonbase base;
	integrated_arpeggiator arp;
	integrated_phobos_part_pointer * parts;
	integrated_lfo lfo1;
	integrated_lfo lfo2;
	integrated_pan pan;
	double pan_ctrl;
	double X, Y, pitch, auto_ctrl;
	integrated_stereo_chorus chorus;
	integrated_delay delay;
	integrated_drywet dry_wet;
	integrated_stereo_amplifier volume;
	integrated_map key_map;
	operator_structure operators [4];
	double operator_algo;
	struct {double amp, time [4], level [4];} noise;
	struct {double freq, resonance, amp; struct {double eg; pb_struct key;} sens;} filter;
	struct {
		struct {double attack, decay, sustain, release; pb_struct egscal;} amp;
		struct {double time [4], level [4]; pb_struct egscal;} freq;
	} adsr;
	struct {double porta, time, legato, hold;} portamento;
	double lsb;
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
		/*switch (ctrl) {
		case 1: lfo . vibrato = value * 128.0 + lsb; lsb = 0.0; break;
		case 71: lfo . tremolo = value * 128.0 + lsb; lsb = 0.0; break;
		case 7: volume . gateway = value * 128.0 + lsb; lsb = 0.0; break;
		case 10: pan . pan = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 11: trigger . porta_time = value * 128.0 + lsb; lsb = 0.0; break;
		case 64: trigger . hold = value * 128.0 + lsb; lsb = 0.0; break;
		case 65: trigger . porta = value * 128.0 + lsb; lsb = 0.0; break;
		case 68: trigger . legato = value * 128.0 + lsb; lsb = 0.0; break;
		case 74: vco . freq = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 91: dry_wet . balance = value * 128.0 -8192.0 + lsb; lsb = 0.0; break;
		case 95: lfo . speed = value * 128.0 -8192.0 + lsb; lsb = 0.0; break;
		case 73: adsr . attack = value * 128.0 + lsb; lsb = 0.0; break;
		case 93: adsr . decay = value * 128.0 + lsb; lsb = 0.0; break;
		case 94: adsr . sustain = value * 128.0 - 16384.0 + lsb; lsb = 0.0; break;
		case 72: adsr . release = value * 128.0 + lsb; lsb = 0.0; break;
		case 126: case 127: lsb = 0.0; break;
		default: lsb = value; break;
		}*/
	}
	double getControl (int ctrl) {
		switch (ctrl) {
		case 7: return volume . gateway * 0.0078125; break;
		}
		/*switch (ctrl) {
		case 1: return lfo . vibrato * 0.0078125; break;
		case 71: return lfo . tremolo * 0.0078125; break;
		case 7: return volume . gateway * 0.0078125; break;
		case 10: return pan . pan * 0.0078125 + 64.0; break;
		case 11: return trigger . porta_time * 0.0078125; break;
		case 64: return trigger . hold *  0.0078125; break;
		case 65: return trigger . porta * 0.0078125; break;
		case 68: return trigger . legato *  0.0078125; break;
		case 74: return vco . freq * 0.0078125 + 64.0; break;
		case 91: return dry_wet . balance * 0.0078125 + 64.0; break;
		case 95: return lfo . speed * 0.0078125; + 64.0; break;
		case 73: return adsr . attack * 0.0078125; break;
		case 93: return adsr . decay * 0.0078125; break;
		case 94: return adsr . sustain * 0.0078125 + 128.0; break;
		case 72: return adsr . release * 0.0078125; break;
		case 126: return 1.0; break;
		case 127: return 0.0; break;
		default: break;
		}*/
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
		//switch (ind) {
		//case 0: return & volume . left; break;
		//case 1: return & volume . right; break;
		//default: break;
		//}
		return CommandModule :: outputAddress (ind);
	}
	void move (void) {
		chorus . move ();
		delay . move ();
		for (int ind = 0; ind < polyphony; ind++) parts [ind] -> move (this);
		/*
		trigger . move ();
		adsr . trigger = trigger . trigger;
		adsr . move ();
		lfo . move ();
		vco . freq = freq + trigger . signal + lfo . vibrato_signal;
		vco . amp = amp + lfo . tremolo_signal;
		vco . move ();
		pan . enter = vco . signal * adsr . signal;
		pan . pan = pan_ctrl + lfo . pan_signal;
		pan . move ();
		delay . enter = pan . left;
		delay . enter_right = pan . right;
		delay . move ();
		dry_wet . dry_left = pan . left;
		dry_wet . dry_right = pan . right;
		dry_wet . wet_left = delay . signal;
		dry_wet . wet_right = delay . signal_right;
		dry_wet . move ();
		volume . enter_left = dry_wet . left;
		volume . enter_right = dry_wet . right;
		volume . volume_move ();
		*/
	}
	integrated_phobos (orbiter_core * core, int polyphony = 8)
			: CommandModule (core), base (core), arp (core, & base), lfo1 (core), lfo2 (core), pan (core), chorus (core), delay (core), volume (core, 12800.0) {
		this -> polyphony = polyphony;
		parts = new integrated_phobos_part_pointer [polyphony];
		for (int ind = 0; ind < polyphony; ind++) {
			integrated_phobos_part_pointer p = new integrated_phobos_part (core);
			base . insert_trigger (& p -> trigger);
			parts [ind] = p;
		}
	}
	~ integrated_phobos (void) {for (int ind = 0; ind < polyphony; ind++) delete parts [ind]; delete [] parts; parts = 0;}
	/*integrated_phobos (orbiter_core * core) : CommandModule (core), trigger (core, true, 0), adsr (core), lfo (core), vco (core), pan (core), delay (core), dry_wet (), volume (core, 12800.00) {
		lsb = 0.0;
		freq = amp = 0.0;
		pan_ctrl = 0.0;
		trigger . key_map = & key_map;
		initialise (); activate ();
	}
	*/
};

void integrated_phobos_part :: move (integrated_phobos * phobos) {
	trigger . move ();
}

class native_integrated_phobos : public native_moonbase {
public:
	PrologAtom * core_atom;
	PrologAtom * volume, * pan, * X, * Y, * pitch, * auto_atom;
	PrologAtom * chorus, * level, * time, * bias, * speed, * phase, * amp;
	PrologAtom * delay, * balance, * feedback, * highdamp;
	PrologAtom * arpeggiator, * subdivision, * active, * algo, * hold;
	PrologAtom * lfo, * wave, * pulse, * sync, * vibrato, * tremolo, * wahwah;
	PrologAtom * operator_atom, * freq, * ratio;
	PrologAtom * eg, * time1, * time2, * time3, * time4, * level1, * level2, * level3, * level4;
	PrologAtom * egscal, * BP, * left, * right;
	PrologAtom * sens, * key, * velocity;
	PrologAtom * noise, * filter, * resonance, * adsr;
	PrologAtom * attack, * decay, * sustain, * release;
	PrologAtom * portamento, * porta, * legato;
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
				integrated_phobos * ph = (integrated_phobos *) module;
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
									if (sa == volume) return update_value (& ph -> volume . gateway, v, o, 1);
									if (sa == pan) return update_value (& ph -> pan_ctrl, v, o, 1);
									if (sa == chorus) {
										path = path -> getRight ();
										if (path -> isPair ()) {
											se = path -> getLeft ();
											if (se -> isAtom ()) {
												sa = se -> getAtom ();
												if (sa == level) return update_value (& ph -> chorus . level, v, o, 1);
												if (sa == time) return update_value (& ph -> chorus . time, v, o, 1);
												if (sa == bias) return update_value (& ph -> chorus . bias, v, o, 1);
												if (sa == speed) return update_value (& ph -> chorus . speed, v, o, 1);
												if (sa == phase) return update_value (& ph -> chorus . phase, v, o, 1);
												if (sa == amp) return update_value (& ph -> chorus . amp, v, o, 1);
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
												if (sa == balance) return update_value (& ph -> dry_wet . balance, v, o, 1);
												if (sa == feedback) return update_value (& ph -> delay . feedback, v, o, 1);
												if (sa == time) return update_value (& ph -> delay . time, v, o, 1);
												if (sa == highdamp) return update_value (& ph -> delay . high_damp, v, o, 1);
											}
										}
										return false;
									}
									if (sa == X) return update_value (& ph -> X, v, o, 1);
									if (sa == Y) return update_value (& ph -> Y, v, o, 1);
									if (sa == pitch) return update_value (& ph -> pitch, v, o, 1);
									if (sa == auto_atom) return update_value (& ph -> auto_ctrl, v, o, 5);
								}
							}
						}
						if (sa == arpeggiator) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == speed) return update_value (& ph -> arp . tempo, v, o, 1);
									if (sa == subdivision) return update_value (& ph -> arp . division, v, o, 1);
									if (sa == active) return update_value (& ph -> arp . active, v, o, 5);
									if (sa == algo) return update_value (& ph -> arp . current_algo, v, o, 1);
									if (sa == hold) return update_value (& ph -> arp . hold, v, o, 5);
								}
							}
							return false;
						}
						if (sa == lfo) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (! se -> isInteger ()) return false;
								int ind = se -> getInteger ();
								if (ind == 1) {
									path = path -> getRight ();
									if (path -> isPair ()) {
										se = path -> getLeft ();
										if (se -> isAtom ()) {
											sa = se -> getAtom ();
											if (sa == speed) return update_value (& ph -> lfo1 . speed, v, o, 1);
											if (sa == wave) return update_value (& ph -> lfo1 . wave, v, o, 6);
											if (sa == pulse) return update_value (& ph -> lfo1 . pulse, v, o, 1);
											if (sa == phase) return update_value (& ph -> lfo1 . phase, v, o, 1);
											if (sa == sync) return update_value (& ph -> lfo1 . sync, v, o, 5);
											if (sa == vibrato) return update_value (& ph -> lfo1 . vibrato, v, o, 1);
										}
									}
									return false;
								}
								if (ind == 2) {
									path = path -> getRight ();
									if (path -> isPair ()) {
										se = path -> getLeft ();
										if (se -> isAtom ()) {
											sa = se -> getAtom ();
											if (sa == speed) return update_value (& ph -> lfo2 . speed, v, o, 1);
											if (sa == wave) return update_value (& ph -> lfo2 . wave, v, o, 6);
											if (sa == pulse) return update_value (& ph -> lfo2 . pulse, v, o, 1);
											if (sa == phase) return update_value (& ph -> lfo2 . phase, v, o, 1);
											if (sa == sync) return update_value (& ph -> lfo2 . sync, v, o, 5);
											if (sa == vibrato) return update_value (& ph -> lfo2 . vibrato, v, o, 1);
											if (sa == tremolo) return update_value (& ph -> lfo2 . tremolo, v, o, 1);
											if (sa == wahwah) return update_value (& ph -> lfo2 . wahwah, v, o, 1);
											if (sa == pan) return update_value (& ph -> lfo2 . pan, v, o, 1);
										}
									}
									return false;
								}
							}
							return false;
						}
						if (sa == operator_atom) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {sa = se -> getAtom (); if (sa == algo) return update_value (& ph -> operator_algo, v, o, 8); return false;}
								if (se -> isInteger ()) {
									int ind = se -> getInteger () - 1;
									if (ind < 0 || ind > 3) return false;
									path = path -> getRight ();
									if (path -> isPair ()) {
										se = path -> getLeft ();
										if (se -> isAtom ()) {
											sa = se -> getAtom ();
											if (sa == freq) return update_value (& ph -> operators [ind] . freq, v, o, 2);
											if (sa == amp) return update_value (& ph -> operators [ind] . amp, v, o, 3);
											if (sa == ratio) return update_value (& ph -> operators [ind] . ratio, v, o, 7);
											if (sa == feedback) return update_value (& ph -> operators [ind] . feedback, v, o, 1);
											if (sa == eg) {
												path = path -> getRight ();
												if (path -> isPair ()) {
													se = path -> getLeft ();
													if (se -> isAtom ()) {
														sa = se -> getAtom ();
														if (sa == time1) return update_value (& ph -> operators [ind] . eg . time [0], v, o, 4);
														if (sa == time2) return update_value (& ph -> operators [ind] . eg . time [1], v, o, 4);
														if (sa == time3) return update_value (& ph -> operators [ind] . eg . time [2], v, o, 4);
														if (sa == time4) return update_value (& ph -> operators [ind] . eg . time [3], v, o, 4);
														if (sa == level1) return update_value (& ph -> operators [ind] . eg . level [0], v, o, 3);
														if (sa == level2) return update_value (& ph -> operators [ind] . eg . level [1], v, o, 3);
														if (sa == level3) return update_value (& ph -> operators [ind] . eg . level [2], v, o, 3);
														if (sa == level4) return update_value (& ph -> operators [ind] . eg . level [3], v, o, 3);
														if (sa == egscal) {
															path = path -> getRight ();
															if (path -> isPair ()) {
																se = path -> getLeft ();
																if (se -> isAtom ()) {
																	sa = se -> getAtom ();
																	if (sa == BP) return update_value (& ph -> operators [ind] . eg . egscal . BP, v, o, 1);
																	if (sa == left) return update_value (& ph -> operators [ind] . eg . egscal . left, v, o, 1);
																	if (sa == right) return update_value (& ph -> operators [ind] . eg . egscal . right, v, o, 1);
																}
															}
															return false;
														}
													}
												}
												return false;
											}
											if (sa == sens) {
												path = path -> getRight ();
												if (path -> isPair ()) {
													se = path -> getLeft ();
													if (se -> isAtom ()) {
														sa = se -> getAtom ();
														if (sa == freq) {
															path = path -> getRight ();
															if (path -> isPair ()) {
																se = path -> getLeft ();
																if (se -> isAtom ()) {
																	sa = se -> getAtom ();
																	if (sa == key) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> operators [ind] . sens . freq . key . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> operators [ind] . sens . freq . key . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> operators [ind] . sens . freq . key . right, v, o, 1);
																			}
																		}
																		return false;
																	}
																	if (sa == eg)  return update_value (& ph -> operators [ind] . sens . freq . eg, v, o, 1);
																}
															}
															return false;
														}
														if (sa == amp) {
															path = path -> getRight ();
															if (path -> isPair ()) {
																se = path -> getLeft ();
																if (se -> isAtom ()) {
																	sa = se -> getAtom ();
																	if (sa == key) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> operators [ind] . sens . amp . key . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> operators [ind] . sens . amp . key . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> operators [ind] . sens . amp . key . right, v, o, 1);
																			}
																		}
																		return false;
																	}
																	if (sa == X) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> operators [ind] . sens . amp . X . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> operators [ind] . sens . amp . X . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> operators [ind] . sens . amp . X . right, v, o, 1);
																			}
																		}
																		return false;
																	}
																	if (sa == Y) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> operators [ind] . sens . amp . Y . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> operators [ind] . sens . amp . Y . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> operators [ind] . sens . amp . Y . right, v, o, 1);
																			}
																		}
																		return false;
																	}
																	if (sa == velocity) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> operators [ind] . sens . amp . velocity . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> operators [ind] . sens . amp . velocity . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> operators [ind] . sens . amp . velocity . right, v, o, 1);
																			}
																		}
																		return false;
																	}
																}
															}
															return false;
														}
													}
												}
												return false;
											}
											if (sa == noise) {
												path = path -> getRight ();
												if (path -> isPair ()) {
													se = path -> getLeft ();
													if (se -> isAtom ()) {
														sa = se -> getAtom ();
														if (sa == amp) return update_value (& ph -> noise . amp, v, o, 3);
														if (sa == time1) return update_value (& ph -> noise . time [0], v, o, 4);
														if (sa == time2) return update_value (& ph -> noise . time [1], v, o, 4);
														if (sa == time3) return update_value (& ph -> noise . time [2], v, o, 4);
														if (sa == time4) return update_value (& ph -> noise . time [3], v, o, 4);
														if (sa == level1) return update_value (& ph -> noise . level [0], v, o, 3);
														if (sa == level2) return update_value (& ph -> noise . level [1], v, o, 3);
														if (sa == level3) return update_value (& ph -> noise . level [2], v, o, 3);
														if (sa == level4) return update_value (& ph -> noise . level [3], v, o, 3);
													}
												}
												return false;
											}
											if (sa == filter) {
												path = path -> getRight ();
												if (path -> isPair ()) {
													se = path -> getLeft ();
													if (se -> isAtom ()) {
														sa = se -> getAtom ();
														if (sa == freq) return update_value (& ph -> filter . freq, v, o, 2);
														if (sa == resonance) return update_value (& ph -> filter . resonance, v, o, 1);
														if (sa == amp) return update_value (& ph -> filter . amp, v, 0, 3);
														if (sa == sens) {
															path = path -> getRight ();
															if (path -> isPair ()) {
																se = path -> getLeft ();
																if (se -> isAtom ()) {
																	sa = se -> getAtom ();
																	if (sa == eg) return update_value (& ph -> filter . sens . eg, v, o, 1);
																	if (sa == key) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> filter . sens . key . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> filter . sens . key . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> filter . sens . key . right, v, o, 1);
																			}
																		}
																		return false;
																	}
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
													if (se -> isAtom ()) {
														sa = se -> getAtom ();
														if (sa == amp) {
															path = path -> getRight ();
															if (path -> isPair ()) {
																se = path -> getLeft ();
																if (se -> isAtom ()) {
																	sa = se -> getAtom ();
																	if (sa == attack) return update_value (& ph -> adsr . amp . attack, v, o, 4);
																	if (sa == decay) return update_value (& ph -> adsr . amp . decay, v, o, 4);
																	if (sa == sustain) return update_value (& ph -> adsr . amp . sustain, v, o, 3);
																	if (sa == release) return update_value (& ph -> adsr . amp . release, v, o, 4);
																	if (sa == egscal) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> adsr . amp . egscal . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> adsr . amp . egscal . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> adsr . amp . egscal . right, v, o, 1);
																			}
																		}
																		return false;
																	}
																}
															}
															return false;
														}
														if (sa == freq) {
															path = path -> getRight ();
															if (path -> isPair ()) {
																se = path -> getLeft ();
																if (se -> isAtom ()) {
																	sa = se -> getAtom ();
																	if (sa == time1) return update_value (& ph -> adsr . freq . time [0], v, o, 4);
																	if (sa == time2) return update_value (& ph -> adsr . freq . time [1], v, o, 4);
																	if (sa == time3) return update_value (& ph -> adsr . freq . time [2], v, o, 4);
																	if (sa == time4) return update_value (& ph -> adsr . freq . time [3], v, o, 4);
																	if (sa == level1) return update_value (& ph -> adsr . freq . level [0], v, o, 3);
																	if (sa == level2) return update_value (& ph -> adsr . freq . level [1], v, o, 3);
																	if (sa == level3) return update_value (& ph -> adsr . freq . level [2], v, o, 3);
																	if (sa == level4) return update_value (& ph -> adsr . freq . level [3], v, o, 3);
																	if (sa == egscal) {
																		path = path -> getRight ();
																		if (path -> isPair ()) {
																			se = path -> getLeft ();
																			if (se -> isAtom ()) {
																				sa = se -> getAtom ();
																				if (sa == BP) return update_value (& ph -> adsr . freq . egscal . BP, v, o, 1);
																				if (sa == left) return update_value (& ph -> adsr . freq . egscal . left, v, o, 1);
																				if (sa == right) return update_value (& ph -> adsr . freq . egscal . right, v, o, 1);
																			}
																		}
																		return false;
																	}
																}
															}
															return false;
														}
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
														if (sa == porta) return update_value (& ph -> portamento . porta, v, o, 5);
														if (sa == time) return update_value (& ph -> portamento . time, v, o, 4);
														if (sa == legato) return update_value (& ph -> portamento . legato, v, o, 5);
														if (sa == hold) return update_value (& ph -> portamento . hold, v, o, 5);
													}
												}
												return false;
											}
										}
									}
								}
							}
							return false;
						}
					}
					return false;
				}
			}
		}
		return native_moonbase :: code (parameters, resolution);
	}
	native_integrated_phobos (PrologAtom * atom, orbiter_core * core, orbiter * module, PrologDirectory * directory) : native_moonbase (directory, atom, core, module) {
		core_atom = 0;
		volume = pan = X = Y = pitch = auto_atom = 0;
		chorus = level = time = bias = speed = phase = amp = 0;
		delay = balance = feedback = highdamp = 0;
		arpeggiator = subdivision = active = algo = hold = 0;
		lfo = wave = pulse = sync = vibrato = tremolo = wahwah = 0;
		operator_atom = freq = ratio = 0;
		eg = time1 = time2 = time3 = time4 = level1 = level2 = level3 = level4 = 0;
		egscal = BP = left = right = 0;
		sens = key = velocity = 0;
		noise = filter = resonance = adsr = 0;
		attack = decay = sustain = release = 0;
		portamento = porta = legato = 0;
		if (directory == 0) return;
		core_atom = directory -> searchAtom ("core");
		volume = directory -> searchAtom ("volume");
		pan = directory -> searchAtom ("pan");
		X = directory -> searchAtom ("X");
		Y = directory -> searchAtom ("Y");
		pitch = directory -> searchAtom ("pitch");
		auto_atom = directory -> searchAtom ("auto");
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
		vibrato = directory -> searchAtom ("vibrato");
		tremolo = directory -> searchAtom ("tremolo");
		wahwah = directory -> searchAtom ("wahwah");
		operator_atom = directory -> searchAtom ("operator");
		freq = directory -> searchAtom ("freq");
		ratio = directory -> searchAtom ("ratio");
		eg = directory -> searchAtom ("eg");
		time1 = directory -> searchAtom ("time1");
		time2 = directory -> searchAtom ("time2");
		time3 = directory -> searchAtom ("time3");
		time4 = directory -> searchAtom ("time4");
		level1 = directory -> searchAtom ("level1");
		level2 = directory -> searchAtom ("level2");
		level3 = directory -> searchAtom ("level3");
		level4 = directory -> searchAtom ("level4");
		egscal = directory -> searchAtom ("egscal");
		BP = directory -> searchAtom ("BP");
		left = directory -> searchAtom ("left");
		right = directory -> searchAtom ("right");
		sens = directory -> searchAtom ("sens");
		key = directory -> searchAtom ("key");
		velocity = directory -> searchAtom ("velocity");
		noise = directory -> searchAtom ("noise");
		filter = directory -> searchAtom ("filter");
		resonance = directory -> searchAtom ("resonance");
		adsr = directory -> searchAtom ("adsr");
		attack = directory -> searchAtom ("attack");
		decay = directory -> searchAtom ("decay");
		sustain = directory -> searchAtom ("sustain");
		release = directory -> searchAtom ("release");
		portamento = directory -> searchAtom ("portamento");
		porta = directory -> searchAtom ("porta");
		legato = directory -> searchAtom ("legato");
	}
};

orbiter * integrated_phobos_class :: create_orbiter (PrologElement * parameters) {
	PrologElement * polyphony = 0;
	while (parameters -> isPair ()) {
		PrologElement * el = parameters -> getLeft ();
		if (el -> isInteger ()) polyphony = el;
		parameters = parameters -> getRight ();
	}
	return new integrated_phobos (core, polyphony != 0 ? polyphony -> getInteger () : 8);
}
PrologNativeOrbiter * integrated_phobos_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new native_integrated_phobos (atom, core, module, directory);}
integrated_phobos_class :: integrated_phobos_class (orbiter_core * core, PrologDirectory * directory) : PrologNativeOrbiterCreator (core) {this -> directory = directory;}

