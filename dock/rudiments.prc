
import conductor
import lunar

program rudiments [
					R01_SingleStrokeRoll R02_SingleStrokeFour R03_SingleStrokeSeven
					R04_MultipleBounceRoll
					]



[[R01_SingleStrokeRoll *cb]
	[beat] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[beat] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
]

[[R02_SingleStrokeFour *cb]
	[beat] [*cb keyon 66 100]
	[wt 4] [*cb keyon 66 100]
	[wt 4] [*cb keyon 66 100]
	[wt 4] [*cb keyon 66 100]
	[beat] [*cb keyon 66 100]
	[wt 4] [*cb keyon 66 100]
	[wt 4] [*cb keyon 66 100]
	[wt 4] [*cb keyon 66 100]
]

[[R03_SingleStrokeSeven *cb]
	[beat] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[beat] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
	[wt 2] [*cb keyon 66 100]
]

[[R04_MultipleBounceRoll *cb]
	[beat] [*cb keyon 66 100]
		[wt] [*cb keyon 66 80]
		[wt] [*cb keyon 66 80]
		[wt] [*cb keyon 66 80]
		[wt] [*cb keyon 66 80]
		[wt] [*cb keyon 66 80]
	[wt 1] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[beat] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
	[wt 6] [*cb keyon 66 100]
]
end .

