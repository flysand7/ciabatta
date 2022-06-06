@echo off
setlocal enabledelayedexpansion

set PLATFORM=win32
set CIABATTA_OPTIONS=--crt none -I %% -I inc

del ciabatta.lib
cuik %CIABATTA_OPTIONS% code\*.c platform\%PLATFORM%\*.c -c -o ciabatta.obj
lib /out:ciabatta.lib ciabatta.obj

cuik test\test.c --lib ciabatta.lib,kernel32.lib,user32.lib,shell32.lib %CIABATTA_OPTIONS%
del ciabatta.obj
