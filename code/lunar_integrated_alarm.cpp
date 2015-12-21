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

///////////////////////////////////////////////////////////////////////////
// This file was created on Thursday, 10th December 2015 at 13:53:52 PM. //
///////////////////////////////////////////////////////////////////////////

#include "lunar_prolog_landers.h"
#include "lunar_landers.h"
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

class integrated_alarm : public CommandModule {
public:
	integrated_trigger trigger;
	integrated_adsr adsr;
	integrated_lfo lfo;
	double lfo_vibrato, lfo_tremolo, lfo_pan;
	integrated_vco vco;
	double freq, amp;
	integrated_pan pan;
	double pan_ctrl;;
	integrated_delay delay;
	integrated_drywet dry_wet;
	integrated_stereo_amplifier volume;
	double lsb;
	bool insert_trigger (lunar_trigger * trigger) {return false;}
	bool insert_controller (orbiter * controller, int location, double shift) {return false;}
	void keyon (int key, int velocity) {trigger . keyon (key, velocity);}
	void keyon (int key) {trigger . keyon (key);}
	void keyoff (void) {trigger . keyoff ();}
	void keyoff (int key, int velocity) {trigger . keyoff (key);}
	void mono (void) {}
	void poly (void) {}
	bool isMonoMode (void) {return true;}
	void control (int ctrl, double value) {
		switch (ctrl) {
		case 1: lfo_vibrato = value * 128.0 + lsb; lsb = 0.0; break;
		case 71: lfo_tremolo = value * 128.0 + lsb; lsb = 0.0; break;
		case 7: volume . gateway = value * 128.0 + lsb; lsb = 0.0; break;
		case 10: pan . pan = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 11: trigger . porta_time = value * 128.0 + lsb; lsb = 0.0; break;
		case 65: trigger . porta = value * 128.0 + lsb; lsb = 0.0; break;
		case 74: vco . freq = value * 128.0 - 8192.0 + lsb; lsb = 0.0; break;
		case 91: dry_wet . balance = value * 128.0 -8192.0 + lsb; lsb = 0.0; break;
		case 95: lfo . speed = value * 128.0 -8192.0 + lsb; lsb = 0.0; break;
		case 73: adsr . attack = value * 128.0 + lsb; lsb = 0.0; break;
		case 93: adsr . decay = value * 128.0 + lsb; lsb = 0.0; break;
		case 94: adsr . sustain = value * 128.0 - 16384.0 + lsb; lsb = 0.0; break;
		case 72: adsr . release = value * 128.0 + lsb; lsb = 0.0; break;
		default: lsb = value; break;
		}
	}
	double getControl (int ctrl) {
		switch (ctrl) {
		case 1: return lfo_vibrato * 0.0078125; break;
		case 71: return lfo_tremolo * 0.0078125; break;
		case 7: return volume . gateway * 0.0078125; break;
		case 10: return pan . pan * 0.0078125 + 64.0; break;
		case 11: return trigger . porta_time * 0.0078125; break;
		case 65: return trigger . porta * 0.0078125; break;
		case 74: return vco . freq * 0.0078125 + 64.0; break;
		case 91: return dry_wet . balance * 0.0078125 + 64.0; break;
		case 95: return lfo . speed * 0.0078125; + 64.0; break;
		case 73: return adsr . attack * 0.0078125; break;
		case 93: return adsr . decay * 0.0078125; break;
		case 94: return adsr . sustain * 0.0078125 + 128.0; break;
		case 72: return adsr . release * 0.0078125; break;
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
		trigger . move ();
		adsr . trigger = trigger . trigger;
		adsr . move ();
		lfo . move ();
		vco . freq = freq + trigger . signal + lfo . signal * lfo_vibrato;
		vco . amp = amp + adsr . signal + lfo . negative * lfo_tremolo;
		vco . move ();
		pan . enter = vco . signal;
		pan . pan = pan_ctrl + lfo . signal * lfo_pan;
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
	}
	integrated_alarm (orbiter_core * core) : CommandModule (core), trigger (core, true, 0), adsr (core), lfo (core), vco (core), pan (core), delay (core), dry_wet (), volume (core, 12800.00) {
		lsb = 0.0;
		lfo_vibrato = lfo_tremolo = lfo_pan = 0.0;
		freq = amp = 0.0;
		pan_ctrl = 0.0;
		initialise (); activate ();
	}
};

class native_integrated_alarm : public native_moonbase {
public:
	PrologAtom * volume, * pan, *delay, * portamento, * vco, * adsr, * lfo;
	PrologAtom * balance, * feedback, * time, * highdamp;
	PrologAtom * porta, * legato, * hold;
	PrologAtom * freq, * amp, * ratio, * wave;
	PrologAtom * attack, * decay, * sustain, * release;
	PrologAtom * speed, * pulse, * phase, * sync, * vibrato, * tremolo;
	bool update_value (double * location, PrologElement * el) {
		if (el -> isNumber ()) * location = el -> getNumber ();
		else el -> setDouble (* location);
		return true;
	}
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (native_moonbase :: code (parameters, resolution)) return true;
		if (parameters -> isPair ()) {
			PrologElement * v = parameters -> getLeft ();
			if (v -> isNumber () || v -> isVar ()) {
				integrated_alarm * al = (integrated_alarm *) module;
				PrologElement * path = parameters -> getRight ();
				PrologElement * se;
				PrologAtom * sa;
				if (path -> isPair ()) {
					se = path -> getLeft ();
					if (se -> isAtom ()) {
						sa = se -> getAtom ();
						if (sa == volume) return update_value (& al -> volume . gateway, v);
						if (sa == pan) return update_value (& al -> pan_ctrl, v);
						if (sa == delay) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == balance) return update_value (& al -> dry_wet . balance, v);
									if (sa == feedback) return update_value (& al -> delay . feedback, v);
									if (sa == time) return update_value (& al -> delay . time, v);
									if (sa == highdamp) return update_value (& al -> delay .high_damp, v);
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
									if (sa == porta) return update_value (& al -> trigger . porta, v);
									if (sa == time) return update_value (& al -> trigger . porta_time, v);
									if (sa == legato) return update_value (& al -> trigger . legato, v);
									if (sa == hold) return update_value (& al -> trigger . hold, v);
								}
							}
							return false;
						}
						if (sa == vco) {
							path = path -> getRight ();
							if (path -> isPair ()) {
								se = path -> getLeft ();
								if (se -> isAtom ()) {
									sa = se -> getAtom ();
									if (sa == freq) return update_value (& al -> freq, v);
									if (sa == amp) return update_value (& al -> amp, v);
									if (sa == ratio) return update_value (& al -> vco . ratio, v);
									if (sa == wave) return update_value (& al -> vco . wave, v);
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
									if (sa == attack) return update_value (& al -> adsr . attack, v);
									if (sa == decay) return update_value (& al -> adsr . decay, v);
									if (sa == sustain) return update_value (& al -> adsr . sustain, v);
									if (sa == release) return update_value (& al -> adsr . release, v);
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
									if (sa == speed) return update_value (& al -> lfo . speed, v);
									if (sa == wave) return update_value (& al -> lfo . wave, v);
									if (sa == pulse) return update_value (& al -> lfo . pulse, v);
									if (sa == phase) return update_value (& al -> lfo . phase, v);
									if (sa == sync) return update_value (& al -> lfo . sync, v);
									if (sa == vibrato) return update_value (& al -> lfo_vibrato, v);
									if (sa == tremolo) return update_value (& al -> lfo_tremolo, v);
									if (sa == pan) return update_value (& al -> lfo_pan, v);
								}
							}
							return false;
						}
					}
					return false;
				}
			}
		}
		return PrologNativeOrbiter :: code (parameters, resolution);
	}
	native_integrated_alarm (PrologAtom * atom, orbiter_core * core, orbiter * module, PrologDirectory * directory) : native_moonbase (directory, atom, core, module) {
		volume = pan = delay = portamento = vco = adsr = lfo = 0;
		balance = feedback = time = highdamp = 0;
		porta = legato = hold = 0;
		freq = amp = ratio = wave = 0;
		attack = decay = sustain = release = 0;
		speed = pulse = phase = sync = vibrato = tremolo = 0;
		if (directory == 0) return;
		volume = directory -> searchAtom ("volume");
		pan = directory -> searchAtom ("pan");
		delay = directory -> searchAtom ("delay");
		portamento = directory -> searchAtom ("portamento");
		vco = directory -> searchAtom ("vco");
		adsr = directory -> searchAtom ("adsr");
		lfo = directory -> searchAtom ("lfo");
		balance = directory -> searchAtom ("balance");
		feedback = directory -> searchAtom ("feedback");
		time = directory -> searchAtom ("time");
		highdamp = directory -> searchAtom ("highdamp");
		porta = directory -> searchAtom ("porta");
		legato = directory -> searchAtom ("legato");
		hold = directory -> searchAtom ("hold");
		freq = directory -> searchAtom ("freq");
		amp = directory -> searchAtom ("amp");
		ratio = directory -> searchAtom ("ratio");
		wave = directory -> searchAtom ("wave");
		attack = directory -> searchAtom ("attack");
		decay = directory -> searchAtom ("decay");
		sustain = directory -> searchAtom ("sustain");
		release = directory -> searchAtom ("release");
		speed = directory -> searchAtom ("speed");
		pulse = directory -> searchAtom ("pulse");
		phase = directory -> searchAtom ("phase");
		sync = directory -> searchAtom ("sync");
		vibrato = directory -> searchAtom ("vibrato");
		tremolo = directory -> searchAtom ("tremolo");
	}
};

