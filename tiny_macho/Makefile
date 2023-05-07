SDK_PATH	:= $(shell xcrun --show-sdk-path --sdk macosx)

LDFLAGS		+= -L$(SDK_PATH)/usr/lib -lSystem

.PHONY: all
all: tiny

tiny: tiny.o
	$(LD) $(LDFLAGS) -o $@ $<

tiny.o:	tiny.s
	$(AS) -o $@ $<

.PHONY: clean
clean:
	rm -f tiny tiny.o
