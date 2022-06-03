@echo off
setlocal enabledelayedexpansion

set PLATFORM=win32
set CIABATTA_OPTIONS=-Iinc -g -gcodeview -nodefaultlibs -D_CRT_SECURE_NO_WARNINGS

for /R code %%F in (*.c) do (
	echo %%F
	clang -c -o build\%%~nF.obj %%F %CIABATTA_OPTIONS%
)
for /R platform\%PLATFORM% %%F in (*.c) do (
	echo %%F
	clang -c -o build\%%~nF.obj %%F %CIABATTA_OPTIONS%
)
llvm-ar rc ciabatta.lib build\*.obj

clang test\test.c ciabatta.lib -lkernel32 -luser32 -lshell32 -nostdlib %CIABATTA_OPTIONS%
