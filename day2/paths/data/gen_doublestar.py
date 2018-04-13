#!/usr/bin/python3

import sys
import random

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    assert default is not None, name
    return default

#Only works for k=5
#m <= 1e5 for n <= 75000

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
k = 5
endsize = n//3
midsize = n - 2*endsize - 2

colors = []
eds = []

for i in range(0,endsize):
    eds.append((i,endsize))
    colors.append(1)
colors.append(2)

for i in range(0,midsize):
    eds.append((endsize,endsize+1+i))
    colors.append(3)

colors.append(4)

for i in range(0,midsize):
    eds.append((endsize+midsize+1,endsize+1+i))

for i in range(0,endsize):
    eds.append((endsize+midsize+1,endsize+midsize+2+i))
    colors.append(5)


m = len(eds)
print(n, m, k)
print(*colors)
for (a, b) in eds:
    print(a+1, b+1)
