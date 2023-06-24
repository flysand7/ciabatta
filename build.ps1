
# Generate the resources for UTF8
windres -o utf8.obj utf8.rc

# Compile chkstk
nasm src\os_win\chkstk.asm -o chkstk.o -fwin64

# Compile the rest of the party
clang -Wall src\ciabatta.c -o ciabatta.obj -c -DCIABATTA_WIN -I src -I include -nodefaultlibs -g -mfma
# rem cuik src\ciabatta.c -o ciabatta.obj -c -DCIABATTA_WIN -I src -I inc -nostdlib

# Package up the .lib file
lib /nologo /out:ciabatta.lib chkstk.o utf8.obj ciabatta.obj
