import os
import sys

import numpy as np
import matplotlib.pyplot as plt

def plot_floorplan(filename):
    floorplan = np.loadtxt(filename)
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    for line in floorplan:
        ax.add_patch(plt.Rectangle(xy=(line[0], line[1]), width=line[2], height=line[3], color="blue", alpha=0.2))
    plt.xlim((0, floorplan[:, 0].max() + floorplan[:, 2].max()))
    plt.ylim((0, floorplan[:, 1].max() + floorplan[:, 3].max()))
    plt.show()
    pass

if __name__ == "__main__":
    floorplan = sys.argv[1]
    plot_floorplan(floorplan)
