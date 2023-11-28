#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "Heliotrope 4"

benchmarks = ("Byrep", "Respawn", "Lua")
times = {
    "O2": (3.338, 6.880, 3.587),
    "O2 + Limoncello": (5.322, 14.208, 41.753),
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
ax.set_ylabel("Build time (seconds)")
ax.set_ylim(0, 50)
ax.set_xticks(x + bar_width / 2, benchmarks)
ax.legend(loc="upper left")

fig.savefig("build_time.png")
