ifneq ($(IOS),)
  CC		:= $(shell xcrun -f clang)
  CFLAGS	:= -arch arm64e -isysroot $(shell xcrun --show-sdk-path --sdk iphoneos)
  CFLAGS	+= -DTARGET_OS_IPHONE=1 -DTARGET_OS_BRIDGE=0
endif

CFLAGS		+= -O2 -fobjc-arc
CFLAGS		+= -Wall -Wextra
LDFLAGS		+= -framework Foundation -framework IOKit

STORECTL_BIN	:= storectl
STORECTL_SRC	:= $(wildcard *.m)

.PHONY: all
all: $(STORECTL_BIN)

$(STORECTL_BIN): $(STORECTL_SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	strip $@
#	codesign --force -s - --entitlements entitlements.plist $@

.PHONY: clean
clean:
	rm -fr $(STORECTL_BIN)
