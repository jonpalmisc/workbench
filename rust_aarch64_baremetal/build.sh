#!/bin/sh

export RUSTFLAGS="$RUSTFLAGS -Ctarget-cpu=generic"
export RUSTFLAGS="$RUSTFLAGS -Ctarget-feature=+strict-align,-neon,-fp-armv8,-sve"
export RUSTFLAGS="$RUSTFLAGS -Clink-arg=-Tsrc/image.ld"

rustup run nightly cargo build --target aarch64-unknown-none --release
