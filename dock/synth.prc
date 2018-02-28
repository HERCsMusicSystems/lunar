
import studio
import setup
import vonforman

program synth [synth_setup synth_close vc-1 VF-0]

[[synth_setup]
  [auto_atoms]
  [BuildVonForman 66 4]
  [CommandCentre commander]
  ;[VonFormanEditor VF-0 6 6]
  [setup]
]

[[synth_close]
  [TRY [radar]] [TRY [commander]]
  [close]
  [CloseAllMoons]
]

end := [[synth_setup] [batch "VonFormanEditor.prb"] [gtk_command] [synth_close]] .
