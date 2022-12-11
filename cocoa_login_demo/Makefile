CFLAGS		?= -Wall -Wextra
LDFLAGS		?= -framework Cocoa

ALL_S		:= $(wildcard *.m)
ALL_H		:= $(wildcard *.h)

DEMO_T		:= LoginDemo

.PHONY: all
all: $(DEMO_T)

$(DEMO_T): $(ALL_S) $(ALL_H)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(ALL_S)

.PHONY: clean
clean:
	rm $(DEMO_T)
