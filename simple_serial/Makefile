CFLAGS		+= -std=c99
CFLAGS		+= -Wall -Wextra
CFLAGS		+= -O2 -fno-inline -fno-unroll-loops -fno-vectorize -fno-slp-vectorize

ifneq ($(LMCO_PATH),)
  CC		:= clang
  CFLAGS	+= -fplugin=$(LMCO_PATH)
  CFLAGS	+= -fpass-plugin=$(LMCO_PATH)
  CFLAGS	+= -mllvm -limoncello-config=limoncello.yml
endif

.PHONY: all
all: simple_serial

simple_serial: main.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -fr simple_serial
