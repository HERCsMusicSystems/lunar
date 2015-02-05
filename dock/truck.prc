
import studio
import control
import lunar

program truck [jcb js trucks get_sounds reactor g1 panner effect drw radar]

[[get_sounds]
	[wave trucks "LargeTruck/truck.wav" "LargeTruck/large_truck_start.wav" "LargeTruck/large_truck_idle.wav"]
	[sampler_operator g1 trucks]
	[pan panner] [delay effect] [drywet drw]
	[drw "balance" -8192]
	[ConnectStereo effect panner]
	[ConnectDryWet drw panner effect]
	[panner g1]
]

[[jcb 0 1] [g1 "trigger" 1]]
[[jcb 1 1] [g1 "trigger" 0]]
[[jcb 1.0 *x] [mult *x -1536.0 *y] [g1 "freq" *y]]
[[jcb 2.0 *x] [mult *x 8192.0 *y] [panner "pan" *y]]
[[jcb 3.0 *x] [mac *x 16384.0 -8192.0 *y] [drw "balance" *y]]
[[jcb : *x] [show *x]]

end := [
		[get_sounds]
		[TRY [joystick js jcb]]
		[core reactor 330 22050 4096 0 -1]
		[ConnectStereo reactor drw]
		[ConnectStereo reactor panner]
		;[oscilloscope radar] [radar g1]
		[command]
		] .
