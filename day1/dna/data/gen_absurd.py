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
k = int(cmdlinearg('k'))
r = int(cmdlinearg('r'))

s = list(range(k)) + [random.randrange(k) for _ in range(n - k)]
random.shuffle(s)

print(n, k, r)
print(*s)
for i in random.sample(range(k), r):
    print(i, n - random.randrange(0, n // 3 + 1))
