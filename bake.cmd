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

set CIABATTA_OPTIONS=-Iinc -Wall -g -gcodeview -nodefaultlibs -D_CRT_SECURE_NO_WARNINGS
set PLATFORM=win

if "%1"=="test" (
    goto :skip_crt_compilation
)
:: For each C file in code/ we check whether it's OS-dependent.
:: If so, we check whether the platform is the chosen one. If not, we ignore
:: that file (set ok=0). Otherwise we compile it.
:: Man, batch files are sure scary, when you try to do something serious
:: it took me a lot of time to figure out how to write this if it breaks
:: im gonna be seriously disappointed :kekw:
del ciabatta.lib 2> nul
for /R code %%F in (*.c) do (
    set ok=1
    set os_dependent=0
    set is_cur_os=0
    echo %%F | findstr /c:"%~pd0code\os" > nul
    if !errorlevel! neq 1 (
        set os_dependent=1
    )
    echo %%F | findstr /c:"%~pd0code\os\%PLATFORM%" > nul
    if !errorlevel! neq 1 (
        set is_cur_os=1
    )
    if "!os_dependent!"=="1" if "!is_cur_os!"=="0" (
        set ok=0
    )
    if "!ok!"=="1" (
        echo %%F
        clang -c -o build\%%~nF.obj %%F %CIABATTA_OPTIONS%
    )
)
llvm-ar rc ciabatta.lib build\*.obj
del build\*.obj

:skip_crt_compilation
echo Compiling test..
clang test\test5.c ciabatta.lib -std=c11 -lkernel32 -luser32 -lshell32 -nostdlib %CIABATTA_OPTIONS%
