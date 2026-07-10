let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
doautoall SessionLoadPre
silent only
silent tabonly
cd ~/Documents/RadioPhobia/radiophobia
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
set shortmess+=aoO
badd +20 main/radiophobia.c
badd +31 components/wifiStuff/deauther.c
badd +25 components/wifiStuff/include/deauther.h
badd +150 components/wifiStuff/beaconScanner.c
badd +8 components/wifiStuff/include/beaconScanner.h
badd +1 CMakeLists.txt
badd +22 ~/Documents/RadioPhobia/raspberrypi_testing/include/80211.h
badd +36 ~/.espressif/tools/esp-clang/esp-20.1.1_20250829/esp-clang/lib/clang-runtimes/riscv32-esp-unknown-elf/rv32imac-zicsr-zifencei_ilp32/include/sys/_stdint.h
badd +316 ~/.espressif/v6.0.1/esp-idf/components/esp_wifi/include/esp_wifi_types_generic.h
badd +1 ~/.espressif/v6.0.1/esp-idf/components/esp_event/include/esp_event_base.h
badd +27 ~/.espressif/v6.0.1/esp-idf/components/esp_libc/src/port/riscv/memcpy.c
badd +20 ~/.espressif/tools/esp-clang/esp-20.1.1_20250829/esp-clang/lib/clang/20/include/__stddef_size_t.h
badd +3 components/wifiStuff/CMakeLists.txt
badd +1013 ~/.espressif/v6.0.1/esp-idf/components/esp_wifi/include/esp_wifi.h
badd +4 main/CMakeLists.txt
badd +1579 ~/.espressif/v6.0.1/esp-idf/components/freertos/FreeRTOS-Kernel/tasks.c
badd +1 ~/.espressif/v6.0.1/esp-idf/components/log/include/esp_log.h
badd +7 components/captivePortal/captivePortal.c
badd +5 components/wifiStuff/wifiSetup.c
badd +99 ~/.espressif/v6.0.1/esp-idf/examples/wifi/getting_started/softAP/main/softap_example_main.c
badd +1 ~/.espressif/v6.0.1/esp-idf/components/esp_hw_support/include/esp_mac.h
badd +179 ~/.espressif/v6.0.1/esp-idf/components/nvs_flash/src/nvs_api.cpp
badd +5 components/wifiStuff/include/wifiSetup.h
badd +159 ~/.espressif/v6.0.1/esp-idf/components/lwip/lwip/src/include/lwip/inet.h
badd +2639 ~/.espressif/v6.0.1/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c
argglobal
%argdel
set stal=2
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit main/radiophobia.c
argglobal
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 20 - ((19 * winheight(0) + 20) / 41)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 20
normal! 0
lcd ~/Documents/RadioPhobia/radiophobia
tabnext
edit ~/Documents/RadioPhobia/radiophobia/components/wifiStuff/wifiSetup.c
argglobal
balt ~/.espressif/v6.0.1/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 7 - ((6 * winheight(0) + 20) / 41)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 7
normal! 035|
lcd ~/Documents/RadioPhobia/radiophobia
tabnext
edit ~/Documents/RadioPhobia/radiophobia/components/wifiStuff/beaconScanner.c
argglobal
balt ~/.espressif/v6.0.1/esp-idf/components/esp_wifi/include/esp_wifi_types_generic.h
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 131 - ((10 * winheight(0) + 20) / 41)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 131
normal! 019|
lcd ~/Documents/RadioPhobia/radiophobia
tabnext
edit ~/Documents/RadioPhobia/radiophobia/components/captivePortal/captivePortal.c
argglobal
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 7 - ((6 * winheight(0) + 20) / 41)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 7
normal! 0
lcd ~/Documents/RadioPhobia/radiophobia
tabnext
edit ~/Documents/RadioPhobia/radiophobia/components/wifiStuff/deauther.c
argglobal
balt ~/.espressif/v6.0.1/esp-idf/components/log/include/esp_log.h
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 49 - ((30 * winheight(0) + 20) / 41)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 49
normal! 013|
lcd ~/Documents/RadioPhobia/radiophobia
tabnext 4
set stal=1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
