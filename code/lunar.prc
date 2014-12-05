;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This file was create on Monday, 14th July 2014 at 11:36:34 AM. ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program lunar #machine := "prolog.lunar"
			[
				small_keyboard keyboard big_keyboard oscilloscope vector CommandCentre
				dock undock noise orbiter
				core moonbase arpeggiator operator parameter_block auto auto_data key_map velocity_map impulse
				trigger inactive_trigger mixer stereo_mixer gateway amplifier lfo adsr eg
				square_operator fm4 fm6 dx9 dx7 saw_operator noise_operator sampler_operator sensitivity filter delay pan power_pan linear_pan
				drywet drywet_mono balance
				level1 level2 level3 level4 time1 time2 time3 time4 attack decay sustain release
				signal freq amp ratio sync resonance hold busy portamento porta  legato time speed wave pulse phase poly feedback highdamp
				mono left right mic mic_left mic_right breakpoint algo key_ratio
				keyon keyoff polyaftertouch control programchange aftertouch pitch
				sysex timingclock START CONTINUE STOP activesensing
				division active
				Lunar Lander Activate Deactivate
				Connect ConnectStereo Disconnect DisconnectStereo
				AddParameterBlock
				Moonbase Insert InsertIO Store Restore SubRestore Moons
				Cbb Cb C C# Cx
				Dbb Db D D# Dx
				Ebb Eb E E# Ex
				Fbb Fb F F# Fx
				Gbb Gb G G# Gx
				Abb Ab A A# Ax
				Bbb Bb B B# Bx
				midi
				ParameterBlockPanel AdsrPanel
				MoveModules PropagateSignals MoveCore LunarDrop
			]

#machine small_keyboard := "small_keyboard"
#machine keyboard := "keyboard"
#machine big_keyboard := "big_keyboard"
#machine oscilloscope := "oscilloscope"
#machine vector := "vector"
#machine CommandCentre := "CommandCentre"
;#machine dock := "dock"
;#machine undock := "undock"
#machine parameter_block := "parameter_block"
#machine auto_data := "auto_data"
#machine auto := "auto"
#machine key_map := "key_map"
#machine wave := "wave"
#machine velocity_map := "velocity_map"
#machine impulse := "impulse"
#machine trigger := "trigger"
#machine inactive_trigger := "inactive_trigger"
#machine mixer := "mixer"
#machine stereo_mixer := "stereo_mixer"
#machine lfo := "lfo"
#machine adsr := "adsr"
#machine eg := "eg"
#machine delay := "delay"
#machine pan := "pan"
#machine drywet := "drywet"
#machine drywet_mono := "drywet_mono"
#machine power_pan := "power_pan"
#machine linear_pan := "linear_pan"
#machine gateway := "gateway"
#machine amplifier := "amplifier"
#machine sensitivity := "sensitivity"
#machine moonbase := "moonbase"
#machine arpeggiator := "arpeggiator"

#machine core := "core"
#machine operator := "operator"
#machine fm4 := "fm4"
#machine square_operator := "square_operator"
#machine saw_operator := "saw_operator"
#machine noise_operator := "noise_operator"
#machine sampler_operator := "sampler_operator"
#machine filter := "filter"

#machine orbiter := "orbiter"

#machine midi := "midi"

#machine ParameterBlockPanel := "ParameterBlockPanel"
#machine AdsrPanel := "AdsrPanel"

#machine MoveModules := "MoveModules"
#machine PropagateSignals := "PropagateSignals"

[[MoveCore] [MoveModules] [PropagateSignals]]

[[Connect : *command] *command]
[[ConnectStereo *to *from]
	[*to "LEFT" *from "LEFT"]
	[*to "RIGHT" *from "RIGHT"]
]
[[Disconnect *to : *command] [*to [] : *command]]
[[DisconnectStereo *to *from]
	[*to "LEFT" *from "LEFT" []]
	[*to "RIGHT" *from "RIGHT" []]
]

