let SessionLoad = 1
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd ~/src/termwrap
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +1 driver.cpp
badd +1 cell_style.cpp
badd +40 cell_style.hpp
badd +1 main.cpp
badd +131 /opt/termbox/include/termbox.h
badd +27 driver.hpp
badd +1 1
badd +1 error.hpp
badd +1 demo.cpp
badd +15 impl/optional.hpp
badd +1 key_event.hpp
badd +1 types.hpp
badd +194 /opt/gcc-7.1.0/include/c++/7.1.0/chrono
badd +3700 ~/.gdbinit
badd +11 CMakeLists.txt
badd +0 ~/src/termbox/src/utf8.c
argglobal
silent! argdel *
argadd driver.cpp
set stal=2
edit error.hpp
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 64 - ((49 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
64
normal! 0122|
tabedit /opt/termbox/include/termbox.h
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 314 - ((50 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
314
normal! 023|
tabedit ~/src/termbox/src/utf8.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 43 - ((35 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
43
normal! 017|
tabedit driver.cpp
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 119 + 118) / 237)
exe 'vert 2resize ' . ((&columns * 117 + 118) / 237)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 191 - ((49 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
191
normal! 050|
wincmd w
argglobal
edit driver.hpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 47 - ((28 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
47
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 119 + 118) / 237)
exe 'vert 2resize ' . ((&columns * 117 + 118) / 237)
tabedit demo.cpp
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 102 - ((12 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
102
normal! 013|
tabedit types.hpp
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 21 - ((20 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
21
normal! 017|
tabedit key_event.hpp
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 13 - ((12 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
13
normal! 011|
tabedit cell_style.cpp
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
set nosplitbelow
set nosplitright
wincmd t
set winminheight=1 winminwidth=1 winheight=1 winwidth=1
exe 'vert 1resize ' . ((&columns * 119 + 118) / 237)
exe 'vert 2resize ' . ((&columns * 117 + 118) / 237)
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 74 - ((40 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
74
normal! 09|
wincmd w
argglobal
edit cell_style.hpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let s:l = 63 - ((62 * winheight(0) + 36) / 72)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
63
normal! 010|
wincmd w
exe 'vert 1resize ' . ((&columns * 119 + 118) / 237)
exe 'vert 2resize ' . ((&columns * 117 + 118) / 237)
tabnext 5
set stal=1
if exists('s:wipebuf') && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 winminheight=1 winminwidth=1 shortmess=filnxtToO
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
