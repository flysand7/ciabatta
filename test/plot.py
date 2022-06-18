
import matplotlib.pyplot as plt
import numpy as np
import subprocess;
import math;
import csv;

plt.style.use('_mpl-gallery')
pi=math.pi

with open('bin/data.csv', newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    rows = [row for row in reader];

# make data
x = [float.fromhex(xi[:-1]) for xi in rows[0][:-1]]
y = [float.fromhex(yi[:-1]) for yi in rows[1][:-1]]
yex = [math.asin(x0) for x0 in x]

err = np.subtract(y, yex);
maxerrv = max(err)
minerrv = min(err)
maxerr = [maxerrv for xi in rows[0][:-1]]
minerr = [minerrv for xi in rows[0][:-1]]
errscale = 10**-14

lo = x[0]
hi = x[-1]

fig, a = plt.subplots(2, 1, constrained_layout=True)
# a.step(x, y, linewidth=1)
a[0].set_xlim(lo, hi);
# a[0].set_ylim(-1.000001, 1.000001);
a[0].set_xlabel('x');
a[0].set_ylabel('f(x)');
a[0].plot(x, y, x, yex)

a[1].set_xlim(lo, hi);
a[1].ticklabel_format(useOffset=False)
a[1].set_ylim(-errscale, errscale);
a[1].set_xlabel('x');
a[1].set_ylabel('absolute error');
a[1].plot(x, err, x, maxerr, x, minerr);

plt.show()


