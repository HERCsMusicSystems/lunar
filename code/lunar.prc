;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This file was create on Monday, 14th July 2014 at 11:36:34 AM. ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio
import prcgtk

program lunar #machine := "lunar"
			[
				small_keyboard keyboard big_keyboard oscilloscope spectroscope big_oscilloscope big_spectroscope vector CommandCentre detector
				noise orbiter X Y modulation vibrato tremolo wahwah range
				core base moonbase arpeggiator sequencer polysequencer operator parameter_block auto auto_data key_map velocity_map impulse
				trigger inactive_trigger delay1 delay2 mixer stereo_mixer gateway stereo_gateway amplifier stereo_amplifier volume mono_volume
				lfo adsr eg egscaling egscal vco DCOffsetFilter DCOffsetFilterMono
				index shift bias
				square_operator fm4 fm6 dx9 dx7 saw_operator noise_operator sampler_operator sampler
				sensitivity sens filter delay reverb freeverb chorus stereo_chorus
				pan power_pan linear_pan stereo_pan stereo_power_pan stereo_linear_pan
				drywet drywet_mono dry wet balance
				level level1 level2 level3 level4 time1 time2 time3 time4 attack decay sustain release
				freq amp ratio sync cutoff resonance formant hold busy portamento porta
				legato time speed wave pulse phase poly feedback highdamp diffusion
				mono left right mic mic_left mic_right breakpoint BP algo key_ratio
				key velocity keyon keyoff polyaftertouch control programchange aftertouch pitch
				sysex timingclock START CONTINUE STOP activesensing
				subdivision active
				Lunar Lander Activate Deactivate
				Connect ConnectStereo ConnectDryWet Disconnect DisconnectStereo DisconnectDryWet
				AddParameterBlock AddNamedParameterBlock
				Moonbase AddModule Insert InsertPB InsertController InsertIO InsertBlock Store Restore SubRestore
				Moons AllocateChannel CloseAllMoons ConnectAllMoons
				Cbb Cb C C# Cx
				Dbb Db D D# Dx
				Ebb Eb E E# Ex
				Fbb Fb F F# Fx
				Gbb Gb G G# Gx
				Abb Ab A A# Ax
				Bbb Bb B B# Bx
				midi midi_monitor
				ParameterBlockPanel AdsrPanel EGPanel FEGPanel FM4Panel CorePanel LfoPanel FilterPanel
				DelayPanel ChorusPanel StereoChorusPanel FreeverbPanel
				BuildParameterBlockPanel BuildAdsrPanel BuildEGPanel BuildFEGPanel BuildFM4Panel BuildLfoPanel BuildFilterPanel
				BuildDelayPanel BuildChorusPanel BuildStereoChorusPanel BuildFreeverbPanel
				FindLfoKnob
				MoveModules PropagateSignals MoveCore LunarDrop FUNCTION_KEY
				CreateDistributor CloseDistributor Distribute Redistribute
				CCCB cb_callback cb_path cb_edit_path process_mode CBsub
				LoopWave unicar
				MIDI_CHANNELS MIDI_BACK income_midi
				GenerateInstrumentName InstrumentIndex
				radar reactor commander Core Midi
			]

#machine small_keyboard := "small_keyboard"
#machine keyboard := "keyboard"
#machine big_keyboard := "big_keyboard"
#machine oscilloscope := "oscilloscope"
#machine spectroscope := "spectroscope"
#machine big_oscilloscope := "big_oscilloscope"
#machine big_spectroscope := "big_spectroscope"
#machine vector := "vector"
#machine CommandCentre := "CommandCentre"
#machine parameter_block := "parameter_block"
#machine auto_data := "auto_data"
#machine auto := "auto"
#machine key_map := "key_map"
#machine wave := "wave"
#machine velocity_map := "velocity_map"
#machine impulse := "impulse"
#machine trigger := "trigger"
#machine inactive_trigger := "inactive_trigger"
#machine mixer := "mixer"
#machine stereo_mixer := "stereo_mixer"
#machine control := "control"
#machine gateway := "gateway"
#machine stereo_gateway := "stereo_gateway"
#machine amplifier := "amplifier"
#machine stereo_amplifier := "stereo_amplifier"
#machine volume := "volume"
#machine mono_volume := "mono_volume"
#machine lfo := "lfo"
#machine adsr := "adsr"
#machine eg := "eg"
#machine delay := "delay"
#machine freeverb := "freeverb"
#machine chorus := "chorus"
#machine stereo_chorus := "stereo_chorus"
#machine pan := "pan"
#machine power_pan := "power_pan"
#machine linear_pan := "linear_pan"
#machine stereo_pan := "stereo_pan"
#machine stereo_power_pan := "stereo_power_pan"
#machine stereo_linear_pan := "stereo_linear_pan"
#machine drywet := "drywet"
#machine drywet_mono := "drywet_mono"
#machine sensitivity := "sensitivity"
#machine base := "moonbase"
#machine moonbase := "moonbase"
#machine arpeggiator := "arpeggiator"
#machine sequencer := "sequencer"
#machine polysequencer := "polysequencer"
#machine detector := "detector"

