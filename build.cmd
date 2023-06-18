
:: Compile UTF-8 resource into .obj file
:: this .obj file has to be linked to the executable using it, NOT archived
:: together with ciabatta.lib.
windres -o utf8.obj utf8.rc

:: Compile chkstk
nasm src\os_win\chkstk.asm -ochkstk.o -fwin64

:: Compile the rest of the party
clang -Wall src\ciabatta.c -o ciabatta.obj -c -DCIABATTA_WIN -I src -I include -nodefaultlibs -g -mfma
rem cuik src\ciabatta.c -o ciabatta.obj -c -DCIABATTA_WIN -I src -I inc -nostdlib
lib /nologo /out:ciabatta.lib chkstk.o utf8.obj ciabatta.obj
