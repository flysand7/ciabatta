
clang src\unity.c -o ciabatta.obj -c -I inc -I fdec64 -I unicope\inc -I src\win -nodefaultlibs -g  -mfma
lib /out:ciabatta.lib ciabatta.obj fdec64\fdec64.lib unicope\unicope.lib
