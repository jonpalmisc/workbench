#!/usr/bin/env python3
#
# decode.py -- Decode the data inside of a Monopic file

import gzip
import sys

MONOPIC_MAGIC = b"\xFF\x4D\x4F\x4E\x4F\x50\x49\x43\x01"


def main():
    user_data = sys.stdin.buffer.read()
    if user_data[0:9] != MONOPIC_MAGIC:
        sys.exit(1)

    user_data = gzip.decompress(user_data[9:])
    sys.stdout.buffer.write(user_data)


if __name__ == "__main__":
    main()
