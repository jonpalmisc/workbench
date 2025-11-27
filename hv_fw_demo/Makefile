CFLAGS	+= -std=c99
CFLAGS	+= -Wall -Wextra
CFLAGS	+= -g

LDFLAGS	+= -framework Hypervisor

.PHONY: all
all: demo

demo: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	codesign -s - --entitlements entitlements.plist $@

.PHONY: clean
clean:
	rm -fr demo
