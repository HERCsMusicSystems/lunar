
import lunar

program astronaut [BuildAstronaut Astronaut]

[[BuildAstronaut *astronaut *astronautcb *line *feed]
  [Moonbase *astronaut *astronautcb Astronaut *line]
  [moonbase *astronautcb] [*astronautcb mono]
  [stereo_pan *pan]
  [stereo_gateway *dry]
  [delay *delay]
  [drywet *drywet]
  [volume *line]
  [ConnectStereo *volume *drywet]
  [ConnectDryWet *drywet *dry *delay]
  [ConnectStereo *dry *pan]
  [ConnectStereo *pan *feed]
  [Insert *volume *alarm]
  [Insert *pan *alarm]
  [Insert *dry *alarm delay]
  [Insert *drywet *alarm delay]
  [Insert *delay *alarm delay]
]

end .
