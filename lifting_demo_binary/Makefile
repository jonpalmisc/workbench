CFLAGS		+= -O1

LIB_T		:= libdemo.dylib
LIB_S		:= demo.c

.PHONY: all
all: $(LIB_T)

$(LIB_T): $(LIB_S)
	$(CC) $(CFLAGS) -shared -o $@ $^

.PHONY: clean
clean:
	rm -f $(LIB_T)
