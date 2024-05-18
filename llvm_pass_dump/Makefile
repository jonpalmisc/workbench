LLVM_BIN_DIR	:= /opt/homebrew/opt/llvm/bin

CXXFLAGS	+= -std=c++17
CXXFLAGS	+= -O3 
CXXFLAGS	+= -emit-llvm
CXXFLAGS	+= -fno-exceptions -fno-rtti
CXXFLAGS	+= -Wall -Wextra


.PHONY: all
all: dummy

dummy: dummy.s
	$(LLVM_BIN_DIR)/clang $<

dummy.s: dummy.opt.ll
	$(LLVM_BIN_DIR)/llc -o $@ $<

dummy.opt.ll: dummy.ll
	@echo ""
	$(LLVM_BIN_DIR)/opt -O3 -print-pipeline-passes $< 2>/dev/null
	@echo ""
	$(LLVM_BIN_DIR)/opt -O3 -S -o $@ $<

dummy.ll: main.ll
	$(LLVM_BIN_DIR)/llvm-link -S -o $@ $<

main.ll: main.cpp
	$(LLVM_BIN_DIR)/clang $(CXXFLAGS) -S $<

.PHONY: clean
clean:
	rm -fr dummy *.ll
