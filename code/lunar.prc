;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This file was create on Monday, 14th July 2014 at 11:36:34 AM. ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program lunar #machine := "prolog.lunar"
			[
				small_keyboard keyboard big_keyboard oscilloscope
				dock undock
				core moonbase operator parameter_block key_map velocity_map impulse trigger porta_trigger mixer stereo_mixer gateway lfo adsr eg
				square_operator saw_operator sensitivity
				signal freq amp ratio sync attack decay sustain release busy time speed wave pulse phase poly
				mono left right mic mic_left mic_right breakpoint
				keyon keyoff
				Lunar Lander Activate Deactivate
				AddParameterBlock
				Moonbase Insert InsertIO Store Restore
			]

#machine small_keyboard := "small_keyboard"
#machine keyboard := "keyboard"
#machine big_keyboard := "big_keyboard"
#machine oscilloscope := "oscilloscope"
;#machine dock := "dock"
;#machine undock := "undock"
#machine parameter_block := "parameter_block"
#machine key_map := "key_map"
#machine velocity_map := "velocity_map"
#machine impulse := "impulse"
#machine trigger := "trigger"
#machine porta_trigger := "porta_trigger"
#machine mixer := "mixer"
#machine stereo_mixer := "stereo_mixer"
#machine lfo := "lfo"
#machine adsr := "adsr"
#machine eg := "eg"
#machine gateway := "gateway"
#machine sensitivity := "sensitivity"
#machine moonbase := "moonbase"

#machine core := "core"
#machine operator := "operator"
#machine square_operator := "square_operator"
#machine saw_operator := "saw_operator"

[[Moonbase *base *distributor]
	[create_atom *modules] [create_atom *parameters]
	[moonbase *distributor]
	[addcl [[*base *parameters *modules]]]
	[addcl [[*modules *distributor]]]
]

[[AddParameterBlock *parameters *parameter *module *selector *initial]
	[APPEND *selector [*parameter] *selectors]
	[*parameters *pb : *selectors] /
	[text_term *name *parameter]
	[*module *name *pb]
]
[[AddParameterBlock *parameters *parameter *module *selector *initial]
	[parameter_block *pb]
	[*pb *initial]
	[APPEND *selector [*parameter] *selectors]
	[addcl [[*parameters *pb : *selectors]]]
	[text_term *name *parameter]
	[*module *name *pb]
]

[[Insert *operator *base : *selector]
	[*base *parameters *modules : *]
	[addcl [[*modules *operator : *selector]]]
	[*operator : *io]
	[InsertIO *parameters *operator *selector *io]
]

[[InsertIO *parameters *operator *selector [["FREQ" "AMP" "RATIO" "SYNC" : *] *o]]
	[AddParameterBlock *parameters freq *operator *selector 0]
	[AddParameterBlock *parameters amp *operator *selector 0]
	[AddParameterBlock *parameters ratio *operator *selector 1]
	[AddParameterBlock *parameters sync *operator *selector 0]
]

[[InsertIO *parameters *adsr *selector [["TRIGGER" "ATTACK" "DECAY" "SUSTAIN" "RELEASE" : *] *]]
	[AddParameterBlock *parameters attack *adsr *selector 0]
	[AddParameterBlock *parameters decay *adsr *selector 0]
	[AddParameterBlock *parameters sustain *adsr *selector 0]
	[AddParameterBlock *parameters release *adsr *selector 0]
]

[[InsertIO *parameters *trigger *selector [["BUSY" "TIME" : *] *]]
	[AddParameterBlock *parameters time *trigger *selector 0]
]

[[InsertIO *parameters *lfo *selector [["SPEED" "WAVE" "PULSE" "PHASE" "SYNC" : *] *]]
	[AddParameterBlock *parameters speed *lfo *selector 0]
	[AddParameterBlock *parameters wave *lfo *selector 0]
	[AddParameterBlock *parameters pulse *lfo *selector 0]
	[AddParameterBlock *parameters phase *lfo *selector 0]
	[AddParameterBlock *parameters sync *lfo *selector 0]
]

[[InsertIO *parameters *sensitivity *selector [["ENTER" "BREAKPOINT" "LEFT" "RIGHT" : *] *]]
	[AddParameterBlock *parameters breakpoint *sensitivity *selector 0]
	[AddParameterBlock *parameters left *selector *sensitivity -128]
	[AddParameterBlock *parameters right *sensitivity *selector 128]
]

[[InsertIO *parameters *mixer *selector [["ENTER" : *] *]]]

[[InsertIO *parameters *operator *selector [*i *o]] [show "Failed inserting IO at " [*parameters *operator *i *o]]]

[[Lander *base *moonbase : *selector] [*moonbase * *modules : *] [*modules *base : *selector]]

[[Lunar *v *moonbase : *selector] [*moonbase *parameters : *] [*parameters *base : *selector] [*base 0 *v]]

[[Store *moonbase *file_name]
	[*moonbase *parameters *modules : *]
	[file_writer *tc *file_name]
	[TRY
		[*parameters *orbiter : *selector]
		[*orbiter *x]
		[show *selector " => " *x]
		[*tc [*selector] " => " [*x] "\n"]
		fail
	]
	[*tc]
]

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

private [AddParameterBlock]

end .

