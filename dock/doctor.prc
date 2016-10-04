
import studio
import lunar

program doctor [Doctor BuildDoctor BuildDoctorPart cs_waveforms]

;[[BuildDoctor *polyphony]
;	[TRY
;		[wave *waveforms
;			"Doctor/snare_h.wav"
;			"Doctor/snare_m.wav"
;			"Doctor/snare_l.wav"
;			"Doctor/snare_roll.wav"
;			"Doctor/snare_hit_roll.wav"
;			"Doctor/snare_soft_roll.wav"
;			"Doctor/snare_loop.wav"
[[cs_waveforms *waveforms]
	[TRY
		[wave *waveforms
			"Doctor/surdo_mute.wav"
			"Doctor/surdo_open.wav"
			"Doctor/hi_q.wav"
			"Doctor/whip_slap.wav"
			"Doctor/scratch_push.wav"
			"Doctor/scratch_pull.wav"
			"Doctor/finger_snap.wav"
			"Doctor/click_noise.wav"
			"Doctor/metronome_click.wav"
			"Doctor/metronome_bell.wav"
			"Doctor/seq_click_l.wav"
			"Doctor/seq_click_h.wav"
			"Doctor/brush_tap.wav"
			"Doctor/brush_swirl_l.wav"
			"Doctor/brush_slap.wav"
			"Doctor/brush_swirl_h.wav"
			"Doctor/snare_roll.wav"
			"Doctor/castanet.wav"
			"Doctor/snare_l.wav"
			"Doctor/sticks.wav"
			"Doctor/bass_drum_l.wav"
			"Doctor/open_rim_shot.wav"
			"Doctor/bass_drum_m.wav"
			"Doctor/bass_drum_h.wav"
			"Doctor/side_stick.wav"
			"Doctor/snare_m.wav"
			"Doctor/hand_clap.wav"
			"Doctor/snare_h.wav"
			"Doctor/floor_tom_l.wav"
			"Doctor/hi_hat_closed.wav"
			"Doctor/floor_tom_h.wav"
			"Doctor/hi_hat_pedal.wav"
			"Doctor/low_tom.wav"
			"Doctor/hi_hat_open.wav"
			"Doctor/mid_tom_l.wav"
			"Doctor/mid_tom_h.wav"
			"Doctor/crash_cymbal_1.wav"
			"Doctor/high_tom.wav"
			"Doctor/ride_cymbal_1.wav"
			"Doctor/chinese_cymbal.wav"
			"Doctor/ride_cymbal_cup.wav"
			"Doctor/tambourine.wav"
			"Doctor/splash_cymbal.wav"
			"Doctor/cowbell.wav"
			"Doctor/crash_cymbal_2.wav"
			"Doctor/vibraslap.wav"
			"Doctor/ride_cymbal_2.wav"
			"Doctor/bongo_h.wav"
			"Doctor/bongo_l.wav"
			"Doctor/conga_h_mute.wav"
			"Doctor/conga_h_open.wav"
			"Doctor/conga_l.wav"
			"Doctor/timbale_h.wav"
			"Doctor/timbale_l.wav"
			"Doctor/agogo_h.wav"
			"Doctor/agogo_l.wav"
			"Doctor/cabasa.wav"
			"Doctor/maracas.wav"
			"Doctor/samba_whistle_h.wav"
			"Doctor/samba_whistle_l.wav"
			"Doctor/guiro_short.wav"
			"Doctor/guiro_long.wav"
			"Doctor/claves.wav"
			"Doctor/wood_block_h.wav"
			"Doctor/wood_block_l.wav"
			"Doctor/cuica_mute.wav"
			"Doctor/cuica_open.wav"
			"Doctor/triangle_mute.wav"
			"Doctor/triangle_open.wav"
			"Doctor/shaker.wav"
			"Doctor/jingle_bell.wav"
			"Doctor/bell_tree.wav"
		]
	]
]

[[BuildDoctor *polyphony] / [cs_waveforms *waveforms] / [BuildDoctor *polyphony *waveforms]]
[[BuildDoctor *polyphony *waveforms] / [BuildDoctor *polyphony * * * *waveforms]]

[[BuildDoctor *polyphony *Doctor *DoctorCB *volume *waveforms]
	[Moonbase *Doctor *DoctorCB Doctor *volume] [moonbase *DoctorCB]
	[parameter_block *pitch "index"] [gateway *pitchsens] [*pitchsens *pitch]
	[parameter_block *modulation "index"] [gateway *vibrato] [*vibrato *modulation]
	[lfo *lfo] [*lfo "vibrato" *vibrato]
	[stereo_pan *pan] [delay *delay] [drywet *drywet] [volume *volume] [stereo_chorus *chorus]
	[*pan "pan" *lfo "pan"]
	[ConnectStereo *pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[Insert *volume *Doctor core]
	[Insert *pan *Doctor core]
	[InsertPB *pitch *Doctor core pitch]
	[InsertPB *modulation *Doctor core modulation]
	[Insert *chorus *Doctor core chorus]
	[Insert *drywet *Doctor core delay]
	[Insert *delay *Doctor core delay]
	[Insert *lfo *Doctor lfo]
	[InsertFor *Doctor lfo vibrato]
	[InsertFor *Doctor lfo tremolo]
	[InsertFor *Doctor lfo wahwah]
	[InsertFor *Doctor lfo pan]
	[REPEAT *polyphony [BuildDoctorPart *Doctor *DoctorCB *chorus *pitchsens *lfo *waveforms]]
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
	[Lunar 16256 *Doctor sens velocity BP]
	[Lunar -64 *Doctor sens velocity left]
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

