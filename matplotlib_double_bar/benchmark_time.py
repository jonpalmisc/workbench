#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "Heliotrope 4"

benchmarks = ("Fasta", "Binary Trees", "Mandelbrot")
times = {
    "O2": (1.317, 2.251, 3.421),
    "O2 + Limoncello": (20.226, 27.144, 65.399),
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
ax.set_ylabel("Execution time (seconds)")
ax.set_ylim(0, 75)
ax.set_xticks(x + bar_width / 2, benchmarks)
ax.legend(loc="upper left")

fig.savefig("benchmark_time.png")
