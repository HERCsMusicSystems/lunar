
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
	[parameter_block *freq "freq"]
	[parameter_block *index "index"]
	[pan *pan]
	[delay *delay]
	[drywet *drywet]
	[volume *volume]
	[ConnectStereo *delay *pan]
	[ConnectDryWet *drywet *pan *delay]
	[ConnectStereo *volume *drywet]
	[Insert *volume *Doctor]
	[Insert *pan *Doctor]
	[InsertPB *freq *Doctor freq]
	[InsertPB *index *Doctor index]
	[Insert *delay *Doctor delay]
	[Insert *drywet *Doctor delay]
	[REPEAT *polyphony
		[BuildDoctorPart *Doctor *DoctorCB *freq *index *pan *DoctorWaveforms]
	]
	[InsertController 7 *Doctor volume]
	[InsertController 10 -64 *Doctor pan]
	[InsertController 91 -64 *Doctor delay balance]
	[Lunar -8192 *Doctor delay balance]
	[AddMoon *Doctor *DoctorCB *volume]
]

[[BuildDoctorPart *Doctor *DoctorCB *freq *index *line *DoctorWaveforms]
	[trigger *trigger]
	[*DoctorCB *trigger]
	[sampler_operator *sam *DoctorWaveforms]
	[sensitivity *sens]
	[*sens "signal" *trigger "velocity"]
	[*sam "trigger" *trigger "delay1"]
	[*sam "amp" *sens "signal"]
	[*sam "index" *index]
	[*sam "index" *trigger "index"]
	[*sam "freq" *freq]
	[*line *sam]
	[*sens "breakpoint" 16256]
	[*sens "left" -48]
]

end .

