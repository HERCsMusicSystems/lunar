
import studio
import lunar

program abakos [Abakos BuildAbakos BuildAbakosPart]

[[BuildAbakos *Abakos *AbakosCB *drywet]
	[moonbase *Abakoscb]
	[arpeggiator *AbakosCB *Abakoscb]
	[Moonbase *Abakos *AbakosCB Abakos]
	[pan *pan]
	[delay *delay]
	[drywet *drywet]
	[ConnectStereo *delay *pan]
	[*drywet "dryleft" *pan "left"]
	[*drywet "dryright" *pan "right"]
	[*drywet "wetleft" *delay "left"]
	[*drywet "wetright" *delay "right"]
	[Insert *AbakosCB *Abakos arpeggiator]
	[Insert *pan *Abakos core]
	[Insert *drywet *Abakos core]
	[Insert *delay *Abakos core]
	[BuildAbakosPart *Abakos *pan *Abakoscb]
	[BuildAbakosPart *Abakos *pan *Abakoscb]
	[BuildAbakosPart *Abakos *pan *Abakoscb]
	[BuildAbakosPart *Abakos *pan *Abakoscb]
	[InsertController 16 *Abakos core pan]
	[InsertController 17 *Abakos core time]
	[InsertController 65 *Abakos portamento porta]
	[AddMoon *Abakos *AbakosCB]
]

[[BuildAbakosPart *Abakos *mixer *cb]
	[operator *op1]
	[adsr *adsr1]
	[trigger *trigger] [*cb *trigger]
	[*trigger "busy" *adsr1 "busy"]
	[*op1 "freq" *trigger "key"]
	[*adsr1 "trigger" *trigger "trigger"]
	[*op1 "amp" *adsr1 "signal"]
	[*mixer *op1]
	[Insert *op1 *Abakos operator]
	[Insert *adsr1 *Abakos adsr]
	[Insert *trigger *Abakos portamento]

]

end .
