
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar
import control

program eclipse [
					commander cb_edit_path moon mooncb
					Phobos PhobosCB BuildPhobos BuildPhobosPart BuildPhobosBak BuildPhobosPartBak
					build_Abakos build_Abakos_part Abakos Abakoscb
					reactor left_radar right_radar radar
					kb kbcb
					mdi mdicb
					js jcb
					MM audio phobos_mixer abakos_mixer
				]


[[BuildPhobos *polyphony *Phobos *PhobosCB *volume *pan]
	[addcl [[Moons *Phobos]]]
	[Moonbase *Phobos *PhobosCB Phobos]

	[moonbase *moonbase] [key_map *mapper] [*moonbase *mapper]
	[arpeggiator *PhobosCB *moonbase]
	[auto_data *XData] [parameter_block *X "index"] [*XData *X]
	[auto_data *YData] [parameter_block *Y "index"] [*YData *Y]
	[parameter_block *AutoCtrl "index"] [*XData "control" *AutoCtrl] [*YData "control" *AutoCtrl]
	[parameter_block *pitch "index"] [parameter_block *freq "index"]
	[lfo *lfo1] [lfo *lfo2]

	[control *vibrato]
	[gateway *lfosens1] [gateway *lfosens2] [gateway *lfosens3] [gateway *lfosens4] [gateway *lfosensfilter]
	[*vibrato *lfo1] [*lfosens1 *vibrato] [*lfosens2 *vibrato] [*lfosens3 *vibrato] [*lfosens4 *vibrato] [*lfosensfilter *vibrato]

	[control *tremolo] [control *negative]
	[gateway *lfo2freq1] [gateway *lfo2freq2] [gateway *lfo2freq3] [gateway *lfo2freq4]
	[gateway *lfo2amp1] [gateway *lfo2amp2] [gateway *lfo2amp3] [gateway *lfo2amp4]
	[gateway *lfo2filter] [gateway *lfo2amp] [gateway *lfo2pan]
	[*tremolo *lfo2] [*negative "enter" *lfo2 "negative"]
	[*lfo2freq1 *tremolo] [*lfo2freq2 *tremolo] [*lfo2freq3 *tremolo] [*lfo2freq4 *tremolo]
	[*lfo2amp1 *negative] [*lfo2amp2 *negative] [*lfo2amp3 *negative] [*lfo2amp4 *negative]
	[*lfo2filter *tremolo] [*lfo2amp *negative] [*lfo2pan *tremolo]

	[gateway *pitch_ctrl1] [gateway *pitch_ctrl2] [gateway *pitch_ctrl3] [gateway *pitch_ctrl4] [gateway *pitch_ctrlfilter]
	[*pitch_ctrl1 *pitch] [*pitch_ctrl2 *pitch] [*pitch_ctrl3 *pitch] [*pitch_ctrl4 *pitch] [*pitch_ctrlfilter *pitch]

	[pan *pan] [delay *delay] [drywet *mixer] [volume *volume]
	[*pan "pan" *lfo2pan]
	[ConnectStereo *delay *pan]
	[ConnectDryWet *mixer *pan *delay]
	[ConnectStereo *volume *mixer]

	[Insert *volume *Phobos core]
	[Insert *pan *Phobos core]
	[Insert *mixer *Phobos core]
	[Insert *delay *Phobos core delay]
	[InsertPB *X *Phobos core X]
	[InsertPB *Y *Phobos core Y]
	[InsertPB *freq *Phobos core freq]
	[InsertPB *pitch *Phobos core pitch]
	[InsertPB *AutoCtrl *Phobos core auto]
	[Insert *PhobosCB *Phobos arpeggiator]
	[Insert *lfo1 *Phobos lfo 1]
	[Insert *vibrato *Phobos lfo 1 vibrato]
	[Insert *lfo2 *Phobos lfo 2]
	[Insert *tremolo *Phobos lfo 2 vibrato]
	[Insert *negative *Phobos lfo 2 vibrato]
	[Insert *lfo2amp *Phobos lfo 2 tremolo]
	[Insert *lfo2pan *Phobos lfo 2 pan]

	[show "STAGE 1 (common)"]

	[REPEAT *polyphony
		[BuildPhobosPart *Phobos *moonbase *pan *XData *YData *lfo1 *lfo2
			*lfosens1 *lfosens2 *lfosens3 *lfosens4 *lfosensfilter
			*lfo2freq1 *lfo2freq2 *lfo2freq3 *lfo2freq4
			*lfo2amp1 *lfo2amp2 *lfo2amp3 *lfo2amp4
			*lfo2filter *lfo2amp
			*pitch_ctrl1 *pitch_ctrl2 *pitch_ctrl3 *pitch_ctrl4
			*pitch_ctrlfilter
			]
	]

	[show "STAGE 2 (parts)"]

	[Insert *pitch_ctrl1 *Phobos operator 1 pitch]
	[Insert *pitch_ctrl2 *Phobos operator 2 pitch]
	[Insert *pitch_ctrl3 *Phobos operator 3 pitch]
	[Insert *pitch_ctrl4 *Phobos operator 4 pitch]
	[Insert *lfosens1 *Phobos operator 1 vibrato]
	[Insert *lfosens2 *Phobos operator 2 vibrato]
	[Insert *lfosens3 *Phobos operator 3 vibrato]
	[Insert *lfosens4 *Phobos operator 4 vibrato]
	[Insert *lfo2freq1 *Phobos operator 1 lfo vibrato]
	[Insert *lfo2freq2 *Phobos operator 2 lfo vibrato]
	[Insert *lfo2freq3 *Phobos operator 3 lfo vibrato]
	[Insert *lfo2freq4 *Phobos operator 4 lfo vibrato]
	[Insert *lfo2amp1 *Phobos operator 1 lfo tremolo]
	[Insert *lfo2amp2 *Phobos operator 2 lfo tremolo]
	[Insert *lfo2amp3 *Phobos operator 3 lfo tremolo]
	[Insert *lfo2amp4 *Phobos operator 4 lfo tremolo]

	[Insert *pitch_ctrlfilter *Phobos filter pitch]
	[Insert *lfosensfilter *Phobos filter lfo 1]
	[Insert *lfo2filter *Phobos filter lfo 2]

	[InsertBlock *XData *Phobos vector X]
	[InsertBlock *YData *Phobos vector Y]
	[InsertBlock *mapper *Phobos key_map]

	[InsertController 1 *Phobos lfo 1 vibrato]
	[InsertController 7 *Phobos core volume]
	[InsertController 10 -64 *Phobos core pan]
	[InsertController 11 -64 *Phobos portamento time]
	[InsertController 12 -64 *Phobos core X]
	[InsertController 13 -64 *Phobos core Y]
	[InsertController 65 *Phobos portamento porta]
	[InsertController 74 -64 *Phobos core freq]
	[InsertController 91 -64 *Phobos core balance]
	[InsertController 95 -64 *Phobos lfo 1 speed]
	[InsertController 73 *Phobos adsr 1 attack]
	[InsertController 93 *Phobos adsr 1 decay]
	[InsertController 94 -128 *Phobos adsr 1 sustain]
	[InsertController 71 *Phobos lfo 2 vibrato]
	[InsertController 72 *Phobos adsr 1 release]
	[InsertController 128 -64 *Phobos core pitch]

	[show "STAGE 3 (completed)"]

	[Lunar -128 *Phobos operator 1 key freq left]
	[Lunar 128 *Phobos operator 1 key freq right]
	[Lunar -128 *Phobos operator 2 key freq left]
	[Lunar 128 *Phobos operator 2 key freq right]
	[Lunar -128 *Phobos operator 3 key freq left]
	[Lunar 128 *Phobos operator 3 key freq right]
	[Lunar -128 *Phobos operator 4 key freq left]
	[Lunar 128 *Phobos operator 4 key freq right]
]


