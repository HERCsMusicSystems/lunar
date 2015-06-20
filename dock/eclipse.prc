
import studio
import conductor
import control
import lunar

program eclipse []


[[LunarDrop *x *y]]
[[LunarDrop *x *y *file : *files] [batch *file] / [LunarDrop *x *y : *files]]

end := [
		[TRY [midi Midi income_midi]]
		[CorePanel Core]
		[command]
		[TRY [Midi]]
		] .
