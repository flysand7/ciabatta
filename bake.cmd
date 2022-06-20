@echo off
setlocal enabledelayedexpansion

:: Make sure that if I ctrl+c out of the script the dir is popped
:: If there's anything that requires user input, this script will break
:: in that case I recommend you removing the 'echo y' part.
:: although you will have to confirm termination of the script twice
if "%~1" neq "_start_" (
    pushd %~pd0
    echo y | cmd /c "%~f0" _start_ %*
    popd
    exit /b
)
shift /1

set CIABATTA_OPTIONS=-Iinc -Wall -g -gcodeview -nodefaultlibs -D_CRT_SECURE_NO_WARNINGS -mfma
set PLATFORM=win

if "%1"=="test" (
    goto :skip_crt_compilation
)

if not exist src\code\unicode\unicode_data.h (
    py src\code\unicode\unicode_compile.py
)

if exist bin rd/s/q bin
mkdir bin
mkdir bin\%PLATFORM%

del ciabatta.lib 2> nul
for /R src\%PLATFORM% %%F in (*.c) do (
    echo %%F
    clang -Isrc/win -c -o bin\%PLATFORM%\%%~nF.obj %%F %CIABATTA_OPTIONS%
)
for /R src\code %%F in (*.c) do (
    echo %%F
    clang -c -o bin\%%~nF.obj %%F %CIABATTA_OPTIONS%
)
llvm-ar rc ciabatta.lib bin\*.obj bin\%PLATFORM%\*.obj

:skip_crt_compilation

if "%TEST%"=="" set TEST=assert

echo Compiling test_%TEST%.c
clang -fno-builtin test\test_%TEST%.c ciabatta.lib -std=c11 -lDbghelp -lkernel32 -luser32 -lshell32 -nostdlib %CIABATTA_OPTIONS%
::cl test\test_math.c /Iinc -D_CRT_SECURE_NO_WARNINGS /Z7 /link ciabatta.lib kernel32.lib user32.lib shell32.lib -nostdlib -nodefaultlibs
