#!/usr/bin/env python3

"""
Easily query Windows product catalog XML

Usage:
  catalog-query.py [-h] [-a ARCH] [-e EDITION] [-l LANG] <catalog>

Arguments:
  catalog               Product catalog (products.xml)

Options:
  -a, --arch ARCH           Filter by architecture
  -e, --edition EDITION     Filter by edition
  -l, --lang LANG           Filter by language
  -h, --help                Show this help message
"""

import subprocess
from docopt import docopt


def main():
    args = docopt(__doc__)

    query = "[.MCT.Catalogs.Catalog.PublishedMedia.Files.File.[]"
    if arch := args.get("--arch"):
        query += f' | select(.Architecture == "{arch}")'
    if edition := args.get("--edition"):
        query += f' | select(.Edition == "{edition}")'
    if lang := args.get("--lang"):
        query += f' | select(.LanguageCode == "{lang}")'
    query += "]"

    subprocess.run(["yq", "-p=xml", query, args["<catalog>"]])


if __name__ == "__main__":
    main()
