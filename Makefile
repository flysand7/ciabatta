
CC=clang
SRC_DIR := src
OBJ_DIR := bin
IFLAGS := -Iinc -Isrc/win -Iunicope/inc

# Detect target operating system
ifeq ($(OS),Windows_NT) 
    PLATFORM := win
else
	PLATFORM := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
	PLATFORM := $(shell sh -c 'echo $(PLATFORM) | tr A-Z a-z')
endif
ifeq ($(PLATFORM),Unknown)
	echo Unknown platform
	exit 1
endif

# If we're compiling under windows we'll link to these libraries
ifeq ($(PLATFORM),win)
	LIBS := -lDbghelp -lkernel32 -luser32 -lshell32
	MKDIR_P_FLAG := 
	NUL_FILE := NUL
else
	LIBS := -lrt -lpthread -ldl
	MKDIR_P_FLAG := '-p'
	NUL_FILE := /dev/null
endif

# Compiler flags
ifeq ($(CC), clang)
	CFLAGS=$(GNUFLAGS) -Wall -msse2 -mfma $(IFLAGS)
else
	echo BAD CC
	exit 1
endif

# Figure out what we want to compile at the end
SRC_FILES := \
	$(wildcard $(SRC_DIR)/code/*.c)  \
    $(wildcard $(SRC_DIR)/code/**/*.c)  \
    $(wildcard $(SRC_DIR)/$(PLATFORM)/*.c) \
    $(wildcard $(SRC_DIR)/$(PLATFORM)/**/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.obj,$(SRC_FILES))

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c
	@-mkdir $(MKDIR_P_FLAG) "$(dir $@)" 2> $(NUL_FILE)
	$(CC) $(CFLAGS) -c -o $@ $<

ciabatta.lib: $(OBJ_FILES) unicope/unicope.lib
	llvm-ar rc $@ $^

unicope/unicope.lib:
	unicope\bake


test: ciabatta.lib
	clang -g test/test_$(test).c ciabatta.lib -std=c11 $(LIBS) -nostdlib -Iinc

clean:
	rd/s/q bin || true
	rm -Rf bin || true
	rm -f ciabatta.lib || true
	del ciabatta.lib || true

.PHONY: ciabatta.lib test
