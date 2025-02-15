CFLAGS		+= -Wall -Wextra

GPERF		?= gperf
GPERFFLAGS	+= -t -G -LC
GPERFFLAGS	+= -l -m 100

.PHONY: all
all: build2enum.phf build2enum.naive

build2enum.naive: main.c convert_naive.h
	$(CC) $(CFLAGS) -DCONVERT_NAIVE -o $@ $<

build2enum.phf: main.c convert_phf.h
	$(CC) $(CFLAGS) -DCONVERT_PHF -o $@ $<

convert_phf.h: builds.h config.gperf
	$(GPERF) $(GPERFFLAGS) -H build_hash -N build_from_string config.gperf >$@

builds.h convert_naive.h config.gperf: generate.py
	./generate.py

.PHONY: clean
clean:
	rm -fr build2enum.naive build2enum.phf convert_naive.h convert_phf.h builds.h config.gperf
