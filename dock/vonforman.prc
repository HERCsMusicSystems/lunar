import studio
import lunar

program vonforman [BuildVonForman BuildVonFormanPart VF LGV LGT LGP STK STV AEG FEGS MORPH MORPHFM PBF STKF
					BuildOscillatorBlock BuildFilterBlock InsertOscillatorBlock InsertFilterBlock
					BuildOscillatorControllers InsertOscillatorControllers
					CheckOrder Preset FormantPreset
					]

[[PBF *pbf *fm *name] [parameter_block *pbf 0] [*fm *name *pbf]]
[[LGV *gate *lfo] [gateway *gate] [*gate "enter" *lfo "vibrato"]]
[[LGT *gate *lfo] [gateway *gate] [*gate "enter" *lfo "tremolo"]]
[[LGP *gate *pb] [gateway *gate] [*gate *pb]]
[[STK *sens *module *input *trigger] [sensitivity *sens] [*sens "signal" *trigger "key"] [*module *input *sens]]
[[STV *sens *module *input *trigger] [sensitivity *sens] [*sens "signal" *trigger "velocity"] [*module *input *sens]]
[[STKF *sens *module *input *pb] [sensitivity *sens] [*sens *pb] [*module *input *sens]]
[[AEG *eg *fm *input *trigger *egscal] [egscal *eg *egscal *trigger] [*fm *input *eg]]
[[FEGS *sens *eg *fm *input] [gateway *sens] [*sens *eg] [*fm *input *sens]]
[[MORPH *morph *ff *name *x *y *lfo1x *lfo1y *lfo2x *lfo2y]
	[morph *morph]
	[*morph "x" *x] [*morph "y" *y]
	[*morph "x" *lfo1x] [*morph "y" *lfo1y] [*morph "x" *lfo2x] [*morph "y" *lfo2y]
	[*ff *name *morph]
]
[[MORPHFM *morph *ff *name *x *y *lfo1x *lfo1y *lfo2x *lfo2y]
	[morph *morph 16384.0]
	[*morph "x" *x] [*morph "y" *y]
	[*morph "x" *lfo1x] [*morph "y" *lfo1y] [*morph "x" *lfo2x] [*morph "y" *lfo2y]
	[*ff *name *morph]
]

[[CheckOrder 66 6 6] /]
[[CheckOrder 64 6 4] /]
[[CheckOrder 46 4 6] /]
[[CheckOrder *  4 4]]

[[BuildOscillatorControllers 0 *pitch *modulation *lfo1 *lfo2 []] /]
[[BuildOscillatorControllers *order *pitch *modulation *lfo1 *lfo2 [[*pitchfreq *modbias *lfo1freq *lfo2freq *lfo1amp *lfo2amp] : *ctrls]]
	[LGP *pitchfreq *pitch] [sensitivity *modbias] [*modbias *modulation]
	[LGV *lfo1freq *lfo1] [LGV *lfo2freq *lfo2] [LGT *lfo1amp *lfo1] [LGT *lfo2amp *lfo2]
	[-- *order *next] / [BuildOscillatorControllers *next *pitch *modulation *lfo1 *lfo2 *ctrls]
]

[[InsertOscillatorControllers *index *VF []] /]
[[InsertOscillatorControllers *index *VF [[*pitchfreq *modbias *lfo1freq *lfo2freq *lfo1amp *lfo2amp] : *ctrls]]
	[Insert *pitchfreq *VF vco *index sens freq pitch]
	[Insert *modbias *VF vco *index sens amp modulation]
	[Insert *lfo1freq *VF vco *index sens freq lfo 1]
	[Insert *lfo2freq *VF vco *index sens freq lfo 2]
	[Insert *lfo1amp *VF vco *index sens amp lfo 1]
	[Insert *lfo2amp *VF vco *index sens amp lfo 2]
	[++ *index *next] / [InsertOscillatorControllers *next *VF *ctrls]
]

[[BuildVonForman *order *polyphony] / [BuildVonForman *order *polyphony * * *]]