#machine core := "core"
#machine operator := "operator"
#machine fm4 := "fm4"
#machine square_operator := "square_operator"
#machine saw_operator := "saw_operator"
#machine noise_operator := "noise_operator"
#machine sampler_operator := "sampler_operator"
#machine vco := "vco"
#machine filter := "filter"

#machine DCOffsetFilter := "DCOffsetFilter"
#machine DCOffsetFilterMono := "DCOffsetFilterMono"

#machine orbiter := "orbiter"

#machine midi := "midi"

#machine ParameterBlockPanel := "ParameterBlockPanel"
#machine AdsrPanel := "AdsrPanel"
#machine EGPanel := "EGPanel"
#machine FEGPanel := "FEGPanel"
#machine FM4Panel := "FM4Panel"
#machine CorePanel := "CorePanel"
#machine LfoPanel := "LfoPanel"
#machine FilterPanel := "FilterPanel"
#machine DelayPanel := "DelayPanel"
#machine ChorusPanel := "ChorusPanel"
#machine StereoChorusPanel := "StereoChorusPanel"
#machine FreeverbPanel := "FreeverbPanel"

#machine MoveModules := "MoveModules"
#machine PropagateSignals := "PropagateSignals"
#machine MoveCore := "MoveCore"

#machine LoopWave := "LoopWave"

#machine unicar := "unicar"

[[BuildParameterBlockPanel *panel *from *to *instrument : *path]
	[*instrument *parameters : *] [*parameters *parameter : *path]
	[ParameterBlockPanel *panel *parameter *from *to]
]

[[BuildAdsrPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [attack] *attack_path] [*parameters *attack : *attack_path]
	[APPEND *path [decay] *decay_path] [*parameters *decay : *decay_path]
	[APPEND *path [sustain] *sustain_path] [*parameters *sustain : *sustain_path]
	[APPEND *path [release] *release_path] [*parameters *release : *release_path]
	[AdsrPanel *panel *attack *decay *sustain *release]
]

[[BuildEGPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [time1] *time1_path] [*parameters *time1 : *time1_path]
	[APPEND *path [time2] *time2_path] [*parameters *time2 : *time2_path]
	[APPEND *path [time3] *time3_path] [*parameters *time3 : *time3_path]
	[APPEND *path [time4] *time4_path] [*parameters *time4 : *time4_path]
	[APPEND *path [level1] *level1_path] [*parameters *level1 : *level1_path]
	[APPEND *path [level2] *level2_path] [*parameters *level2 : *level2_path]
	[APPEND *path [level3] *level3_path] [*parameters *level3 : *level3_path]
	[APPEND *path [level4] *level4_path] [*parameters *level4 : *level4_path]
	[EGPanel *panel *time1 *time2 *time3 *time4 *level1 *level2 *level3 *level4]
]

[[BuildFEGPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [time1] *time1_path] [*parameters *time1 : *time1_path]
	[APPEND *path [time2] *time2_path] [*parameters *time2 : *time2_path]
	[APPEND *path [time3] *time3_path] [*parameters *time3 : *time3_path]
	[APPEND *path [time4] *time4_path] [*parameters *time4 : *time4_path]
	[APPEND *path [level1] *level1_path] [*parameters *level1 : *level1_path]
	[APPEND *path [level2] *level2_path] [*parameters *level2 : *level2_path]
	[APPEND *path [level3] *level3_path] [*parameters *level3 : *level3_path]
	[APPEND *path [level4] *level4_path] [*parameters *level4 : *level4_path]
	[FEGPanel *panel *time1 *time2 *time3 *time4 *level1 *level2 *level3 *level4]
]

[[BuildFM4Panel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [algo] *algo_path] [*parameters *algo : *algo_path]
	[APPEND *path [1 freq] *freq1_path] [*parameters *freq1 : *freq1_path]
	[APPEND *path [1 amp] *amp1_path] [*parameters *amp1 : *amp1_path]
	[APPEND *path [1 ratio] *ratio1_path] [*parameters *ratio1 : *ratio1_path]
	[APPEND *path [1 feedback] *feedback1_path] [*parameters *feedback1 : *feedback1_path]
	[APPEND *path [2 freq] *freq2_path] [*parameters *freq2 : *freq2_path]
	[APPEND *path [2 amp] *amp2_path] [*parameters *amp2 : *amp2_path]
	[APPEND *path [2 ratio] *ratio2_path] [*parameters *ratio2 : *ratio2_path]
	[APPEND *path [2 feedback] *feedback2_path] [*parameters *feedback2 : *feedback2_path]
	[APPEND *path [3 freq] *freq3_path] [*parameters *freq3 : *freq3_path]
	[APPEND *path [3 amp] *amp3_path] [*parameters *amp3 : *amp3_path]
	[APPEND *path [3 ratio] *ratio3_path] [*parameters *ratio3 : *ratio3_path]
	[APPEND *path [3 feedback] *feedback3_path] [*parameters *feedback3 : *feedback3_path]
	[APPEND *path [4 freq] *freq4_path] [*parameters *freq4 : *freq4_path]
	[APPEND *path [4 amp] *amp4_path] [*parameters *amp4 : *amp4_path]
	[APPEND *path [4 ratio] *ratio4_path] [*parameters *ratio4 : *ratio4_path]
	[APPEND *path [4 feedback] *feedback4_path] [*parameters *feedback4 : *feedback4_path]
	[FM4Panel *panel *algo
		*freq1 *amp1 *ratio1 *feedback1
		*freq2 *amp2 *ratio2 *feedback2
		*freq3 *amp3 *ratio3 *feedback3
		*freq4 *amp4 *ratio4 *feedback4
	]
]

