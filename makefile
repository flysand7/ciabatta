
# TODO: make shell variable-based path
CLFLAGS=/X /GS- /Iinc /Icode $(shell test\inctoarg)

CC=clang-cl
CFLAGS=$(CLFLAGS)
LDFLAGS=/nologo /nodefaultlib /entry:mainCRTStartup /subsystem:console

SRC_DIR := code
OBJ_DIR := build
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.obj,$(SRC_FILES))

ciabatta.lib: $(OBJ_FILES)
	lib $(LDFLAGS) /out:$@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	del /F /Q build\*
	del /F /Q lib\ciabatta.lib

inctoarg:
	cl test\inctoarg.c

.PHONY: inctoarg ciabatta.lib
