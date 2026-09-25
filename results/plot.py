import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results/results.csv")
df = df[df["result"] == True]

# Numeric test index
tests = df["test"].unique()
test_numbers = {test: i for i, test in enumerate(tests)}
df["test_number"] = df["test"].map(test_numbers)

plt.figure(figsize=(14, 7))

for algorithm, group in df.groupby("algorithm"):
    group = group.sort_values("test_number")

    # Raw measurements
    plt.scatter(
        group["test_number"],
        group["time_us"],
        s=4,
        alpha=0.35,
    )

    rolling = group["time_us"].rolling(
        window=200,
        min_periods=1
    ).median()

    plt.plot(
        group["test_number"],
        rolling,
        linewidth=2.5,
        label=f"{algorithm} rolling median"
    )

plt.yscale("log")

plt.xlabel("")
plt.ylabel("Time (µs)")
plt.title("Connect 4 Algorithm Performance")

plt.xticks([])

plt.grid(
    axis="y",
    which="both",
    alpha=0.2
)

plt.legend()
plt.tight_layout()

plt.savefig("results/ab_vs_mnmx.png", dpi=300, bbox_inches="tight")

plt.show()