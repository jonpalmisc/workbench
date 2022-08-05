#!/usr/bin/env python3
#
# Copyright (c) 2022 Jon Palmisciano. All rights reserved.
#
# Use of this source code is governed by the BSD 3-Clause license; the full
# terms of the license can be found in the LICENSE.txt file.

from functools import lru_cache
from glob import glob
import sys
from typing import List, Set

# ===-- OPTIONS -------------------------------------------------------------===

# Path to macOS SDK root
SDK_ROOT = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/"

# Wildcard patterns of files to parse
SOURCE_PATTERNS = [
    "kern/*.h",
    "libkern/*.h",
    "mach/*.h",
    "malloc/*.h",
    "sys/*.h",
    "xpc/xpc.h",
]

# Files which should be ignored
BLACKLIST = {
    "mach_eventlink.h",
    "sys/ubc.h",
    "sys/_symbol_aliasing.h",
    "sys/_posix_availability.h",
}

# Include directories and macro definitions to use when parsing
INCLUDE_DIRS = [SDK_ROOT]
DEFINITIONS = ["__arm64__", "__GNUC__", "__APPLE_CPP__", "__APPLE__"]

# Architecture and platform for the BNTL
ARCH_NAME = "aarch64"
PLATFORM_NAME = "mac-" + ARCH_NAME

# BNTL output file path and library name
LIBRARY_NAME = "libSystem.B.dylib"
OUTPUT_PATH = (
    "/Applications/Binary Ninja.app/Contents/Resources/typelib/aarch64/"
    + LIBRARY_NAME
    + ".bntl"
)

# ===------------------------------------------------------------------------===


def eprint(*args, **kwargs):
    """Print to standard error."""

    print(*args, file=sys.stderr, **kwargs)


def expand_root_wildcard(query: str) -> List[str]:
    """Expand a path wildcard based in the root directory."""

    return glob(SDK_ROOT + query, recursive=True)


def is_blacklisted(path: str) -> bool:
    """Checks if a header is blacklisted for being problematic."""

    for s in BLACKLIST:
        if s in path:
            eprint(f"Ignoring file {path}")
            return True

    return False


def discover_source_files() -> Set[str]:
    """Find all source files in the SDK root matching the given queries."""

    result = []
    for q in SOURCE_PATTERNS:
        result += [p for p in expand_root_wildcard(q) if not is_blacklisted(p)]

    return set(result)


@lru_cache
def compiler_flags() -> List[str]:
    """Get all flags needed for the parser to successfully parse the headers."""

    flags = []
    for d in DEFINITIONS:
        flags.append(f"-D{d}=1")

    return flags


# ===------------------------------------------------------------------------===

from binaryninja import *


def format_error(e: TypeParserError) -> str:
    """Format a parsing error as a string."""

    return f"{e.message} ({e.file_name}:{e.line}:{e.column})"


arch = Architecture[ARCH_NAME]
platform = Platform[PLATFORM_NAME]
clang = TypeParser["ClangTypeParser"]

all_types = {}
all_functions = {}

for file_path in discover_source_files():
    with open(file_path, "r") as f:
        source = f.read()

    (result, errors) = clang.parse_types_from_source(
        source, file_path, platform, [], compiler_flags(), INCLUDE_DIRS
    )

    if result is None:
        eprint(f"\nFailed to parse {file_path}")
        for e in errors:
            eprint(f"  > {format_error(e)}")
        eprint("")

        continue

    for t in result.types:
        all_types[t.name] = t.type
    for f in result.functions:
        all_functions[f.name] = f.type

    print(file_path)

typelib = TypeLibrary.new(arch, LIBRARY_NAME)
typelib.add_platform(platform)

for t in all_types:
    typelib.add_named_type(t, all_types[t])
for f in all_functions:
    typelib.add_named_object(f"_{f}", all_functions[f])

typelib.finalize()
typelib.write_to_file(OUTPUT_PATH)