[[BuildPhobosPart *Phobos *PhobosCB *mixer *X *Y *lfo1 *lfo2
					*lfosens1 *lfosens2 *lfosens3 *lfosens4 *lfosensfilter
					*lfo2freq1 *lfo2freq2 *lfo2freq3 *lfo2freq4
					*lfo2amp1 *lfo2amp2 *lfo2amp3 *lfo2amp4
					*lfo2filter *lfo2amp
					*pitchsens1 *pitchsens2 *pitchsens3 *pitchsens4
					*pitchsensfilter
					]
	[trigger *trigger]
	[fm4 *op] [noise_operator *noise]
	[filter *filter]
	[adsr *adsr]
	[parameter_block *trigger_delay 0]
	[sensitivity *freq1] [sensitivity *freq2] [sensitivity *freq3] [sensitivity *freq4]
	[eg *ampeg1] [eg *ampeg2] [eg *ampeg3] [eg *ampeg4] [eg *noiseeg]
	[eg *freqeg] [gateway *freqeg1] [gateway *freqeg2] [gateway *freqeg3] [gateway *freqeg4] [gateway *filtereg]
	[sensitivity *velocity1] [sensitivity *velocity2] [sensitivity *velocity3] [sensitivity *velocity4]
	[sensitivity *key1] [sensitivity *key2] [sensitivity *key3] [sensitivity *key4]
	[sensitivity *egscal] [sensitivity *egscalfreq] [sensitivity *egscal1] [sensitivity *egscal2] [sensitivity *egscal3] [sensitivity *egscal4]
	[sensitivity *filter_key]

	[*PhobosCB *trigger]
	[*trigger_delay "signal" *trigger "trigger"]
	[*adsr "trigger" *trigger_delay]
	[*freqeg "trigger" *trigger "trigger"] [*noiseeg "trigger" *trigger_delay]
	[*ampeg1 "trigger" *trigger_delay] [*ampeg2 "trigger" *trigger_delay] [*ampeg3 "trigger" *trigger_delay] [*ampeg4 "trigger" *trigger_delay]
	[*trigger "busy" *adsr "busy"]
	[*lfo1 "trigger" *trigger "trigger"] [*lfo2 "trigger" *trigger "trigger"]

	[*noise "amp" *noiseeg]
	[*freq1 "signal" *trigger "key"] [*freq2 "signal" *trigger "key"] [*freq3 "signal" *trigger "key"] [*freq4 "signal" *trigger "key"]
	[*filter_key "signal" *trigger "key"]
	[*op "trigger" *trigger_delay]
	[*op "freq1" *freq1] [*op "freq2" *freq2] [*op "freq3" *freq3] [*op "freq4" *freq4]
	[*op "freq1" *lfosens1] [*op "freq2" *lfosens2] [*op "freq3" *lfosens3] [*op "freq4" *lfosens4]
	[*op "freq1" *pitchsens1] [*op "freq2" *pitchsens2] [*op "freq3" *pitchsens3] [*op "freq4" *pitchsens4]
	[*op "freq1" *lfo2freq1] [*op "freq2" *lfo2freq2] [*op "freq3" *lfo2freq3] [*op "freq4" *lfo2freq4]

	[auto *vectorX *X] [*vectorX "trigger" *trigger "trigger"] [*X "trigger" *trigger "trigger"]
	[auto *vectorY *Y] [*vectorY "trigger" *trigger "trigger"] [*Y "trigger" *trigger "trigger"]
	[sensitivity *Xsa1] [sensitivity *Xsa2] [sensitivity *Xsa3] [sensitivity *Xsa4]
	[sensitivity *Ysa1] [sensitivity *Ysa2] [sensitivity *Ysa3] [sensitivity *Ysa4]
	[*Xsa1 *vectorX] [*Xsa2 *vectorX] [*Xsa3 *vectorX] [*Xsa4 *vectorX]
	[*Ysa1 *vectorY] [*Ysa2 *vectorY] [*Ysa3 *vectorY] [*Ysa4 *vectorY]
	[*op "amp1" *Xsa1] [*op "amp2" *Xsa2] [*op "amp3" *Xsa3] [*op "amp4" *Xsa4]
	[*op "amp1" *Ysa1] [*op "amp2" *Ysa2] [*op "amp3" *Ysa3] [*op "amp4" *Ysa4]
	[*op "amp1" *lfo2amp1] [*op "amp2" *lfo2amp2] [*op "amp3" *lfo2amp3] [*op "amp4" *lfo2amp4]

	[*freqeg1 *freqeg] [*freqeg2 *freqeg] [*freqeg3 *freqeg] [*freqeg4 *freqeg] [*filtereg *freqeg]
	[*op "freq1" *freqeg1] [*op "freq2" *freqeg2] [*op "freq3" *freqeg3] [*op "freq4" *freqeg4]
	[*op "amp1" *ampeg1] [*op "amp2" *ampeg2] [*op "amp3" *ampeg3] [*op "amp4" *ampeg4]

	[*velocity1 "signal" *trigger "velocity"] [*velocity2 "signal" *trigger "velocity"]
	[*velocity3 "signal" *trigger "velocity"] [*velocity4 "signal" *trigger "velocity"]
	[*op "amp1" *velocity1] [*op "amp2" *velocity2] [*op "amp3" *velocity3] [*op "amp4" *velocity4]
	[*key1 "signal" *trigger "key"] [*key2 "signal" *trigger "key"] [*key3 "signal" *trigger "key"] [*key4 "signal" *trigger "key"]
	[*op "amp1" *key1] [*op "amp2" *key2] [*op "amp3" *key3] [*op "amp4" *key4]

	[*egscal "signal" *trigger "key"] [*egscalfreq "signal" *trigger "key"]
	[*egscal1 "signal" *trigger "key"] [*egscal2 "signal" *trigger "key"] [*egscal3 "signal" *trigger "key"] [*egscal4 "signal" *trigger "key"]
	[*adsr "attack" *egscal] [*adsr "decay" *egscal] [*adsr "release" *egscal]
	[*freqeg "time1" *egscalfreq] [*freqeg "time2" *egscalfreq] [*freqeg "time3" *egscalfreq] [*freqeg "time4" *egscalfreq]
	[*ampeg1 "time1" *egscal1] [*ampeg1 "time2" *egscal1] [*ampeg1 "time3" *egscal1] [*ampeg1 "time4" *egscal1]
	[*ampeg2 "time1" *egscal2] [*ampeg2 "time2" *egscal2] [*ampeg1 "time3" *egscal2] [*ampeg1 "time4" *egscal2]
	[*ampeg3 "time1" *egscal3] [*ampeg3 "time2" *egscal3] [*ampeg1 "time3" *egscal3] [*ampeg1 "time4" *egscal3]
	[*ampeg4 "time1" *egscal4] [*ampeg4 "time2" *egscal4] [*ampeg1 "time3" *egscal4] [*ampeg1 "time4" *egscal4]

	[*filter "freq" *lfosensfilter]
	[*filter "freq" *lfo2filter]
	[*filter "freq" *filtereg]
	[*filter "freq" *filter_key]
	[*filter "freq" *pitchsensfilter]
	[*filter "amp" *lfo2amp]

	[*filter *op] [*filter *noise]
	[*filter "amp" *adsr]
	[*mixer *filter]

	[Insert *op *Phobos operator]
	[Insert *freq1 *Phobos operator 1 key freq]
	[Insert *freq2 *Phobos operator 2 key freq]
	[Insert *freq3 *Phobos operator 3 key freq]
	[Insert *freq4 *Phobos operator 4 key freq]
	[Insert *key1 *Phobos operator 1 key amp]
	[Insert *key2 *Phobos operator 2 key amp]
	[Insert *key3 *Phobos operator 3 key amp]
	[Insert *key4 *Phobos operator 3 key amp]
	[Insert *egscal1 *Phobos operator 1 key egscal]
	[Insert *egscal2 *Phobos operator 2 key egscal]
	[Insert *egscal3 *Phobos operator 3 key egscal]
	[Insert *egscal4 *Phobos operator 4 key egscal]
	[Insert *Xsa1 *Phobos operator 1 X]
	[Insert *Xsa2 *Phobos operator 2 X]
	[Insert *Xsa3 *Phobos operator 3 X]
	[Insert *Xsa4 *Phobos operator 4 X]
	[Insert *Ysa1 *Phobos operator 1 Y]
	[Insert *Ysa2 *Phobos operator 2 Y]
	[Insert *Ysa3 *Phobos operator 3 Y]
	[Insert *Ysa4 *Phobos operator 4 Y]
	[Insert *velocity1 *Phobos operator 1 velocity]
	[Insert *velocity2 *Phobos operator 2 velocity]
	[Insert *velocity3 *Phobos operator 3 velocity]
	[Insert *velocity4 *Phobos operator 4 velocity]
	[Insert *noise *Phobos noise]
	[Insert *noiseeg *Phobos noise]
	[Insert *ampeg1 *Phobos operator 1 eg amp]
	[Insert *ampeg2 *Phobos operator 2 eg amp]
	[Insert *ampeg3 *Phobos operator 3 eg amp]
	[Insert *ampeg4 *Phobos operator 4 eg amp]
	[Insert *freqeg1 *Phobos operator 1 eg freq]
	[Insert *freqeg2 *Phobos operator 2 eg freq]
	[Insert *freqeg3 *Phobos operator 3 eg freq]
	[Insert *freqeg4 *Phobos operator 4 eg freq]
	[Insert *filter *Phobos filter]
	[Insert *filtereg *Phobos filter eg]
	[Insert *filter_key *Phobos filter key]
	[Insert *adsr *Phobos adsr 1]
	[Insert *egscal *Phobos adsr 1 egscal]
	[Insert *freqeg *Phobos adsr 2]
	[Insert *egscalfreq *Phobos adsr 2 egscal]
	[Insert *trigger *Phobos portamento]

	[AddModule *vectorX *Phobos]
	[AddModule *vectorY *Phobos]

	[show ".... PART"]
]

