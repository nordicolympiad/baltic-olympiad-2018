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
nowrap = int(cmdlinearg('nowrap', 0))

print(n, m)
for _ in range(m):
    a = random.randrange(1, n+1)
    b = random.randrange(1, n+1)
    if nowrap and b < a:
        a,b = b,a
    print(a, b)
