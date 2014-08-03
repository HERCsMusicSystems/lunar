
; This file was create on Thursday, 31st July 2014 at

import studio
import lunar

program lunar_eclipse [base op1 op2 radar tg map manual]

;end := [[command]] .
end := [
			[auto_atoms]
			[keyboard manual tg]
			[moonbase base 330 22050 1024]
			[trigger tg]
			[key_map map]
				;[map tg]
			[operator op1]
			[operator op2]
				[op2 "AMP" -3000]
				[op2 "FREQ" 1]
			;[oscilloscope radar]
				;[radar op1]
				[op1 "SHIFT" op2]
				[op2 "SYNC" tg "TRIGGER"]
				[op1 "SYNC" tg "TRIGGER"]
				[base op1]
				[op1 "FREQ" tg "KEY"]
				[op2 "FREQ" tg "KEY"]
			[command]
		] .