[[BuildLfoPanel *panel *instrument *lfo *vibrato *tremolo *wah_wah *pan]
	[*instrument *parameters : *]
	[APPEND *lfo [speed] *speed_path] [*parameters *speed : *speed_path]
	[APPEND *lfo [wave] *wave_path] [*parameters *wave : *wave_path]
	[APPEND *lfo [pulse] *pulse_path] [*parameters *pulse : *pulse_path]
	[APPEND *lfo [phase] *phase_path] [*parameters *phase : *phase_path]
	[APPEND *lfo [sync] *sync_path] [*parameters *sync : *sync_path]
	[FindLfoKnob *vibrato_knob *parameters : *vibrato]
	[FindLfoKnob *tremolo_knob *parameters : *tremolo]
	[FindLfoKnob *wah_wah_knob *parameters : *wah_wah]
	[FindLfoKnob *pan_knob *parameters : *pan]
	[LfoPanel *panel *speed *wave *pulse *phase *sync *vibrato_knob *tremolo_knob *wah_wah_knob *pan_knob]
]

[[BuildFilterPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [freq] *freq_path] [*parameters *freq : *freq_path]
	[APPEND *path [resonance] *resonance_path] [*parameters *resonance : *resonance_path]
	[APPEND *path [amp] *amp_path] [*parameters *amp : *amp_path]
	[FilterPanel *panel *freq *resonance *amp]
]

[[BuildDelayPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [feedback] *feedback_path] [*parameters *feedback : *feedback_path]
	[APPEND *path [time] *time_path] [*parameters *time : *time_path]
	[APPEND *path [highdamp] *highdamp_path] [*parameters *highdamp : *highdamp_path]
	[DelayPanel *panel *feedback *time *highdamp]
]

[[BuildChorusPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [level] *level_path] [*parameters *level : *level_path]
	[APPEND *path [time] *time_path] [*parameters *time : *time_path]
	[APPEND *path [speed] *speed_path] [*parameters *speed : *speed_path]
	[APPEND *path [amp] *amp_path] [*parameters *amp : *amp_path]
	[ChorusPanel *panel *level *time *speed *amp]
]

[[BuildStereoChorusPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [level] *level_path] [*parameters *level : *level_path]
	[APPEND *path [time] *time_path] [*parameters *time : *time_path]
	[APPEND *path [bias] *bias_path] [*parameters *bias : *bias_path]
	[APPEND *path [speed] *speed_path] [*parameters *speed : *speed_path]
	[APPEND *path [phase] *phase_path] [*parameters *phase : *phase_path]
	[APPEND *path [amp] *amp_path] [*parameters *amp : *amp_path]
	[StereoChorusPanel *panel *level *time *bias *speed *phase *amp]
]

[[BuildFreeverbPanel *panel *instrument : *path]
	[*instrument *parameters : *]
	[APPEND *path [feedback] *feedback_path] [*parameters *feedback : *feedback_path]
	[APPEND *path [diffusion] *diffusion_path] [*parameters *diffusion : *diffusion_path]
	[APPEND *path [highdamp] *highdamp_path] [*parameters *highdamp : *highdamp_path]
	[APPEND *path [dry] *dry_path] [*parameters *dry : *dry_path]
	[APPEND *path [wet] *wet_path] [*parameters *wet : *wet_path]
	[FreeverbPanel *panel *feedback *diffusion *highdamp *dry *wet]
]

[[FindLfoKnob [] *parameters]]
[[FindLfoKnob *knob *parameters : *path] [*parameters *knob : *path]]

[[CreateDistributor *distributor]
	[create_atom *distributor]
	[mutex *mx]
	[addcl [[*distributor *mx]]]
]

[[CloseDistributor *distributor]
	[*distributor *mx]
	[*mx]
	[delallcl *distributor]
]

[[Distribute *distributor *key *keyon_actions *keyoff_actions]
	[*distributor *mx]
	[*mx wait]
	[TRY
		[res : *keyon_actions]
		[addcl [[*distributor *key *keyoff_actions]]]
	]
	[*mx signal]
]

[[Redistribute *distributor *key]
	[*distributor *mx]
	[*mx wait]
	[TRY
		[cl *index [[*distributor *key *action]]]
		[res : *action]
		[DELCL *index *distributor]
	]
	[*mx signal]
]

