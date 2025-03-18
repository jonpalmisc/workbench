CFLAGS		+= -O2
CFLAGS		+= -Wall -Wextra -Wpedantic

.PHONY: all
all: example libptl.dylib

example: main.c libptl.dylib
	$(CC) $(CFLAGS) -L. -lptl -o $@ $<

libptl.dylib: library.c
	$(CC) $(CFLAGS) -shared -o $@ $^

.PHONY: clean
clean:
	rm -f example libptl.dylib
