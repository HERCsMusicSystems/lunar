
; This file was create on Thursday, 31st July 2014 at

import studio
import conductor
import control
import lunar
import phobos
import abakos

program eclipse [
					commander
					BuildMidi mdi BuildJoystick js jscb
					PhobosCB AbakosCB
					reactor left_radar right_radar radar
					BuildAudio RemoveAudio
					phobos_mixer abakos_mixer
					shm
				]


[[@ lunar . LunarDrop : *command] [show *command]]

[[BuildAudio] [core reactor 330 32000 1024 30 15] [show "USB Audio Device ready."]/]
[[BuildAudio] [core reactor 330 22050 2048] [show "Motherboard Audio Device ready."]/]
[[BuildAudio] [core reactor 330 22050 4096 0 -1] [show "Windows Audio Device ready."]/]
[[BuildAudio] [show "Audio device not found."]]
[[RemoveAudio] [reactor []] [reactor] [show "Audio Device stopped."]/]
[[RemoveAudio] [show "Audio Device not present."]]

[[shm]
	[list Moons]
	[FOR *i 0 15 1 [MIDI_CHANNELS *i : *m] [show "Callback " *i " = " *m]]
]

[[BuildMidi] [midi mdi income_midi] /]
[[BuildMidi] [show "MIDI Device not found."]]

[[BuildJoystick] [joystick js jscb]/]
[[BuildJoystick] [show "Joystick not found."]]

[[jscb 11 1] [BuildAudio]]
[[jscb 10 1] [RemoveAudio]]
[[jscb 9 1] [ConnectAllMoons reactor]]
[[jscb : *x] [show *x]]

end := [
		[BuildPhobos 2 Phobos PhobosCB phobos_mixer *feed]
		[BuildMidi] [BuildJoystick]
		;[build_Abakos *Abakos abakos_mixer]
		;[oscilloscope radar]
		[CommandCentre commander CCCB]
		[commander 3000 1180]
		;[radar 2000 1180]
		;[TRY [midi mdi mdicb]]
		;[TRY [joystick js jcb]]
		;[radar *feed]
		;[audio]
		/ [command] /
		[TRY [mdi]]
		[TRY [js]]
		[TRY [commander]]
		;[Moonbase Phobos]
		;[Moonbase *Abakos]
		[CloseAllMoons]
		] .

