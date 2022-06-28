
GNUFLAGS=-Werror -Wall -Iinc -Isrc/win
CLFLAGS=/I:inc /I:src/win /link /incremental:no /subsystem:windows /nodefaultlib kernel32.lib 

CC=clang
CFLAGS=$(GNUFLAGS)
LDFLAGS=/nologo /nodefaultlib /entry:mainCRTStartup

SRC_DIR := src
OBJ_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/code/*.c) $(wildcard $(SRC_DIR)/win/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.obj,$(SRC_FILES))

ciabatta.lib: $(OBJ_FILES)
	lib $(LDFLAGS) /out:$@ $^

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: ciabatta.lib