[[build_Abakos *Abakos *drywet]
	[moonbase *Abakoscb]
	[arpeggiator Abakoscb *Abakoscb]
	[Moonbase *Abakos Abakoscb Abakos]
	[pan *pan]
	[delay *delay]
	[drywet *drywet]
	[ConnectStereo *delay *pan]
	[*drywet "dryleft" *pan "left"]
	[*drywet "dryright" *pan "right"]
	[*drywet "wetleft" *delay "left"]
	[*drywet "wetright" *delay "right"]
	[Insert Abakoscb *Abakos arpeggiator]
	[Insert *pan *Abakos reactor]
	[Insert *drywet *Abakos reactor]
	[Insert *delay *Abakos reactor]
	[build_Abakos_part *Abakos *pan *Abakoscb]
	[build_Abakos_part *Abakos *pan *Abakoscb]
	[build_Abakos_part *Abakos *pan *Abakoscb]
	[build_Abakos_part *Abakos *pan *Abakoscb]
	[build_Abakos_part *Abakos *pan *Abakoscb]
	[build_Abakos_part *Abakos *pan *Abakoscb]
	[InsertController 16 *Abakos reactor pan]
	[InsertController 17 *Abakos reactor time]
	[InsertController 65 *Abakos portamento porta]
	[addcl [[Moons *Abakos]]]
]

