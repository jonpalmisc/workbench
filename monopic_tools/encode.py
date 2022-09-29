#!/usr/bin/env python3
#
# encode.py -- Encode arbitrary data into a Monopic file

import gzip
import sys

MONOPIC_MAGIC = b"\xFF\x4D\x4F\x4E\x4F\x50\x49\x43\x01"


def main():
    user_data = sys.stdin.buffer.read()
    gzip_data = gzip.compress(user_data, 2)

    sys.stdout.buffer.write(MONOPIC_MAGIC)
    sys.stdout.buffer.write(gzip_data)


if __name__ == "__main__":
    main()
