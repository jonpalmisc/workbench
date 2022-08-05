#!/usr/bin/env python3
#
# Adapted from Zack Orndorff's stub library generator, which can be found at:
# <https://github.com/zackorndorff/binja_printk/blob/master/scripts/gen_lib.py>

import platform
import re
import subprocess
import sys
from typing import List, Set


def extract_symbols_from_source(header_path: str) -> Set[str]:
    """Parse the binaryninjacore.h header and index all function symbols."""

    with open(header_path, "r") as f:
        source = f.read()

    export_re = re.compile(
        r"^\W*BINARYNINJACOREAPI\W+[A-Za-z0-9_ ]+\W+([A-Za-z0-9_]+)\([^;]*\);\W*$",
        re.MULTILINE,
    )

    unique_symbols = set()
    for match in export_re.finditer(source):
        func_name = match.groups(1)[0]
        unique_symbols.add(func_name)

    return unique_symbols


def generate_stub_assembly(symbols: Set[str], os: str) -> str:
    """Generate assembly source for the stub library."""

    prefix = "_" if os == "Darwin" else ""
    lines = [".text" if os == "Darwin" else ".section .text", ""]
    for symbol in symbols:
        lines.append(".align 4")
        lines.append(".global " + prefix + symbol)
        lines.append(prefix + symbol + ":")
        lines.append("\tret")

    lines.append("")
    return "\n".join(lines)


def build_stub_library(os: str):
    """Assemble the stub library source into a dynamic library."""

    library_name = "libbinaryninjacore.1"
    library_name += ".dylib" if os == "Darwin" else ".so"

    command = ["cc", "-dynamiclib" if os == "Darwin" else "-shared"]
    if os == "Darwin":
        command += ["-arch", "arm64", "-arch", "x86_64"]
        command += ["-Wl,-install_name," + library_name]
    elif os == "Linux":
        command += ["-fPIC"]
        command += ["-Wl,-soname," + library_name]
    else:
        raise ValueError("Unsupported operating system")

    command += ["binaryninjacore.s", "-o", library_name]
    subprocess.run(command, check=True)


def main():
    if len(sys.argv) < 2:
        print("Usage: generate_stub.py <header>")
        return

    os = platform.system()
    if os != "Darwin" and os != "Linux":
        raise ValueError("Only macOS and Linux are supported")

    symbols = extract_symbols_from_source(sys.argv[1])
    asm = generate_stub_assembly(symbols, os)
    with open("binaryninjacore.s", "w") as f:
        f.write(asm)

    build_stub_library(os)


if __name__ == "__main__":
    main()
