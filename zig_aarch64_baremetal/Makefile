# ===------------------------------------------------------------------------===

# Target triple flag shorthand
TT_FLAGS := -target aarch64-freestanding-eabihf -mcpu=generic-fp_armv8-neon-crypto

# Zig compiler and flags
ZC := zig
ZC_FLAGS = $(TT_FLAGS) --cache-dir out/zig-cache

# QEMU and emulation flags
QEMU := qemu-system-aarch64
QEMU_FLAGS := -M virt -cpu cortex-a57 -m 256 -nographic

# ===------------------------------------------------------------------------===

ZIG_SRC := src/boot.s src/kernel.zig

out/kernel.elf: $(ZIG_SRC) src/image.ld
	mkdir -p out
	$(ZC) build-exe -T src/image.ld $(ZC_FLAGS) $(ZIG_SRC) -femit-bin=$@

# ===------------------------------------------------------------------------===

run: out/kernel.elf
	$(QEMU) $(QEMU_FLAGS) -kernel $<

fmt:
	zig fmt src/

clean:
	rm -fr out

.PHONY:	fmt clean
