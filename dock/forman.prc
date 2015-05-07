
import studio
import lunar

program forman [Forman BuildForman BuildFormanPart]

[[BuildForman *polyphony]
	[GenerateInstrumentName Forman *forman]
	[BuildForman *polyphony *forman * *]
]

[[BuildForman *polyphony *forman] [BuildForman *polyphony *forman * *]]

[[BuildForman *polyphony *Forman *FormanCB *volume]
	[Moonbase *Forman *FormanCB Forman]
	[moonbase *FormanCB]
	[chorus *chorus] [pan *pan] [delay *delay] [drywet *drywet] [volume *volume]
	[*pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[Insert *volume *Forman core]
	[Insert *pan *Forman core]
	[Insert *chorus *Forman core chorus]
	[Insert *drywet *Forman core delay]
	[Insert *delay *Forman core delay]
	[REPEAT *polyphony
		[BuildFormanPart *Forman *FormanCB *chorus]
	]
	[InsertController 91 -64 *Forman core delay balance]
	[InsertController 73 *Forman adsr attack]
	[InsertController 93 *Forman adsr decay]
	[InsertController 94 -128 *Forman adsr sustain]
	[InsertController 72 *Forman adsr release]
	[Lunar -16384 *Forman filter 2 amp]
	[Lunar -16384 *Forman filter 3 amp]
	[Lunar -16384 *Forman filter 4 amp]
	[Lunar -16384 *Forman filter 5 amp]
	[Lunar -16384 *Forman filter 6 amp]
	[AddMoon *Forman *FormanCB *volume]
]

[[BuildFormanPart *Forman *FormanCB *chorus]
	[trigger *trigger]
	[saw_operator *op] [adsr *adsr]
	[filter *f1] [filter *f2] [filter *f3] [filter *f4] [filter *f5] [filter *f6]
	[*FormanCB *trigger]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[*op "trigger" *trigger "trigger"]
	[*op "freq" *trigger "key"]
	[*f1 *op] [*f1 "amp" *adsr] [*chorus "signal" *f1 "bandpass"]
	[*f2 *op] [*f2 "amp" *adsr] [*chorus "signal" *f2 "bandpass"]
	[*f3 *op] [*f3 "amp" *adsr] [*chorus "signal" *f3 "bandpass"]
	[*f4 *op] [*f4 "amp" *adsr] [*chorus "signal" *f4 "bandpass"]
	[*f5 *op] [*f5 "amp" *adsr] [*chorus "signal" *f5 "bandpass"]
	[*f6 *op] [*f6 "amp" *adsr] [*chorus "signal" *f6 "bandpass"]
	[Insert *trigger *Forman trigger]
	[Insert *op *Forman operator]
	[Insert *f1 *Forman filter 1]
	[Insert *f2 *Forman filter 2]
	[Insert *f3 *Forman filter 3]
	[Insert *f4 *Forman filter 4]
	[Insert *f5 *Forman filter 5]
	[Insert *f6 *Forman filter 6]
	[Insert *adsr *Forman adsr]
]


end .
