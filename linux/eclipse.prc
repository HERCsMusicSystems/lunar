
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program eclipse [commander cb]

;[[cb "Attack" : *command] [show *command]]
[[cb *ret 1 *v] [add *ret "Attack = " *v]]
[[cb *ret 2 *v] [add *ret "Decay = " *v]]
[[cb *ret 3 *v] [add *ret "Sustain = " *v]]
[[cb *ret 4 *v] [add *ret "Release = " *v]]
[[cb *ret 5 *x *y] [add *ret "Vector = [" *x " / " *y "]"]]

end := [
		;[core reactor 330 22050 2048]
		[CommandCentre commander cb]
		[command]
		] .

