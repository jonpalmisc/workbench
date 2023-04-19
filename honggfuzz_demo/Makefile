CFLAGS		+= -g -Wall -Wextra -O2

CORPUS_DIR	?= corpus
WORK_DIR	?= hf_work

HF		?= honggfuzz
HFFLAGS		?= -i $(CORPUS_DIR) -W $(WORK_DIR) -x

all: weak_format

weak_format: weak_format.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: fuzz
fuzz: weak_format
	mkdir -p $(WORK_DIR)
	$(HF) $(HFFLAGS) -x -- ./$< ___FILE___

.PHONY: clean
clean:
	rm -fr weak_format *.dSYM $(WORK_DIR)
