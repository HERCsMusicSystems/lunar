
import studio
import control
import conductor
import lunar
import rudiments

program doctor [
				start_audio start_midi
				reactor commander mdi mdicb
				Doctor cb BuildDoctor BuildDoctorPart DoctorWaveforms
				]

auto := [[wave DoctorWaveforms
					"Doctor/snare_h.wav"
					"Doctor/snare_m.wav"
					"Doctor/snare_l.wav"
					"Doctor/snare_roll.wav"
			]]

[[BuildDoctor *polyphony *Doctor *DoctorCB *volume]
	[addcl [[Moons *Doctor]]]
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
	[BuildDoctorPart *Doctor *DoctorCB *freq *index *pan]
	[InsertController 7 *Doctor volume]
	[InsertController 10 -64 *Doctor pan]
	[InsertController 91 -64 *Doctor delay balance]
	[Lunar -8192 *Doctor delay balance]
]

[[BuildDoctorPart *Doctor *DoctorCB *freq *index *line]
	[trigger *trigger]
	[*DoctorCB *trigger]
	[sampler_operator *sam DoctorWaveforms]
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

[[start_audio] [core reactor 330 32000 1024 30 -1]]
[[start_audio] [core reactor 330 22050 4096]]

[[start_midi] [midi mdi mdicb]]
[[start_midi] [midi mdi mdicb "/dev/snd/midiC3D0"]]
[[start_midi]]

[[mdicb *command *ch : *x] [cb *command : *x] [show [*command : *x]]]

end := [
		[BuildDoctor 1 Doctor cb *line]
		[start_audio] [start_midi]
		[ConnectStereo reactor *line]
		[CommandCentre commander CCCB]
		[command]
		[Moonbase Doctor]
		].

