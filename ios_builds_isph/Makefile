CFLAGS		+= -Wall -Wextra
CFLAGS		+= -Os

.PHONY: all
all: build2enum.cmp build2enum.phf

build2enum.cmp: main.c build.c
	$(CC) $(CFLAGS) -o $@ $^

build2enum.phf: main.c  build.c
	$(CC) $(CFLAGS) -DCONFIG_PHF -o $@ $^

build.c: build.in.c build.h build_table.h build_cmp.h make_headers.py
	./perfect_hash.py --hft 2 --trials 100 --indent 8 --width 80 -o build.c builds.txt build.in.c

build.h build_table.h build_cmp.h: builds.txt
	./make_headers.py

.PHONY: clean
clean:
	rm -fr build2enum.cmp build2enum.phf build.h build_table.h build_cmp.h build.c
