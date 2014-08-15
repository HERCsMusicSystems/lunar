
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program lunar_eclipse [reactor base radar eagle kb]

[[eagle]
	[Dock moonbase base *]
	[Dock mixer base mixer]
	[Dock operator base operator 1]
	[Dock adsr base adsr 1]
	[Dock porta_trigger base trigger]

	[Dock [base operator 1] [trigger]]
	[Dock [base adsr 1] [trigger trigger]]
	[Dock [base operator 1 amp] [adsr 1]]
	[Dock [base mixer] [operator 1]]

	[Lunar 9000 base trigger time]
	[Lunar 7000 base adsr 1 attack]
	[Lunar 9000 base adsr 1 release]


	[oscilloscope radar]
	[Lander *trig base trigger]
	[small_keyboard kb *trig]
	[Lander *mix base mixer]
	[radar *mix]

	[Activate reactor base mixer]

]

end := [
		[core reactor 330 22050 1024]
		[eagle]
		[command]
		] .
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
