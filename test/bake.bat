@echo off
pushd %~dp0

cl /nologo inctoarg.c
inctoarg > temp
set /P includes=<temp
del temp

cl /Zi /X /c /I ..\inc %includes% /nologo /GS- /Gm- /GR- /EHa- /Oi /W4 test.c
::clang -c -Wall test.c -otest1.obj

link /nologo test.obj /nodefaultlib user32.lib kernel32.lib ..\ciabatta.lib -debug -subsystem:console
::link /nologo test1.obj /nodefaultlib user32.lib kernel32.lib ..\lib\ciabatta.lib -subsystem:console

popd
