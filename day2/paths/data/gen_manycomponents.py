#!/usr/bin/python3

from helper import output, cmdlinearg
import random

# Graph with many components of size c.
# Will contain <= m edges

n = int(cmdlinearg('n'))
m = int(cmdlinearg('m'))
k = int(cmdlinearg('k'))
c = int(cmdlinearg("compsize"))


colors = list(range(k)) + [random.randrange(k) for _ in range(n - k)]
random.shuffle(colors)
colors = colors[:n]
eds = []

edset = set()
failures = 0
failurelim = 100
while len(edset) < m:
    if failures >= failurelim:
        break
    a = random.randrange(n)
    b = 1+random.randrange(c-1)
    b += a//c * c
    if a > b:
        a,b = b,a
    if a == b or max(a,b) >= n or ((a,b) in edset):
        failures += 1
        continue
    else:
        failures = 0
    edset.add((a,b))
eds = list(edset)

output(k, colors, eds)
