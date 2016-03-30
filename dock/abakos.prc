
import lunar

program abakos [BuildAbakos BuildAbakosPart]

[[BuildAbakos *polyphony] / [BuildAbakos *polyphony * * *]]
[[BuildAbakos *polyphony *Abakos *AbakosCB *volume]
	[Moonbase *Abakos *AbakosCB Abakos *volume]
	[moonbase *base] [key_map *key_map]
	[arpeggiator *AbakosCB *base]
	[lfo *lfo]
	[volume *volume] [drywet *drywet] [delay *delay] [stereo_pan *pan] [stereo_chorus *chorus]
	[ConnectStereo *pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[parameter_block *pitch "index"] [gateway *pitchfreq] [*pitchfreq *pitch]
	[parameter_block *modulation "index"] [gateway *vibrato] [*vibrato *modulation] [*lfo "vibrato" *vibrato]
	[Insert *volume *Abakos core]
	[Insert *pan *Abakos core]
	[InsertPB *pitch *Abakos core pitch]
	[InsertPB *modulation *Abakos core modulation]
	[Insert *pitchfreq *Abakos core sens pitch]
	[Insert *vibrato *Abakos core sens vibrato]
	[Insert *chorus *Abakos core chorus]
	[Insert *drywet *Abakos core delay]
	[Insert *delay *Abakos core delay]
	[Insert *AbakosCB *Abakos arpeggiator]
	[Insert *lfo *Abakos lfo]
	[InsertFor *Abakos lfo vibrato]
	[InsertFor *Abakos lfo tremolo]
	[InsertFor *Abakos lfo wahwah]
	[REPEAT *polyphony [BuildAbakosPart *Abakos *AbakosCB *chorus *key_map *lfo *pitchfreq]]
	[InsertBlock *key_map *Abakos key_map]
	[InsertController 1 *Abakos core modulation]
	[InsertController 7 *Abakos core volume]
	[InsertController 10 -64 *Abakos core pan]
	[InsertController 5 *Abakos portamento time]
	;[InsertController 16 -64 *Phobos core X]
	;[InsertController 17 -64 *Phobos core Y]
	[InsertController 65 *Abakos portamento porta]
	[InsertController 74 -64 *Abakos filter freq]
	[InsertController 71 *Abakos filter resonance]
	[InsertController 77 -64 *Abakos core delay balance]
	[InsertController 78 -64 *Abakos lfo speed]
	[InsertController 73 *Abakos adsr 1 attack]
	[InsertController 75 *Abakos adsr 1 decay]
	[InsertController 76 -128 *Abakos adsr 1 sustain]
	[InsertController 72 *Abakos adsr 1 release]
	[InsertController 79 *Abakos lfo wahwah]
	[InsertController 128 -64 *Abakos core pitch]
	; INVISIBLE....
	[InsertController 64 *Abakos portamento hold]
	[InsertController 84 *Abakos portamento legato]
	;[InsertController 85 *Phobos core auto]
	[InsertController 80 *Abakos arpeggiator active]
	[InsertController 66 *Abakos arpeggiator hold]

	[Lunar -16384 *Abakos noise amp]
	[Lunar -16384 *Abakos vco 2 amp]
	[Lunar -8192 *Abakos core delay balance]
	[Lunar 16384 *Abakos vco 1 sens key]
	[Lunar 16384 *Abakos vco 2 sens key]
]

[[BuildAbakosPart *Abakos *cb *mixer *key_map *lfo *pitchfreq]
	[trigger *trigger *key_map] [*cb *trigger]
	[adsr *adsr] [eg *adsr2]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[vco *vco1] [vco *vco2] [noise_operator *noise] [filter *filter]
	[control *ring] [*ring *vco1] [*ring "gateway" *vco2] [gateway *ringmod] [*ringmod *ring]
	[*mixer *filter] [*filter *vco1] [*filter *vco2] [*filter *ringmod] [*filter *noise]
	[gateway *follow1] [gateway *follow2] [gateway *followf]
	[*follow1 "enter" *trigger "key"] [*follow2 "enter" *trigger "key"] [*followf "enter" *trigger "key"]
	[gateway *adsr2sens1] [gateway *adsr2sens2] [gateway *adsr2sensf]
	[*adsr2sens1 *adsr2] [*adsr2sens2 *adsr2] [*adsr2sensf *adsr2]
	[*vco1 "freq" *follow1] [*vco1 "freq" *adsr2sens1] [*vco1 "freq" *lfo "vibrato"] [*vco1 "freq" *pitchfreq]
	[*vco2 "freq" *follow2] [*vco2 "freq" *adsr2sens2] [*vco2 "freq" *lfo "vibrato"] [*vco2 "freq" *pitchfreq]
	[*filter "freq" *followf] [*filter "freq" *adsr2sensf] [*filter "freq" *lfo "wahwah"]
	[*filter "gain" *adsr] [*filter "amp" *lfo "tremolo"]
	[Insert *vco1 *Abakos vco 1]
	[Insert *follow1 *Abakos vco 1 sens key]
	[Insert *adsr2sens1 *Abakos vco 1 sens adsr]
	[Insert *vco2 *Abakos vco 2]
	[Insert *follow2 *Abakos vco 2 sens key]
	[Insert *adsr2sens2 *Abakos vco 2 sens adsr]
	[Insert *ringmod *Abakos vco ringmod]
	[Insert *noise *Abakos noise]
	[Insert *filter *Abakos filter]
	[Insert *followf *Abakos filter sens key]
	[Insert *adsr2sensf *Abakos filter sens adsr]
	[Insert *adsr *Abakos adsr 1]
	[Insert *adsr2 *Abakos adsr 2]
	[Insert *trigger *Abakos portamento]
]

end .
