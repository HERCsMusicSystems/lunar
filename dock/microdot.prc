
import lunar

program microdot [BuildMicrodot MicrodotEditor Lfo-1 Adsr-1 Vco-1 Filter-1]

[[MicrodotEditor *microdot]
	[BuildLfoPanel Lfo-1 *microdot lfo] [Lfo-1 100 100]
	[BuildAdsrPanel Adsr-1 *microdot adsr] [Adsr-1 430 100]
	[BuildFilterPanel Filter-1 *microdot filter] [Filter-1 430 250]
]

[[MicrodotEditor] [TRY [Lfo-1]] [TRY [Adsr-1]]]

[[BuildMicrodot] [BuildMicrodot * * *]]
[[BuildMicrodot *microdot *microdotcb *volume]
	[Moonbase *microdot *microdotcb Microdot *volume]
	[moonbase *microdotcb] [*microdotcb mono]
	[pan *pan] [delay *delay] [drywet *drywet] [volume *volume]
	[ConnectStereo *volume *drywet]
	[ConnectDryWet *drywet *pan *delay]
	[ConnectStereo *delay *pan]
	[Insert *volume *microdot]
	[Insert *pan *microdot]
	[Insert *drywet *microdot delay]
	[Insert *delay *microdot delay]
	;;;;;;;;;;;;;;;;;
	[key_map *key_map] [trigger *trigger] [adsr *adsr] [vco *vco] [filter *vcf] [lfo *lfo]
	[*microdotcb *trigger] [*trigger *key_map] [*adsr "trigger" *trigger "trigger"] [*trigger "busy" *adsr "busy"] [*lfo "trigger" *adsr "busy"]
	[*vco "freq" *trigger "key"] [*vco "freq" *lfo "vibrato"]
	[*vcf "gain" *adsr] [*vcf "amp" *lfo "tremolo"] [*vcf "freq" *lfo "wahwah"]
	[*vcf *vco] [*pan *vcf] [*pan "pan" *lfo "pan"]
	[parameter_block *pitch "index"] [gateway *pitchfreq] [*pitchfreq *pitch] [*vco "freq" *pitchfreq]
	[parameter_block *modulation "index"] [gateway *vibrato] [*vibrato *modulation] [*lfo "vibrato" *vibrato]
	[gateway *filter_key_sensitivity] [*filter_key_sensitivity *trigger] [*vcf "freq" *filter_key_sensitivity]
	[Insert *trigger *microdot portamento]
	[Insert *vco *microdot vco]
	[InsertPB *pitch *microdot vco pitch]
	[Insert *pitchfreq *microdot vco sens]
	[Insert *vcf *microdot filter]
	[Insert *filter_key_sensitivity *microdot filter sens]
	[Insert *adsr *microdot adsr]
	[Insert *lfo *microdot lfo]
	[InsertFor *microdot lfo vibrato]
	[InsertFor *microdot lfo tremolo]
	[InsertFor *microdot lfo wahwah]
	[InsertFor *microdot lfo pan]
	[InsertPB *modulation *microdot lfo modulation]
	[Insert *vibrato *microdot lfo sens]
	[InsertBlock *key_map *microdot key_map]
	[InsertController 1 *microdot lfo modulation]
	[InsertController 79 *microdot lfo vibrato]
	[InsertController 16 *microdot lfo tremolo]
	[InsertController 17 *microdot lfo wahwah]
	[InsertController 7 *microdot volume]
	[InsertController 10 -64 *microdot pan]
	[InsertController 5 *microdot portamento time]
	[InsertController 64 *microdot portamento hold]
	[InsertController 65 *microdot portamento porta]
	[InsertController 84 *microdot portamento legato]
	[InsertController 74 -64 *microdot filter freq]
	[InsertController 71 *microdot filter resonance]
	[InsertController 77 -64 *microdot delay balance]
	[InsertController 78 -64 *microdot lfo speed]
	[InsertController 73 *microdot adsr attack]
	[InsertController 75 *microdot adsr decay]
	[InsertController 76 -128 *microdot adsr sustain]
	[InsertController 72 *microdot adsr release]
	[InsertController 128 -64 *microdot vco pitch]
	[Lunar 512 *microdot vco sens]
	[Lunar 256 *microdot lfo sens]
]

end .
