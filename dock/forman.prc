
import studio
import lunar

program forman [BuildForman BuildFormanPart]

[[BuildForman *polyphony] / [BuildForman *polyphony * * *]]
[[BuildForman *polyphony *Forman *formancb *volume]
	[Moonbase *Forman *formancb Forman *volume]
	[moonbase *formancb]
	[lfo *lfo1] [lfo *lfo2]
	[auto_data *XData] [parameter_block *X "index"] [*XData *X]
	[auto_data *YData] [parameter_block *Y "index"] [*YData *Y]
	[parameter_block *AutoCtrl "index"] [*XData "control" *AutoCtrl] [*YData "control" *AutoCtrl]
	[parameter_block *pitch "index"]
	[gateway *pitchvco] [*pitchvco *pitch] [gateway *pitchfilter] [*pitchfilter *pitch]
	[parameter_block *modulation "index"]
		[gateway *vibrato] [*vibrato *modulation] [*lfo1 "vibrato" *vibrato]
		[gateway *wahwah] [*wahwah *modulation] [*lfo1 "wahwah" *wahwah]
	[volume *volume] [drywet *drywet] [delay *delay] [stereo_pan *pan] [stereo_chorus *chorus]
	[*pan "pan" *lfo1 "pan"] [*pan "pan" *lfo2 "pan"]
	[ConnectStereo *pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[gateway *lfo1x] [gateway *lfo1y] [gateway *lfo2x] [gateway *lfo2y]
	[*lfo1x *lfo1] [*lfo1y *lfo1] [*lfo2x *lfo2] [*lfo2y *lfo2]
	[Insert *volume *Forman core]
	[Insert *pan *Forman core]
	[Insert *chorus *Forman core chorus]
	[Insert *drywet *Forman core delay]
	[Insert *delay *Forman core delay]
	[InsertPB *X *Forman core X]
	[InsertPB *Y *Forman core Y]
	[InsertPB *pitch *Forman core pitch]
	[InsertPB *modulation *Forman core modulation]
	[Insert *vibrato *Forman core sens vibrato]
	[Insert *wahwah *Forman core sens wahwah]
	[InsertPB *AutoCtrl *Forman core auto]
	[Insert *lfo1 *Forman lfo 1]
	[Insert *lfo1x *Forman lfo 1 vector X]
	[Insert *lfo1y *Forman lfo 1 vector Y]
	[InsertFor *Forman lfo 1 vibrato]
	[InsertFor *Forman lfo 1 wahwah]
	[InsertFor *Forman lfo 1 pan]
	[Insert *lfo2 *Forman lfo 2]
	[Insert *lfo2x *Forman lfo 2 vector X]
	[Insert *lfo2y *Forman lfo 2 vector Y]
	[InsertFor *Forman lfo 2 vibrato]
	[InsertFor *Forman lfo 2 wahwah]
	[InsertFor *Forman lfo 2 pan]
	[InsertFor *Forman lfo 2 tremolo]
	[REPEAT *polyphony [BuildFormanPart *Forman *formancb *chorus *XData *YData *pitchvco *pitchfilter *lfo1 *lfo2 *lfo1x *lfo1y *lfo2x *lfo2y]]
	[Insert *pitchvco *Forman vco sens pitch]
	[Insert *pitchfilter *Forman filter sens pitch]
	[InsertController 1 *Forman core modulation]
	[InsertController 7 *Forman core volume]
	[InsertController 5 *Forman portamento time]
	[InsertController 10 -64 *Forman core pan]
	[InsertController 16 *Forman core X]
	[InsertController 17 *Forman core Y]
	[InsertController 65 *Forman portamento porta]
	[InsertController 71 *Forman filter resonance]
	[InsertController 72 *Forman adsr amp release]
	[InsertController 73 *Forman adsr amp attack]
	[InsertController 74 -64 *Forman filter freq]
	[InsertController 75 *Forman adsr amp decay]
	[InsertController 76 -128 *Forman adsr amp sustain]
	[InsertController 77 -64 *Forman core delay balance]
	[InsertController 78 -64 *Forman lfo 1 speed]
	[InsertController 79 *Forman lfo 2 vibrato]
	[InsertController 128 -64 *Forman core pitch]
	; INVISIBLE....
	[InsertController 64 *Forman portamento hold]
	[InsertController 84 *Forman portamento legato]
	[InsertController 85 *Forman core auto]
	;[InsertController 80 *Forman arpeggiator active]
	;[InsertController 66 *Forman arpeggiator hold]

	[Lunar -16384 *Forman noise amp]
	[Lunar -8192 *Forman core delay balance]
	[Lunar 1 *Forman vco wave]
	[Lunar 2048 *Forman filter resonance]
	[Lunar 512 *Forman vco sens pitch]
	[Lunar 256 *Forman core sens vibrato]
]

[[BuildFormanPart *Forman *cb *line *XData *YData *pitchvco *pitchfilter *lfo1 *lfo2 *lfo1x *lfo1y *lfo2x *lfo2y]
	[trigger *trigger] [*cb *trigger]
	[adsr *adsr]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"] [*lfo1 "trigger" *adsr "busy"] [*lfo2 "trigger" *adsr "busy"]
	[vco *vco] [noise_operator *noise]
	[*vco "freq" *trigger "key"]
	[*vco "freq" *pitchvco]
	[filter *filter]
	;============
	[auto *X *XData] [*X "trigger" *trigger "trigger"] [*XData "trigger" *trigger "trigger"]
	[auto *Y *YData] [*Y "trigger" *trigger "trigger"] [*YData "trigger" *trigger "trigger"]
	;============
	[formant_filter *ff1]
		[morph *ff1freq] [*ff1freq "x" *X] [*ff1freq "y" *Y] [*ff1 "freq" *ff1freq]
		[morph *ff1reso] [*ff1reso "x" *X] [*ff1reso "y" *Y] [*ff1 "resonance" *ff1reso]
		[morph *ff1q] [*ff1q "x" *X] [*ff1q "y" *Y] [*ff1 "q" *ff1q]
		[*ff1freq "x" *lfo1x] [*ff1freq "y" *lfo1y] [*ff1freq "x" *lfo2x] [*ff1freq "y" *lfo2y]
		[*ff1reso "x" *lfo1x] [*ff1reso "y" *lfo1y] [*ff1reso "x" *lfo2x] [*ff1reso "y" *lfo2y]
		[*ff1q "x" *lfo1x] [*ff1q "y" *lfo1y] [*ff1q "x" *lfo2x] [*ff1q "y" *lfo2y]
		[*ff1 *vco] [*ff1 *noise] [*line *ff1]
		[*ff1 "gain" *adsr] [*ff1 "amp" *lfo2 "tremolo"]
	[formant_filter *ff2]
		[morph *ff2freq] [*ff2freq "x" *X] [*ff2freq "y" *Y] [*ff2 "freq" *ff2freq]
		[morph *ff2reso] [*ff2reso "x" *X] [*ff2reso "y" *Y] [*ff2 "resonance" *ff2reso]
		[morph *ff2q] [*ff2q "x" *X] [*ff2q "y" *Y] [*ff2 "q" *ff2q]
		[*ff2freq "x" *lfo1x] [*ff2freq "y" *lfo1y] [*ff2freq "x" *lfo2x] [*ff2freq "y" *lfo2y]
		[*ff2reso "x" *lfo1x] [*ff2reso "y" *lfo1y] [*ff2reso "x" *lfo2x] [*ff2reso "y" *lfo2y]
		[*ff2q "x" *lfo1x] [*ff2q "y" *lfo1y] [*ff2q "x" *lfo2x] [*ff2q "y" *lfo2y]
		[*ff2 *vco] [*ff2 *noise] [*line *ff2]
		[*ff2 "gain" *adsr] [*ff2 "amp" *lfo2 "tremolo"]
	[formant_filter *ff3]
		[morph *ff3freq] [*ff3freq "x" *X] [*ff3freq "y" *Y] [*ff3 "freq" *ff3freq]
		[morph *ff3reso] [*ff3reso "x" *X] [*ff3reso "y" *Y] [*ff3 "resonance" *ff3reso]
		[morph *ff3q] [*ff3q "x" *X] [*ff3q "y" *Y] [*ff3 "q" *ff3q]
		[*ff3freq "x" *lfo1x] [*ff3freq "y" *lfo1y] [*ff3freq "x" *lfo2x] [*ff3freq "y" *lfo2y]
		[*ff3reso "x" *lfo1x] [*ff3reso "y" *lfo1y] [*ff3reso "x" *lfo2x] [*ff3reso "y" *lfo2y]
		[*ff3q "x" *lfo1x] [*ff3q "y" *lfo1y] [*ff3q "x" *lfo2x] [*ff3q "y" *lfo2y]
		[*ff3 *vco] [*ff3 *noise] [*line *ff3]
		[*ff3 "gain" *adsr] [*ff3 "amp" *lfo2 "tremolo"]
	[formant_filter *ff4]
		[morph *ff4freq] [*ff4freq "x" *X] [*ff4freq "y" *Y] [*ff4 "freq" *ff4freq]
		[morph *ff4reso] [*ff4reso "x" *X] [*ff4reso "y" *Y] [*ff4 "resonance" *ff4reso]
		[morph *ff4q] [*ff4q "x" *X] [*ff4q "y" *Y] [*ff4 "q" *ff4q]
		[*ff4freq "x" *lfo1x] [*ff4freq "y" *lfo1y] [*ff4freq "x" *lfo2x] [*ff4freq "y" *lfo2y]
		[*ff4reso "x" *lfo1x] [*ff4reso "y" *lfo1y] [*ff4reso "x" *lfo2x] [*ff4reso "y" *lfo2y]
		[*ff4q "x" *lfo1x] [*ff4q "y" *lfo1y] [*ff4q "x" *lfo2x] [*ff4q "y" *lfo2y]
		[*ff4 *vco] [*ff4 *noise] [*line *ff4]
		[*ff4 "gain" *adsr] [*ff4 "amp" *lfo2 "tremolo"]
	;============
	[*filter *vco] [*filter *noise] [*line *filter]
	[*filter "gain" *adsr]
	[*filter "freq" *pitchfilter]
	[*filter "amp" *lfo2 "tremolo"]
	[eg *freqeg] [gateway *vcoegsens] [gateway *filteregsens] [*vcoegsens *freqeg] [*filteregsens *freqeg] [*freqeg "trigger" *trigger "trigger"]
	[*vco "freq" *vcoegsens] [*filter "freq" *filteregsens]
	[*vco "freq" *lfo1 "vibrato"] [*filter "freq" *lfo1 "wahwah"]
	[*vco "freq" *lfo2 "vibrato"] [*filter "freq" *lfo2 "wahwah"]
	[sensitivity *velocity] [*velocity "signal" *trigger "velocity"]
		[*ff1 "amp" *velocity] [*ff2 "amp" *velocity] [*ff3 "amp" *velocity] [*ff4 "amp" *velocity] [*filter "amp" *velocity]
	[Insert *vco *Forman vco]
	[Insert *vcoegsens *Forman vco sens adsr]
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
	[Insert *filteregsens *Forman filter sens adsr]
	[Insert *adsr *Forman adsr amp]
	[Insert *freqeg *Forman adsr freq]
	[Insert *velocity *Forman velocity]
	[Insert *trigger *Forman portamento]
	[AddModule *X *Forman] [AddModule *Y *Forman]
]

end .