[[Connect : *command] *command]
[[ConnectStereo *to *from]
	[*to "LEFT" *from "LEFT"]
	[*to "RIGHT" *from "RIGHT"]
]
[[ConnectDryWet *to *dry *wet]
	[*to "DRYLEFT" *dry "LEFT"]
	[*to "DRYRIGHT" *dry "RIGHT"]
	[*to "WETLEFT" *wet "LEFT"]
	[*to "WETRIGHT" *wet "RIGHT"]
]
[[Disconnect *to : *command] [*to [] : *command]]
[[DisconnectStereo *to *from]
	[*to "LEFT" *from "LEFT" []]
	[*to "RIGHT" *from "RIGHT" []]
]
[[DisconnectDryWet *to *dry *wet]
	[*to "DRYLEFT" *dry "LEFT" []]
	[*to "DRYRIGHT" *dry "RIGHT" []]
	[*to "WETLEFT" *wet "LEFT" []]
	[*to "WETRIGHT" *wet "RIGHT" []]
]

[[Moons]]

[[AllocateChannel *index *index]
	[MIDI_CHANNELS *index : *original]
	[eq *original MIDI_BACK] /
]
[[AllocateChannel *index *location] [++ *index *next] [MIDI_CHANNELS *next : *] / [AllocateChannel *next *location]]

[[Moonbase *base *distributor *type *line]
	[AllocateChannel 0 *index]
	[GenerateInstrumentName *type *base]
	[create_atoms *distributor *line *modules *parameters *blocks]
	[addcl [[*base *parameters *modules *distributor *type *blocks]]]
	[addcl [[Moons *base *index *distributor *line]]]
	[MIDI_CHANNELS *index *distributor]
]

[[Moonbase *moon]
	[SELECT [[eq *moon *base]] [[eq *moon *id]]]
	[delcl [[Moons *base *id : *x]]]
	[MIDI_CHANNELS *id MIDI_BACK]
	[*base *parameters *modules *callback : *]
	[delallcl *base]
	[TRY [*callback]]
	[TRY [*parameters *parameter : *selector] [*parameter] fail]
	[TRY [*modules *module : *selector] [*module []] [*module] fail]
	[delallcl *parameters]
	[delallcl *modules]
]

[[CloseAllMoons] [Moonbase *] / [CloseAllMoons]]
[[CloseAllMoons]]

[[ConnectAllMoons *reactor]
	[Moons *id *moon *cb *line]
	[SELECT
		[[ConnectStereo *reactor *line]/]
		[[*reactor *line]/]
	]
	fail
]
[[ConnectAllMoons *]]

[[AddNamedParameterBlock *parameters *module *name *selector *initial *style]
	[*parameters *pb : *selector] /
	[*module *name *pb]
]
[[AddNamedParameterBlock *parameters *module *name *selector *initial *style]
	[parameter_block *pb *style]
	[*pb *initial]
	[addcl [[*parameters *pb : *selector]]]
	[*module *name *pb]
]

[[AddParameterBlock *parameters *parameter *module *selector *initial *style]
	[APPEND *selector [*parameter] *selectors]
	[*parameters *pb : *selectors] /
	[text_term *name *parameter]
	[*module *name *pb]
]
[[AddParameterBlock *parameters *parameter *module *selector *initial *style]
	[parameter_block *pb *style]
	[*pb *initial]
	[APPEND *selector [*parameter] *selectors]
	[addcl [[*parameters *pb : *selectors]]]
	[text_term *name *parameter]
	[*module *name *pb]
]

[[AddParameterBlock *parameters *parameter *name *module *selector *initial *style]
	[APPEND *selector [*parameter] *selectors]
	[*parameters *pb : *selectors] /
	[*module *name *pb]
]
[[AddParameterBlock *parameters *parameter *name *module *selector *initial *style]
	[parameter_block *pb *style]
	[*pb *initial]
	[APPEND *selector [*parameter] *selectors]
	[addcl [[*parameters *pb : *selectors]]]
	[*module *name *pb]
]

[[AddModule *operator *base : *selector]
	[*base *parameters *modules : *]
	[addcl [[*modules *operator : *selector]]]
]

[[InsertPB *pb *base : *selector]
	[*base *parameters *modules : *]
	[addcl [[*parameters *pb : *selector]]]
]

[[InsertBlock *block *base : *selector]
	[*base *parameters *modules *callback *type *blocks : *]
	[addcl [[*blocks *block : *selector]]]
]

[[Insert *operator *base : *selector]
	[*base *parameters *modules : *]
	[addcl [[*modules *operator : *selector]]]
	[*operator : *io]
	[InsertIO *parameters *operator *selector *io]
]

[[InsertIO *parameters *operator *selector [["FREQ" "AMP" "RATIO" "INDEX" : *] *o]]
	[AddParameterBlock *parameters freq *operator *selector 0 "freq"]
	[AddParameterBlock *parameters amp *operator *selector 0 "amp"]
	[AddParameterBlock *parameters ratio *operator *selector 1 "ratio"]
	[AddParameterBlock *parameters index *operator *selector 0 "index"]
]

[[InsertIO *parameters *operator *selector [["FREQ" "AMP" "RATIO" "TRIGGER" "WAVE" : *] *o]]
	[AddParameterBlock *parameters freq *operator *selector 0 "freq"]
	[AddParameterBlock *parameters amp *operator *selector 0 "amp"]
	[AddParameterBlock *parameters ratio *operator *selector 1 "ratio"]
	[AddParameterBlock *parameters wave *operator *selector 0 "index"]
]