[[Moonbase *base *distributor *type]
	[create_atom *modules] [create_atom *parameters]
	[moonbase *distributor]
	[addcl [[*base *parameters *modules *distributor *type]]]
]

[[Moonbase *base]
	[*base *parameters *modules *callback : *]
	[delallcl *base]
	[*callback]
	[TRY [*parameters *parameter : *selector] [*parameter] fail]
	[TRY [*modules *module : *selector] [*module []] [*module] fail]
	[delallcl *parameters]
	[delallcl *modules]
]

[[AddParameterBlock *parameters *parameter *module *selector *initial *style]
	[APPEND *selector [*parameter] *selectors]
	[*parameters *pb : *selectors] /
	[text_term *name *parameter]
	[*module *name *pb]
]
[[AddParameterBlock *parameters *parameter *module *selector *initial *style]
	[parameter_block *pb *style]
	[*pb *initial]
	[APPEND *selector [*parameter] *selectors]
	[addcl [[*parameters *pb : *selectors]]]
	[text_term *name *parameter]
	[*module *name *pb]
]

[[AddParameterBlock *parameters *parameter *name *module *selector *initial *style]
	[APPEND *selector [*parameter] *selectors]
	[*parameters *pb : *selectors] /
	[*module *name *pb]
]
[[AddParameterBlock *parameters *parameter *name *module *selector *initial *style]
	[parameter_block *pb *style]
	[*pb *initial]
	[APPEND *selector [*parameter] *selectors]
	[addcl [[*parameters *pb : *selectors]]]
	[*module *name *pb]
]

[[Insert *operator *base : *selector]
	[*base *parameters *modules : *]
	[addcl [[*modules *operator : *selector]]]
	[*operator : *io]
	[InsertIO *parameters *operator *selector *io]
]

[[InsertIO *parameters *operator *selector [["FREQ" "AMP" "RATIO" "SYNC" : *] *o]]
	[AddParameterBlock *parameters freq *operator *selector 0 "freq"]
	[AddParameterBlock *parameters amp *operator *selector 0 "amp"]
	[AddParameterBlock *parameters ratio *operator *selector 1 "ratio"]
	[AddParameterBlock *parameters sync *operator *selector 0 "onoff"]
]

[[InsertIO *parameters *noise *selector [["AMP"] ["SIGNAL"]]]
	[AddParameterBlock *parameters amp *noise *selector 0 "amp"]
]

[[InsertIO *parameters *arpeggiator *selector [["SPEED" "DIVISION" "ACTIVE" "ALGO" : *] *]]
	[AddParameterBlock *parameters speed *arpeggiator *selector 140 "index"]
	[AddParameterBlock *parameters division *arpeggiator *selector 24 "index"]
	[AddParameterBlock *parameters active *arpeggiator *selector 0 "onoff"]
	[AddParameterBlock *parameters algo *arpeggiator *selector 0 "index"]
]

[[InsertIO *parameters *adsr *selector [["TRIGGER" "ATTACK" "DECAY" "SUSTAIN" "RELEASE" : *] *]]
	[AddParameterBlock *parameters attack *adsr *selector 0 "time"]
	[AddParameterBlock *parameters decay *adsr *selector 0 "time"]
	[AddParameterBlock *parameters sustain *adsr *selector 0 "amp"]
	[AddParameterBlock *parameters release *adsr *selector 0 "time"]
]

[[InsertIO *parameters *adsr *selector [["TRIGGER" "LEVEL1" "LEVEL2" "LEVEL3" "LEVEL4" "TIME1" "TIME2" "TIME3" "TIME4" : *] *]]
	[AddParameterBlock *parameters level1 *adsr *selector 0 "index"]
	[AddParameterBlock *parameters level2 *adsr *selector 0 "index"]
	[AddParameterBlock *parameters level3 *adsr *selector 0 "index"]
	[AddParameterBlock *parameters level4 *adsr *selector 0 "index"]
	[AddParameterBlock *parameters time1 *adsr *selector 0 "time"]
	[AddParameterBlock *parameters time2 *adsr *selector 0 "time"]
	[AddParameterBlock *parameters time3 *adsr *selector 0 "time"]
	[AddParameterBlock *parameters time4 *adsr *selector 0 "time"]
]

