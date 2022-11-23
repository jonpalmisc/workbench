#!/bin/sh

STYLES="Chromium Google LLVM Microsoft Mozilla WebKit"
VERSION=$(clang-format --version | awk '{print $3}')

for style in $STYLES; do
    clang-format --style="$style" --dump-config > "$style-$VERSION.yml"
done
