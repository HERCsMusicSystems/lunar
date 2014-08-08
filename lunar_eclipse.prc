
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program lunar_eclipse [base op1 op2 radar tg map manual lf gate eagle reactor]

[[eagle]
	[Dock moonbase base]
	[Dock operator base operator 1]
	[oscilloscope radar]
	[Lander *op base operator 1]
	[Dock [base] "mono" [operator 1]]
	[radar *op]
]

end := [[core reactor 330 22050 1024] [eagle] [command]] .
end := [
			[auto_atoms]
			;[keyboard manual tg]
			[core base 330 22050 1024]
			[trigger tg]
			[key_map map]
				[tg map]
			[operator op1]
			[operator op2]
				[op2 "AMP" -3000]
				[op2 "FREQ" 1]
			[oscilloscope radar]
				[radar op1]
				[op1 "SHIFT" op2]
				[op2 "SYNC" tg "TRIGGER"]
				[op1 "SYNC" tg "TRIGGER"]
				[base op1]
				[op1 "FREQ" tg "KEY"]
				[op2 "FREQ" tg "KEY"]
			[op1 : *x] [show [op1 : *x]]
			[tg : *y] [show [tg : *y]]
			[base : *z] [show [base : *z]]
			[lfo lf]
				[lf "wave" 1]
			[gateway gate]
				[gate "ENTER" lf]
				[gate "gateway" 128]
				[op1 "FREQ" gate]
				[op2 "FREQ" gate]
			[command]
		] .
