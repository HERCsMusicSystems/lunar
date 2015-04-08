
import lunar

program alarm [Alarm BuildAlarm AlarmEditor]


[[BuildAlarm]
	[GenerateInstrumentName Alarm *alarm]
	[BuildAlarm *alarm * *]
]

[[BuildAlarm *alarm *alarmcb *volume]
	[create_atoms *alarm *alarmcb *volume]
	[Moonbase *alarm *alarmcb Alarm]
	[moonbase *alarmcb]
	[pan *pan]
	[delay *delay]
	[drywet *drywet]
	[volume *volume]
	[ConnectStereo *delay *pan]
	[ConnectDryWet *drywet *pan *delay]
	[ConnectStereo *volume *drywet]
	[Insert *volume *alarm]
	[Insert *pan *alarm]
	[Insert *drywet *alarm delay]
	[Insert *delay *alarm delay]
	;;;;;;;;;
	[trigger *trigger] [adsr *adsr] [vco *vco] [lfo *lfo]
	[control *vibrato] [control *tremolo] [control *autopan]
	[*vibrato *lfo] [*tremolo "enter" *lfo "negative"] [*autopan *lfo]
	[*alarmcb *trigger]
	[*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[*vco "amp" *adsr]
	[*vco "amp" *tremolo]
	[*vco "freq" *trigger "key"]
	[*vco "freq" *vibrato]
	[*pan *vco]
	[*pan "pan" *autopan]
	[Insert *trigger *alarm trigger]
	[Insert *vco *alarm vco]
	[Insert *adsr *alarm adsr]
	[Insert *lfo *alarm lfo]
	[Insert *vibrato *alarm lfo vibrato]
	[Insert *tremolo *alarm lfo tremolo]
	[Insert *autopan *alarm lfo pan]
	[AddMoon *alarm *alarmcb *volume]
]


end .
