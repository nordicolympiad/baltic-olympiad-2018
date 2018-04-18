#!/usr/bin/python3

from helper import output, cmdlinearg
import random

n = int(cmdlinearg('n'))
m = int(cmdlinearg('m'))
k = int(cmdlinearg('k'))

colors = list(range(k)) + [random.randrange(k) for _ in range(n - k)]
random.shuffle(colors)
colors = colors[:n]
eds = []
assert m <= n*(n-1)//2
if m > n*(n-1) // 4:
    cands = []
    for i in range(n):
        for j in range(i):
            cands.append((i, j))
    eds = random.sample(cands, m)
else:
    edset = set()
    while len(edset) < m:
        a = random.randrange(n)
        b = random.randrange(n)
        if a == b:
            continue
        if a > b:
            a,b = b,a
        edset.add((a,b))
    eds = list(edset)

output(k, colors, eds)
