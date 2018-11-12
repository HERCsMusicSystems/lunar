
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
  [InsertController 10 *astronaut pan]
  [InsertController 11 *astronaut delay gateway]
]

end .
