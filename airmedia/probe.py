#!/usr/bin/env python3

import sys
from pprint import pprint
import requests

args = sys.argv
if len(args) < 2:
    print(f"Usage: {args[0]} <host>")
    exit(1)

response = requests.post(
    f"http://{args[1]}:47000/probe-m360",
    headers={
        "User-Agent": "MirroringAssist",
    },
)
print(response.content)
pprint(response.json())
