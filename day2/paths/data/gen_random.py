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
m = int(cmdlinearg('m'))
k = int(cmdlinearg('k'))

colors = list(range(1, k+1)) + [random.randrange(1, k+1) for _ in range(n - k)]
random.shuffle(colors)
colors = colors[:n]
edset = set()
eds = []
while len(edset) < m:
    a = random.randrange(n)
    b = random.randrange(n)
    flip = False
    if a == b:
        continue
    if a > b:
        a,b = b,a
        flip = True
    if (a,b) in edset:
        continue
    edset.add((a,b))
    if flip:
        a,b = b,a
    eds.append((a,b))

print(n, m, k)
print(*colors)
for (a, b) in eds:
    print(a+1, b+1)
