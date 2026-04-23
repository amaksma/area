.PHONY: all clean

CC        = clang
CPPFLAGS  = -I include
CFLAGS    = -std=c99 -MJ $@.json -MMD
CFLAGS   += -Wall -Wextra -Wpedantic -Wconversion -Wimplicit-fallthrough
CFLAGS   += -Werror=format-security -Werror=implicit -Werror=incompatible-pointer-types
CFLAGS   += -Wno-unused-parameter
LDFLAGS   =
LDLIBS    = -lm

SRC := main.c myutility.c
SRC += function/node.c function/parser.c function/print_tree.c function/derivative.c function/simplify.c
SRC += root.c integral.c area.c test.c
OBJ := $(SRC:%.c=build/%.o)
SRC := $(SRC:%=src/%)
DEP := $(OBJ:%.o=%.d)

TYPE ?= Debug
ifeq ($(TYPE), Debug)
	CPPFLAGS += -DDEBUG
	CFLAGS   += -g3 -fsanitize=address,undefined,leak -fno-omit-frame-pointer
	LDFLAGS  += -g3 -fsanitize=address,undefined,leak
endif
ifeq ($(TYPE), Release)
	CPPFLAGS += -DNDEBUG
	CFLAGS   += -O2
	LDFLAGS  += -O2
endif

METHOD ?= Newton
ifeq ($(METHOD), Newton)
	CPPFLAGS += -DNEWTON
endif


all: build/out build/compile_commands.json

build/out: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

-include $(DEP)
build/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

build/compile_commands.json:
	(echo "["; cat $(@D)/*.o.json; echo "]") > $@

clean:
	rm -rf ./build
