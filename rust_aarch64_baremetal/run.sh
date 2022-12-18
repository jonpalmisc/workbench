#!/bin/sh

QEMU="${QEMU:-qemu-system-aarch64}"
QEMU_FLAGS="${QEMU_FLAGS:--M virt -cpu cortex-a57 -m 1024M -nographic -singlestep}"
IMAGE="target/aarch64-unknown-none/release/rust_aarch64_baremetal"

$QEMU $QEMU_FLAGS -kernel "$IMAGE"
