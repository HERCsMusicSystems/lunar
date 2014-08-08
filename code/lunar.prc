
; This file was create on Monday, 14th July 2014 at 11:36:34 AM.

import studio

program lunar #machine := "prolog.lunar"
			[
				small_keyboard keyboard big_keyboard oscilloscope
				dock undock
				core moonbase operator parameter_block key_map impulse trigger mixer gateway lfo
				freq amp ratio sync
				keyon keyoff
				Lunar GetLander DockLander
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

[[AddParameterBlock *parameters *parameter *module *selector *name]
	[parameter_block *pb]
	[APPEND *selector [*parameter] *selectors]
	[addcl [[*parameters *pb : *selectors]]]
	[*module *name *pb]
]

[[DockLander moonbase *base]
	[create_atom *modules]
	[create_atom *parameters]
	[moonbase *moonbase]
	[addcl [[*base *parameters *modules]]]
	[addcl [[*modules *moonbase]]]
]

[[DockLander operator *moonbase : *selector]
	[*moonbase *parameters *modules : *]
	[operator *operator]
	[addcl [[*modules *operator : *selector]]]
	[parameter_block *freq]
	[AddParameterBlock *parameters freq *operator *selector "FREQ"]
	[AddParameterBlock *parameters amp *operator *selector "AMP"]
]

[[GetLander *base *moonbase : *selector] [*moonbase * *modules : *] [*modules *base : *selector]]

[[Lunar *v *moonbase : *selector] [*moonbase *parameters : *] [*parameters *base : *selector] [*base 0 *v]]

private [AddParameterBlock]

end .
