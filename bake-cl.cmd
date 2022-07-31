
nasm src\_win\chkstk.asm -ochkstk.o -fwin64
cl src\ciabatta.c -c -DCIABATTA_WIN -I inc -I src\_win /Z7 /showIncludes /arch:sse2
lib /nologo /out:ciabatta.lib chkstk.o ciabatta.obj src\fdec64\fdec64.lib src\unicope\unicope.lib
