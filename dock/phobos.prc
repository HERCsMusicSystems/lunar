
import studio
import lunar

program phobos [BuildPhobos BuildPhobosPart PhobosEditor Operator-1 Operator-2 Operator-3 Operator-4 Operators Lfo-1 Lfo-2 Noise-EG Noise-Amp Filter-1]

[[PhobosEditor *phobos]
	[BuildEGPanel Operator-1 *phobos operator 1 eg] [Operator-1 10 10]
	[BuildEGPanel Operator-2 *phobos operator 2 eg] [Operator-2 330 10]
	[BuildEGPanel Operator-3 *phobos operator 3 eg] [Operator-3 10 240]
	[BuildEGPanel Operator-4 *phobos operator 4 eg] [Operator-4 330 240]
	[BuildFM4Panel Operators *phobos operator] [Operators 10 490]
	[BuildLfoPanel Lfo-1 *phobos lfo 1] [Lfo-1 660 10]
	[BuildLfoPanel Lfo-2 *phobos lfo 2] [Lfo-2 660 240]
	[BuildFilterPanel Filter-1 *phobos filter] [Filter-1 500 730]
	[BuildEGPanel Noise-EG *phobos noise] [Noise-EG 720 490]
	[BuildParameterBlockPanel Noise-Amp -16384.0 0.0 *phobos noise amp] [Noise-Amp 800 730]
]
[[PhobosEditor]
	[TRY [Operator-1]] [TRY [Operator-2]] [TRY [Operator-3]] [TRY [Operator-4]]
	[TRY [Operators]] [TRY [Lfo-1]] [TRY [Lfo-2]] [TRY [Filter-1]] [TRY [Noise-EG]] [TRY [Noise-Amp]]
]

