
; This file was create on Monday, 14th July 2014 at 11:36:34 AM.

import studio

program lunar #machine := "prolog.lunar"
			[
				small_keyboard keyboard big_keyboard oscilloscope
				dock undock
				moonbase operator
			]

#machine small_keyboard := "small_keyboard"
#machine keyboard := "keyboard"
#machine big_keyboard := "big_keyboard"
#machine oscilloscope := "oscilloscope"
#machine dock := "dock"
#machine undock := "undock"

#machine moonbase := "moonbase"
#machine operator := "operator"

end := [[command]] .
