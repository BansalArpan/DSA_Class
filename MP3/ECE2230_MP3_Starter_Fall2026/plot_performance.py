"""Usage: python3 plot_performance.py [performance_data.txt]
Requires matplotlib. Writes three PNG graphs into graphs/ beside this script.
"""
from collections import defaultdict
from pathlib import Path
import sys
import math
import os
import re
import tempfile

# Use a writable cache when the home directory is restricted.
os.environ.setdefault("MPLCONFIGDIR", str(Path(tempfile.gettempdir()) / "mp3-matplotlib"))

import matplotlib
matplotlib.use("Agg")  # Save graphs without opening a desktop window.
import matplotlib.pyplot as plt

NAMES = {
    1: "Insertion sort",
    2: "Recursive selection sort",
    3: "Iterative selection sort",
    4: "Merge sort",
    5: "Quick Sort",
}
ORDERS = {1: "Random", 2: "Ascending", 3: "Descending"}
ALGORITHM_COLORS = {
    1: "tab:blue",
    2: "tab:orange",
    3: "tab:green",
    4: "tab:red",
    5: "tab:purple",
}
ALGORITHM_LINE_STYLES = {1: "-", 2: "--", 3: "-.", 4: ":", 5: (0, (3, 1, 1, 1))}
ALGORITHM_MARKERS = {1: "o", 2: "s", 3: "^", 4: "D", 5: "P"}


def load_data(path):
    data = defaultdict(list)
    current_algorithm = None
    current_order = None

    algorithm_names = {
        "insertion sort": 1,
        "recursive selection sort": 2,
        "iterative selection sort": 3,
        "merge sort": 4,
        "qsort": 5,
    }
    order_names = {
        "random": 1,
        "ascending": 2,
        "descending": 3,
    }

    header = re.compile(
        r"^(insertion sort|recursive selection sort|iterative selection sort|"
        r"merge sort|qsort(?:_r)?) with (random|ascending|descending|decending) list$",
        re.IGNORECASE,
    )
    algorithm_names["qsort_r"] = 5
    order_names["decending"] = 3

    for number, line in enumerate(path.read_text().splitlines(), 1):
        text = line.strip()
        if not text or text.startswith("#"):
            continue
        # Older scripts run through sh may print echo's -e literally.
        if text.startswith("-e "):
            text = text[3:].strip()
        if text.lower().startswith("the date today is "):
            continue
        if text.lower() == "end":
            break

        match = header.fullmatch(text)
        if match:
            current_algorithm = algorithm_names[match[1].lower()]
            current_order = order_names[match[2].lower()]
            continue

        fields = text.split()
        try:
            if len(fields) == 4:
                size, time_ms, algorithm, order = fields
                order = int(order)
            elif len(fields) == 3:
                if current_algorithm is None or current_order is None:
                    raise ValueError("three-column measurement needs a section header")
                size, time_ms, algorithm = fields
                order = current_order
                if int(algorithm) != current_algorithm:
                    raise ValueError("algorithm does not match section header")
            else:
                raise ValueError("expected three or four measurement columns")
            size, time_ms, algorithm = int(size), float(time_ms), int(algorithm)
            if (size < 1 or not math.isfinite(time_ms) or time_ms < 0
                    or algorithm not in NAMES or order not in ORDERS):
                raise ValueError("invalid measurement")
        except ValueError as error:
            raise ValueError(f"{path}:{number}: {error}") from error
        data[algorithm, order].append((size, time_ms))

    for algorithm in NAMES:
        for order in ORDERS:
            points = data[algorithm, order]
            if len({n for n, _ in points}) < 5:
                raise ValueError(
                    f"Need five sizes: {NAMES[algorithm]}, {ORDERS[order]}"
                )
            points.sort()

        if max(t for _, t in data[algorithm, 1]) <= 1000:
            print(f"WARNING: increase random sizes for {NAMES[algorithm]}")

    return data


def style(ax, title):
    # Add 15% vertical padding in the axis scale, including logarithmic scales.
    ax.margins(y=0.15)
    ax.autoscale(enable=True, axis="y")
    ax.set_ylim(bottom=0)
    ax.set_title(title)
    ax.set_xlabel("Number of records")
    ax.set_ylabel("CPU time (milliseconds)")
    ax.grid(True, alpha=0.3)
    ax.legend(loc="lower right")


def save(fig, directory, name):
    fig.tight_layout()
    fig.savefig(directory / f"{name}.png", dpi=200)
    plt.close(fig)


def main():
    root = Path(__file__).resolve().parent
    source = Path(sys.argv[1]) if len(sys.argv) > 1 else root / "performance_data.txt"
    data = load_data(source)
    output = root / "graphs"
    output.mkdir(exist_ok=True)

    # Plot all algorithms for each input order.
    for order, order_name in ORDERS.items():
        fig, ax = plt.subplots(figsize=(8, 5))
        for algorithm, name in NAMES.items():
            points = data[algorithm, order]
            x, y = zip(*points)
            # The scale includes zero and keeps fast algorithms visible.
            ax.plot(
                x,
                y,
                label=name,
                color=ALGORITHM_COLORS[algorithm],
                linestyle=ALGORITHM_LINE_STYLES[algorithm],
                marker=ALGORITHM_MARKERS[algorithm],
            )
        ax.set_xscale("log")
        ax.set_yscale("symlog", linthresh=1)
        style(
            ax,
            f"Execution Time vs. Input Size for {order_name.lower()} input.",
        )
        save(fig, output, f"comparison_{order_name.lower()}")
    print(f"Saved 3 graphs as PNGs in {output}")


if __name__ == "__main__":
    main()
