@echo off
setlocal enabledelayedexpansion

set PLATFORM=win
set CIABATTA_OPTIONS=--crt none -I %% -I inc -I unicope/inc

del ciabatta.lib
del unicope\unicope.lib
cuik unicope\src\unicode.c -I unicope\inc -c -o unicope\unicope.lib
cuik %CIABATTA_OPTIONS% src\code\*.c src\%PLATFORM%\*.c -c -o ciabatta.obj
lib /out:ciabatta.lib ciabatta.obj

if "%TEST%"=="" set TEST=assert

cuik test\test_%TEST%.c --lib ciabatta.lib,unicope/unicope.lib,kernel32.lib,user32.lib,shell32.lib %CIABATTA_OPTIONS%
del ciabatta.obj
