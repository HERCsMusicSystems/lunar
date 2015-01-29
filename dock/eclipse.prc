
; This file was create on Thursday, 31st July 2014 at

import studio
import conductor
import control
import lunar
import phobos
import abakos

program eclipse [
					commander cb_edit_path moon mooncb
					PhobosCB AbakosCB
					reactor left_radar right_radar radar
					kb kbcb
					mdi mdicb
					js jcb
					MM audio phobos_mixer abakos_mixer
					shm
				]


[[@ lunar . LunarDrop : *command] [show *command]]

[[mdicb *command * : *t]
	;[show [PhobosCB *command : *t]]
	[PhobosCB *command : *t]
]

[[jcb 0.0 *v]
	[times *v 64.0 *vv] [add *vv 64.0 *vvv] [trunc *vvv *vvvv]
	[PhobosCB control 12 *vvvv]
]

[[jcb 1.0 *v]
	[times *v -64.0 *vv] [add *vv 64.0 *vvv] [trunc *vvv *vvvv]
	[PhobosCB control 13 *vvvv]
]

[[audio] [core reactor 330 32000 1024 30 15] [ConnectStereo reactor phobos_mixer] [ConnectStereo reactor abakos_mixer]]
[[audio] [core reactor 330 22050 2048] [ConnectStereo reactor phobos_mixer] [ConnectStereo reactor abakos_mixer]]

[[MM *base]
	[*base *parameters *modules *callback : *]
	[delallcl *base]
	[TRY [*parameters *parameter : *selector] [*parameter] fail]
	[TRY [*modules *module : *selector] [show *selector *module] [*module []] [*module] fail]
	[delallcl *parameters]
	[delallcl *modules]
]

[[shm]
	[list Moons]
	[FOR *i 0 15 1 [MIDI_CHANNELS *i : *m] [show "Callback " *i " = " *m]]
]

end := [
		;[BuildPhobos 2 Phobos PhobosCB phobos_mixer *feed]
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
		;[TRY [mdi]]
		;[TRY [js]]
		[commander]
		;[Moonbase Phobos]
		;[Moonbase *Abakos]
		] .

