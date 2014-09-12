
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program eclipse [
					commander cb moonbase moon mooncb
					paths modules adjacent next_path previous_path next_module previous_module
				]

[[paths *base]
	[*base *parameters : *]
	[isallr *list *el [*parameters *module : *el]]
	[show *list]
]

[[modules *base]
	[*base * *modules : *]
	[isallr *list *el [*modules *module : *el]]
	[show *list]
]

[[adjacent [*p1 *p2 : *] *p1 *p2] /]
[[adjacent [* : *p] *p1 *p2] / [adjacent *p *p1 *p2]]

[[next_path *base *p1 *p2]
	[*base *p : *]
	[isallr *list *el [*p * : *el]]
	[adjacent *list *p1 *p2]
]
[[next_path *base *p *p]]

[[previous_path *base *p1 *p2]
	[*base *p : *]
	[isall *list *el [*p * : *el]]
	[adjacent *list *p1 *p2]
]
[[previous_path *base *p *p]]

[[cb *ret 1 *v] [add *ret "Attack = " *v]]
[[cb *ret 2 *v] [add *ret "Decay = " *v]]
[[cb *ret 3 *v] [add *ret "Sustain = " *v]]
[[cb *ret 4 *v] [add *ret "Release = " *v]]
[[cb *ret 5 *x *y] [add *ret "Vector = [" *x " / " *y "]"]]
[[cb *program *id] [add "parameter [" *id "]" *program]]

[[moonbase]
	[Moonbase moon mooncb]
	[adsr *adsr1]
	[operator *op1]
	[adsr *adsr2]
	[operator *op2]
	[lfo *lfo1]
	[lfo *lfo2]
	[noise_operator *noise]
	[filter *fir]
	[Insert *op1 moon operator 1]
	[Insert *op2 moon operator 2]
	[Insert *noise moon noise]
	[Insert *adsr1 moon adsr 1]
	[Insert *adsr2 moon adsr 2]
	[Insert *lfo1 moon lfo 1]
	[Insert *lfo2 moon lfo 2]
	[Insert *fir moon filter]
]

end := [
		;[core reactor 330 22050 2048]
		[CommandCentre commander cb]
		[moonbase]
		[command]
		] .

