#!/usr/bin/env python
import math
# import matplotlib
# matplotlib.use('Qt5Agg')
import matplotlib.pyplot as pl
from matplotlib import gridspec
import numpy as np
import csv
import sys

time = []
data = []
labels = set()
with open(sys.argv[1], 'r') as csvFile:
    reader = csv.DictReader(csvFile)
    labels = reader.fieldnames
    data.append([])
    for row in reader:
        time.append(float(row[labels[0]]))
        for var in range(1,len(labels)):
            data.append([])
            data[var].append(float(row[labels[var]]))
csvFile.close()

N = len(labels) - 1
cols = int(math.ceil(N / 4))
rows = int(math.ceil(N / cols))

gs = gridspec.GridSpec(rows, cols)
fig = pl.figure()
for var in range(1,len(labels)):
    ax = fig.add_subplot(gs[var - 1])
    ax.plot(time, data[var])
    ax.set_title(labels[var])
    ax.set_xlabel(labels[0])
#    ax.set_ylabel(labels[var])

fig.set_tight_layout(True)
fig.show()
input()
