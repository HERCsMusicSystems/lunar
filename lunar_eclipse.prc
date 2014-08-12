
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program lunar_eclipse [base op1 op2 radar tg map manual lf gate eagle reactor t moon mapper]

[[t *v] [Lander *adsr base adsr] [*adsr *v]]

[[eagle]
	[Dock moonbase base moon]
	[key_map mapper]
	[moon mapper]
	[Dock mixer base signal]
	[Dock mixer base mic]
	[Dock operator base operator 1]
	[Dock adsr base adsr]
	[Dock [base signal] [operator 1]]
	[Dock [base operator 1 amp] [adsr]]
	[Dock [base signal] [mic]]

	[oscilloscope radar]

	[Lander *base_signal base signal]
	[radar *base_signal]

	[Lander *base_mic base mic]
	[*base_mic reactor]

	[Lunar 9728 base adsr attack]
	[Lunar 9728 base adsr decay]
	[Lunar -4096 base adsr sustain]
	[Lunar 9728 base adsr release]
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
