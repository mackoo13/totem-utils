import matplotlib as mpl
from matplotlib.colors import LogNorm
import matplotlib.pyplot as plt
import numpy as np

# plot a 2d histogram based on the output of 

xedges = np.arange(-20, 20, 0.05)
yedges = np.arange(-20, 20, 0.05)

with open('particles.txt') as f:
	lines = f.readlines()

lines2 = [l.split(":") for l in lines if l.startswith("Particle added")]

xs = [float(l[2].split(" ")[0]) for l in lines2]
ys = [float(l[3].split(" ")[0]) for l in lines2]

plt.hist2d(xs, ys, bins=40, norm=LogNorm())
plt.colorbar()
plt.show()