[[BuildVonForman *order *polyphony *VonForman *VonFormanCB *volume]
	[CheckOrder *order *order1 *order2]
	[Moonbase *VonForman *VonFormanCB VF *volume]
	[moonbase *CB] [key_map *key_map] [arpeggiator *VonFormanCB *CB]
	[auto_data *XData] [parameter_block *X "index"] [*XData *X]
	[auto_data *YData] [parameter_block *Y "index"] [*YData *Y]
	[parameter_block *pitch "index"]
	[LGP *pitchfreqf *pitch]
	[parameter_block *modulation "index"]
	[sensitivity *modulationfreqf] [*modulationfreqf *modulation]
	[sensitivity *noise_modbias] [*noise_modbias *modulation]
	[lfo *lfo1] [lfo *lfo2]
		[gateway *vibrato] [*vibrato *modulation] [*lfo1 "vibrato" *vibrato]
		[gateway *tremolo] [*tremolo *modulation] [*lfo1 "tremolo" *tremolo]
		[gateway *wahwah] [*wahwah *modulation] [*lfo1 "wahwah" *wahwah]
		[gateway *lfopan] [*lfopan *modulation] [*lfo1 "pan" *lfopan]
		[control *lfo1x] [control *lfo1y] [control *lfo2x] [control *lfo2y]
		[*lfo1x *lfo1] [*lfo1y *lfo1] [*lfo2x *lfo2] [*lfo2y *lfo2]
	[BuildOscillatorControllers *order1 *pitch *modulation *lfo1 *lfo2 *ctrls]
	[LGT *noise_lfo1amp *lfo1] [LGT *noise_lfo2amp *lfo2]
	[stereo_pan *pan] [stereo_gateway *dry] [delay *delay] [drywet *drywet] [volume *volume] [stereo_chorus *chorus]
	[*pan "pan" *lfo1 "pan"] [*pan "pan" *lfo2 "pan"]
	[ConnectStereo *pan *chorus] [ConnectStereo *dry *pan] [ConnectStereo *delay *dry] [ConnectDryWet *drywet *dry *delay]
	[freeverb *verb]
	[ConnectStereo *verb *drywet]
	[ConnectStereo *volume *verb]
	[InsertRamp *volume "volume" 6 12800 *VonForman core volume]
	[InsertRamp *pan "pan" 6 0 *VonForman core pan]
	[Insert *chorus *VonForman core chorus]
	[Insert *dry *VonForman core delay]
	[Insert *drywet *VonForman core delay]
	[Insert *delay *VonForman core delay]
	[Insert *verb *VonForman core freeverb]
	[InsertPB *X *VonForman core vector X]
	[InsertPB *Y *VonForman core vector Y]
	[Insert *XData *VonForman core vector auto]
	[Insert *YData *VonForman core vector auto]
	[InsertPB *pitch *VonForman core pitch]
	[InsertPB *modulation *VonForman core modulation]
	[Insert *vibrato *VonForman core sens lfo vibrato]
	[Insert *tremolo *VonForman core sens lfo tremolo]
	[Insert *wahwah *VonForman core sens lfo wahwah]
	[Insert *lfopan *VonForman core sens lfo pan]
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
	[REPEAT *polyphony [BuildVonFormanPart *order1 *order2 *VonForman *VonFormanCB *chorus *key_map
						*lfo1 *lfo2 *XData *YData *lfo1x *lfo1y *lfo2x *lfo2y
						*pitchfreqf *modulationfreqf *noise_modbias *noise_lfo1amp *noise_lfo2amp *ctrls]
	]
	[InsertOscillatorControllers 1 *VonForman *ctrls]
	[Insert *noise_modbias *VonForman vco noise sens modulation]
	[Insert *noise_lfo1amp *VonForman vco noise sens lfo 1]
	[Insert *noise_lfo2amp *VonForman vco noise sens lfo 2]
	[Insert *pitchfreqf *VonForman filter sens pitch]
	[Insert *modulationfreqf *VonForman filter sens modulation]
	;==============================
	[InsertBlock *XData *VonForman vector X]
	[InsertBlock *YData *VonForman vector Y]
	[InsertBlock *key_map *VonForman key_map]
	;==============================
	[InsertController 1 *VonForman core modulation]
	[InsertController 5 *VonForman portamento time]
	[InsertController 7 *VonForman core volume]
	[InsertController 10 -64 *VonForman core pan]
	[InsertController 16 *VonForman core vector X]
	[InsertController 17 *VonForman core vector Y]
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
	[InsertController 11 *VonForman core delay gateway]
	[InsertController 64 *VonForman portamento hold]
	[InsertController 84 *VonForman portamento legato]
	[InsertController 81 -48 *VonForman portamento transpose]
	[InsertController 82 -1 *VonForman portamento mode]
	[InsertController 85 *VonForman core vector auto control]
	[InsertController 80 *VonForman arpeggiator active]
	[InsertController 66 *VonForman arpeggiator hold]
	;==============================
	[Lunar -16384 *VonForman vco noise amp]
	[Lunar -8192 *VonForman core delay balance]
	[Lunar 256 *VonForman core sens lfo vibrato]
	[Lunar 1792 *VonForman lfo 1 speed]
	[Lunar 12800 *VonForman core sens velocity BP]
	[Lunar -64 *VonForman core sens velocity left]
	[Lunar 12800 *VonForman filter sens velocity BP]
	[Lunar 16384 *VonForman mixer filter dry]
	[Lunar 16384 *VonForman mixer vco filter]
	[Lunar 16384 *VonForman mixer noise filter]
	[Lunar 16384 *VonForman vector filter amp A]
	[Lunar 16384 *VonForman vector filter amp B]
	[Lunar 16384 *VonForman vector filter amp C]
	[Lunar 16384 *VonForman vector filter amp D]
	[Lunar 16384 *VonForman vector noise A]
	[Lunar 16384 *VonForman vector noise B]
	[Lunar 16384 *VonForman vector noise C]
	[Lunar 16384 *VonForman vector noise D]
	[Preset 1 *order1 *VonForman]
	[FormantPreset 1 *order2 *VonForman]
]