[[BuildPhobos *polyphony] / [BuildPhobos *polyphony * * *]]
[[BuildPhobos *polyphony *Phobos *PhobosCB *volume]
	[Moonbase *Phobos *PhobosCB Phobos *volume]
	[moonbase *CB] [key_map *key_map] [arpeggiator *PhobosCB *CB]
	[auto_data *XData] [parameter_block *X "index"] [*XData *X]
	[auto_data *YData] [parameter_block *Y "index"] [*YData *Y]
	[parameter_block *pitch "index"]
	[lfo *lfo1] [lfo *lfo2]
	[gateway *lfo1freq1] [gateway *lfo1freq2] [gateway *lfo1freq3] [gateway *lfo1freq4]
	[*lfo1freq1 "enter" *lfo1 "vibrato"] [*lfo1freq2 "enter" *lfo1 "vibrato"] [*lfo1freq3 "enter" *lfo1 "vibrato"] [*lfo1freq4 "enter" *lfo1 "vibrato"]
	[gateway *lfo2freq1] [gateway *lfo2freq2] [gateway *lfo2freq3] [gateway *lfo2freq4]
	[*lfo2freq1 "enter" *lfo2 "vibrato"] [*lfo2freq2 "enter" *lfo2 "vibrato"] [*lfo2freq3 "enter" *lfo2 "vibrato"] [*lfo2freq4 "enter" *lfo2 "vibrato"]
	[gateway *lfo2amp1] [gateway *lfo2amp2] [gateway *lfo2amp3] [gateway *lfo2amp4]
	[*lfo2amp1 "enter" *lfo2 "tremolo"] [*lfo2amp2 "enter" *lfo2 "tremolo"] [*lfo2amp3 "enter" *lfo2 "tremolo"] [*lfo2amp4 "enter" *lfo2 "tremolo"]
	[gateway *pitchfreq1] [gateway *pitchfreq2] [gateway *pitchfreq3] [gateway *pitchfreq4] [gateway *pitchfreqf]
	[*pitchfreq1 *pitch] [*pitchfreq2 *pitch] [*pitchfreq3 *pitch] [*pitchfreq4 *pitch] [*pitchfreqf *pitch]
	[gateway *lfo1filter] [gateway *lfo2filter]
	[*lfo1filter "enter" *lfo1 "vibrato"] [*lfo2filter "enter" *lfo2 "wahwah"]
	;==============
	[stereo_pan *pan] [stereo_gateway *dry] [delay *delay] [drywet *drywet] [volume *volume] [stereo_chorus *chorus]
	[*pan "pan" *lfo2 "pan"]
	[ConnectStereo *pan *chorus] [ConnectStereo *dry *pan] [ConnectStereo *delay *dry] [ConnectDryWet *drywet *dry *delay] [ConnectStereo *volume *drywet]
	[Insert *volume *Phobos core]
	[Insert *pan *Phobos core]
	[Insert *chorus *Phobos core chorus]
	[Insert *dry *Phobos core delay]
	[Insert *drywet *Phobos core delay]
	[Insert *delay *Phobos core delay]
	[InsertPB *X *Phobos core X]
	[InsertPB *Y *Phobos core Y]
	[Insert *XData *Phobos core auto]
	[Insert *YData *Phobos core auto]
	[InsertPB *pitch *Phobos core pitch]
	[Insert *PhobosCB *Phobos arpeggiator]
	[Insert *lfo1 *Phobos lfo 1]
	[InsertFor *Phobos lfo 1 vibrato]
	[Insert *lfo2 *Phobos lfo 2]
	[InsertFor *Phobos lfo 2 vibrato]
	[InsertFor *Phobos lfo 2 tremolo]
	[InsertFor *Phobos lfo 2 wahwah]
	[InsertFor *Phobos lfo 2 pan]
	;==================
	[REPEAT *polyphony [BuildPhobosPart *Phobos *CB *chorus *key_map *XData *YData *lfo1 *lfo2
						*lfo1freq1 *lfo1freq2 *lfo1freq3 *lfo1freq4
						*lfo2freq1 *lfo2freq2 *lfo2freq3 *lfo2freq4
						*lfo2amp1 *lfo2amp2 *lfo2amp3 *lfo2amp4
						*pitchfreq1 *pitchfreq2 *pitchfreq3 *pitchfreq4 *pitchfreqf
						*lfo1filter *lfo2filter]]
	;==================
	[Insert *pitchfreq1 *Phobos operator 1 sens freq pitch]
	[Insert *pitchfreq2 *Phobos operator 2 sens freq pitch]
	[Insert *pitchfreq3 *Phobos operator 3 sens freq pitch]
	[Insert *pitchfreq4 *Phobos operator 4 sens freq pitch]
	[Insert *lfo1freq1 *Phobos operator 1 sens freq lfo 1]
	[Insert *lfo1freq2 *Phobos operator 2 sens freq lfo 1]
	[Insert *lfo1freq3 *Phobos operator 3 sens freq lfo 1]
	[Insert *lfo1freq4 *Phobos operator 4 sens freq lfo 1]
	[Insert *lfo2freq1 *Phobos operator 1 sens freq lfo 2]
	[Insert *lfo2freq2 *Phobos operator 2 sens freq lfo 2]
	[Insert *lfo2freq3 *Phobos operator 3 sens freq lfo 2]
	[Insert *lfo2freq4 *Phobos operator 4 sens freq lfo 2]
	[Insert *lfo2amp1 *Phobos operator 1 sens amp lfo]
	[Insert *lfo2amp2 *Phobos operator 2 sens amp lfo]
	[Insert *lfo2amp3 *Phobos operator 3 sens amp lfo]
	[Insert *lfo2amp4 *Phobos operator 4 sens amp lfo]
	[Insert *pitchfreqf *Phobos filter sens pitch]
	[Insert *lfo1filter *Phobos filter sens lfo 1]
	[Insert *lfo2filter *Phobos filter sens lfo 2]

	[InsertBlock *XData *Phobos vector X]
	[InsertBlock *YData *Phobos vector Y]
	[InsertBlock *key_map *Phobos key_map]

	[InsertController 1 *Phobos lfo 1 vibrato]
	[InsertController 7 *Phobos core volume]
	[InsertController 10 -64 *Phobos core pan]
	[InsertController 5 *Phobos portamento time]
	[InsertController 16 *Phobos core X]
	[InsertController 17 *Phobos core Y]
	[InsertController 65 *Phobos portamento porta]
	[InsertController 74 -64 *Phobos filter freq]
	[InsertController 71 *Phobos filter resonance]
	[InsertController 77 -64 *Phobos core delay balance]
	[InsertController 78 -64 *Phobos lfo 1 speed]
	[InsertController 73 *Phobos adsr amp attack]
	[InsertController 75 *Phobos adsr amp decay]
	[InsertController 76 -128 *Phobos adsr amp sustain]
	[InsertController 72 *Phobos adsr amp release]
	[InsertController 79 *Phobos lfo 2 vibrato]
	[InsertController 128 -64 *Phobos core pitch]
	; INVISIBLE....
	[InsertController 11 *Phobos core delay gateway]
	[InsertController 64 *Phobos portamento hold]
	[InsertController 84 *Phobos portamento legato]
	[InsertController 81 -48 *Phobos portamento transpose]
	[InsertController 82 -1 *Phobos portamento mode]
	[InsertController 85 *Phobos core auto control]
	[InsertController 80 *Phobos arpeggiator active]
	[InsertController 66 *Phobos arpeggiator hold]

	[Lunar 16384 *Phobos operator 1 vector A]
	[Lunar 16384 *Phobos operator 1 vector B]
	[Lunar 16384 *Phobos operator 1 vector C]
	[Lunar 16384 *Phobos operator 1 vector D]
	[Lunar 16384 *Phobos operator 2 vector A]
	[Lunar 16384 *Phobos operator 2 vector B]
	[Lunar 16384 *Phobos operator 2 vector C]
	[Lunar 16384 *Phobos operator 2 vector D]
	[Lunar 16384 *Phobos operator 3 vector A]
	[Lunar 16384 *Phobos operator 3 vector B]
	[Lunar 16384 *Phobos operator 3 vector C]
	[Lunar 16384 *Phobos operator 3 vector D]
	[Lunar 16384 *Phobos operator 4 vector A]
	[Lunar 16384 *Phobos operator 4 vector B]
	[Lunar 16384 *Phobos operator 4 vector C]
	[Lunar 16384 *Phobos operator 4 vector D]
	[Lunar -128 *Phobos operator 1 sens freq key left]
	[Lunar 128 *Phobos operator 1 sens freq key right]
	[Lunar -128 *Phobos operator 2 sens freq key left]
	[Lunar 128 *Phobos operator 2 sens freq key right]
	[Lunar -128 *Phobos operator 3 sens freq key left]
	[Lunar 128 *Phobos operator 3 sens freq key right]
	[Lunar -128 *Phobos operator 4 sens freq key left]
	[Lunar 128 *Phobos operator 4 sens freq key right]
	[Lunar 512 *Phobos operator 1 sens freq pitch]
	[Lunar 512 *Phobos operator 2 sens freq pitch]
	[Lunar 512 *Phobos operator 3 sens freq pitch]
	[Lunar 512 *Phobos operator 4 sens freq pitch]
	[Lunar 128 *Phobos operator 1 sens freq lfo 1]
	[Lunar 128 *Phobos operator 2 sens freq lfo 1]
	[Lunar 128 *Phobos operator 3 sens freq lfo 1]
	[Lunar 128 *Phobos operator 4 sens freq lfo 1]
	[Lunar 12800 *Phobos operator 1 sens amp velocity BP]
	[Lunar 12800 *Phobos operator 2 sens amp velocity BP]
	[Lunar 12800 *Phobos operator 3 sens amp velocity BP]
	[Lunar 12800 *Phobos operator 4 sens amp velocity BP]
	[Lunar 12800 *Phobos filter sens velocity BP]
	[Lunar 1792 *Phobos lfo 1 speed]
	[Lunar -16384 *Phobos noise amp]
	[Lunar -8192 *Phobos core delay balance]
	[Lunar 8192 *Phobos core X]
	[Lunar 8192 *Phobos core Y]
	[Lunar 12800 *Phobos core velocity BP]
	[Lunar -64 *Phobos core velocity left]
	[Lunar 1 *Phobos portamento porta]
	[*PhobosCB mono]
]