[[build_Abakos_part *Abakos *mixer *cb]
	[operator *op1]
	[adsr *adsr1]
	[trigger *trigger] [*cb *trigger]
	[*trigger "busy" *adsr1 "busy"]
	[*op1 "freq" *trigger "key"]
	[*adsr1 "trigger" *trigger "trigger"]
	[*op1 "amp" *adsr1 "signal"]
	[*mixer *op1]
	[Insert *op1 *Abakos operator]
	[Insert *adsr1 *Abakos adsr]
	[Insert *trigger *Abakos portamento]

]


[[@ lunar . LunarDrop : *command] [show *command]]

[[mdicb *command * : *t]
	;[show [PhobosCB *command : *t]]
	[PhobosCB *command : *t]
]

[[jcb 0.0 *v]
	[times *v 64.0 *vv] [add *vv 64.0 *vvv] [trunc *vvv *vvvv]
	[PhobosCB control 12 *vvvv]
]

[[jcb 1.0 *v]
	[times *v -64.0 *vv] [add *vv 64.0 *vvv] [trunc *vvv *vvvv]
	[PhobosCB control 13 *vvvv]
]

[[audio] [core reactor 330 22050 2048 0 -1] [ConnectStereo reactor phobos_mixer] [ConnectStereo reactor abakos_mixer]]

[[MM *base]
	[*base *parameters *modules *callback : *]
	[delallcl *base]
	[TRY [*parameters *parameter : *selector] [*parameter] fail]
	[TRY [*modules *module : *selector] [show *selector *module] [*module []] [*module] fail]
	[delallcl *parameters]
	[delallcl *modules]
]

end := [
		[BuildPhobos 2 Phobos PhobosCB phobos_mixer *feed]
		[build_Abakos *Abakos abakos_mixer]
		[oscilloscope radar]
		[CommandCentre commander CCCB]
		;[commander 3000 1180] [radar 2000 1180]
		[TRY [midi mdi mdicb]]
		[TRY [joystick js jcb]]
		[radar *feed]
		[audio]
		/ [command] /
		[TRY [mdi]]
		[TRY [js]]
		[Moonbase Phobos]
		[Moonbase *Abakos]
		] .

