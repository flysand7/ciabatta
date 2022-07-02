@echo off
setlocal enabledelayedexpansion

if not exist src\code\unicode\data.h (
    py src\code\unicode\compile.py
)

set PLATFORM=win
set CIABATTA_OPTIONS=--crt none -I %% -I inc

del ciabatta.lib
cuik %CIABATTA_OPTIONS% src\code\*.c src\%PLATFORM%\*.c -c -o ciabatta.obj
lib /out:ciabatta.lib ciabatta.obj

if "%TEST%"=="" set TEST=assert

cuik test\test_%TEST%.c --lib ciabatta.lib,kernel32.lib,user32.lib,shell32.lib %CIABATTA_OPTIONS%
del ciabatta.obj
