#!/usr/bin/python3

import sys
import random
from collections import Counter

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    assert default is not None, name
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
k = int(cmdlinearg('k'))
r = int(cmdlinearg('r', k-1))
count = int(cmdlinearg('count', 1))
padding = int(cmdlinearg('padding', 0))

rep = list(range(k-1 if padding > 0 else k))
random.shuffle(rep)

ask = random.sample(rep, r)

seq = rep * (n // len(rep) + 2)
assert len(seq) >= n
seq = seq[:n]

assert padding <= n
pad = padding * [0] + (n - padding) * [1]
random.shuffle(pad)

ind = 0
out = []
for c in pad:
    if c:
        out.append(seq[ind])
        ind += 1
    else:
        out.append(k-1)

print(n, k, r)
print(*out)
for w in ask:
    print(w, count)