[[InsertIO *parameters *operator *selector [["FREQ" "AMP" "RATIO" : *] *o]]
	[AddParameterBlock *parameters freq *operator *selector 0 "freq"]
	[AddParameterBlock *parameters amp *operator *selector 0 "amp"]
	[AddParameterBlock *parameters ratio *operator *selector 1 "ratio"]
]

[[InsertIO *parameters *noise *selector [["AMP"] ["SIGNAL"]]]
	[AddParameterBlock *parameters amp *noise *selector 0 "amp"]
]

[[InsertIO *parameters *arpeggiator *selector [["SPEED" "DIVISION" "ACTIVE" "ALGO" "HOLD" : *] *]]
	[AddParameterBlock *parameters speed *arpeggiator *selector 140 "index"]
	[AddParameterBlock *parameters subdivision "division" *arpeggiator *selector 24 "index"]
	[AddParameterBlock *parameters active *arpeggiator *selector 0 "onoff"]
	[AddParameterBlock *parameters algo *arpeggiator *selector 0 "index"]
	[AddParameterBlock *parameters hold *arpeggiator *selector 0 "onoff"]
]

[[InsertIO *parameters *adsr *selector [["TRIGGER" "ATTACK" "DECAY" "SUSTAIN" "RELEASE" : *] *]]
	[AddParameterBlock *parameters attack *adsr *selector 0 "time"]
	[AddParameterBlock *parameters decay *adsr *selector 0 "time"]
	[AddParameterBlock *parameters sustain *adsr *selector 0 "amp"]
	[AddParameterBlock *parameters release *adsr *selector 0 "time"]
]

[[InsertIO *parameters *adsr *selector [["TRIGGER" "TIME1" "TIME2" "TIME3" "TIME4" "LEVEL1" "LEVEL2" "LEVEL3" "LEVEL4" : *] *]]
	[AddParameterBlock *parameters time1 *adsr *selector 0 "time"]
	[AddParameterBlock *parameters time2 *adsr *selector 0 "time"]
	[AddParameterBlock *parameters time3 *adsr *selector 0 "time"]
	[AddParameterBlock *parameters time4 *adsr *selector 0 "time"]
	[AddParameterBlock *parameters level1 *adsr *selector 0 "index"]
	[AddParameterBlock *parameters level2 *adsr *selector 0 "index"]
	[AddParameterBlock *parameters level3 *adsr *selector 0 "index"]
	[AddParameterBlock *parameters level4 *adsr *selector 0 "index"]
]

[[InsertIO *parameters *trigger *selector [["BUSY" "HOLD" "PORTA" "TIME" "LEGATO" : *] *]]
	[AddParameterBlock *parameters porta *trigger *selector 0 "onoff"]
	[AddParameterBlock *parameters time *trigger *selector 0 "time"]
	[AddParameterBlock *parameters legato *trigger *selector 0 "onoff"]
	[AddParameterBlock *parameters hold *trigger *selector 0 "onoff"]
]

[[InsertIO *parameters *lfo *selector [["TRIGGER" "SPEED" "WAVE" "PULSE" "PHASE" "SYNC" : *] *]]
	[AddParameterBlock *parameters speed *lfo *selector 0 "time"]
	[AddParameterBlock *parameters wave *lfo *selector 0 "wave"]
	[AddParameterBlock *parameters pulse *lfo *selector 0 "index"]
	[AddParameterBlock *parameters phase *lfo *selector 0 "index"]
	[AddParameterBlock *parameters sync *lfo *selector 0 "onoff"]
]

[[InsertIO *parameters *filter *selector [["ENTER" "FREQ" "RESONANCE" "AMP" : *] *]]
	[AddParameterBlock *parameters freq *filter *selector 5120 "freq"]
	[AddParameterBlock *parameters resonance *filter *selector 0 "index"]
	[AddParameterBlock *parameters amp *filter *selector 0 "amp"]
]

[[InsertIO *parameters *pan *selector [["SIGNAL" "PAN" : *] *]]
	[AddParameterBlock *parameters pan *pan *selector 0 "index"]
]

[[InsertIO *parameters *pan *selector [["LEFT" "RIGHT" "PAN" : *] *]]
	[AddParameterBlock *parameters pan *pan *selector 0 "index"]
]

[[InsertIO *parameters *delay *selector [["LEFT" "RIGHT" "FEEDBACK" "TIME" "HIGHDAMP"] ["LEFT" "RIGHT"]]]
	[AddParameterBlock *parameters feedback *delay *selector 8192 "index"]
	[AddParameterBlock *parameters time *delay *selector 8192 "time"]
	[AddParameterBlock *parameters highdamp *delay *selector 0 "index"]
]

[[InsertIO *parameters *chorus *selector [["SIGNAL" "LEVEL" "TIME" "SPEED" "AMP"] ["SIGNAL"]]]
	[AddParameterBlock *parameters level *chorus *selector 0 "index"]
	[AddParameterBlock *parameters time *chorus *selector 1024 "index"]
	[AddParameterBlock *parameters speed *chorus *selector 0 "index"]
	[AddParameterBlock *parameters amp *chorus *selector 8192 "index"]
]

