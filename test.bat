
clang -g test\%1.c -I include utf8.obj -nostdlib -mfma -lciabatta.lib
if %errorlevel%==0 (
    "./a.exe"
)
