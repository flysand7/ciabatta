for /R src\%PLATFORM% %%F in (*.c) do (
    echo %%F
    start /B clang -Isrc/win -c -o bin\%PLATFORM%\%%~nF.obj %%F %CIABATTA_OPTIONS%
)
for /R src\code %%F in (*.c) do (
    echo %%F
    start /B clang -c -o bin\%%~nF.obj %%F %CIABATTA_OPTIONS%
)