[[InsertIO *parameters *chorus *selector [["MONO" "LEFT" "RIGHT" "LEVEL" "TIME" "BIAS" "SPEED" "PHASE" "AMP"] ["LEFT" "RIGHT"]]]
	[AddParameterBlock *parameters level *chorus *selector 0 "index"]
	[AddParameterBlock *parameters time *chorus *selector 1024 "index"]
	[AddParameterBlock *parameters bias *chorus *selector 0 "index"]
	[AddParameterBlock *parameters speed *chorus *selector 0 "index"]
	[AddParameterBlock *parameters phase *chorus *selector 0 "index"]
	[AddParameterBlock *parameters amp *chorus *selector 8192 "index"]
]

[[InsertIO *parameters *freeverb *selector [["MONO" "LEFT" "RIGHT" "FEEDBACK" "DIFFUSION" "HIGHDAMP" "DRY" "WET"] ["LEFT" "RIGHT"]]]
	[AddParameterBlock *parameters feedback *freeverb *selector 0 "index"]
	[AddParameterBlock *parameters diffusion *freeverb *selector  0 "index"]
	[AddParameterBlock *parameters highdamp *freeverb *selector 0 "index"]
	[AddParameterBlock *parameters dry *freeverb *selector 16384 "index"]
	[AddParameterBlock *parameters wet *freeverb *selector 0 "index"]
]

[[InsertIO *parameters *volume *selector [["LEFT" "RIGHT" "VOLUME"] ["LEFT" "RIGHT"]]]
	[AddParameterBlock *parameters volume *volume *selector 12800 "index"]
]

[[InsertIO *parameters *drywet *selector [["DRYLEFT" "DRYRIGHT" "WETLEFT" "WETRIGHT" "BALANCE" : *] *]]
	[AddParameterBlock *parameters balance *drywet *selector 0 "index"]
]

[[InsertIO *parameters *sensitivity *selector [["SIGNAL" "BREAKPOINT" "LEFT" "RIGHT"] ["SIGNAL"]]]
	[AddParameterBlock *parameters BP "breakpoint" *sensitivity *selector 0 "index"]
	[AddParameterBlock *parameters left *sensitivity *selector 0 "index"]
	[AddParameterBlock *parameters right *sensitivity *selector 0 "index"]
]

[[InsertIO *parameters *gate *selector [["ENTER" "GATEWAY"] ["SIGNAL"]]]
	[AddNamedParameterBlock *parameters *gate "GATEWAY" *selector 0 "index"]
]

[[InsertIO *parameters *pb *selector [["SIGNAL"] ["SIGNAL"]]]
	[addcl [[*parameters *pb : *selector]]]
]

[[InsertIO *parameters *op *selector [["ALGO" "TRIGGER" "FREQ1" "AMP1" "RATIO1" "FEEDBACK1" "FREQ2" "AMP2" "RATIO2" "FEEDBACK2" "FREQ3" "AMP3" "RATIO3" "FEEDBACK3" "FREQ4" "AMP4" "RATIO4" "FEEDBACK4"] ["SIGNAL"]]]
	[AddParameterBlock *parameters algo *op *selector 0 "fm4algo"]
	[APPEND *selector [1] *selector1]
	[AddParameterBlock *parameters freq "freq1" *op *selector1 0 "freq"]
	[AddParameterBlock *parameters amp "amp1" *op *selector1 0 "amp"]
	[AddParameterBlock *parameters ratio "ratio1" *op *selector1 1 "ratio"]
	[AddParameterBlock *parameters feedback "feedback1" *op *selector1 0 "index"]
	[APPEND *selector [2] *selector2]
	[AddParameterBlock *parameters freq "freq2" *op *selector2 0 "freq"]
	[AddParameterBlock *parameters amp "amp2" *op *selector2 -16384.0 "amp"]
	[AddParameterBlock *parameters ratio "ratio2" *op *selector2 1 "ratio"]
	[AddParameterBlock *parameters feedback "feedback2" *op *selector2 0 "index"]
	[APPEND *selector [3] *selector3]
	[AddParameterBlock *parameters freq "freq3" *op *selector3 0 "freq"]
	[AddParameterBlock *parameters amp "amp3" *op *selector3 -16384.0 "amp"]
	[AddParameterBlock *parameters ratio "ratio3" *op *selector3 1 "ratio"]
	[AddParameterBlock *parameters feedback "feedback3" *op *selector3 0 "index"]
	[APPEND *selector [4] *selector4]
	[AddParameterBlock *parameters freq "freq4" *op *selector4 0 "freq"]
	[AddParameterBlock *parameters amp "amp4" *op *selector4 -16384.0 "amp"]
	[AddParameterBlock *parameters ratio "ratio4" *op *selector4 1 "ratio"]
	[AddParameterBlock *parameters feedback "feedback4" *op *selector4 0 "index"]
]

[[InsertIO *parameters *mixer *selector [["ENTER" : *] *]]]

[[InsertIO *parameters *moonbase *selector [[] []]]]

[[InsertIO *parameters *operator *selector [*i *o]] [show "Failed inserting IO at " [*parameters *operator *i *o]]]

[[InsertController *ctrl *shift *moonbase : *selector]
	[is_integer *shift]
	[*moonbase *parameters *modules *cb : *]
	[*parameters *base : *selector]
	[*cb *base *ctrl *shift]
]

