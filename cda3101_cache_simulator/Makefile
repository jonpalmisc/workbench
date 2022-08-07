OUT := ./build
BIN := $(OUT)/cachesim

INPUT := input/gcc.trace
LINE_COUNTS := 16 32 64
SET_SIZES := 2 4 8

HEADER := type,size,lines,blocksize,span,sets,policy,rate

FIFO_RF := report/fifolru.csv
SETS_RF := report/setsize.csv
ALL_RF := report/overall.csv

cachesim:
	@mkdir -p $(OUT)
	@go build -o $(BIN) ./cmd/cachesim/main.go

fastats: cachesim
	rm -f $(FIFO_RF)
	touch $(FIFO_RF)

	echo $(HEADER) >> $(FIFO_RF)
	for lines in 4 8 16 32; do \
	for policy in fifo lru; do \
		$(BIN) -csv -kind fa -lines $$lines -width 16 -policy $$policy $(INPUT) >> $(FIFO_RF); \
	done \
	done

	cd report; ./plot_fifolru.py; cd ..

sastats: cachesim
	rm -f $(SETS_RF)
	touch $(SETS_RF)

	echo $(HEADER) >> $(SETS_RF)
	for span in 2 4 8; do \
	for lines in 8 16 32; do \
		$(BIN) -csv -kind sa -lines $$lines -width 16 -policy lru -span $$span $(INPUT) >> $(SETS_RF); \
	done \
	done

	cd report; ./plot_setsize.py; cd ..

oastats:
	rm -f $(ALL_RF)
	touch $(ALL_RF)

	echo $(HEADER) >> $(ALL_RF)
	for lines in 16 32 64; do \
		$(BIN) -csv -kind dm -lines $$lines -width 32 -policy lru $(INPUT) >> $(ALL_RF); \
		$(BIN) -csv -kind fa -lines $$lines -width 32 -policy lru $(INPUT) >> $(ALL_RF); \
		$(BIN) -csv -kind sa -lines $$lines -width 32 -span 8 -policy lru $(INPUT) >> $(ALL_RF); \
	done

	cd report; ./plot_overall.py; cd ..

report: fastats sastats oastats
	cd report; lualatex report.tex; cd ..

fmt:
	go fmt cachesim/...
