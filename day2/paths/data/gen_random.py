#!/usr/bin/python3

from helper import output, cmdlinearg
import random

n = int(cmdlinearg('n'))
m = int(cmdlinearg('m'))
k = int(cmdlinearg('k'))

colors = list(range(k)) + [random.randrange(k) for _ in range(n - k)]
random.shuffle(colors)
colors = colors[:n]
edset = set()
eds = []
while len(edset) < m:
    a = random.randrange(n)
    b = random.randrange(n)
    if a == b:
        continue
    if a > b:
        a,b = b,a
    edset.add((a,b))

output(k, colors, list(edset))
