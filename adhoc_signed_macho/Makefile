CC		:= $(shell xcrun -f clang)

SDK_PATH	:= $(shell xcrun --show-sdk-path --sdk iphoneos)
LD_LIBRARY_PATH	:= $(SDK_PATH)/usr/lib

TFLAGS		:= -isysroot $(SDK_PATH) -arch arm64e
CFLAGS		:= $(TFLAGS) -I$(SDK_PATH)/usr/include \
 			-DTARGET_OS_IPHONE=1 -DTARGET_OS_BRIDGE=0 \
 			-DPROGRAM=\"$(PROGRAM)\"
LDFLAGS		:= $(TFLAGS)
CPPFLAGS	:= $(TFLAGS) -DUSE_GETCWD

.PHONY: all
all: program

program: program.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
	codesign -vvvvv -s - --force $@

.PHONY: clean
clean:
	rm -f program
