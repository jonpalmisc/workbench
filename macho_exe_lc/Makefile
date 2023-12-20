.PHONY: all
all: program1 program2 libfakelib.dylib

program1: program1.c libfakelib.dylib
	$(CC) $(CFLAGS) -L. -lfakelib -o $@ $<
	install_name_tool -change libfakelib.dylib program2 program1

program2: program2.c libfakelib.dylib
	$(CC) $(CFLAGS) -L. -lfakelib -o $@ $<

libfakelib.dylib: fakelib.c
	$(CC) $(CFLAGS) -shared -o $@ $<

.PHONY: clean
clean:
	rm -fr program1 program2 libfakelib.dylib