[[InsertController *ctrl *moonbase : *selector]
	[is_atom *moonbase]
	[*moonbase *parameters *modules *cb : *]
	[*parameters *base : *selector]
	[*cb *base *ctrl]
]

[[Lander *base *moonbase : *selector] [*moonbase * *modules : *] [*modules *base : *selector]]

[[Lunar *v *moonbase : *selector] [*moonbase *parameters : *] [*parameters *base : *selector] [*base 0 *v]]

[[Store *moonbase *program]
	[is_integer *program] /
	[*moonbase * * * *type : *]
	[add *file_name *type "/" *program ".txt"]
	[Store *moonbase *file_name]
]

[[Store *moonbase *file_name]
	[*moonbase *parameters *modules *cb *type *blocks : *]
	[file_writer *tc *file_name]
	[*tc [*type] "\n"]
	[*cb control : *mono] [*tc [*mono] "\n"] [show *mono]
	[TRY
		[*parameters *orbiter : *selector]
		[*orbiter *x]
		[show *selector " => " *x]
		[*tc [*selector] [*x] "\n"]
		fail
	]
	[list *blocks]
	[TRY
		[*blocks *block : *selector]
		[*block *x]
		[*tc [*selector] [*x] "\n"]
		fail
	]
	[*tc "\n"]
	[*tc]
]

[[Restore *moonbase *program]
	[is_integer *program] /
	[*moonbase * * * *type : *]
	[add *file_name *type "/" *program ".txt"]
	[Restore *moonbase *file_name]
]

[[Restore *moonbase *file_name]
	[file_reader *fr *file_name]
	[*moonbase * * *cb *type : *]
	[*fr *type]
	[*fr *mono] [*cb *mono] [show [*cb *mono]]
	[SubRestore *moonbase *fr]
	[*fr]
]

[[SubRestore *moonbase *fr]
	[*fr *one] [*fr *two]
	[show *one " => " *two] /
	[SELECT
		[[is_number *two] [Lunar *two *moonbase : *one]]
		[[*moonbase *parameters *modules *cb *type *blocks] [*blocks *block : *one] [*block *two]]
	]
	/ [SubRestore *moonbase *fr]
]
[[SubRestore : *]]

[[Activate *core : *moonbase]
	[Lander *base : *moonbase]
	[*base : *io]
	[SELECT
		[[eq *io [* ["LEFT" "RIGHT"]]] [*core "LEFT" *base "LEFT"] [*core "RIGHT" *base "RIGHT"]]
		[[eq *io [* ["SIGNAL"]]] [*core *base]]
	]
]
[[Deactivate *core : *moonbase]
	[Lander *base : *moonbase]
	[*base : *io]
	[SELECT
		[[eq *io [* ["LEFT" "RIGHT"]]] [*core "LEFT" *base "LEFT" []] [*core "RIGHT" *base "RIGHT" []]]
		[[eq *io [* ["SIGNAL"]]] [*core *base []]]
	]
]

[[CCCB *key *velocity] [cb_callback : *callback] [*callback keyon *key *velocity]]
[[CCCB *ret 73 *v] [cb_callback : *callback] [*callback control 73 *v] [add *ret "Attack = " *v]]
[[CCCB *ret 93 *v] [cb_callback : *callback] [*callback control 93 *v] [add *ret "Decay = " *v]]
[[CCCB *ret 94 *v] [cb_callback : *callback] [*callback control 94 *v] [add *ret "Sustain = " *v]]
[[CCCB *ret 72 *v] [cb_callback : *callback] [*callback control 72 *v] [add *ret "Release = " *v]]
[[CCCB *ret 74 *v] [cb_callback : *callback] [*callback control 74 *v] [- *v 64 *vv] [add *ret "Freq = " *vv]]
[[CCCB *ret 91 *v] [cb_callback : *callback] [*callback control 91 *v] [- *v 64 *vv] [add *ret "Dry/Wet = " *vv]]
[[CCCB *ret 10 *v] [cb_callback : *callback] [*callback control 10 *v] [- *v 64 *vv] [add *ret "Pan = " *vv]]
[[CCCB *ret 7 *v] [cb_callback : *callback] [*callback control 7 *v] [add *ret "Volume = " *v]]
[[CCCB *ret 11 *v] [cb_callback : *callback] [*callback control 11 *v] [add *ret "Porta = " *v]]
[[CCCB *ret 65 *v] [cb_callback : *callback] [*callback control 65 *v] [add *ret "Porta Switch = " *v]]
[[CCCB *ret 95 *v] [cb_callback : *callback] [*callback control 95 *v] [- *v 64 *vv] [add *ret "Speed = " *vv]]
[[CCCB *ret 71 *v] [cb_callback : *callback] [*callback control 71 *v] [add *ret "Vibrato = " *v]]
[[CCCB *ret 128 *v] [cb_callback : *callback] [*callback control 128 *v] [- *v 64 *vv] [add *ret "Pitch = " *vv]]
[[CCCB *ret 1 *v] [cb_callback : *callback] [*callback control 1 *v] [add *ret "Modulation = " *v]]
[[CCCB "Mode = Mono" 126 *v] [cb_callback : *callback] [*callback control 126 0]]
[[CCCB "Mode = Poly" 127 *v] [cb_callback : *callback] [*callback control 127 0]]
[[CCCB *ret 12 *x *y]
	[cb_callback : *callback] [*callback control 12 *x] [*callback control 13 *y]
	[- *x 64 *xx] [- *y 64 *yy] [add *ret "Vector = [" *xx " / " *yy "]"]
]

