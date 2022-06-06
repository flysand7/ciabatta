@echo off
setlocal enabledelayedexpansion

set CIABATTA_OPTIONS=-Iinc -g -gcodeview -nodefaultlibs -D_CRT_SECURE_NO_WARNINGS
set PLATFORM=win32
if NOT "%1"=="fast" (

	del ciabatta.lib
	for /R code %%F in (*.c) do (
		echo %%F
		clang -c -o build\%%~nF.obj %%F %CIABATTA_OPTIONS%
	)
	for /R platform\%PLATFORM% %%F in (*.c) do (
		echo %%F
		clang -c -o build\%%~nF.obj %%F %CIABATTA_OPTIONS%
	)
	llvm-ar rc ciabatta.lib build\*.obj
	del build\*.obj
)

echo Compiling test..
clang test\test.c ciabatta.lib -std=c11 -lkernel32 -luser32 -lshell32 -nostdlib %CIABATTA_OPTIONS%

