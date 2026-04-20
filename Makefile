.PHONY: all clean analyze

CC        = clang
CPPFLAGS  = -I include
CFLAGS    = -std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wimplicit-fallthrough -MJ $@.json -MMD
LFLAGS    =
LLIBS     = -lm

SRC := main.c myutility.c function/node.c function/parser.c function/print_tree.c function/derivative.c function/simplify.c
OBJ := $(SRC:%.c=build/%.o)
SRC := $(SRC:%=src/%)
DEP := $(OBJ:%.o=%.d)

TYPE ?= Debug
ifeq ($(TYPE),Debug)
	CPPFLAGS += -DDEBUG
	CFLAGS   += -g3 -fsanitize=address,undefined,leak -fno-omit-frame-pointer
	LFLAGS   += -g3 -fsanitize=address,undefined,leak
else
	CPPFLAGS += -DNDEBUG
endif


all: build build/out build/compile_commands.json

build/out: $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) $(LLIBS) -o $@

-include $(DEP)
build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

build/compile_commands.json:
	(echo "["; cat build/*.o.json; echo "]") > $@

build:
	mkdir $@

clean:
	rm -rf ./build