[[InsertIO *parameters *trigger *selector [["BUSY" "HOLD" "PORTA" "TIME" "LEGATO" : *] *]]
	[AddParameterBlock *parameters porta *trigger *selector 0 "onoff"]
	[AddParameterBlock *parameters time *trigger *selector 0 "time"]
	[AddParameterBlock *parameters legato *trigger *selector 0 "onoff"]
	[AddParameterBlock *parameters hold *trigger *selector 0 "onoff"]
]

[[InsertIO *parameters *lfo *selector [["SPEED" "WAVE" "PULSE" "PHASE" "SYNC" : *] *]]
	[AddParameterBlock *parameters speed *lfo *selector 0 "time"]
	[AddParameterBlock *parameters wave *lfo *selector 0 "wave"]
	[AddParameterBlock *parameters pulse *lfo *selector 0 "index"]
	[AddParameterBlock *parameters phase *lfo *selector 0 "index"]
	[AddParameterBlock *parameters sync *lfo *selector 0 "onoff"]
]

[[InsertIO *parameters *filter *selector [["ENTER" "FREQ" "RESONANCE" "AMP" : *] *]]
	[AddParameterBlock *parameters freq *filter *selector 0 "freq"]
	[AddParameterBlock *parameters resonance *filter *selector 0 "amp"]
	[AddParameterBlock *parameters amp *filter *selector 0 "amp"]
]

[[InsertIO *parameters *sensitivity *selector [["ENTER" "BREAKPOINT" "LEFT" "RIGHT" : *] *]]
	[AddParameterBlock *parameters breakpoint *sensitivity *selector 0 "index"]
	[AddParameterBlock *parameters left *sensitivity *selector -128 "index"]
	[AddParameterBlock *parameters right *sensitivity *selector 128 "index"]
]

[[InsertIO *parameters *pan *selector [["SIGNAL" "PAN" : *] *]]
	[AddParameterBlock *parameters pan *pan *selector 0 "index"]
]

[[InsertIO *parameters *delay *selector [["LEFT" "RIGHT" "FEEDBACK" "TIME" "HIGHDAMP"] ["LEFT" "RIGHT"]]]
	[AddParameterBlock *parameters feedback *delay *selector 0 "index"]
	[AddParameterBlock *parameters time *delay *selector 0 "time"]
	[AddParameterBlock *parameters highdamp *delay *selector 0 "index"]
]

[[InsertIO *parameters *drywet *selector [["DRYLEFT" "DRYRIGHT" "WETLEFT" "WETRIGHT" "BALANCE" : *] *]]
	[AddParameterBlock *parameters balance *drywet *selector 0 "index"]
]

