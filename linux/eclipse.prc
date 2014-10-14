
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program eclipse [
					commander cb cb_path moon_base moon mooncb phobos_base phobos phoboscb
					paths modules adjacent next_path previous_path next_module previous_module
					build_abakos build_abakos_part abakos abakoscb
					reactor
					AT sub
				]

[[AT * *x [*x]]/]
[[AT 0 *x [*x : *]]/]
[[AT *i *x [*a : *b]] [-- *i *i1] / [AT *i1 *x *b]]

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

[[sub *moon [*i : *is] *path [*pp : *tail]]
	[isallr *p *pp [*moon * : *path]]
	[NODUP *p *pn]
	[AT *i *pp *pn]
	/ [sub *moon *is *path *tail]
]
[[sub * * * []]]

[[cb *ret 1 *v] [add *ret "Attack = " *v]]
[[cb *ret 2 *v] [add *ret "Decay = " *v]]
[[cb *ret 3 *v] [add *ret "Sustain = " *v]]
[[cb *ret 4 *v] [add *ret "Release = " *v]]
[[cb *ret 5 *x *y] [add *ret "Vector = [" *x " / " *y "]"]]

[[cb *ret [] *delta]
	;[add *ret "delta [" *delta "]"]
	[cb_path : *path]
	[eq *path [*m : *p]]
	[*m *parameters : *]
	[*parameters *pb : *p]
	[*pb *v1]
	[add *v1 *delta *v2]
	[*pb *v2]
	[*pb : *v3]
	[text_term *pather *path]
	[add *ret *pather " = " *v3]
]

[[cb *program *i1 : *is]
	[isallr *y *x [Moons *x]]
	[AT *i1 *moon *y]
	[*moon *parameters : *]
	[sub *parameters *is *path *path]
	[text_term *pather [*moon : *path]]
	[*parameters *pb : *path] [*pb : *v]
	[add *program *pather " = " *v]
	[cb_path [*moon : *path]]
]


[[moon_base]
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
	[addcl [[Moons moon]]]
]

[[phobos_base]
	[Moonbase phobos phoboscb]
	[operator *op1]
	[lfo *lfo1]
	[delay *dl1]
	[Insert *dl1 phobos delay]
	[Insert *lfo1 phobos lfo]
	[Insert *op1 phobos operator]
	[addcl [[Moons phobos]]]
]

[[build_abakos *mixer]
	[Moonbase abakos abakoscb]
	[mixer *mixer]
	[build_abakos_part abakos *mixer abakoscb]
	[build_abakos_part abakos *mixer abakoscb]
	[addcl [[Moons abakos]]]
]

[[build_abakos_part *abakos *mixer *cb]
	[operator *op1]
	[adsr *adsr1]
	[trigger *trigger] [*cb *trigger]
	[*op1 "freq" *trigger "key"]
	[*adsr1 "trigger" *trigger "trigger"]
	[*op1 "amp" *adsr1 "signal"]
	[*mixer *op1]
	[Insert *op1 *abakos operator]
	[Insert *adsr1 *abakos adsr]
]

end := [
		[core reactor 330 22050 2048]
		[var cb_path]
		[CommandCentre commander cb]
		[build_abakos *abakos_mixer]
		[moon_base] [phobos_base]
		[reactor *abakos_mixer]
		[command]
		] .