orbiter * integrated_alarm_class :: create_orbiter (PrologElement * parameters) {return new integrated_alarm (core);}
PrologNativeOrbiter * integrated_alarm_class :: create_native_orbiter (PrologAtom * atom, orbiter * module) {return new native_integrated_alarm (atom, core, module, directory);}
integrated_alarm_class :: integrated_alarm_class (orbiter_core * core, PrologDirectory * directory) : PrologNativeOrbiterCreator (core) {this -> directory = directory;}






















































/*
class native_midi_processor {
private:
	PrologAtom * keyon, * keyoff, *pitch, * control, * mono, * poly, * timingclock;
	chromatograph graph;
public:
	virtual bool insert_trigger (lunar_trigger * trigger) {return false;}
	virtual bool insert_controller (orbiter * controller, int index, int shift) {return false;}
	virtual void keyon_command (int key, int velocity) {}
	virtual void keyon_command (int key) {keyon_command (key, 100);}
	virtual void keyoff_command (int key, int velocity) {}
	virtual void keyoff_command (void) {}
	virtual double getControl (int ctrl) {return 0.0;}
	virtual void control_command (int ctrl, double value) {}
	virtual bool isMonoMode (void) {return false;}
	virtual void mono_command (void) {}
	virtual void poly_command (void) {}
	virtual void timing_clock_command (void) {}
	bool code (PrologElement * parameters) {
		PrologElement * atom = 0;
		PrologElement * key = 0;
		PrologElement * velocity = 0;
		PrologElement * note = 0;
		PrologElement * octave = 0;
		PrologElement * var = 0;
		PrologElement * pp = parameters;
		while (pp -> isPair ()) {
			PrologElement * el = pp -> getLeft ();
			if (el -> isAtom ()) atom = el;
			if (el -> isEarth ()) atom = el;
			if (el -> isNumber ()) if (key == 0 && note == 0) key = el; else velocity = el;
			if (el -> isVar ()) var = el;
			if (el -> isPair ()) {
				note = el -> getLeft ();
				octave = el -> getRight ();
				if (octave -> isPair ()) octave = octave -> getLeft ();
				if (! note -> isAtom ()) note = 0;
				if (! octave -> isInteger ()) octave = 0;
			}
			pp = pp -> getRight ();
		}
		if (pp -> isVar ()) var = pp;
		if (atom != 0) {
			if (atom -> isEarth ()) {
				if (key != 0) return insert_controller (0, (int) key -> getNumber (), 0);
				return true;
			}
			if (atom -> isAtom ()) {
				PrologAtom * a = atom -> getAtom ();
				if (a == keyon) {
					int key_note;
					if (key != 0) key_note = (int) key -> getNumber ();
					else if (note != 0 && octave != 0) key_note = graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48;
					else return false;
					if (velocity == 0) keyon_command (key_note);
					else keyon_command (key_note, (int) velocity -> getNumber ());
					return true;
				}
				if (a == keyoff) {
					if (key != 0) keyoff_command ((int) key -> getNumber (), velocity == 0 ? 0 : (int) velocity -> getNumber ());
					else if (note != 0 && octave != 0)
						keyoff_command (graph . chromatic (note -> getAtom ()) + octave -> getInteger () * 12 + 48,
																				velocity == 0 ? 0 : (int) velocity -> getNumber ());
					else keyoff_command ();
					return true;
				}
				if (a == control) {
					if (key != 0) {
						if (var != 0) {var -> setDouble (getControl ((int) key -> getNumber ())); return true;}
						if (velocity != 0) {control_command ((int) key -> getNumber (), velocity -> getNumber ()); return true;}
					}
					if (var != 0) {var -> setAtom (isMonoMode () ? mono : poly); return true;}
					return false;
				}
				if (a == pitch) {
					if (key != 0 && velocity != 0) {
						int lsb = (int) key -> getNumber ();
						int msb = (int) velocity -> getNumber ();
						if (msb == 127 && lsb == 127) {msb = 128; lsb = 0;}
						control_command (128, (double) msb + (double) lsb * 0.0078125);
						return true;
					}
					if (var != 0) {var -> setDouble (getControl (128)); return true;}
					return false;
				}
				if (a == mono) {mono_command (); return true;}
				if (a == poly) {poly_command (); return true;}
				if (a == timingclock) {timing_clock_command (); return true;}
				PrologNativeCode * machine = a -> getMachine ();
				if (machine == 0) return false;
				if (machine -> isTypeOf (trigger_native_orbiter :: name ())) {
					return insert_trigger ((lunar_trigger *) ((trigger_native_orbiter *) machine) -> module);
				}
				if (machine -> isTypeOf (PrologNativeOrbiter :: name ()) && key != 0) {
					return insert_controller ((orbiter *) ((PrologNativeOrbiter *) machine) -> module,
														(int) key -> getNumber (), velocity != 0 ? (int) velocity -> getNumber () : 0);
				}
			}
		}
		return false;
	}
	native_midi_processor (PrologDirectory * dir) : graph (dir) {
		keyon = keyoff = pitch = control = mono = poly = timingclock = 0;
		if (dir == 0) return;
		keyon = dir -> searchAtom ("keyon");
		keyoff = dir -> searchAtom ("keyoff");
		pitch = dir -> searchAtom ("pitch");
		control = dir -> searchAtom ("control");
		mono = dir -> searchAtom ("mono");
		poly = dir -> searchAtom ("poly");
		timingclock = dir -> searchAtom ("timingclock");
	}
};
*/

