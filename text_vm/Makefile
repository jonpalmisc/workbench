TARGET = x86_64-linux-gnu

CC := zig cc -target $(TARGET)
CFLAGS := -std=c99 -Wextra -Wall

OUT := out
BIN := $(OUT)/text_vm

SRC := $(wildcard *.c)

# ===------------------------------------------------------------------------===

.PHONY: fmt clean

$(BIN): $(patsubst %.c, $(OUT)/%.o, $(SRC))
	$(CC) $(CFLAGS) $^ -o $@

$(OUT)/%.o: %.c $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

# ===------------------------------------------------------------------------===

dist: CFLAGS += -O1 -s -fno-inline-functions
dist: $(BIN)

debug: CFLAGS += -g
debug: $(BIN)

# ===------------------------------------------------------------------------===

$(OUT):
	mkdir $@

clean:
	rm -fr $(OUT)

fmt: $(SRC) $(wildcard *.h)
	clang-format -i $^
