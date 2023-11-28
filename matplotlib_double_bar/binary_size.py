#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "Heliotrope 4"

benchmarks = ("Byrep", "Respawn", "Lua")
times = {
    "O2": (0.044, 0.464, 0.276),
    "O2 + Limoncello": (0.132, 0.642, 1.8),
}

bar_width = 0.45
multiplier = 0

x = np.arange(len(benchmarks))
fig, ax = plt.subplots(dpi=300)

for mode, time in times.items():
    offset = bar_width * multiplier
    multiplier += 1

    rects = ax.bar(x + offset, time, bar_width, label=mode)
    ax.bar_label(rects, padding=2)


# ax.set_title('Benchmark Performance')
ax.set_ylabel("Binary size (MB)")
ax.set_ylim(0, 2)
ax.set_xticks(x + bar_width / 2, benchmarks)
ax.legend(loc="upper left")

fig.savefig("binary_size.png")
