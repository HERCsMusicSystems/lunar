
import lunar

program alarm [BuildAlarm AlarmEditor Lfo-1 Adsr-1 Vco-1]



[[AlarmEditor *alarm]
	[BuildLfoPanel Lfo-1 *alarm [lfo] [lfo vibrato] [lfo tremolo] [] [lfo pan]] [Lfo-1 100 100]
	[BuildAdsrPanel Adsr-1 *alarm adsr] [Adsr-1 410 100]
]

[[AlarmEditor] [TRY [Lfo-1]] [TRY [Adsr-1]]]

[[BuildAlarm] [BuildAlarm * * *]]

[[BuildAlarm *alarm *alarmcb *volume]
	[Moonbase *alarm *alarmcb Alarm *volume]
	[moonbase *alarmcb] [*alarmcb mono]
	[pan *pan] [delay *delay] [drywet *drywet] [volume *volume]
	[ConnectStereo *volume *drywet]
	[ConnectDryWet *drywet *pan *delay]
	[ConnectStereo *delay *pan]
	[Insert *volume *alarm]
	[Insert *pan *alarm]
	[Insert *drywet *alarm delay]
	[Insert *delay *alarm delay]
	;;;;;;;;;;;;;;;;;
	[trigger *trigger] [adsr *adsr] [vco *vco] [lfo *lfo]
	[*alarmcb *trigger] [*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"]
	[*vco "freq" *trigger "key"] [*vco "freq" *lfo "vibrato"] [*vco "amp" *adsr] [*vco "amp" *lfo "tremolo"]
	[*pan *vco] [*pan "pan" *lfo "pan"]
	[Insert *trigger *alarm portamento]
	[Insert *vco *alarm vco]
	[Insert *adsr *alarm adsr]
	[Insert *lfo *alarm lfo]
	[InsertController 1 *alarm lfo vibrato]
	[InsertController 71 *alarm lfo tremolo]
	[InsertController 7 *alarm volume]
	[InsertController 10 -64 *alarm pan]
	[InsertController 11 *alarm portamento time]
	[InsertController 64 *alarm portamento hold]
	[InsertController 65 *alarm portamento porta]
	[InsertController 68 *alarm portamento legato]
	[InsertController 74 -64 *alarm vco freq]
	[InsertController 91 -64 *alarm delay balance]
	[InsertController 95 -64 *alarm lfo speed]
	[InsertController 73 *alarm adsr attack]
	[InsertController 93 *alarm adsr decay]
	[InsertController 94 -128 *alarm adsr sustain]
	[InsertController 72 *alarm adsr release]
]

end .
