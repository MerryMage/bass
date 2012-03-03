arch snes.cpu

define styleA 1

if {styleA} == 1
  macro read data
    db {data},{data}
  endmacro
else
  macro read data
    db {data}
  endmacro
endif

macro write data
  if {data} == 1
    db {data}
  else
    db $bb
  endif
endmacro

org $0000; fill $8000
org $0000
  if {styleA} == 1
    db $ff
  elseif {styleA} == 1
    db $ee
  endif

  {read $12}
  {write 1}
