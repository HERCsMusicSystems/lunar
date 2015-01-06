
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar
import control

program eclipse [
					commander cb cb_path cb_callback moon mooncb process_mode
					Phobos PhobosCB BuildPhobos BuildPhobosPart BuildPhobosBak BuildPhobosPartBak
					paths modules adjacent next_path previous_path next_module previous_module
					build_Abakos build_Abakos_part Abakos Abakoscb
					reactor left_radar right_radar radar
					kb kbcb
					AT sub mdi mdicb
					js jcb
					MM audio phobos_mixer abakos_mixer
				]

[[AT * *x [*x]]/]
[[AT 0 *x [*x : *]]/]
[[AT *i *x [*a : *b]] [-- *i *i1] / [AT *i1 *x *b]]

[[paths *base]
	[*base *parameters : *]
	[isallr *list *el [*parameters *module : *el]]
	[show *list]
]

[[modules *base]
	[*base * *modules : *]
	[isallr *list *el [*modules *module : *el]]
	[show *list]
]

[[adjacent [*p1 *p2 : *] *p1 *p2] /]
[[adjacent [* : *p] *p1 *p2] / [adjacent *p *p1 *p2]]

[[next_path *base *p1 *p2]
	[*base *p : *]
	[isallr *list *el [*p * : *el]]
	[adjacent *list *p1 *p2]
]
[[next_path *base *p *p]]

[[previous_path *base *p1 *p2]
	[*base *p : *]
	[isall *list *el [*p * : *el]]
	[adjacent *list *p1 *p2]
]
[[previous_path *base *p *p]]

[[sub *moon [*i : *is] *path [*pp : *tail]]
	[isall *pr *pp [*moon * : *path]]
	[NODUP *pr *pnr] [REVERSE *pnr *pn]
	[AT *i *pp *pn]
	/ [sub *moon *is *path *tail]
]
[[sub * * * []]]

[[cb *key *velocity] [cb_callback : *callback] [*callback keyon *key *velocity]]
[[cb *ret 73 *v] [cb_callback : *callback] [*callback control 73 *v] [add *ret "Attack = " *v]]
[[cb *ret 93 *v] [cb_callback : *callback] [*callback control 93 *v] [add *ret "Decay = " *v]]
[[cb *ret 94 *v] [cb_callback : *callback] [*callback control 94 *v] [add *ret "Sustain = " *v]]
[[cb *ret 72 *v] [cb_callback : *callback] [*callback control 72 *v] [add *ret "Release = " *v]]
[[cb *ret 74 *v] [cb_callback : *callback] [*callback control 74 *v] [- *v 64 *vv] [add *ret "Freq = " *vv]]
[[cb *ret 91 *v] [cb_callback : *callback] [*callback control 91 *v] [- *v 64 *vv] [add *ret "Dry/Wet = " *vv]]
[[cb *ret 10 *v] [cb_callback : *callback] [*callback control 10 *v] [- *v 64 *vv] [add *ret "Pan = " *vv]]
[[cb *ret 7 *v] [cb_callback : *callback] [*callback control 7 *v] [add *ret "Volume = " *v]]
[[cb *ret 11 *v] [cb_callback : *callback] [*callback control 11 *v] [add *ret "Porta = " *v]]
[[cb *ret 65 *v] [cb_callback : *callback] [*callback control 65 *v] [add *ret "Porta Switch = " *v]]
[[cb *ret 95 *v] [cb_callback : *callback] [*callback control 95 *v] [- *v 64 *vv] [add *ret "Speed = " *vv]]
[[cb *ret 71 *v] [cb_callback : *callback] [*callback control 71 *v] [add *ret "Vibrato = " *v]]
[[cb *ret 128 *v] [cb_callback : *callback] [*callback control 128 *v] [- *v 64 *vv] [add *ret "Pitch = " *vv]]
[[cb *ret 1 *v] [cb_callback : *callback] [*callback control 1 *v] [add *ret "Modulation = " *v]]
[[cb "Mode = Mono" 126 *v] [cb_callback : *callback] [*callback control 126 0]]
[[cb "Mode = Poly" 127 *v] [cb_callback : *callback] [*callback control 127 0]]
[[cb *ret 12 *x *y]
	[cb_callback : *callback] [*callback control 12 *x] [*callback control 13 *y]
	[- *x 64 *xx] [- *y 64 *yy] [add *ret "Vector = [" *xx " / " *yy "]"]
]

