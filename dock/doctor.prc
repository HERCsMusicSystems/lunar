
import studio
import lunar

program doctor [Doctor BuildDoctor BuildDoctorPart]

[[BuildDoctor *polyphony]
	[TRY
		[wave *waveforms
			"Doctor/snare_h.wav"
			"Doctor/snare_m.wav"
			"Doctor/snare_l.wav"
			"Doctor/snare_roll.wav"
			"Doctor/snare_hit_roll.wav"
			"Doctor/snare_soft_roll.wav"
			"Doctor/snare_loop.wav"
		]
	]
	[BuildDoctor *polyphony *waveforms]
]
[[BuildDoctor *polyphony *waveforms] / [BuildDoctor *polyphony * * * *waveforms]]

[[BuildDoctor *polyphony *Doctor *DoctorCB *volume *waveforms]
	[Moonbase *Doctor *DoctorCB Doctor *volume] [moonbase *DoctorCB]
	[parameter_block *pitch "index"] [gateway *pitchsens] [*pitchsens *pitch]
	[parameter_block *modulation "index"] [gateway *vibrato] [*vibrato *modulation]
	[lfo *lfo] [*lfo "vibrato" *vibrato]
	[stereo_pan *pan] [delay *delay] [drywet *drywet] [volume *volume]
	[ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[*pan "pan" *lfo "pan"]
	[Insert *volume *Doctor core]
	[Insert *pan *Doctor core]
	[InsertPB *pitch *Doctor core pitch]
	[InsertPB *modulation *Doctor core modulation]
	[Insert *drywet *Doctor core delay]
	[Insert *delay *Doctor core delay]
	[Insert *lfo *Doctor lfo]
	[InsertFor *Doctor lfo vibrato]
	[InsertFor *Doctor lfo tremolo]
	[InsertFor *Doctor lfo wahwah]
	[InsertFor *Doctor lfo pan]
	[REPEAT *polyphony [BuildDoctorPart *Doctor *DoctorCB *pan *pitchsens *lfo *waveforms]]
	[Insert *pitchsens *Doctor sens pitch]
	[Insert *vibrato *Doctor sens modulation]
	[InsertController 1 *Doctor core modulation]
	[InsertController 5 *Doctor portamento time]
	[InsertController 7 *Doctor core volume]
	[InsertController 10 -64 *Doctor core pan]
	;[InsertController 16]
	;[InsertController 17]
	[InsertController 65 *Doctor portamento porta]
	[InsertController 71 *Doctor filter resonance]
	[InsertController 72 *Doctor adsr release]
	[InsertController 73 *Doctor adsr attack]
	[InsertController 74 -64 *Doctor filter freq]
	[InsertController 75 *Doctor adsr decay]
	[InsertController 76 -128 *Doctor adsr sustain]
	[InsertController 77 -64 *Doctor core delay balance]
	[InsertController 78 -64 *Doctor lfo speed]
	[InsertController 79 *Doctor lfo wahwah]
	[InsertController 128 -64 *Doctor core pitch]
	;;==============================
	;; INVISIBLE....
	[InsertController 64 *Doctor portamento hold]
	[InsertController 84 *Doctor portamento legato]
	;[InsertController 85 *Doctor core auto]
	;[InsertController 80 *Doctor arpeggiator active]
	;[InsertController 66 *Doctor arpeggiator hold]
	[Lunar -8192 *Doctor core delay balance]
	[Lunar 512 *Doctor sens pitch]
	[Lunar 256 *Doctor sens modulation]
	[Lunar 16384 *Doctor sens index]
	[Lunar 1792 *Doctor lfo speed]
]

[[BuildDoctorPart *Doctor *cb *line *pitch *lfo *waveforms]
	[trigger *trigger] [*cb *trigger]
	[adsr *adsr]
		[*adsr "trigger" *trigger "delay1"]
	[sampler_operator *sam *waveforms]
		[*sam "trigger" *trigger "delay1"]
		[*sam "freq" *pitch]
		[*sam "freq" *lfo "vibrato"]
		[gateway *freqsens] [*freqsens *trigger] [*sam "freq" *freqsens]
		[gateway *indexsens] [*indexsens *trigger] [*sam "index" *indexsens]
	[control *busy] [*busy "enter" *adsr "busy"] [*busy "gateway" *sam "busy"] [*trigger "busy" *busy]
	[filter *left_filter] [filter *right_filter]
		[*left_filter "enter" *sam "left"]
		[*left_filter "gain" *adsr]
		[*left_filter "freq" *lfo "wahwah"]
		[*left_filter "amp" *lfo "tremolo"]
		[*right_filter "enter" *sam "right"]
		[*right_filter "gain" *adsr]
		[*right_filter "freq" *lfo "wahwah"]
		[*right_filter "amp" *lfo "tremolo"]
	[sensitivity *velocity] [*velocity "signal" *trigger "velocity"] [*left_filter "amp" *velocity] [*right_filter "amp" *velocity]
	[*line "left" *left_filter] [*line "right" *right_filter]
	[Insert *sam *Doctor vco]
	[Insert *left_filter *Doctor filter]
	[Insert *right_filter *Doctor filter]
	[Insert *adsr *Doctor adsr]
	[Insert *freqsens *Doctor sens freq]
	[Insert *indexsens *Doctor sens index]
	[Insert *velocity *Doctor sens velocity]
	[Insert *trigger *Doctor portamento]
]

end .
