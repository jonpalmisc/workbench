#!/usr/bin/env python3

from enum import Enum, auto

from argparse import ArgumentParser

def analyze(path: str, mode: str):
    import binaryninja

    bv = binaryninja.load(path, update_analysis=False, options={"analysis.mode": mode})
    if not bv:
        raise ValueError("failed to load binary")

    bv.update_analysis_and_wait()
    bv.create_database(path + ".bndb")

def main():
    parser = ArgumentParser()
    parser.add_argument("input")
    parser.add_argument("-m", "--mode", default="full", help="analysis mode to use")

    args = parser.parse_args()

    analyze(args.input, args.mode)


if __name__ == "__main__":
    main()
