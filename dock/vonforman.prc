import studio
import lunar

program vonforman [BuildVonForman BuildVonFormanPart LGV LGT LGP]

[[LGV *gate *lfo] [gateway *gate] [*gate "enter" *lfo "vibrato"]]
[[LGT *gate *lfo] [gateway *gate] [*gate "enter" *lfo "tremolo"]]
[[LGP *gate *pb] [gateway *gate] [*gate *pb]]

[[BuildVonForman *polyphony] / [BuildVonForman *polyphony * * *]]

[[BuildVonForman *polyphony *VonForman *VonFormanCB *volume]
	[Moonbase *VonForman *VonFormanCB VonForman *volume]
	[moonbase *CB] [key_map *key_map] [arpeggiator *VonFormanCB *CB]
	[auto_data *XData] [parameter_block *X "index"] [*XData *X]
	[auto_data *YData] [parameter_block *Y "index"] [*YData *Y]
	[parameter_block *AutoCtrl "index"] [*XData "control" *AutoCtrl] [*YData "control" *AutoCtrl]
	[parameter_block *pitch "index"]
	[LGP *pitchfreq1 *pitch] [LGP *pitchfreq2 *pitch] [LGP *pitchfreq3 *pitch] [LGP *pitchfreq4 *pitch] [LGP *pitchfreqf *pitch]
	[parameter_block *modulation "index"]
	[lfo *lfo1] [lfo *lfo2]
		[gateway *vibrato] [*vibrato *modulation] [*lfo1 "vibrato" *vibrato]
		[gateway *wahwah] [*wahwah *modulation] [*lfo1 "wahwah" *wahwah]
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
	[InsertPB *pitch *VonForman core pitch]
	[InsertPB *modulation *VonForman core modulation]
	[Insert *vibrato *VonForman core sens vibrato]
	[Insert *wahwah *VonForman core sens wahwah]
	[InsertPB *AutoCtrl *VonForman core auto]
	[Insert *VonFormanCB *VonForman arpeggiator]
	[show "constructed...."]
]

private [LGV LGT]

end .
