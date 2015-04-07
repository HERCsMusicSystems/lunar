
import lunar

program doctor [
				Doctor BuildDoctor BuildDoctorPart DoctorWaveforms
				]

[[BuildDoctor *polyphony *Doctor *DoctorCB *volume]
	[TRY
		[show "trying...."]
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
	[create_atoms *Doctor *DoctorCB *volume]
	[Moonbase *Doctor *DoctorCB Doctor]
	[moonbase *DoctorCB]
	[pan *pan]
	[delay *delay]
	[drywet *drywet]
	[volume *volume]
	[ConnectStereo *delay *pan]
	[ConnectDryWet *drywet *pan *delay]
	[ConnectStereo *volume *drywet]
	[Insert *volume *Doctor]
	[Insert *pan *Doctor]
	[Insert *delay *Doctor delay]
	[Insert *drywet *Doctor delay]
	[REPEAT *polyphony
		[BuildDoctorPart *Doctor *DoctorCB *pan *DoctorWaveforms]
	]
	[InsertController 7 *Doctor volume]
	[InsertController 10 -64 *Doctor pan]
	[InsertController 91 -64 *Doctor delay balance]
	[InsertController 73 *Doctor adsr attack]
	[InsertController 93 *Doctor adsr decay]
	[InsertController 94 -128 *Doctor adsr sustain]
	[InsertController 72 *Doctor adsr release]
	[Lunar -8192 *Doctor delay balance]
	[Lunar 16384 *Doctor adsr release]
	[Lunar 16384 *Doctor sensitivity BP]
	[Lunar -48 *Doctor sensitivity left]
	[AddMoon *Doctor *DoctorCB *volume]
]

[[BuildDoctorPart *Doctor *DoctorCB *line *DoctorWaveforms]
	[trigger *trigger]
	[*DoctorCB *trigger]
	[sampler_operator *sam *DoctorWaveforms]
	[sensitivity *sens]
	[adsr *adsr]
	[*sens "signal" *trigger "velocity"]
	[*adsr "trigger" *trigger "trigger"]
	[*sam "trigger" *trigger "delay1"]
	[*sam "amp" *sens "signal"]
	[*sam "amp" *adsr "signal"]
	[*sam "index" *trigger "index"]
	[*line *sam]
	[Insert *sam *Doctor operator]
	[Insert *adsr *Doctor adsr]
	[Insert *sens *Doctor sensitivity]
]

end .

