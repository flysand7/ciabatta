@echo off
cl /c /I ..\inc /nologo /Gm- /GR- /EHa- /Oi /W4 /Z7 test.c
clang -c -Wall test.c -otest1.obj

link /nologo test.obj /nodefaultlib user32.lib kernel32.lib ..\lib\ciabatta.lib -subsystem:console
link /nologo test1.obj /nodefaultlib user32.lib kernel32.lib ..\lib\ciabatta.lib -subsystem:console