[[InsertIO *parameters *op *selector [["ALGO" "TRIGGER" "FREQ1" "AMP1" "RATIO1" "SYNC1" "FEEDBACK1" "FREQ2" "AMP2" "RATIO2" "SYNC2" "FEEDBACK2" "FREQ3" "AMP3" "RATIO3" "SYNC3" "FEEDBACK3" "FREQ4" "AMP4" "RATIO4" "SYNC4" "FEEDBACK4"] ["SIGNAL"]]]
	[AddParameterBlock *parameters algo *op *selector 0 "index"]
	[APPEND *selector [1] *selector1]
	[AddParameterBlock *parameters freq "freq1" *op *selector1 0 "freq"]
	[AddParameterBlock *parameters amp "amp1" *op *selector1 0 "amp"]
	[AddParameterBlock *parameters ratio "ratio1" *op *selector1 1 "ratio"]
	[AddParameterBlock *parameters sync "sync1" *op *selector1 0 "onoff"]
	[AddParameterBlock *parameters feedback "feedback1" *op *selector1 0 "index"]
	[APPEND *selector [2] *selector2]
	[AddParameterBlock *parameters freq "freq2" *op *selector2 0 "freq"]
	[AddParameterBlock *parameters amp "amp2" *op *selector2 0 "amp"]
	[AddParameterBlock *parameters ratio "ratio2" *op *selector1 2 "ratio"]
	[AddParameterBlock *parameters sync "sync2" *op *selector2 0 "onoff"]
	[AddParameterBlock *parameters feedback "feedback2" *op *selector2 0 "index"]
	[APPEND *selector [3] *selector3]
	[AddParameterBlock *parameters freq "freq3" *op *selector3 0 "freq"]
	[AddParameterBlock *parameters amp "amp3" *op *selector3 0 "amp"]
	[AddParameterBlock *parameters ratio "ratio3" *op *selector3 1 "ratio"]
	[AddParameterBlock *parameters sync "sync3" *op *selector3 0 "onoff"]
	[AddParameterBlock *parameters feedback "feedback3" *op *selector3 0 "index"]
	[APPEND *selector [4] *selector4]
	[AddParameterBlock *parameters freq "freq4" *op *selector4 0 "freq"]
	[AddParameterBlock *parameters amp "amp4" *op *selector4 0 "amp"]
	[AddParameterBlock *parameters ratio "ratio4" *op *selector4 1 "ratio"]
	[AddParameterBlock *parameters sync "sync4" *op *selector4 0 "onoff"]
	[AddParameterBlock *parameters feedback "feedback4" *op *selector4 0 "index"]
]

[[InsertIO *parameters *mixer *selector [["ENTER" : *] *]]]

[[InsertIO *parameters *operator *selector [*i *o]] [show "Failed inserting IO at " [*parameters *operator *i *o]]]

[[Lander *base *moonbase : *selector] [*moonbase * *modules : *] [*modules *base : *selector]]

[[Lunar *v *moonbase : *selector] [*moonbase *parameters : *] [*parameters *base : *selector] [*base 0 *v]]

[[Store *moonbase *program]
	[is_integer *program] /
	[*moonbase * * * *type : *]
	[add *file_name *type "/" *program ".txt"]
	[Store *moonbase *file_name]
]

[[Store *moonbase *file_name]
	[*moonbase *parameters *modules *cb *type : *]
	[file_writer *tc *file_name]
	[*tc [*type] "\n"]
	[*cb control : *mono] [*tc [*mono] "\n"] [show *mono]
	[TRY
		[*parameters *orbiter : *selector]
		[*orbiter *x]
		[show *selector " => " *x]
		[*tc [*selector] [*x] "\n"]
		fail
	]
	[*tc]
]

[[Restore *moonbase *program]
	[is_integer *program] /
	[*moonbase * * * *type : *]
	[add *file_name *type "/" *program ".txt"]
	[Restore *moonbase *file_name]
]

[[Restore *moonbase *file_name]
	[file_reader *fr *file_name]
	[*moonbase * * *cb *type : *]
	[*fr *type]
	[*fr *mono] [*cb *mono] [show [*cb *mono]]
	[SubRestore *moonbase *fr]
	[*fr]
]

[[SubRestore *moonbase *fr]
	[*fr *one] [*fr *two]
	[show *one " => " *two]
	[show [Lunar *two *moonbase : *one]]
	[Lunar *two *moonbase : *one]
	/ [SubRestore *moonbase *fr]
]
[[SubRestore : *]]

[[Activate *core : *moonbase]
	[Lander *base : *moonbase]
	[*base : *io]
	[SELECT
		[[eq *io [* ["LEFT" "RIGHT"]]] [*core "LEFT" *base "LEFT"] [*core "RIGHT" *base "RIGHT"]]
		[[eq *io [* ["SIGNAL"]]] [*core *base]]
	]
]
[[Deactivate *core : *moonbase]
	[Lander *base : *moonbase]
	[*base : *io]
	[SELECT
		[[eq *io [* ["LEFT" "RIGHT"]]] [*core "LEFT" *base "LEFT" []] [*core "RIGHT" *base "RIGHT" []]]
		[[eq *io [* ["SIGNAL"]]] [*core *base []]]
	]
]

private [AddParameterBlock SubRestore]

end .

