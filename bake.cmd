
:: Compile UTF-8 resource into .obj file
:: this .obj file has to be linked to the executable using it, NOT archived
:: together with ciabatta.lib.
rem windres -o utf8.obj utf8.rc

:: Compile chkstk
nasm src\_win\chkstk.asm -ochkstk.o -fwin64

:: Compile the rest of the party
rem clang -Wall src\ciabatta.c -o ciabatta.obj -c -DCIABATTA_WIN -I inc -I src\_win -nodefaultlibs -g -mfma
cuik src\ciabatta.c -o ciabatta.obj -c -DCIABATTA_WIN -I inc -I src\_win -nostdlib
lib /nologo /out:ciabatta.lib chkstk.o ciabatta.obj