[[Preset *index *order *VF] [> *index *order] /]
[[Preset *index *order *VF]
	[Lunar 512 *VF vco *index sens freq pitch]
	[Lunar 16384 *VF vco *index sens freq lfo 1]
	[Lunar 128   *VF vco *index sens freq key right]
	[Lunar -128  *VF vco *index sens freq key left]
	[Lunar 12800 *VF vco *index sens amp velocity BP]
	[Lunar 16384 *VF vco *index sens freq eg]
	[Lunar 16384 *VF vector amp *index A]
	[Lunar 16384 *VF vector amp *index B]
	[Lunar 16384 *VF vector amp *index C]
	[Lunar 16384 *VF vector amp *index D]
	[++ *index *next] / [Preset *next *order *VF]
]

[[FormantPreset *index *order *VF] [> *index *order] /]
[[FormantPreset *index *order *VF]
	[Lunar 16384 *VF mixer vco formant *index]
	[Lunar 16384 *VF mixer noise formant *index]
	[++ *index *next] / [FormantPreset *next *order *VF]
]

[[BuildOscillatorBlock 4 *fm *trigger *freqeg *X *Y *lfo1x *lfo1y *lfo2x *lfo2y
						[
							[*freq1 *amp1 *velocity1 *ampeg1 *egscal1 *freqeg1 *fma1 *fmf1]
							[*freq2 *amp2 *velocity2 *ampeg2 *egscal2 *freqeg2 *fma2 *fmf2]
							[*freq3 *amp3 *velocity3 *ampeg3 *egscal3 *freqeg3 *fma3 *fmf3]
							[*freq4 *amp4 *velocity4 *ampeg4 *egscal4 *freqeg4 *fma4 *fmf4]
						]
						[
							[*pitchfreq1 *modbias1 *lfo1freq1 *lfo2freq1 *lfo1amp1 *lfo2amp1]
							[*pitchfreq2 *modbias2 *lfo1freq2 *lfo2freq2 *lfo1amp2 *lfo2amp2]
							[*pitchfreq3 *modbias3 *lfo1freq3 *lfo2freq3 *lfo1amp3 *lfo2amp3]
							[*pitchfreq4 *modbias4 *lfo1freq4 *lfo2freq4 *lfo1amp4 *lfo2amp4]
						]
		]
	[fm4 *fm] [*fm "trigger" *trigger "trigger"]
	[PBF *pbf1 *fm "freq1"] [PBF *pbf2 *fm "freq2"] [PBF *pbf3 *fm "freq3"] [PBF *pbf4 *fm "freq4"]
	[STK *freq1 *pbf1 "signal" *trigger] [STK *freq2 *pbf2 "signal" *trigger] [STK *freq3 *pbf3 "signal" *trigger] [STK *freq4 *pbf4 "signal" *trigger]
	[STKF *amp1 *fm "amp1" *pbf1] [STKF *amp2 *fm "amp2" *pbf2] [STKF *amp3 *fm "amp3" *pbf3] [STKF *amp4 *fm "amp4" *pbf4]
	[STV *velocity1 *fm "amp1" *trigger] [STV *velocity2 *fm "amp2" *trigger] [STV *velocity3 *fm "amp3" *trigger] [STV *velocity4 *fm "amp4" *trigger]
	[AEG *ampeg1 *fm "amp1" *trigger *egscal1] [AEG *ampeg2 *fm "amp2" *trigger *egscal2] [AEG *ampeg3 *fm "amp3" *trigger *egscal3] [AEG *ampeg4 *fm "amp4" *trigger *egscal4]
	[FEGS *freqeg1 *freqeg *pbf1 "signal"] [FEGS *freqeg2 *freqeg *pbf2 "signal"] [FEGS *freqeg3 *freqeg *pbf3 "signal"] [FEGS *freqeg4 *freqeg *pbf4 "signal"]
	[*pbf1 *pitchfreq1] [*pbf2 *pitchfreq2] [*pbf3 *pitchfreq3] [*pbf4 *pitchfreq4]
	[*pbf1 *lfo1freq1] [*pbf2 *lfo1freq2] [*pbf3 *lfo1freq3] [*pbf4 *lfo1freq4]
	[*pbf1 *lfo2freq1] [*pbf2 *lfo2freq2] [*pbf3 *lfo2freq3] [*pbf4 *lfo2freq4]
	[*fm "amp1" *lfo1amp1] [*fm "amp2" *lfo1amp2] [*fm "amp3" *lfo1amp3] [*fm "amp4" *lfo1amp4]
	[*fm "amp1" *lfo2amp1] [*fm "amp2" *lfo2amp2] [*fm "amp3" *lfo2amp3] [*fm "amp4" *lfo2amp4]
	[*fm "amp1" *modbias1] [*fm "amp2" *modbias2] [*fm "amp3" *modbias3] [*fm "amp4" *modbias4]
	[MORPHFM *fma1 *fm "gain1" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma2 *fm "gain2" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma3 *fm "gain3" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma4 *fm "gain4" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf1 *pbf1 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf2 *pbf2 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf3 *pbf3 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf4 *pbf4 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
]

[[BuildOscillatorBlock 6 *fm *trigger *freqeg *X *Y *lfo1x *lfo1y *lfo2x *lfo2y
						[
							[*freq1 *amp1 *velocity1 *ampeg1 *egscal1 *freqeg1 *fma1 *fmf1]
							[*freq2 *amp2 *velocity2 *ampeg2 *egscal2 *freqeg2 *fma2 *fmf2]
							[*freq3 *amp3 *velocity3 *ampeg3 *egscal3 *freqeg3 *fma3 *fmf3]
							[*freq4 *amp4 *velocity4 *ampeg4 *egscal4 *freqeg4 *fma4 *fmf4]
							[*freq5 *amp5 *velocity5 *ampeg5 *egscal5 *freqeg5 *fma5 *fmf5]
							[*freq6 *amp6 *velocity6 *ampeg6 *egscal6 *freqeg6 *fma6 *fmf6]
						]
						[
							[*pitchfreq1 *modbias1 *lfo1freq1 *lfo2freq1 *lfo1amp1 *lfo2amp1]
							[*pitchfreq2 *modbias2 *lfo1freq2 *lfo2freq2 *lfo1amp2 *lfo2amp2]
							[*pitchfreq3 *modbias3 *lfo1freq3 *lfo2freq3 *lfo1amp3 *lfo2amp3]
							[*pitchfreq4 *modbias4 *lfo1freq4 *lfo2freq4 *lfo1amp4 *lfo2amp4]
							[*pitchfreq5 *modbias5 *lfo1freq5 *lfo2freq5 *lfo1amp5 *lfo2amp5]
							[*pitchfreq6 *modbias6 *lfo1freq6 *lfo2freq6 *lfo1amp6 *lfo2amp6]
						]
		]
	[fm6 *fm] [*fm "trigger" *trigger "trigger"]
	[PBF *pbf1 *fm "freq1"] [PBF *pbf2 *fm "freq2"] [PBF *pbf3 *fm "freq3"] [PBF *pbf4 *fm "freq4"] [PBF *pbf5 *fm "freq5"] [PBF *pbf6 *fm "freq6"]
	[STK *freq1 *pbf1 "signal" *trigger] [STK *freq2 *pbf2 "signal" *trigger]
		[STK *freq3 *pbf3 "signal" *trigger] [STK *freq4 *pbf4 "signal" *trigger]
		[STK *freq5 *pbf5 "signal" *trigger] [STK *freq6 *pbf6 "signal" *trigger]
	[STKF *amp1 *fm "amp1" *pbf1] [STKF *amp2 *fm "amp2" *pbf2]
		[STKF *amp3 *fm "amp3" *pbf3] [STKF *amp4 *fm "amp4" *pbf4]
		[STKF *amp5 *fm "amp5" *pbf5] [STKF *amp6 *fm "amp6" *pbf6]
	[STV *velocity1 *fm "amp1" *trigger] [STV *velocity2 *fm "amp2" *trigger]
		[STV *velocity3 *fm "amp3" *trigger] [STV *velocity4 *fm "amp4" *trigger]
		[STV *velocity5 *fm "amp5" *trigger] [STV *velocity6 *fm "amp6" *trigger]
	[AEG *ampeg1 *fm "amp1" *trigger *egscal1] [AEG *ampeg2 *fm "amp2" *trigger *egscal2]
		[AEG *ampeg3 *fm "amp3" *trigger *egscal3] [AEG *ampeg4 *fm "amp4" *trigger *egscal4]
		[AEG *ampeg5 *fm "amp5" *trigger *egscal5] [AEG *ampeg6 *fm "amp6" *trigger *egscal6]
	[FEGS *freqeg1 *freqeg *pbf1 "signal"] [FEGS *freqeg2 *freqeg *pbf2 "signal"]
		[FEGS *freqeg3 *freqeg *pbf3 "signal"] [FEGS *freqeg4 *freqeg *pbf4 "signal"]
		[FEGS *freqeg5 *freqeg *pbf5 "signal"] [FEGS *freqeg6 *freqeg *pbf6 "signal"]
	[*pbf1 *pitchfreq1] [*pbf2 *pitchfreq2] [*pbf3 *pitchfreq3] [*pbf4 *pitchfreq4] [*pbf5 *pitchfreq5] [*pbf6 *pitchfreq6]
	[*pbf1 *lfo1freq1] [*pbf2 *lfo1freq2] [*pbf3 *lfo1freq3] [*pbf4 *lfo1freq4] [*pbf5 *lfo1freq5] [*pbf6 *lfo1freq6]
	[*pbf1 *lfo2freq1] [*pbf2 *lfo2freq2] [*pbf3 *lfo2freq3] [*pbf4 *lfo2freq4] [*pbf5 *lfo2freq5] [*pbf6 *lfo2freq6]
	[*fm "amp1" *lfo1amp1] [*fm "amp2" *lfo1amp2] [*fm "amp3" *lfo1amp3] [*fm "amp4" *lfo1amp4] [*fm "amp5" *lfo1amp5] [*fm "amp6" *lfo1amp6]
	[*fm "amp1" *lfo2amp1] [*fm "amp2" *lfo2amp2] [*fm "amp3" *lfo2amp3] [*fm "amp4" *lfo2amp4] [*fm "amp5" *lfo2amp5] [*fm "amp6" *lfo2amp6]
	[*fm "amp1" *modbias1] [*fm "amp2" *modbias2] [*fm "amp3" *modbias3] [*fm "amp4" *modbias4] [*fm "amp5" *modbias5] [*fm "amp6" *modbias6]
	[MORPHFM *fma1 *fm "gain1" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma2 *fm "gain2" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma3 *fm "gain3" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma4 *fm "gain4" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma5 *fm "gain5" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *fma6 *fm "gain6" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf1 *pbf1 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf2 *pbf2 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf3 *pbf3 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf4 *pbf4 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf5 *pbf5 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *fmf6 *pbf6 "signal" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
]

[[BuildVonFormanPart *order1 *order2 *VF *cb *line *key_map
						*lfo1 *lfo2 *XData *YData *lfo1x *lfo1y *lfo2x *lfo2y
						*pitchfreqf *modulationfreqf *noise_modbias *noise_lfo1amp *noise_lfo2amp *ctrls
	]
	;========== TRIGGER ==============
	[trigger *trigger *key_map] [*cb *trigger] [*lfo1 "trigger" *trigger "trigger"] [*lfo2 "trigger" *trigger "trigger"]
	;========== AUTO VECTOR ==========
	[auto *X *XData 6] [*X "trigger" *trigger "trigger"] [*XData "trigger" *trigger "trigger"]
	[auto *Y *YData 6] [*Y "trigger" *trigger "trigger"] [*YData "trigger" *trigger "trigger"]
	;========== ADSR =================
	[adsrscal *adsr *egscal *trigger] [*trigger "busy" *adsr "busy"]
	[control *mixer] [*mixer "gateway" *adsr]
	[*lfo1 "trigger" *adsr "busy"] [*lfo2 "trigger" *adsr "busy"]
	;========== FREQ EG ==============
	[egscal *freqeg *fegscal *trigger]
	;========== NOISE ================
	[noise_operator *noise] [egscal *noise_eg *noise_egscal *trigger] [*noise "amp" *noise_eg]
	[STK *noise_keyamp *noise "amp" *trigger] [STV *noise_velocityamp *noise "amp" *trigger]
	[*noise "amp" *noise_modbias] [*noise "amp" *noise_lfo1amp] [*noise "amp" *noise_lfo2amp]
	[MORPHFM *noisegain *noise "gain" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	;========== FM VCO ===============
	[BuildOscillatorBlock *order1 *fm *trigger *freqeg *X *Y *lfo1x *lfo1y *lfo2x *lfo2y *oscillators *ctrls]
	;========== FILTER ===============
	[filter *filter]
	;[*filter *noise] [*filter *fm]
	[STK *filter_key *filter "freq" *trigger]
	[FEGS *freqegf *freqeg *filter "freq"]
	[*filter "freq" *pitchfreqf] [*filter "freq" *modulationfreqf]
	[*filter "freq" *lfo1 "wahwah"] [*filter "freq" *lfo2 "wahwah"]
	[egscal *cteg *ctegscal *trigger] [*filter "freq" *cteg]
	[MORPH *vcffreq *filter "freq" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *vcfreso *filter "resonance" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPHFM *vcfgain *filter "gain" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	;========== VELOCITY =============
	[sensitivity *velocity] [*velocity "signal" *trigger "velocity"] [*filter "amp" *velocity]
	[sensitivity *ctvelocity] [*ctvelocity "signal" *trigger "velocity"] [*filter "freq" *ctvelocity]
	;========== LINE CONNECTION ======
	[gateway *fmdry] [*fmdry *fm] [*mixer *fmdry]
	[gateway *noisedry] [*noisedry *noise] [*mixer *noisedry]
	[gateway *filterdry] [*filterdry *filter] [*mixer *filterdry]
	[gateway *fmfilter] [*fmfilter *fm] [*filter *fmfilter]
	[gateway *noisefilter] [*noisefilter *noise] [*filter *noisefilter]
	[*line *mixer]
	;========== FORMANT FILTERS ======
	[BuildFilterBlock *order2 *X *Y *lfo1x *lfo1y *lfo2x *lfo2y *fm *noise *filter *lfo1 *lfo2 *velocity *mixer *formant_filters]
	;========== SUPERSTRUCTURE =======
	[Insert *fm *VF vco]
	[Insert *noise *VF vco noise]
	[Insert *noise_eg *VF vco noise eg]
	[Insert *noise_egscal *VF vco noise egscal]
	[Insert *noise_keyamp *VF vco noise sens key]
	[Insert *noise_velocityamp *VF vco noise sens velocity]
	[Insert *filter *VF filter]
	[Insert *cteg *VF filter eg]
	[Insert *ctegscal *VF filter eg egscal]
	[Insert *filter_key *VF filter sens key]
	[Insert *ctvelocity *VF filter sens velocity]
	[Insert *freqegf *VF filter sens eg]
	[InsertFilterBlock *VF 1 *formant_filters]
	[Insert *vcffreq *VF vector filter freq]
	[Insert *vcfreso *VF vector filter resonance]
	[Insert *vcfgain *VF vector filter amp]
	[Insert *fmdry *VF mixer vco dry]
	[Insert *noisedry *VF mixer noise dry]
	[Insert *filterdry *VF mixer filter dry]
	[Insert *fmfilter *VF mixer vco filter]
	[Insert *noisefilter *VF mixer noise filter]
	[InsertOscillatorBlock *VF 1 *oscillators]
	[Insert *noisegain *VF vector noise]
	[Insert *adsr *VF adsr amp]
	[Insert *egscal *VF adsr amp egscal]
	[Insert *freqeg *VF adsr freq]
	[Insert *fegscal *VF adsr freq egscal]
	[Insert *trigger *VF portamento]
	[Insert *velocity *VF core sens velocity]
	[AddModule *X *VF] [AddModule *Y *VF]
]

[[BuildFilterBlock 0 *X *Y *lfo1x *lfo1y *lfo2x *lfo2y *fm *noise *vcf *lfo1 *lfo2 *velocity *line []]]
[[BuildFilterBlock *order *X *Y *lfo1x *lfo1y *lfo2x *lfo2y *fm *noise *vcf *lfo1 *lfo2 *velocity *line [[*filter *freq *reso *q *fmsend *noisesend *vcfsend] : *filters]]
  [< 0 *order]
	[formant_filter *filter]
	[MORPH *freq *filter "freq"      *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *reso *filter "resonance" *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[MORPH *q    *filter "q"         *X *Y *lfo1x *lfo1y *lfo2x *lfo2y]
	[gateway *fmsend] [*fmsend *fm] [*filter *fmsend]
	[gateway *noisesend] [*noisesend *noise] [*filter *noisesend]
	[gateway *vcfsend] [*vcfsend *vcf] [*filter *vcfsend]
	[*filter "amp" *lfo1 "tremolo"] [*filter "amp" *lfo2 "tremolo"]
	[*filter "amp" *velocity] [*line *filter]
	[-- *order *next] / [BuildFilterBlock *next *X *Y *lfo1x *lfo1y *lfo2x *lfo2y *fm *noise *vcf *lfo1 *lfo2 *velocity *line *filters]
]

[[InsertFilterBlock *VF *index []] /]
[[InsertFilterBlock *VF *index [[*filter *freq *reso *q *fmsend *noisesend *vcfsend] : *filters]]
	[Insert *filter *VF formant *index]
	[Insert *freq *VF vector formant *index freq]
	[Insert *reso *VF vector formant *index resonance]
	[Insert *q *VF vector formant *index Q]
	[Insert *fmsend *VF mixer vco formant *index]
	[Insert *noisesend *VF mixer noise formant *index]
	[Insert *vcfsend *VF mixer filter formant *index]
	[++ *index *next] / [InsertFilterBlock *VF *next *filters]
]

[[InsertOscillatorBlock *VF *index []] /]
[[InsertOscillatorBlock *VF *index [[*freq *amp *velocity *ampeg *egscal *freqeg *vectoramp *vectorfreq] : *oscillators]]
	[Insert *ampeg *VF vco *index eg]
	[Insert *egscal *VF vco *index eg egscal]
	[Insert *freq *VF vco *index sens freq key]
	[Insert *freqeg *VF vco *index sens freq eg]
	[Insert *amp *VF vco *index sens amp key]
	[Insert *velocity *VF vco *index sens amp velocity]
	[Insert *vectoramp *VF vector amp *index]
	[Insert *vectorfreq *VF vector freq *index]
	[++ *index *next] / [InsertOscillatorBlock *VF *next *oscillators]
]

private [
			LGV LGT LGP STK STV AEG FEGS MORPH MORPHFM PBF STKF
			BuildOscillatorBlock BuildFilterBlock InsertOscillatorBlock InsertFilterBlock
			BuildOscillatorControllers InsertOscillatorControllers
			CheckOrder Preset
		]

end .

