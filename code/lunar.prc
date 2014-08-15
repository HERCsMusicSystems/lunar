;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This file was create on Monday, 14th July 2014 at 11:36:34 AM. ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program lunar #machine := "prolog.lunar"
			[
				small_keyboard keyboard big_keyboard oscilloscope
				dock undock
				core moonbase operator parameter_block key_map velocity_map impulse trigger porta_trigger mixer stereo_mixer gateway lfo adsr eg
				square_operator
				signal freq amp ratio sync mono left right attack decay sustain release busy time
				mono left right mic mic_left mic_right
				keyon keyoff
				Lunar Lander Dock Activate Deactivate
				AddParameterBlock FindModuleAndPort
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
#machine moonbase := "moonbase"

#machine core := "core"
#machine operator := "operator"
#machine square_operator := "square_operator"

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

[[Dock moonbase *base *moonbase]
	[create_atom *modules]
	[create_atom *parameters]
	[moonbase *moonbase]
	[addcl [[*base *parameters *modules]]]
	[addcl [[*modules *moonbase]]]
]

[[Dock mixer *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[mixer *mixer]
	[addcl [[*modules *mixer : *selector]]]
]

[[Dock stereo_mixer *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[stereo_mixer *mixer]
	[addcl [[*modules *mixer : *selector]]]
]

[[Dock trigger *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[trigger *trigger]
	[addcl [[*modules *trigger : *selector]]]
]

[[Dock porta_trigger *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[porta_trigger *trigger]
	[addcl [[*modules *trigger : *selector]]]
	[AddParameterBlock *parameters time *trigger *selector 0]
]

[[Dock operator *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[square_operator *operator]
	[addcl [[*modules *operator : *selector]]]
	[AddParameterBlock *parameters freq *operator *selector 0]
	[AddParameterBlock *parameters amp *operator *selector 0]
	[AddParameterBlock *parameters ratio *operator *selector 1]
	[AddParameterBlock *parameters sync *operator *selector 0]
]

[[Dock adsr *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[adsr *adsr]
	[addcl [[*modules *adsr : *selector]]]
	[AddParameterBlock *parameters attack *adsr *selector 0]
	[AddParameterBlock *parameters decay *adsr *selector 0]
	[AddParameterBlock *parameters sustain *adsr *selector 0]
	[AddParameterBlock *parameters release *adsr *selector 0]
]

[[Dock [*moonbase : *destination_selector] *source_selector] /
	[FindModuleAndPort *moonbase *destination_selector *destination *destination_port]
	[FindModuleAndPort *moonbase *source_selector *source *source_port]
	[*destination *destination_port *source *source_port]
]

[[FindModuleAndPort *moonbase *path *module 0] [Lander *module *moonbase : *path] /]
[[FindModuleAndPort *moonbase *path *module *port]
	[APPEND *selector [*i_port] *path]
	[Lander *module *moonbase : *selector] /
	[FindModuleAndPort *i_port *port]
]
[[FindModuleAndPort *i_port *port] [is_atom *i_port] / [text_term *port *i_port]]
[[FindModuleAndPort *port *port]]

[[Lander *base *moonbase : *selector] [*moonbase * *modules : *] [*modules *base : *selector]]

[[Lunar *v *moonbase : *selector] [*moonbase *parameters : *] [*parameters *base : *selector] [*base 0 *v]]

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

private [AddParameterBlock FindModuleAndPort]

end .

