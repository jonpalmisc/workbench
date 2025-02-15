#!/usr/bin/env python3
import subprocess

with open("builds.txt", "r") as f:
    builds = f.read().splitlines()

had_error = False
for build in builds:
    cmp_out = subprocess.run(
        ["./build2enum.cmp", build], capture_output=True, text=True
    )
    phf_out = subprocess.run(
        ["./build2enum.phf", build], capture_output=True, text=True
    )

    status = "OK" if cmp_out.stdout == phf_out.stdout else "ERROR"
    if status != "OK":
        had_error = True

    print(f"{build:<20}{status}")

exit(1 if had_error else 0)
