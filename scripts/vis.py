import matplotlib.pyplot as plt
import numpy as np
import warnings

warnings.filterwarnings("ignore", category=np.lib.polynomial.RankWarning)

# Read input.txt
with open("input.txt", "r") as f:
    n = int(f.readline())
    points = []
    for i in range(n):
        x, y = map(float, f.readline().split())
        points.append((x, y))
    SEGMENT_COST = float(f.readline())

points.sort()

# Plot all the points and their indices
for i, point in enumerate(points):
    plt.scatter(point[0], point[1], marker="+")
    plt.annotate(str(i), xy=point, xytext=(5, 5), textcoords="offset points")

total_cost = 0

# Read output.txt and plot the best fit line for each segment
with open("output.txt", "r") as f:
    num_segments = int(f.readline())
    for i in range(num_segments):
        a, b = map(int, f.readline().split())
        x_values = [points[j][0] for j in range(a, b + 1)]
        y_values = [points[j][1] for j in range(a, b + 1)]
        label=f"Segment {i + 1}: {a} to {b}"
        if len(x_values) == 1:
            plt.scatter(x_values, y_values, label=label)
        else:
            best_fit = np.polyfit(x_values, y_values, deg=1)
            plt.plot(x_values, np.polyval(best_fit, x_values), label=label)
            mse = np.mean((np.polyval(best_fit, x_values) - y_values) ** 2)
            total_cost += mse

# Add total segment cost to plot
total_cost += num_segments * SEGMENT_COST
plt.title(f"Total cost: {total_cost:.9f}")

plt.subplots_adjust(left=0.001, bottom=0.001, right=0.999, top=0.999)

# Move legend outside the plot and adjust its position
box = plt.gca().get_position()
plt.gca().set_position([box.x0, box.y0, box.width * 0.7, box.height])
plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))

# Save the plot as a PNG file
plt.savefig("plot.png", bbox_inches='tight')