[[CCCB CCCB *poly *porta *pitch *modulation *x *y *volume *attack *decay *sustain *release
			*freq *drywet *pan *porta_time *speed *vibrato : *]
	[is_var *poly]
	[cb_callback : *cb] [is_atom *cb]
	[*cb control 127 : *poly]
	[*cb control 65 : *porta]
	[*cb control 128 : *pitch]
	[*cb control 1 : *modulation]
	[*cb control 12 : *x]
	[*cb control 13 : *y]
	[*cb control 7 : *volume]
	[*cb control 73 : *attack]
	[*cb control 93 : *decay]
	[*cb control 94 : *sustain]
	[*cb control 72 : *release]
	[*cb control 74 : *freq]
	[*cb control 91 : *drywet]
	[*cb control 10 : *pan]
	[*cb control 11 : *porta_time]
	[*cb control 95 : *speed]
	[*cb control 71 : *vibrato]
]

[[CCCB *ret Store *file_name]
	[cb_path : *path]
	[eq *path [*moon : *]]
	[Store *moon *file_name]
	[relativise_path *file_name *file]
	[add *ret *file " Stored"]
]
[[CCCB *ret Store *file_name] [add *ret "Failed to store " *file_name]]

[[CCCB *ret Restore *file_name]
	[cb_path : *path]
	[eq *path [*moon : *]]
	[Restore *moon *file_name]
	[relativise_path *file_name *file]
	[add *ret *file " Restored"]
]
[[CCCB *ret Restore *file_name] [add *ret "Failed to restore " *file_name]]

[[CCCB *ret [] *delta]
	[cb_path : *path]
	[eq *path [*m : *p]]
	[*m *parameters : *]
	[*parameters *pb : *p]
	[*pb *v1]
	[add *v1 *delta *v2]
	[*pb *v2]
	[*pb : *v3]
	[cb_edit_path : *edit_path]
	[text_term *pather *edit_path]
	[add *ret *pather " = " *v3]
]

[[CCCB *program 0 *i1 : *]
	[isallr *y *x [Moons *x : *]]
	[AT *i1 *moon *y]
	[cb_path [*moon]]
	[cb_edit_path [*moon]]
	[*moon * * *moon_callback *type : *]
	[cb_callback *moon_callback]
	[add *program *type]
]

[[CCCB *program *mode *i1 : *is]
	[isallr *y *x [Moons *x : *]]
	[AT *i1 *moon *y]
	[*moon *parameters : *]
	[CBsub *parameters *is *path *path]
	[process_mode [*moon : *path] *mode *processed]
	[text_term *pather *processed]
	[*parameters *pb : *path] [*pb : *v]
	[add *program *pather " = " *v]
	[cb_path [*moon : *path]]
	[cb_edit_path *processed]
	[*moon * * *moon_callback : *]
	[cb_callback *moon_callback]
]

[[CCCB *ret START *file_name] [core *file_name] [add *ret "Recording started " *file_name]]
[[CCCB STOP] [core] [show "STOP RECORDING"]]

[[process_mode [*h : *t] 0 [[*h] : *t]]]
[[process_mode [*h : *t] *i [*h : *t2]] [less 0 *i] / [-- *i *next] [process_mode *t *next *t2]]

[[CBsub *moon [*i : *is] *path [*pp : *tail]]
	[unicar *unicar] [TRY [*moon * : *path] [*unicar *pp] fail] [*unicar : *pn] [*unicar]
	;[isall *pr *pp [*moon * : *path]] [NODUP *pr *pnr] [REVERSE *pnr *pn]
	[AT *i *pp *pn]
	/ [CBsub *moon *is *path *tail]
]
[[CBsub * * * []]]

[[MIDI_BACK activesensing]]
[[MIDI_BACK : *command] [show *command]]
[[income_midi *command *ch : *t]
	[MIDI_CHANNELS *ch : *cb]
	[*cb *command : *t]
]

[[GenerateInstrumentName * *name] [is_atom *name] /]
[[GenerateInstrumentName *prefix *name]
	[InstrumentIndex : *i]
	[add *prefix "-" *i *atom_name]
	[create_atom *atom_name *name]
	[inc InstrumentIndex]
]

[[midi_monitor activesensing]]
[[midi_monitor : *command] [show *command]]

auto := [
			[var cb_callback cb_path cb_edit_path [InstrumentIndex 0]]
			[ARRAY MIDI_CHANNELS 128]
			[FOR *i 0 127 1 [MIDI_CHANNELS *i MIDI_BACK]]
		]

private [AddParameterBlock SubRestore cb_callback cb_path cb_edit_path CBsub process_mode FindLfoKnob]

end := [[auto_atoms] [gtk_command]] .

