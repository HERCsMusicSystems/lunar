import studio
import lunar

program vonforman [BuildVonForman BuildVonFormanPart VF LGV LGT LGP STK STV AEG FEGS MORPH]

[[LGV *gate *lfo] [gateway *gate] [*gate "enter" *lfo "vibrato"]]
[[LGT *gate *lfo] [gateway *gate] [*gate "enter" *lfo "tremolo"]]
[[LGP *gate *pb] [gateway *gate] [*gate *pb]]
[[STK *sens *module *input *trigger] [sensitivity *sens] [*sens "signal" *trigger "key"] [*module *input *sens]]
[[STV *sens *module *input *trigger] [sensitivity *sens] [*sens "signal" *trigger "velocity"] [*module *input *sens]]
[[AEG *eg *fm *input *trigger *egscal]
	[eg *eg] [*eg "trigger" *trigger "trigger"] [*fm *input *eg]
	[sensitivity *egscal] [*egscal *trigger] [*eg "time1" *egscal] [*eg "time2" *egscal] [*eg "time3" *egscal] [*eg "time4" *egscal]
]
[[FEGS *sens *eg *fm *input] [gateway *sens] [*sens *eg] [*fm *input *sens]]
[[MORPH *morph *ff *name *x *y *lfo1x *lfo1y *lfo2x *lfo2y]
	[morph *morph]
	[*morph "x" *x] [*morph "y" *y]
	[*morph "x" *lfo1x] [*morph "y" *lfo1y] [*morph "x" *lfo2x] [*morph "y" *lfo2y]
	[*ff *name *morph]
]

[[BuildVonForman *polyphony] / [BuildVonForman *polyphony * * *]]

