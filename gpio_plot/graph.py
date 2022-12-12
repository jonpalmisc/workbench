#!/usr/bin/env python3

import matplotlib.pyplot as plt

# Read trace from standard input
trace = input()

t = []
s = []

for i, c in enumerate(trace):
    t += [i]
    s += [1 if c == "1" else 0]

# Plot the trace with the step function for a logic-analyzer style
plt.step(t, s)

# Configure plot appearance
plt.ylabel("signal")
plt.xlabel("time (us)")
plt.grid()

plt.show()