[[cb cb *poly *porta *pitch *modulation *x *y *volume *attack *decay *sustain *release
			*freq *drywet *pan *porta_time *speed *vibrato : *]
	[is_var *poly]
	[cb_callback : *cb]
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

[[cb *ret Store *file_name]
	[cb_path : *path]
	[eq *path [*moon : *]]
	[Store *moon *file_name]
	[relativise_path *file_name *file]
	[add *ret *file " Stored"]
]
[[cb *ret Store *file_name] [add *ret "Failed to store " *file_name]]

[[cb *ret Restore *file_name]
	[cb_path : *path]
	[eq *path [*moon : *]]
	[Restore *moon *file_name]
	[relativise_path *file_name *file]
	[add *ret *file " Restored"]
]
[[cb *ret Restore *file_name] [add *ret "Failed to restore " *file_name]]

[[cb *ret [] *delta]
	;[add *ret "delta [" *delta "]"]
	[cb_path : *path]
	[eq *path [*m : *p]]
	[*m *parameters : *]
	[*parameters *pb : *p]
	[*pb *v1]
	[add *v1 *delta *v2]
	[*pb *v2]
	[*pb : *v3]
	[text_term *pather *path]
	[add *ret *pather " = " *v3]
]

[[cb *program 0 *i1 : *]
	[isallr *y *x [Moons *x]]
	[AT *i1 *moon *y]
	[cb_path [*moon]]
	[*moon * * *moon_callback *type : *]
	[cb_callback *moon_callback]
	[add *program *type " : channel=" *i1 " " *moon]
]

[[cb *program *mode *i1 : *is]
	[isallr *y *x [Moons *x]]
	[AT *i1 *moon *y]
	[*moon *parameters : *]
	[sub *parameters *is *path *path]
	[process_mode [*moon : *path] *mode *processed]
	[text_term *pather *processed]
	[*parameters *pb : *path] [*pb : *v]
	[add *program *pather " = " *v]
	[show [*moon : *path]]
	[cb_path [*moon : *path]]
	[*moon * * *moon_callback : *]
	[cb_callback *moon_callback]
]

[[cb *ret START *file_name] [core *file_name] [add *ret "Recording started " *file_name]]
[[cb STOP] [core] [show "STOP RECORDING"]]

[[process_mode [*h : *t] 0 [[*h] : *t]]]
[[process_mode [*h : *t] *i [*h : *t2]] [less 0 *i] / [-- *i *next] [process_mode *t *next *t2]]

[[BuildPhobos *Phobos *PhobosCB *volume *pan]
	[addcl [[Moons *Phobos]]]
	[Moonbase *Phobos *PhobosCB Phobos]

	[moonbase *moonbase]
	[arpeggiator *PhobosCB *moonbase]
	[auto_data *XData] [parameter_block *X "index"] [*XData *X]
	[auto_data *YData] [parameter_block *Y "index"] [*YData *Y]
	[parameter_block *AutoCtrl "index"] [*XData "control" *AutoCtrl] [*YData "control" *AutoCtrl]
	[parameter_block *pitch "index"] [parameter_block *modulation "index"] [parameter_block *freq "index"]
	[lfo *lfo1] [lfo *lfo2]

	[control *lfo1_ctrl] [gateway *lfo1mod]
	[gateway *lfosens1] [gateway *lfosens2] [gateway *lfosens3] [gateway *lfosens4]
	[gateway *pitch_ctrl1] [gateway *pitch_ctrl2] [gateway *pitch_ctrl3] [gateway *pitch_ctrl4]

	[*lfo1_ctrl *lfo1] [*lfo1mod *modulation] [*lfo1_ctrl "gateway" *lfo1mod]
	[*lfosens1 *lfo1_ctrl] [*lfosens2 *lfo1_ctrl] [*lfosens3 *lfo1_ctrl] [*lfosens4 *lfo1_ctrl]
	[*pitch_ctrl1 *pitch] [*pitch_ctrl2 *pitch] [*pitch_ctrl3 *pitch] [*pitch_ctrl4 *pitch]

	[pan *pan] [delay *delay] [drywet *mixer] [volume *volume]
	[ConnectStereo *delay *pan]
	[ConnectDryWet *mixer *pan *delay]
	[ConnectStereo *volume *mixer]

	[Insert *volume *Phobos core]
	[Insert *pan *Phobos core]
	[Insert *mixer *Phobos core]
	[Insert *delay *Phobos core delay]
	[InsertPB *X *Phobos core X]
	[InsertPB *Y *Phobos core Y]
	[InsertPB *modulation *Phobos core modulation]
	[InsertPB *freq *Phobos core freq]
	[InsertPB *pitch *Phobos core pitch]
	[InsertPB *AutoCtrl *Phobos core auto]
	[Insert *PhobosCB *Phobos arpeggiator]
	[Insert *lfo1 *Phobos lfo 1]
	[Insert *lfo2 *Phobos lfo 2]
	[Insert *pitch_ctrl1 *Phobos sensitivity pitch 1]
	[Insert *pitch_ctrl2 *Phobos sensitivity pitch 2]
	[Insert *pitch_ctrl3 *Phobos sensitivity pitch 3]
	[Insert *pitch_ctrl4 *Phobos sensitivity pitch 4]

	[show "STAGE 1 (common)"]

	[REPEAT 1
		[BuildPhobosPart *Phobos *moonbase *pan *XData *YData *lfo1 *lfo2
			*lfosens1 *lfosens2 *lfosens3 *lfosens4
			*pitch_ctrl1 *pitch_ctrl2 *pitch_ctrl3 *pitch_ctrl4
			]
	]

	[show "STAGE 2 (parts)"]

	[Insert *lfo1_ctrl *Phobos sensitivity lfo 1 vibrato]
	[Insert *lfo1mod *Phobos sensitivity lfo 1 modulation]
	[Insert *lfosens1 *Phobos sensitivity lfo 1 freq 1]
	[Insert *lfosens2 *Phobos sensitivity lfo 1 freq 2]
	[Insert *lfosens3 *Phobos sensitivity lfo 1 freq 3]
	[Insert *lfosens4 *Phobos sensitivity lfo 1 freq 4]

	[InsertBlock *XData *Phobos vector X]
	[InsertBlock *YData *Phobos vector Y]

	[InsertController 1 *Phobos core modulation]
	[InsertController 7 *Phobos core volume]
	[InsertController 10 -64 *Phobos core pan]
	[InsertController 11 -64 *Phobos portamento time]
	[InsertController 12 -64 *Phobos core X]
	[InsertController 13 -64 *Phobos core Y]
	[InsertController 65 *Phobos portamento porta]
	[InsertController 74 -64 *Phobos core freq]
	[InsertController 91 -64 *Phobos core balance]
	[InsertController 95 -64 *Phobos lfo 1 speed]
	[InsertController 73 *Phobos adsr attack]
	[InsertController 93 *Phobos adsr decay]
	[InsertController 94 -128 *Phobos adsr sustain]
	[InsertController 72 *Phobos adsr release]
	[InsertController 128 -64 *Phobos core pitch]

	[show "STAGE 3 (completed)"]

	[Lunar -128 *Phobos sensitivity freq 1 left]
	[Lunar 128 *Phobos sensitivity freq 1 right]
	[Lunar -128 *Phobos sensitivity freq 2 left]
	[Lunar 128 *Phobos sensitivity freq 2 right]
	[Lunar -128 *Phobos sensitivity freq 3 left]
	[Lunar 128 *Phobos sensitivity freq 3 right]
	[Lunar -128 *Phobos sensitivity freq 4 left]
	[Lunar 128 *Phobos sensitivity freq 4 right]
]


[[BuildPhobosPart *Phobos *PhobosCB *mixer *X *Y *lfo1 *lfo2
					*lfosens1 *lfosens2 *lfosens3 *lfosens4
					*pitchsens1 *pitchsens2 *pitchsens3 *pitchsens4
					]
	[trigger *trigger]
	[fm4 *op]
	[filter *filter]
	[adsr *adsr]
	[amplifier *dca]
	[parameter_block *trigger_delay 0]
	[sensitivity *freq1] [sensitivity *freq2] [sensitivity *freq3] [sensitivity *freq4]
	[eg *ampeg1] [eg *ampeg2] [eg *ampeg3] [eg *ampeg4]
	[eg *freqeg] [gateway *freqeg1] [gateway *freqeg2] [gateway *freqeg3] [gateway *freqeg4]
	[sensitivity *velocity1] [sensitivity *velocity2] [sensitivity *velocity3] [sensitivity *velocity4]
	[sensitivity *key1] [sensitivity *key2] [sensitivity *key3] [sensitivity *key4]
	[sensitivity *egscal] [sensitivity *egscalfreq] [sensitivity *egscal1] [sensitivity *egscal2] [sensitivity *egscal3] [sensitivity *egscal4]

	[*PhobosCB *trigger]
	[*trigger_delay "signal" *trigger "trigger"]
	[*adsr "trigger" *trigger_delay]
	[*freqeg "trigger" *trigger "trigger"]
	[*ampeg1 "trigger" *trigger_delay] [*ampeg2 "trigger" *trigger_delay] [*ampeg3 "trigger" *trigger_delay] [*ampeg4 "trigger" *trigger_delay]
	[*trigger "busy" *adsr "busy"]
	[*lfo1 "trigger" *trigger "trigger"] [*lfo2 "trigger" *trigger "trigger"]

	[*freq1 "signal" *trigger "key"] [*freq2 "signal" *trigger "key"] [*freq3 "signal" *trigger "key"] [*freq4 "signal" *trigger "key"]
	[*op "trigger" *trigger_delay]
	[*op "freq1" *freq1] [*op "freq2" *freq2] [*op "freq3" *freq3] [*op "freq4" *freq4]
	[*op "freq1" *lfosens1] [*op "freq2" *lfosens2] [*op "freq3" *lfosens3] [*op "freq4" *lfosens4]
	[*op "freq1" *pitchsens1] [*op "freq2" *pitchsens2] [*op "freq3" *pitchsens3] [*op "freq4" *pitchsens4]

	[auto *vectorX *X] [*vectorX "trigger" *trigger "trigger"] [*X "trigger" *trigger "trigger"]
	[auto *vectorY *Y] [*vectorY "trigger" *trigger "trigger"] [*Y "trigger" *trigger "trigger"]
	[sensitivity *Xsa1] [sensitivity *Xsa2] [sensitivity *Xsa3] [sensitivity *Xsa4]
	[sensitivity *Ysa1] [sensitivity *Ysa2] [sensitivity *Ysa3] [sensitivity *Ysa4]
	[*Xsa1 *vectorX] [*Xsa2 *vectorX] [*Xsa3 *vectorX] [*Xsa4 *vectorX]
	[*Ysa1 *vectorY] [*Ysa2 *vectorY] [*Ysa3 *vectorY] [*Ysa4 *vectorY]
	[*op "amp1" *Xsa1] [*op "amp2" *Xsa2] [*op "amp3" *Xsa3] [*op "amp4" *Xsa4]
	[*op "amp1" *Ysa1] [*op "amp2" *Ysa2] [*op "amp3" *Ysa3] [*op "amp4" *Ysa4]

	[*freqeg1 *freqeg] [*freqeg2 *freqeg] [*freqeg3 *freqeg] [*freqeg4 *freqeg]
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

	[*filter *op]
	[*dca *filter]
	[*dca "gateway" *adsr]
	[*mixer *dca]

	[Insert *op *Phobos operator]
	[Insert *freq1 *Phobos sensitivity freq 1]
	[Insert *freq2 *Phobos sensitivity freq 2]
	[Insert *freq3 *Phobos sensitivity freq 3]
	[Insert *freq4 *Phobos sensitivity freq 4]
	[Insert *Xsa1 *Phobos sensitivity amp 1 X]
	[Insert *Xsa2 *Phobos sensitivity amp 2 X]
	[Insert *Xsa3 *Phobos sensitivity amp 3 X]
	[Insert *Xsa4 *Phobos sensitivity amp 4 X]
	[Insert *Ysa1 *Phobos sensitivity amp 1 Y]
	[Insert *Ysa2 *Phobos sensitivity amp 2 Y]
	[Insert *Ysa3 *Phobos sensitivity amp 3 Y]
	[Insert *Ysa4 *Phobos sensitivity amp 4 Y]
	[Insert *freqeg *Phobos operator eg freq]
	[Insert *ampeg1 *Phobos operator 1 eg amp]
	[Insert *ampeg2 *Phobos operator 2 eg amp]
	[Insert *ampeg3 *Phobos operator 3 eg amp]
	[Insert *ampeg4 *Phobos operator 4 eg amp]
	[Insert *freqeg1 *Phobos operator 1 eg freq]
	[Insert *freqeg2 *Phobos operator 2 eg freq]
	[Insert *freqeg3 *Phobos operator 3 eg freq]
	[Insert *freqeg4 *Phobos operator 4 eg freq]
	[Insert *key1 *Phobos sensitivity amp 1 key]
	[Insert *key2 *Phobos sensitivity amp 2 key]
	[Insert *key3 *Phobos sensitivity amp 3 key]
	[Insert *key4 *Phobos sensitivity amp 4 key]
	[Insert *velocity1 *Phobos sensitivity amp 1 velocity]
	[Insert *velocity2 *Phobos sensitivity amp 2 velocity]
	[Insert *velocity3 *Phobos sensitivity amp 3 velocity]
	[Insert *velocity4 *Phobos sensitivity amp 4 velocity]
	[Insert *egscal *Phobos sensitivity egscaling adsr]
	[Insert *egscalfreq *Phobos sensitivity egscaling freq]
	[Insert *egscal1 *Phobos sensitivity egscaling 1]
	[Insert *egscal2 *Phobos sensitivity egscaling 2]
	[Insert *egscal3 *Phobos sensitivity egscaling 3]
	[Insert *egscal4 *Phobos sensitivity egscaling 4]
	[Insert *filter *Phobos filter]
	[Insert *adsr *Phobos adsr]
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

[[audio] [core reactor 330 22050 2048] [ConnectStereo reactor phobos_mixer] [ConnectStereo reactor abakos_mixer]]

[[MM *base]
	[*base *parameters *modules *callback : *]
	[delallcl *base]
	[TRY [*parameters *parameter : *selector] [*parameter] fail]
	[TRY [*modules *module : *selector] [show *selector *module] [*module []] [*module] fail]
	[delallcl *parameters]
	[delallcl *modules]
]

end := [
		[var cb_path cb_callback]
		[BuildPhobos Phobos PhobosCB phobos_mixer *feed]
		[build_Abakos *Abakos abakos_mixer]
		[oscilloscope radar]
		[CommandCentre commander cb]
		;[commander 3000 1060] [radar 2000 1060]
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

