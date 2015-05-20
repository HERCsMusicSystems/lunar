
import lunar

program doctor [
				Doctor BuildDoctor BuildDoctorPart DoctorWaveforms
				]

[[BuildDoctor *polyphony] [BuildDoctor *polyphony * * *]]

[[BuildDoctor *polyphony *waveforms] [BuildDoctor *polyphony * * * *waveforms]]

[[BuildDoctor *polyphony *doctor *waveforms] [BuildDoctor *polyphony *doctor * * *waveforms]]

[[BuildDoctor *polyphony *Doctor *DoctorCB *volume]
	[TRY
		[wave DoctorWaveforms
			"Doctor/snare_h.wav"
			"Doctor/snare_m.wav"
			"Doctor/snare_l.wav"
			"Doctor/snare_roll.wav"
			"Doctor/snare_hit_roll.wav"
			"Doctor/snare_soft_roll.wav"
			"Doctor/snare_loop.wav"
		]
		[show "Waves created."]
	]
	[BuildDoctor *polyphony *Doctor *DoctorCB *volume DoctorWaveforms]
]

[[BuildDoctor *polyphony *Doctor *DoctorCB *volume *DoctorWaveforms]
	[Moonbase *Doctor *DoctorCB Doctor *volume]
	[moonbase *DoctorCB]
	[parameter_block *pitch "index"] [gateway *pitch_range]
	[stereo_pan *pan]
	[delay *delay]
	[drywet *drywet]
	[volume *volume]
	[*pitch_range *pitch]
	[ConnectStereo *delay *pan]
	[ConnectDryWet *drywet *pan *delay]
	[ConnectStereo *volume *drywet]
	[Insert *volume *Doctor]
	[Insert *pan *Doctor]
	[InsertPB *pitch *Doctor pitch]
	[Insert *pitch_range *Doctor range]
	[Insert *delay *Doctor delay]
	[Insert *drywet *Doctor delay]
	[REPEAT *polyphony
		[BuildDoctorPart *Doctor *DoctorCB *pitch_range *pan *DoctorWaveforms]
	]
	[InsertController 7 *Doctor volume]
	[InsertController 10 -64 *Doctor pan]
	[InsertController 91 -64 *Doctor delay balance]
	[InsertController 74 -64 *Doctor filter freq]
	[InsertController 73 *Doctor adsr attack]
	[InsertController 93 *Doctor adsr decay]
	[InsertController 94 -128 *Doctor adsr sustain]
	[InsertController 72 *Doctor adsr release]
	[InsertController 128 -64 *Doctor pitch]
	[Lunar -8192 *Doctor delay balance]
	[Lunar 16384 *Doctor adsr release]
	[Lunar 16384 *Doctor sensitivity BP]
	[Lunar -48 *Doctor sensitivity left]
]

[[BuildDoctorPart *Doctor *DoctorCB *pitch_range *line *DoctorWaveforms]
	[trigger *trigger]
	[*DoctorCB *trigger]
	[sampler_operator *sam *DoctorWaveforms]
	[sensitivity *sens]
	[adsr *adsr]
	[filter *left_filter] [filter *right_filter]
	[*sens "signal" *trigger "velocity"]
	[*adsr "trigger" *trigger "trigger"]
	[*sam "trigger" *trigger "delay1"]
	[*sam "freq" *pitch_range]
	[*sam "amp" *sens "signal"]
	[*sam "amp" *adsr "signal"]
	[*sam "index" *trigger "index"]
	[*left_filter "enter" *sam "left"]
	[*right_filter "enter" *sam "right"]
	[*line "left" *left_filter]
	[*line "right" *right_filter]
	[Insert *sam *Doctor operator]
	[Insert *left_filter *Doctor filter]
	[Insert *right_filter *Doctor filter]
	[Insert *adsr *Doctor adsr]
	[Insert *sens *Doctor sensitivity]
]

end .

