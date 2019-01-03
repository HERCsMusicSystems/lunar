
import lunar

program astronaut [BuildAstronaut Astronaut]

[[BuildAstronaut] [BuildAstronaut ****]]

[[BuildAstronaut *astronaut *astronautcb *line *chorus]
  [Moonbase *astronaut *astronautcb Astronaut *line]
  [moonbase *astronautcb] [*astronautcb mono]
  [stereo_chorus *chorus]
  [stereo_pan *pan]
  [stereo_gateway *dry]
  [delay *delay]
  [drywet *drywet]
  [freeverb *freeverb]
  [volume *line]
  [ConnectStereo *line *freeverb]
  [ConnectStereo *freeverb *drywet]
  [ConnectDryWet *drywet *dry *delay]
  [ConnectStereo *delay *dry]
  [ConnectStereo *dry *pan]
  [ConnectStereo *pan *chorus]
  [InsertRamp *line "volume" 6 12800 *astronaut volume]
  [InsertRamp *pan "pan" 6 0 *astronaut pan]
  [Insert *chorus *astronaut chorus]
  [Insert *dry *astronaut delay]
  [Insert *drywet *astronaut delay]
  [Insert *delay *astronaut delay]
  [Insert *freeverb *astronaut freeverb]
  [InsertController 7 *astronaut volume]
  [InsertController 10 -64 *astronaut pan]
  [InsertController 11 *astronaut delay gateway]
  [InsertController 12 -64 *astronaut delay balance]
  [InsertController 13 *astronaut delay feedback]
  [InsertController 14 *astronaut delay time]
  [InsertController 15 *astronaut delay highdamp]
  [InsertController 16 *astronaut chorus level]
  [InsertController 17 *astronaut chorus time]
  [InsertController 18 -64 *astronaut chorus bias]
  [InsertController 19 *astronaut chorus speed]
  [InsertController 20 -64 *astronaut chorus phase]
  [InsertController 21 *astronaut chorus amp]
  [InsertController 22 *astronaut freeverb feedback]
  [InsertController 23 *astronaut freeverb diffusion]
  [InsertController 24 *astronaut freeverb highdamp]
  [InsertController 25 *astronaut freeverb dry]
  [InsertController 26 *astronaut freeverb wet]
]

end .