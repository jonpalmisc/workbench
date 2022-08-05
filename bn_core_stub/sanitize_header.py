#!/usr/bin/env python3

# Adapted from Andrew's original code:
# <https://github.com/lwerdna/workbench/blob/master/binja_api_pure_c/patch.py>

import re
import sys


def main():
    if len(sys.argv) < 2:
        print("Usage: sanitize_header.py <header>")
        return

    with open(sys.argv[1]) as f:
        lines = [x.rstrip() for x in f.readlines()]

    did_fix_includes = False

    i = 0
    while i < len(lines):

        # Include statements need to be fixed/translated exactly once. All
        # subsequent include statements can be ignored.
        if "#include" in lines[i]:
            if not did_fix_includes:
                print("#include <stdint.h>")
                print("#include <stddef.h>")
                print("#include <stdlib.h>")

                did_fix_includes = True

            i += 1
            continue

        match = re.match(r"^(\s+)struct (.*);$", lines[i])
        if match:
            (space, name) = match.group(1, 2)
            print(f"{space}typedef struct {name} {name};")

            i += 1
            continue

        match = re.match(r"^(\s+)(enum|struct) (.*?)( {)?$", lines[i])
        if match:
            (space, keyword, name, brack) = match.group(1, 2, 3, 4)
            if not brack:
                brack = ""

            print(f"{space}typedef {keyword} {name}{brack}")

            i += 1
            while lines[i] != "\t};":
                print(lines[i])
                i += 1

            print("\t} %s;" % name)

            i += 1
            continue

        print(lines[i])
        i += 1


if __name__ == "__main__":
    main()
