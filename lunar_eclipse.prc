
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program lunar_eclipse [reactor base radar eagle kb blep op dl tg eg distro tr1 tr2 tr3 wv smpl sampler filtering noise fir vect callback panel delay_test pn pln ppn]

[[eagle]
	[Moonbase base *]
	[mixer *mixer]              [Insert *mixer base mixer]
	[saw_operator *operator]    [Insert *operator base operator 1]
	[adsr *adsr]                [Insert *adsr base adsr 1]
	[porta_trigger *trigger]    [Insert *trigger base trigger]

	[*operator *trigger]
	[*adsr "trigger" *trigger "trigger"]
	[*operator "amp" *adsr]
	[*mixer *operator]

	[Lunar 9000 base trigger time]
	[Lunar 7000 base adsr 1 attack]
	[Lunar 9000 base adsr 1 release]

	[oscilloscope radar]
	[small_keyboard kb *trigger]
	[radar *mixer]

	[Activate reactor base mixer]

]

[[distro]
	[Moonbase base kb]
	[trigger tr1]
	[trigger tr2]
	[trigger tr3]
	[kb tr1]
	[kb tr2]
	[kb tr3]
]

[[sampler]
	[wave wv "../truck_horn.wav.bin" "../../dock/algos/cs/sfx/starship.wav" "../../dock/algos/cs/sfx/submarine.wav" "../../dock/algos/cs/sfx/jetplane.wav"]
	[sampler_operator smpl wv]
	[reactor smpl]
	[noise_operator noise] [oscilloscope radar] [radar noise]
]

[[filtering]
	[square_operator *op]
	[filter fir]
	[oscilloscope radar]
	[fir *op] [radar fir]
]

[[delay_test]
	[operator op]
	[trigger tg]
	[adsr eg]
	[oscilloscope radar] [radar 3000 1000]
	[op "amp" eg]
	[eg "trigger" tg "trigger"]
	[op tg]
	[delay dl]
	[pan pn] [power_pan ppn] [linear_pan pln]
	[radar op]
]

[[callback *x *y] [times *x 64.0 *xx] [times *y 64.0 *yy] [show [vect *xx *yy]]]
[[callback : *command] [show *command]]

end := [
		[core reactor 330 22050 2048]
		;[eagle]
		;[distro]
		;[sampler]
		;[filtering]
		;[vector vect callback]
		[control_panel panel callback] [panel 2400 1000]
		[delay_test]
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
