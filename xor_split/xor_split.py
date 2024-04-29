import sys

data = sys.stdin.buffer.read()
with open("original.bin", "wb") as xf:
    xf.write(data)

xored = bytes([b ^ 0x66 for b in data])
with open("xored.bin", "wb") as xf:
    xf.write(xored)
