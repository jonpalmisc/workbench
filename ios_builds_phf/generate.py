#!/usr/bin/env python3


def enum_from_build(build: str) -> str:
    return f"BUILD_{build}"


def make_builds_header(path: str, builds: list[str]) -> None:
    with open(path, "w") as header:
        header.write("#pragma once\n\n")

        header.write("typedef enum {\n")
        for build in builds:
            header.write(f"    {enum_from_build(build)},\n")
        header.write("} build_t;\n")


def make_convert_header(path: str, builds: list[str]) -> None:
    with open(path, "w") as header:
        header.write("#pragma once\n\n")
        header.write('#include "builds.h"\n\n')
        header.write("#include <string.h>\n\n")

        header.write("build_t build_from_string(char const* str) {\n")
        for build in builds:
            header.write(
                f'    if (0 == strcmp(str, "{build}")) return {enum_from_build(build)};\n'
            )

        header.write("\n    return (build_t)-1;\n")
        header.write("}")


def make_gperf_config(path: str, builds: list[str]) -> None:
    with open(path, "w") as config:
        config.write("""
%{
#include "builds.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>
%}

struct result_t { char const* name; build_t build; }

%%
                     """)

        for build in builds:
            config.write(f"{build}, {enum_from_build(build)}\n")


with open("builds.txt", "r") as f:
    builds = f.read().splitlines()


make_builds_header("builds.h", builds)
make_convert_header("convert_naive.h", builds)
make_gperf_config("config.gperf", builds)
