
import studio
import lunar

program forman [Forman BuildForman BuildFormanPart]

[[BuildForman *polyphony] [BuildForman *polyphony * * *]]

[[BuildForman *polyphony *forman] [BuildForman *polyphony *forman * *]]

[[BuildForman *polyphony *Forman *FormanCB *volume]
	[Moonbase *Forman *FormanCB Forman *volume]
	[moonbase *FormanCB]
	[lfo *lfo1] [lfo *lfo2] [control *vibrato] [gateway *vibrato_sensitivity]
	[*vibrato *lfo1] [*vibrato_sensitivity *vibrato]
	[parameter_block *formant "index"] [parameter_block *pitch "index"] [gateway *pitch_sensitivity]
	[*pitch_sensitivity *pitch]
	[chorus *chorus] [pan *pan] [delay *delay] [drywet *drywet] [volume *volume]
	[*pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[Insert *volume *Forman core]
	[Insert *pan *Forman core]
	[InsertPB *formant *Forman core formant]
	[InsertPB *pitch *Forman core pitch]
	[Insert *chorus *Forman core chorus]
	[Insert *drywet *Forman core delay]
	[Insert *delay *Forman core delay]
	[REPEAT *polyphony
		[BuildFormanPart *Forman *FormanCB *chorus *vibrato_sensitivity *formant *pitch_sensitivity]
	]
	[Insert *pitch_sensitivity *Forman operator sens]
	[Insert *vibrato *Forman lfo 1 vibrato]
	[Insert *vibrato_sensitivity *Forman lfo 1 sens]
	[Insert *lfo1 *Forman lfo 1]
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
]

[[BuildFormanPart *Forman *FormanCB *chorus *vibrato *formant *pitch]
	[trigger *trigger]
	[saw_operator *op] [adsr *adsr]
	[filter *f1] [filter *f2] [filter *f3] [filter *f4]
	[*FormanCB *trigger]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[*op "trigger" *trigger "trigger"]
	[*op "freq" *trigger "key"]
	[*op "freq" *vibrato]
	[*op "freq" *pitch]
	[*f1 *op] [*f1 "freq" *formant] [*f1 "amp" *adsr] [*chorus "signal" *f1 "bandpass"]
	[*f2 *op] [*f2 "freq" *formant] [*f2 "amp" *adsr] [*chorus "signal" *f2 "bandpass"]
	[*f3 *op] [*f3 "freq" *formant] [*f3 "amp" *adsr] [*chorus "signal" *f3 "bandpass"]
	[*f4 *op] [*f4 "freq" *formant] [*f4 "amp" *adsr] [*chorus "signal" *f4 "bandpass"]
	[Insert *trigger *Forman portamento]
	[Insert *op *Forman operator]
	[Insert *f1 *Forman filter 1]
	[Insert *f2 *Forman filter 2]
	[Insert *f3 *Forman filter 3]
	[Insert *f4 *Forman filter 4]
	[Insert *adsr *Forman adsr]
]


end .