[[BuildPhobosPart *Phobos *cb *line *key_map *XData *YData *lfo1 *lfo2
						*lfo1freq1 *lfo1freq2 *lfo1freq3 *lfo1freq4
						*lfo2freq1 *lfo2freq2 *lfo2freq3 *lfo2freq4
						*lfo2amp1 *lfo2amp2 *lfo2amp3 *lfo2amp4
						*pitchfreq1 *pitchfreq2 *pitchfreq3 *pitchfreq4 *pitchfreqf
						*lfo1filter *lfo2filter]
	[trigger *trigger *key_map] [*cb *trigger]
	[fm4 *fm] [noise_operator *noise] [eg *noise_eg] [filter *filter] [adsr *adsr]
	[*fm "trigger" *trigger "trigger"]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"] [*lfo1 "trigger" *adsr "busy"] [*lfo2 "trigger" *adsr "busy"]
	[*filter *fm] [*filter *noise] [*filter "gain" *adsr] [*line *filter]
	[*lfo1 "trigger" *trigger "trigger"] [*lfo2 "trigger" *trigger "trigger"]
	[*noise_eg "trigger" *trigger "trigger"] [*noise "amp" *noise_eg]
	;=============
	;====> KEY FREQ SENSITIVITY
	[sensitivity *freq1] [sensitivity *freq2] [sensitivity *freq3] [sensitivity *freq4]
	[*freq1 "signal" *trigger "key"] [*freq2 "signal" *trigger "key"] [*freq3 "signal" *trigger "key"] [*freq4 "signal" *trigger "key"]
	[*fm "freq1" *freq1] [*fm "freq2" *freq2] [*fm "freq3" *freq3] [*fm "freq4" *freq4]
	;====> KEY AMP SENSITIVITY
	[sensitivity *amp1] [sensitivity *amp2] [sensitivity *amp3] [sensitivity *amp4]
	[*amp1 "signal" *trigger "key"] [*amp2 "signal" *trigger "key"] [*amp3 "signal" *trigger "key"] [*amp4 "signal" *trigger "key"]
	[*fm "amp1" *amp1] [*fm "amp2" *amp2] [*fm "amp3" *amp3] [*fm "amp4" *amp4]
	;====> VELOCITY AMP SENSITIVITY
	[sensitivity *velocity1] [sensitivity *velocity2] [sensitivity *velocity3] [sensitivity *velocity4]
	[*velocity1 "signal" *trigger "velocity"] [*velocity2 "signal" *trigger "velocity"] [*velocity3 "signal" *trigger "velocity"] [*velocity4 "signal" *trigger "velocity"]
	[*fm "amp1" *velocity1] [*fm "amp2" *velocity2] [*fm "amp3" *velocity3] [*fm "amp4" *velocity4]
	[sensitivity *velocity] [*velocity "signal" *trigger "velocity"] [*filter "amp" *velocity]
	;====> VELOCITY FREQ SENSITIVITY
	[sensitivity *ctvelocity] [*ctvelocity "signal" *trigger "velocity"] [*filter "freq" *ctvelocity]
	;====> AMP EG
	[eg *ampeg1] [eg *ampeg2] [eg *ampeg3] [eg *ampeg4]
	[*ampeg1 "trigger" *trigger "trigger"] [*ampeg2 "trigger" *trigger "trigger"] [*ampeg3 "trigger" *trigger "trigger"] [*ampeg4 "trigger" *trigger "trigger"]
	[*fm "amp1" *ampeg1] [*fm "amp2" *ampeg2] [*fm "amp3" *ampeg3] [*fm "amp4" *ampeg4]
	;====> FREQ EG SENSITIVITY (including filter)
	[eg *freqeg] [gateway *freqeg1] [gateway *freqeg2] [gateway *freqeg3] [gateway *freqeg4] [gateway *freqegf]
	[*freqeg1 *freqeg] [*freqeg2 *freqeg] [*freqeg3 *freqeg] [*freqeg4 *freqeg] [*freqegf *freqeg]
	[*freqeg "trigger" *trigger "trigger"] [*fm "freq1" *freqeg1] [*fm "freq2" *freqeg2] [*fm "freq3" *freqeg3] [*fm "freq4" *freqeg4] [*filter "freq" *freqegf]
	;====> PITCH FREQ SENSITIVITY
	[*fm "freq1" *pitchfreq1] [*fm "freq2" *pitchfreq2] [*fm "freq3" *pitchfreq3] [*fm "freq4" *pitchfreq4]
	;====> X/Y AMP GAIN MORPHING
	[auto *vx *XData] [*vx "trigger" *trigger "trigger"] [*XData "trigger" *trigger "trigger"]
	[auto *vy *YData] [*vy "trigger" *trigger "trigger"] [*YData "trigger" *trigger "trigger"]
	[morph *vm1 16384.0] [morph *vm2 16384.0] [morph *vm3 16384.0] [morph *vm4 16384.0]
	[*vm1 "x" *vx] [*vm2 "x" *vx] [*vm3 "x" *vx] [*vm4 "x" *vx]
	[*vm1 "y" *vy] [*vm2 "y" *vy] [*vm3 "y" *vy] [*vm4 "y" *vy]
	[*fm "gain1" *vm1] [*fm "gain2" *vm2] [*fm "gain3" *vm3] [*fm "gain4" *vm4]
	;====> LFO 1 SENSITIVITY
	[*fm "freq1" *lfo1freq1] [*fm "freq2" *lfo1freq2] [*fm "freq3" *lfo1freq3] [*fm "freq4" *lfo1freq4]
	;====> LFO 2 SENSITIVITY
	[*fm "freq1" *lfo2freq1] [*fm "freq2" *lfo2freq2] [*fm "freq3" *lfo2freq3] [*fm "freq4" *lfo2freq4]
	[*fm "amp1" *lfo2amp1] [*fm "amp2" *lfo2amp2] [*fm "amp3" *lfo2amp3] [*fm "amp4" *lfo2amp4]
	;====> FILTER SENSITIVITY SENSITIVITY
	[sensitivity *filter_key] [*filter_key "signal" *trigger "key"] [*filter "freq" *filter_key]
	[*filter "freq" *pitchfreqf]
	[*filter "freq" *lfo1filter]
	[*filter "freq" *lfo2filter]
	;====> EG SCALING
	[sensitivity *egscal1] [sensitivity *egscal2] [sensitivity *egscal3] [sensitivity *egscal4]
	[*egscal1 "signal" *trigger "key"] [*egscal2 "signal" *trigger "key"] [*egscal3 "signal" *trigger "key"] [*egscal4 "signal" *trigger "key"]
	[*ampeg1 "time1" *egscal1] [*ampeg1 "time2" *egscal1] [*ampeg1 "time3" *egscal1] [*ampeg1 "time4" *egscal1]
	[*ampeg2 "time1" *egscal2] [*ampeg2 "time2" *egscal2] [*ampeg2 "time3" *egscal2] [*ampeg2 "time4" *egscal2]
	[*ampeg3 "time1" *egscal3] [*ampeg3 "time2" *egscal3] [*ampeg3 "time3" *egscal3] [*ampeg3 "time4" *egscal3]
	[*ampeg4 "time1" *egscal4] [*ampeg4 "time2" *egscal4] [*ampeg4 "time3" *egscal4] [*ampeg4 "time4" *egscal4]
	[sensitivity *egscal] [*egscal "signal" *trigger "key"] [*adsr "attack" *egscal] [*adsr "decay" *egscal] [*adsr "release" *egscal]
	[sensitivity *egscalf] [*egscalf "signal" *trigger "key"] [*freqeg "time1" *egscalf] [*freqeg "time2" *egscalf] [*freqeg "time3" *egscalf] [*freqeg "time4" *egscalf]
	;=============
	[Insert *fm *Phobos operator]
	[Insert *ampeg1 *Phobos operator 1 eg] [Insert *ampeg2 *Phobos operator 2 eg] [Insert *ampeg3 *Phobos operator 3 eg] [Insert *ampeg4 *Phobos operator 4 eg]
	[Insert *egscal1 *Phobos operator 1 eg egscal] [Insert *egscal2 *Phobos operator 2 eg egscal] [Insert *egscal3 *Phobos operator 3 eg egscal] [Insert *egscal4 *Phobos operator 4 eg egscal]
	[Insert *vm1 *Phobos operator 1 vector] [Insert *vm2 *Phobos operator 2 vector] [Insert *vm3 *Phobos operator 3 vector] [Insert *vm4 *Phobos operator 4 vector]
	[Insert *freq1 *Phobos operator 1 sens freq key] [Insert *freq2 *Phobos operator 2 sens freq key]
	[Insert *freq3 *Phobos operator 3 sens freq key] [Insert *freq4 *Phobos operator 4 sens freq key]
	[Insert *amp1 *Phobos operator 1 sens amp key] [Insert *amp2 *Phobos operator 2 sens amp key] [Insert *amp3 *Phobos operator 3 sens amp key] [Insert *amp4 *Phobos operator 4 sens amp key]
	[Insert *velocity1 *Phobos operator 1 sens amp velocity] [Insert *velocity2 *Phobos operator 2 sens amp velocity]
	[Insert *velocity3 *Phobos operator 3 sens amp velocity] [Insert *velocity4 *Phobos operator 4 sens amp velocity]
	[Insert *velocity *Phobos core velocity]
	[Insert *noise *Phobos noise] [Insert *noise_eg *Phobos noise]
	[Insert *freqeg1 *Phobos operator 1 sens freq eg] [Insert *freqeg2 *Phobos operator 2 sens freq eg]
	[Insert *freqeg3 *Phobos operator 3 sens freq eg] [Insert *freqeg4 *Phobos operator 4 sens freq eg]
	[Insert *filter *Phobos filter]
	[Insert *freqegf *Phobos filter sens eg]
	[Insert *filter_key *Phobos filter sens key]
	[Insert *ctvelocity *Phobos filter sens velocity]
	[Insert *adsr *Phobos adsr amp]
	[Insert *egscal *Phobos adsr amp egscal]
	[Insert *freqeg *Phobos adsr freq]
	[Insert *egscalf *Phobos adsr freq egscal]
	[Insert *trigger *Phobos portamento]
	[AddModule *vx *Phobos]
	[AddModule *vy *Phobos]
]
end .
