#!/usr/bin/python3

# Star graph where half of the "rays" have length 2.
# Short rays get color 1, center gets color 2,
# mid points of long rays get color 3,
# leaves of long rays get color 4.
# Only works for k = 4.

import sys
import random

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    assert default is not None, name
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
m = n-1
k = 4
longsize = n//3
shortsize = n-2*longsize-1
colors = []
for i in range(0,shortsize):
    colors.append(1)
colors.append(2)
for i in range(0,longsize):
    colors.append(3)
for i in range(0,longsize):
    colors.append(4)
eds = []

for i in range(0,shortsize):
    eds.append((i,shortsize))

for i in range(0,longsize):
    eds.append((shortsize,shortsize+i+1))

for i in range(0,longsize):
    eds.append((shortsize+i+1,shortsize+longsize+i+1))

print(n, m, k)
print(*colors)
for (a, b) in eds:
    print(a+1, b+1)
