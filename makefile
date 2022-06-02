
GNUFLAGS=-g -gcodeview -Werror -Wall -Iinc -Icode
CLFLAGS=/Zi /I:inc /link /incremental:no /subsystem:windows /nodefaultlib kernel32.lib

CC=clang
CFLAGS=$(GNUFLAGS)
LDFLAGS=/nologo /nodefaultlib /entry:mainCRTStartup

SRC_DIR := code
OBJ_DIR := build
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.obj,$(SRC_FILES))

ciabatta.lib: $(OBJ_FILES)
	lib $(LDFLAGS) /out:$@ $^

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: ciabatta.lib