[[BuildVonForman *polyphony *VonForman *VonFormanCB *volume]
	[Moonbase *VonForman *VonFormanCB VF *volume]
	[moonbase *CB] [key_map *key_map] [arpeggiator *VonFormanCB *CB]
	[auto_data *XData] [parameter_block *X "index"] [*XData *X]
	[auto_data *YData] [parameter_block *Y "index"] [*YData *Y]
	[parameter_block *pitch "index"]
	[LGP *pitchfreq1 *pitch] [LGP *pitchfreq2 *pitch] [LGP *pitchfreq3 *pitch] [LGP *pitchfreq4 *pitch] [LGP *pitchfreqf *pitch]
	[parameter_block *modulation "index"]
	[lfo *lfo1] [lfo *lfo2]
		[gateway *vibrato] [*vibrato *modulation] [*lfo1 "vibrato" *vibrato]
		[gateway *wahwah] [*wahwah *modulation] [*lfo1 "wahwah" *wahwah]
		[gateway *lfo1x] [gateway *lfo1y] [gateway *lfo2x] [gateway *lfo2y]
		[*lfo1x *lfo1] [*lfo1y *lfo1] [*lfo2x *lfo2] [*lfo2y *lfo2]
	[LGV *lfo1freq1 *lfo1] [LGV *lfo1freq2 *lfo1] [LGV *lfo1freq3 *lfo1] [LGV *lfo1freq4 *lfo1]
	[LGV *lfo2freq1 *lfo2] [LGV *lfo2freq2 *lfo2] [LGV *lfo2freq3 *lfo2] [LGV *lfo2freq4 *lfo2]
	[LGT *lfo1amp1 *lfo1] [LGT *lfo1amp2 *lfo1] [LGT *lfo1amp3 *lfo1] [LGT *lfo1amp4 *lfo1]
	[LGT *lfo2amp1 *lfo2] [LGT *lfo2amp2 *lfo2] [LGT *lfo2amp3 *lfo2] [LGT *lfo2amp4 *lfo2]
	[stereo_pan *pan] [delay *delay] [drywet *drywet] [volume *volume] [stereo_chorus *chorus]
	[*pan "pan" *lfo1 "pan"] [*pan "pan" *lfo2 "pan"]
	[ConnectStereo *pan *chorus] [ConnectStereo *delay *pan] [ConnectDryWet *drywet *pan *delay] [ConnectStereo *volume *drywet]
	[Insert *volume *VonForman core]
	[Insert *pan *VonForman core]
	[Insert *chorus *VonForman core chorus]
	[Insert *drywet *VonForman core delay]
	[Insert *delay *VonForman core delay]
	[InsertPB *X *VonForman core X]
	[InsertPB *Y *VonForman core Y]
	[Insert *XData *VonForman core auto]
	[Insert *YData *VonForman core auto]
	[InsertPB *pitch *VonForman core pitch]
	[InsertPB *modulation *VonForman core modulation]
	[Insert *vibrato *VonForman core sens vibrato]
	[Insert *wahwah *VonForman core sens wahwah]
	[Insert *VonFormanCB *VonForman arpeggiator]
	[Insert *lfo1 *VonForman lfo 1]
	[Insert *lfo1x *VonForman lfo 1 vector X]
	[Insert *lfo1y *VonForman lfo 1 vector Y]
	[InsertFor *VonForman lfo 1 vibrato]
	[InsertFor *VonForman lfo 1 tremolo]
	[InsertFor *VonForman lfo 1 wahwah]
	[InsertFor *VonForman lfo 1 pan]
	[Insert *lfo2 *VonForman lfo 2]
	[Insert *lfo2x *VonForman lfo 2 vector X]
	[Insert *lfo2y *VonForman lfo 2 vector Y]
	[InsertFor *VonForman lfo 2 vibrato]
	[InsertFor *VonForman lfo 2 tremolo]
	[InsertFor *VonForman lfo 2 wahwah]
	[InsertFor *VonForman lfo 2 pan]
	;==========================================
	[REPEAT *polyphony [BuildVonFormanPart *VonForman *VonFormanCB *chorus *key_map
						*lfo1 *lfo2 *XData *YData *lfo1x *lfo1y *lfo2x *lfo2y
						*pitchfreq1 *pitchfreq2 *pitchfreq3 *pitchfreq4 *pitchfreqf
						*lfo1freq1 *lfo1freq2 *lfo1freq3 *lfo1freq4
						*lfo2freq1 *lfo2freq2 *lfo2freq3 *lfo2freq4
						*lfo1amp1 *lfo1amp2 *lfo1amp3 *lfo1amp4
						*lfo2amp1 *lfo2amp2 *lfo2amp3 *lfo2amp4
	]]
	[Insert *pitchfreq1 *VonForman vco 1 sens freq pitch]
	[Insert *pitchfreq2 *VonForman vco 2 sens freq pitch]
	[Insert *pitchfreq3 *VonForman vco 3 sens freq pitch]
	[Insert *pitchfreq4 *VonForman vco 4 sens freq pitch]
	[Insert *lfo1freq1 *VonForman vco 1 sens freq lfo 1]
	[Insert *lfo1freq2 *VonForman vco 2 sens freq lfo 1]
	[Insert *lfo1freq3 *VonForman vco 3 sens freq lfo 1]
	[Insert *lfo1freq4 *VonForman vco 4 sens freq lfo 1]
	[Insert *lfo2freq1 *VonForman vco 1 sens freq lfo 2]
	[Insert *lfo2freq2 *VonForman vco 2 sens freq lfo 2]
	[Insert *lfo2freq3 *VonForman vco 3 sens freq lfo 2]
	[Insert *lfo2freq4 *VonForman vco 4 sens freq lfo 2]
	[Insert *lfo1amp1 *VonForman vco 1 sens amp lfo 1]
	[Insert *lfo1amp2 *VonForman vco 2 sens amp lfo 1]
	[Insert *lfo1amp3 *VonForman vco 3 sens amp lfo 1]
	[Insert *lfo1amp4 *VonForman vco 4 sens amp lfo 1]
	[Insert *lfo2amp1 *VonForman vco 1 sens amp lfo 2]
	[Insert *lfo2amp2 *VonForman vco 2 sens amp lfo 2]
	[Insert *lfo2amp3 *VonForman vco 3 sens amp lfo 2]
	[Insert *lfo2amp4 *VonForman vco 4 sens amp lfo 2]
	[Insert *pitchfreqf *VonForman filter sens pitch]
	;==============================
	[InsertBlock *XData *VonForman vector X]
	[InsertBlock *YData *VonForman vector Y]
	[InsertBlock *key_map *VonForman key_map]
	;==============================
	[InsertController 1 *VonForman core modulation]
	[InsertController 5 *VonForman portamento time]
	[InsertController 7 *VonForman core volume]
	[InsertController 10 -64 *VonForman core pan]
	[InsertController 16 *VonForman core X]
	[InsertController 17 *VonForman core Y]
	[InsertController 65 *VonForman portamento porta]
	[InsertController 71 *VonForman filter resonance]
	[InsertController 72 *VonForman adsr amp release]
	[InsertController 73 *VonForman adsr amp attack]
	[InsertController 74 -64 *VonForman filter freq]
	[InsertController 75 *VonForman adsr amp decay]
	[InsertController 76 -128 *VonForman adsr amp sustain]
	[InsertController 77 -64 *VonForman core delay balance]
	[InsertController 78 -64 *VonForman lfo 1 speed]
	[InsertController 79 *VonForman lfo 1 wahwah]
	[InsertController 128 -64 *VonForman core pitch]
	;==============================
	; INVISIBLE....
	[InsertController 64 *VonForman portamento hold]
	[InsertController 84 *VonForman portamento legato]
	[InsertController 85 *VonForman core auto control]
	[InsertController 80 *VonForman arpeggiator active]
	[InsertController 66 *VonForman arpeggiator hold]
	;==============================
	[Lunar -16384 *VonForman vco noise amp]
	[Lunar -8192 *VonForman core delay balance]
	[Lunar 512 *VonForman vco 1 sens freq pitch]
	[Lunar 256 *VonForman core sens vibrato]
	[Lunar 16384 *VonForman vco 1 sens freq lfo 1]
	[Lunar 16384 *VonForman vco 2 sens freq lfo 1]
	[Lunar 16384 *VonForman vco 3 sens freq lfo 1]
	[Lunar 16384 *VonForman vco 4 sens freq lfo 1]
	[Lunar 128 *VonForman vco 1 sens freq key right]
	[Lunar 128 *VonForman vco 2 sens freq key right]
	[Lunar 128 *VonForman vco 3 sens freq key right]
	[Lunar 128 *VonForman vco 4 sens freq key right]
	[Lunar -128 *VonForman vco 1 sens freq key left]
	[Lunar -128 *VonForman vco 2 sens freq key left]
	[Lunar -128 *VonForman vco 3 sens freq key left]
	[Lunar -128 *VonForman vco 4 sens freq key left]
	[Lunar 1792 *VonForman lfo 1 speed]
	[Lunar 12800 *VonForman vco 1 sens amp velocity BP]
	[Lunar 12800 *VonForman vco 2 sens amp velocity BP]
	[Lunar 12800 *VonForman vco 3 sens amp velocity BP]
	[Lunar 12800 *VonForman vco 4 sens amp velocity BP]
	[Lunar 12800 *VonForman core sens velocity BP]
	[Lunar -64 *VonForman core sens velocity left]
]

