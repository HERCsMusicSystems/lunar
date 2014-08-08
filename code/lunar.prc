;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This file was create on Monday, 14th July 2014 at 11:36:34 AM. ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

import studio

program lunar #machine := "prolog.lunar"
			[
				small_keyboard keyboard big_keyboard oscilloscope
				dock undock
				core moonbase operator parameter_block key_map impulse trigger mixer gateway lfo
				freq amp ratio sync mono left right
				keyon keyoff
				Lunar Lander Dock Activate Deactivate
				AddParameterBlock
			]

#machine small_keyboard := "small_keyboard"
#machine keyboard := "keyboard"
#machine big_keyboard := "big_keyboard"
#machine oscilloscope := "oscilloscope"
;#machine dock := "dock"
;#machine undock := "undock"
#machine parameter_block := "parameter_block"
#machine key_map := "key_map"
#machine impulse := "impulse"
#machine trigger := "trigger"
#machine mixer := "mixer"
#machine lfo := "lfo"
#machine gateway := "gateway"
#machine moonbase := "moonbase"

#machine core := "core"
#machine operator := "operator"

[[AddParameterBlock *parameters *parameter *module *selector *initial]
	[parameter_block *pb]
	[*pb *initial]
	[APPEND *selector [*parameter] *selectors]
	[addcl [[*parameters *pb : *selectors]]]
	[text_term *name *parameter]
	[*module *name *pb]
]

[[Dock moonbase *base]
	[create_atom *modules]
	[create_atom *parameters]
	[moonbase *moonbase]
	[addcl [[*base *parameters *modules]]]
	[addcl [[*modules *moonbase]]]
]

[[Dock operator *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[operator *operator]
	[addcl [[*modules *operator : *selector]]]
	[parameter_block *freq]
	[AddParameterBlock *parameters freq *operator *selector 0]
	[AddParameterBlock *parameters amp *operator *selector 0]
	[AddParameterBlock *parameters ratio *operator *selector 1]
	[AddParameterBlock *parameters sync *operator *selector 0]
]

[[Dock [*moonbase : *destination_selector] *destination_port *source_selector *source_port] /
	[Lander *destination *moonbase : *destination_selector]
	[Lander *source *moonbase : *source_selector]
	[*destination *destination_port *source *source_port]
]

[[Dock [*moonbase : *destination_selector] *port *source_selector] / [Dock [*moonbase : *destination_selector] *port *source_selector 0]]

[[Lander *base *moonbase : *selector] [*moonbase * *modules : *] [*modules *base : *selector]]

[[Lunar *v *moonbase : *selector] [*moonbase *parameters : *] [*parameters *base : *selector] [*base 0 *v]]

[[Activate *core *moonbase] [Lander *base *moonbase] [*core *base] [*core 1 *base 1] [*core 2 *base 2]]
[[Deactivate *core *moonbase] [Lander *base *moonbase] [*core *base []] [*core 1 *base 1 []] [*core 2 *base 2 []]]

private [AddParameterBlock]

end .
