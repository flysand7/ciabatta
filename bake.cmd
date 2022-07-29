
nasm src\_win\chkstk.asm -ochkstk.o -fwin64
clang src\ciabatta.c -o ciabatta.obj -c -DCIABATTA_WIN -I inc -I src\_win -nodefaultlibs -g -mfma
lib /nologo /out:ciabatta.lib chkstk.o ciabatta.obj src\fdec64\fdec64.lib src\unicope\unicope.lib