[[BuildVonFormanPart *VF *cb *line *key_map
						*lfo1 *lfo2 *XData *YData *lfo1x *lfo1y *lfo2x *lfo2y
						*pitchfreq1 *pitchfreq2 *pitchfreq3 *pitchfreq4 *pitchfreqf
						*lfo1freq1 *lfo1freq2 *lfo1freq3 *lfo1freq4
						*lfo2freq1 *lfo2freq2 *lfo2freq3 *lfo2freq4
						*lfo1amp1 *lfo1amp2 *lfo1amp3 *lfo1amp4
						*lfo2amp1 *lfo2amp2 *lfo2amp3 *lfo2amp4
	]
	;========== TRIGGER ==============
	[trigger *trigger *key_map] [*cb *trigger] [*lfo1 "trigger" *trigger "trigger"] [*lfo2 "trigger" *trigger "trigger"]
	;========== ADSR =================
	[adsr *adsr] [*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[*lfo1 "trigger" *adsr "busy"] [*lfo2 "trigger" *adsr "busy"]
	[sensitivity *egscal] [*egscal *trigger] [*adsr "attack" *egscal] [*adsr "decay" *egscal] [*adsr "release" *egscal]
	;========== FREQ EG
	[eg *freqeg] [*freqeg "trigger" *trigger "trigger"]
	[sensitivity *fegscal] [*fegscal *trigger] [*freqeg "time1" *fegscal] [*freqeg "time2" *fegscal] [*freqeg "time3" *fegscal] [*freqeg "time4" *fegscal]
	;========== NOISE ================
	[noise_operator *noise] [eg *noise_eg] [*noise_eg "trigger" *trigger "trigger"] [*noise "amp" *noise_eg]
	;========== FM VCO ===============
	[fm4 *fm]
	[STK *freq1 *fm "freq1" *trigger] [STK *freq2 *fm "freq2" *trigger] [STK *freq3 *fm "freq3" *trigger] [STK *freq4 *fm "freq4" *trigger]
	[STK *amp1 *fm "amp1" *trigger] [STK *amp2 *fm "amp2" *trigger] [STK *amp3 *fm "amp3" *trigger] [STK *amp4 *fm "amp4" *trigger]
	[STV *velocity1 *fm "amp1" *trigger] [STV *velocity2 *fm "amp2" *trigger] [STV *velocity3 *fm "amp3" *trigger] [STV *velocity4 *fm "amp4" *trigger]
	[AEG *ampeg1 *fm "amp1" *trigger *egscal1] [AEG *ampeg2 *fm "amp2" *trigger *egscal2] [AEG *ampeg3 *fm "amp3" *trigger *egscal3] [AEG *ampeg4 *fm "amp4" *trigger *egscal4]
	[FEGS *freqeg1 *freqeg *fm "freq1"] [FEGS *freqeg2 *freqeg *fm "freq2"] [FEGS *freqeg3 *freqeg *fm "freq3"] [FEGS *freqeg4 *freqeg *fm "freq4"]
	[*fm "freq1" *pitchfreq1] [*fm "freq2" *pitchfreq2] [*fm "freq3" *pitchfreq3] [*fm "freq4" *pitchfreq4]
	[*fm "freq1" *lfo1freq1] [*fm "freq2" *lfo1freq2] [*fm "freq3" *lfo1freq3] [*fm "freq4" *lfo1freq4]
	[*fm "freq1" *lfo2freq1] [*fm "freq2" *lfo2freq2] [*fm "freq3" *lfo2freq3] [*fm "freq4" *lfo2freq4]
	[*fm "amp1" *lfo1amp1] [*fm "amp2" *lfo1amp2] [*fm "amp3" *lfo1amp3] [*fm "amp4" *lfo1amp4]
	[*fm "amp1" *lfo2amp1] [*fm "amp2" *lfo2amp2] [*fm "amp3" *lfo2amp3] [*fm "amp4" *lfo2amp4]
	;========== FILTER ===============
	[filter *filter] [*filter "gain" *adsr] [*filter *noise] [*filter *fm]
	[STK *filter_key *filter "freq" *trigger]
	[FEGS *freqegf *freqeg *filter "freq"]
	[*filter "freq" *pitchfreqf]
	[*filter "freq" *lfo1 "wahwah"] [*filter "freq" *lfo2 "wahwah"]
	;========== AUTO VECTOR ==========
	[auto *X *XData] [*X "trigger" *trigger "trigger"] [*XData "trigger" *trigger "trigger"]
	[auto *Y *YData] [*Y "trigger" *trigger "trigger"] [*YData "trigger" *trigger "trigger"]
	;========== FORMANT FILTERS ======
	[formant_filter *ff1]
		[MORPH *ff1freq *ff1 "freq"      *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[MORPH *ff1reso *ff1 "resonance" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[MORPH *ff1q    *ff1 "q"         *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[*ff1 "gain" *adsr] [*ff1 *noise] [*ff1 *fm]
		[*ff1 "amp" *lfo1 "tremolo"] [*ff1 "amp" *lfo2 "tremolo"]
	[formant_filter *ff2]
		[MORPH *ff2freq *ff2 "freq"      *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[MORPH *ff2reso *ff2 "resonance" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[MORPH *ff2q    *ff2 "q"         *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[*ff2 "gain" *adsr] [*ff2 *noise] [*ff2 *fm]
		[*ff2 "amp" *lfo1 "tremolo"] [*ff2 "amp" *lfo2 "tremolo"]
	[formant_filter *ff3]
		[MORPH *ff3freq *ff3 "freq"      *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[MORPH *ff3reso *ff3 "resonance" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[MORPH *ff3q    *ff3 "q"         *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
		[*ff3 "gain" *adsr] [*ff3 *noise] [*ff3 *fm]
		[*ff3 "amp" *lfo1 "tremolo"] [*ff3 "amp" *lfo2 "tremolo"]
	;========== VELOCITY =============
	[sensitivity *velocity] [*velocity "signal" *trigger "velocity"]
		[*filter "amp" *velocity] [*ff1 "amp" *velocity] [*ff2 "amp" *velocity] [*ff3 "amp" *velocity]
	;========== LINE CONNECTION ======
	[*line *filter] [*line *ff1] [*line *ff2] [*line *ff3]
	;========== SUPERSTRUCTURE =======
	[Insert *fm *VF vco]
	[Insert *ampeg1 *VF vco 1 eg] [Insert *ampeg2 *VF vco 2 eg] [Insert *ampeg3 *VF vco 3 eg] [Insert *ampeg4 *VF vco 4 eg]
	[Insert *egscal1 *VF vco 1 eg egscal] [Insert *egscal2 *VF vco 2 eg egscal] [Insert *egscal3 *VF vco 3 eg egscal] [Insert *egscal4 *VF vco 4 eg egscal]
	[Insert *freq1 *VF vco 1 sens freq key] [Insert *freq2 *VF vco 2 sens freq key] [Insert *freq3 *VF vco 3 sens freq key] [Insert *freq4 *VF vco 4 sens freq key]
	[Insert *freqeg1 *VF vco 1 sens freq eg] [Insert *freqeg2 *VF vco 2 sens freq eg] [Insert *freqeg3 *VF vco 3 sens freq eg] [Insert *freqeg4 *VF vco 4 sens freq eg]
	[Insert *amp1 *VF vco 1 sens amp key] [Insert *amp2 *VF vco 2 sens amp key] [Insert *amp3 *VF vco 3 sens amp key] [Insert *amp4 *VF vco 4 sens amp key]
	[Insert *velocity1 *VF vco 1 sens amp velocity] [Insert *velocity2 *VF vco 2 sens amp velocity]
		[Insert *velocity3 *VF vco 3 sens amp velocity] [Insert *velocity4 *VF vco 4 sens amp velocity]
	[Insert *noise *VF vco noise]
	[Insert *noise_eg *VF vco noise]
	[Insert *filter *VF filter]
	[Insert *filter_key *VF filter sens key]
	[Insert *freqegf *VF filter sens eg]
	[Insert *ff1 *VF formant 1]
	[Insert *ff2 *VF formant 2]
	[Insert *ff3 *VF formant 3]
	[Insert *ff1freq *VF vector 1 freq] [Insert *ff1reso *VF vector 1 resonance] [Insert *ff1q *VF vector 1 Q]
	[Insert *ff2freq *VF vector 2 freq] [Insert *ff2reso *VF vector 2 resonance] [Insert *ff2q *VF vector 2 Q]
	[Insert *ff3freq *VF vector 3 freq] [Insert *ff3reso *VF vector 3 resonance] [Insert *ff3q *VF vector 3 Q]
	[Insert *adsr *VF adsr amp]
	[Insert *egscal *VF adsr amp egscal]
	[Insert *freqeg *VF adsr freq]
	[Insert *fegscal *VF adsr freq egscal]
	[Insert *trigger *VF portamento]
	[Insert *velocity *VF core sens velocity]
	[AddModule *X *VF] [AddModule *Y *VF]
]

private [LGV LGT LGP STK STV AEG FEGS MORPH]

end .
