#!/usr/bin/python3

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
k = int(cmdlinearg('k'))
colors = list(range(1, k+1)) + [random.randrange(1, k+1) for _ in range(n - k)]
random.shuffle(colors)
colors = colors[:n]
eds = []
for i in range(1,n):
    eds.append((0,i))

print(n, m, k)
print(*colors)
for (a, b) in eds:
    print(a+1, b+1)
