.PHONY: all clean

CC        = clang
CPPFLAGS  = -I include
CFLAGS    = -std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wimplicit-fallthrough -MJ $@.json -MMD
LFLAGS    =
LLIBS     = -lm

SRC := main.c myutility.c
SRC += function/node.c function/parser.c function/print_tree.c function/derivative.c function/simplify.c
SRC += vector.c array.c root.c integral/square_vec.c integral/simpson.c
OBJ := $(SRC:%.c=build/%.o)
SRC := $(SRC:%=src/%)
DEP := $(OBJ:%.o=%.d)

TYPE ?= Debug
ifeq ($(TYPE), Debug)
	CPPFLAGS += -DDEBUG
	CFLAGS   += -g3 -fsanitize=address,undefined,leak -fno-omit-frame-pointer
	LFLAGS   += -g3 -fsanitize=address,undefined,leak
endif
ifeq ($(TYPE), Release)
	CPPFLAGS += -DNDEBUG
	CFLAGS   += -O2
	LFLAGS   += -O2
endif

METHOD ?= Newton
ifeq ($(METHOD), Newton)
	CPPFLAGS += -DNEWTON
endif


all: build/out build/compile_commands.json

build/out: $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) $(LLIBS) -o $@

-include $(DEP)
build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

build/compile_commands.json:
	(echo "["; cat $(@D)/*.o.json; echo "]") > $@

clean:
	rm -rf ./build
