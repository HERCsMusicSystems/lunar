
import studio
import lunar

program forman [BuildForman BuildFormanPart]

[[BuildForman *polyphony] / [BuildForman *polyphony * * *]]
[[BuildForman *polyphony *Forman *formancb *volume]
	[Moonbase *Forman *formancb Forman *volume]
	[moonbase *formancb]
	[lfo *lfo1] [lfo *lfo2]
	[parameter_block *X "index"] [parameter_block *Y "index"]
	[volume *volume] [drywet *drywet] [delay *delay] [stereo_pan *pan] [stereo_chorus *chorus]
	[ConnectStereo *pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[Insert *volume *Forman core]
	[Insert *pan *Forman core]
	[InsertPB *X *Forman core X]
	[InsertPB *Y *Forman core Y]
	[Insert *chorus *Forman core chorus]
	[Insert *drywet *Forman core delay]
	[Insert *delay *Forman core delay]
	[Insert *lfo1 *Forman lfo 1]
	[Insert *lfo2 *Forman lfo 2]
	[REPEAT *polyphony [BuildFormanPart *Forman *formancb *chorus *X *Y]]
	[InsertController 7 *Forman core volume]
	[InsertController 10 -64 *Forman core pan]
	[InsertController 16 *Forman core X]
	[InsertController 17 *Forman core Y]
	[InsertController 77 -64 *Forman core delay balance]
	[Lunar -16384 *Forman noise amp]
	[Lunar -8192 *Forman core delay balance]
	[Lunar 1 *Forman vco wave]
]

[[BuildFormanPart *Forman *cb *line *X *Y]
	[trigger *trigger] [*cb *trigger]
	[adsr *adsr]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[vco *vco] [noise_operator *noise]
	[*vco "freq" *trigger "key"]
	[filter *filter]
	;============
	[formant_filter *ff1]
		[morph *ff1freq] [*ff1freq "x" *X] [*ff1freq "y" *Y] [*ff1 "freq" *ff1freq]
		[morph *ff1reso] [*ff1reso "x" *X] [*ff1reso "y" *Y] [*ff1 "resonance" *ff1reso]
		[morph *ff1q] [*ff1q "x" *X] [*ff1q "y" *Y] [*ff1 "q" *ff1q]
	[*ff1 *vco] [*ff1 *noise] [*filter *ff1]
	[formant_filter *ff2]
		[morph *ff2freq] [*ff2freq "x" *X] [*ff2freq "y" *Y] [*ff2 "freq" *ff2freq]
		[morph *ff2reso] [*ff2reso "x" *X] [*ff2reso "y" *Y] [*ff2 "resonance" *ff2reso]
		[morph *ff2q] [*ff2q "x" *X] [*ff2q "y" *Y] [*ff2 "q" *ff2q]
	[*ff2 *vco] [*ff2 *noise] [*filter *ff2]
	[formant_filter *ff3]
		[morph *ff3freq] [*ff3freq "x" *X] [*ff3freq "y" *Y] [*ff3 "freq" *ff3freq]
		[morph *ff3reso] [*ff3reso "x" *X] [*ff3reso "y" *Y] [*ff3 "resonance" *ff3reso]
		[morph *ff3q] [*ff3q "x" *X] [*ff3q "y" *Y] [*ff3 "q" *ff3q]
	[*ff3 *vco] [*ff3 *noise] [*filter *ff3]
	[formant_filter *ff4]
		[morph *ff4freq] [*ff4freq "x" *X] [*ff4freq "y" *Y] [*ff4 "freq" *ff4freq]
		[morph *ff4reso] [*ff4reso "x" *X] [*ff4reso "y" *Y] [*ff4 "resonance" *ff4reso]
		[morph *ff4q] [*ff4q "x" *X] [*ff4q "y" *Y] [*ff4 "q" *ff4q]
	[*ff4 *vco] [*ff4 *noise] [*filter *ff4]
	;============
	[*filter *vco] [*filter *noise]
	[*filter "gain" *adsr]
	[*line *filter]
	[Insert *vco *Forman vco]
	[Insert *noise *Forman noise]
	[Insert *ff1 *Forman formant 1]
	[Insert *ff2 *Forman formant 2]
	[Insert *ff3 *Forman formant 3]
	[Insert *ff4 *Forman formant 4]
	[Insert *ff1freq *Forman vector 1 freq]
	[Insert *ff1reso *Forman vector 1 resonance]
	[Insert *ff1q *Forman vector 1 Q]
	[Insert *ff2freq *Forman vector 2 freq]
	[Insert *ff2reso *Forman vector 2 resonance]
	[Insert *ff2q *Forman vector 2 Q]
	[Insert *ff3freq *Forman vector 3 freq]
	[Insert *ff3reso *Forman vector 3 resonance]
	[Insert *ff3q *Forman vector 3 Q]
	[Insert *ff4freq *Forman vector 4 freq]
	[Insert *ff4reso *Forman vector 4 resonance]
	[Insert *ff4q *Forman vector 4 Q]
	[Insert *filter *Forman filter]
	[Insert *adsr *Forman adsr]
	[Insert *trigger *Forman portamento]
]

end .

program forman [Forman BuildForman BuildFormanPart]

[[BuildForman *polyphony] [BuildForman *polyphony * * *]]

[[BuildForman *polyphony *forman] [BuildForman *polyphony *forman * *]]

[[BuildForman *polyphony *Forman *FormanCB *volume]
	[Moonbase *Forman *FormanCB Forman *volume]
	[moonbase *FormanCB]
	[lfo *lfo1] [lfo *lfo2]
	[control *vibrato] [gateway *vibrato_sensitivity]
	[control *tremolo] [control *wahwah] [control *lfo_pan]
	[control *lfo_freq1] [control *lfo_freq2] [control *lfo_freq3] [control *lfo_freq4]
	[control *lfo_amp1] [control *lfo_amp2] [control *lfo_amp3] [control *lfo_amp4]
	[*vibrato *lfo1] [*vibrato_sensitivity *vibrato]
	[*tremolo "enter" *lfo2 "negative"] [*wahwah *lfo2] [*lfo_pan *lfo2]
	[*lfo_freq1 *lfo2] [*lfo_freq2 *lfo2] [*lfo_freq3 *lfo2] [*lfo_freq4 *lfo2]
	[*lfo_amp1 "enter" *lfo2 "negative"]
	[*lfo_amp2 "enter" *lfo2 "negative"]
	[*lfo_amp3 "enter" *lfo2 "negative"]
	[*lfo_amp4 "enter" *lfo2 "negative"]
	[parameter_block *formant "index"] [parameter_block *pitch "index"] [gateway *pitch_sensitivity]
	[*pitch_sensitivity *pitch]
	[chorus *chorus] [pan *pan] [delay *delay] [drywet *drywet] [volume *volume]
	[*pan "pan" *lfo_pan]
	[*pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[Insert *volume *Forman core]
	[Insert *pan *Forman core]
	[InsertPB *formant *Forman core formant]
	[InsertPB *pitch *Forman core pitch]
	[Insert *chorus *Forman core chorus]
	[Insert *drywet *Forman core delay]
	[Insert *delay *Forman core delay]
	[REPEAT *polyphony
		[BuildFormanPart *Forman *FormanCB *chorus *vibrato_sensitivity *formant *pitch_sensitivity
			*tremolo *wahwah *lfo_freq1 *lfo_freq2 *lfo_freq3 *lfo_freq4 *lfo_amp1 *lfo_amp2 *lfo_amp3 *lfo_amp4
		]
	]
	[Insert *pitch_sensitivity *Forman operator sens]
	[Insert *vibrato *Forman lfo 1 vibrato]
	[Insert *lfo_freq1 *Forman filter 1 lfo freq]
	[Insert *lfo_freq2 *Forman filter 2 lfo freq]
	[Insert *lfo_freq3 *Forman filter 3 lfo freq]
	[Insert *lfo_freq4 *Forman filter 4 lfo freq]
	[Insert *lfo_amp1 *Forman filter 1 lfo amp]
	[Insert *lfo_amp2 *Forman filter 2 lfo amp]
	[Insert *lfo_amp3 *Forman filter 3 lfo amp]
	[Insert *lfo_amp4 *Forman filter 4 lfo amp]
	[Insert *vibrato_sensitivity *Forman lfo 1 sens]
	[Insert *lfo1 *Forman lfo 1]
	[Insert *tremolo *Forman lfo 2 tremolo]
	[Insert *wahwah *Forman lfo 2 wahwah]
	[Insert *lfo_pan *Forman lfo 2 pan]
	[Insert *lfo2 *Forman lfo 2]
	[InsertController 1 *Forman lfo 1 vibrato]
	[InsertController 7 *Forman core volume]
	[InsertController 11 *Forman portamento time]
	[InsertController 10 -64 *Forman core pan]
	[InsertController 65 *Forman portamento porta]
	[InsertController 91 -64 *Forman core delay balance]
	[InsertController 74 -64 *Forman core formant]
	[InsertController 73 *Forman adsr attack]
	[InsertController 93 *Forman adsr decay]
	[InsertController 94 -128 *Forman adsr sustain]
	[InsertController 72 *Forman adsr release]
	[InsertController 95 -64 *Forman lfo 1 speed]
	[InsertController 128 -64 *Forman core pitch]
	[Lunar -16384 *Forman filter 2 amp]
	[Lunar -16384 *Forman filter 3 amp]
	[Lunar -16384 *Forman filter 4 amp]
	[Lunar 128 *Forman lfo 1 sens]
	[Lunar 1792 *Forman lfo 1 speed]
	[Lunar 512 *Forman operator sens]
	[Lunar -16384 *Forman noise amp]
]

[[BuildFormanPart *Forman *FormanCB *chorus *vibrato *formant *pitch
		*tremolo *wahwah *lfo_freq1 *lfo_freq2 *lfo_freq3 *lfo_freq4 *lfo_amp1 *lfo_amp2 *lfo_amp3 *lfo_amp4
		]
	[trigger *trigger]
	[saw_operator *op] [noise_operator *noise] [adsr *adsr]
	[filter *f1] [filter *f2] [filter *f3] [filter *f4]
	[eg *feg] [eg *eg] [eg *neg]
	[eg *faeg1] [eg *faeg2] [eg *faeg3] [eg *faeg4]
	[eg *ffeg1] [eg *ffeg2] [eg *ffeg3] [eg *ffeg4]
	[*FormanCB *trigger]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[*eg "trigger" *trigger "trigger"]
	[*feg "trigger" *trigger "trigger"]
	[*neg "trigger" *trigger "trigger"]
	[*ffeg1 "trigger" *trigger "trigger"]
	[*ffeg2 "trigger" *trigger "trigger"]
	[*ffeg3 "trigger" *trigger "trigger"]
	[*ffeg4 "trigger" *trigger "trigger"]
	[*faeg1 "trigger" *trigger "trigger"]
	[*faeg2 "trigger" *trigger "trigger"]
	[*faeg3 "trigger" *trigger "trigger"]
	[*faeg4 "trigger" *trigger "trigger"]
	[*op "trigger" *trigger "trigger"]
	[*op "freq" *trigger "key"]
	[*op "freq" *vibrato]
	[*op "freq" *pitch]
	[*op "freq" *feg]
	[*op "amp" *eg]
	[*noise "amp" *neg]
	[*f1 *op] [*f1 *noise] [*f1 "freq" *formant] [*f1 "amp" *adsr] [*chorus "signal" *f1 "bandpass"]
	[*f2 *op] [*f2 *noise] [*f2 "freq" *formant] [*f2 "amp" *adsr] [*chorus "signal" *f2 "bandpass"]
	[*f3 *op] [*f3 *noise] [*f3 "freq" *formant] [*f3 "amp" *adsr] [*chorus "signal" *f3 "bandpass"]
	[*f4 *op] [*f4 *noise] [*f4 "freq" *formant] [*f4 "amp" *adsr] [*chorus "signal" *f4 "bandpass"]
	[*f1 "freq" *lfo_freq1] [*f2 "freq" *lfo_freq2] [*f3 "freq" *lfo_freq3] [*f4 "freq" *lfo_freq4]
	[*f1 "freq" *wahwah] [*f2 "freq" *wahwah] [*f3 "freq" *wahwah] [*f4 "freq" *wahwah]
	[*f1 "amp" *lfo_amp1] [*f2 "amp" *lfo_amp2] [*f3 "amp" *lfo_amp3] [*f4 "amp" *lfo_amp4]
	[*f1 "amp" *tremolo] [*f2 "amp" *tremolo] [*f3 "amp" *tremolo] [*f4 "amp" *tremolo]
	[*f1 "freq" *ffeg1] [*f2 "freq" *ffeg2] [*f3 "freq" *ffeg3] [*f4 "freq" *ffeg4]
	[*f1 "amp" *faeg1] [*f2 "amp" *faeg2] [*f3 "amp" *faeg3] [*f4 "amp" *faeg4]
	[Insert *trigger *Forman portamento]
	[Insert *op *Forman operator]
	[Insert *eg *Forman operator eg amp]
	[Insert *feg *Forman operator eg freq]
	[Insert *noise *Forman noise]
	[Insert *neg *Forman noise]
	[Insert *f1 *Forman filter 1] [Insert *ffeg1 *Forman filter 1 eg freq] [Insert *faeg1 *Forman filter 1 eg amp]
	[Insert *f2 *Forman filter 2] [Insert *ffeg2 *Forman filter 2 eg freq] [Insert *faeg2 *Forman filter 2 eg amp]
	[Insert *f3 *Forman filter 3] [Insert *ffeg3 *Forman filter 3 eg freq] [Insert *faeg3 *Forman filter 3 eg amp]
	[Insert *f4 *Forman filter 4] [Insert *ffeg4 *Forman filter 4 eg freq] [Insert *faeg4 *Forman filter 4 eg amp]
	[Insert *adsr *Forman adsr]
]


end .
