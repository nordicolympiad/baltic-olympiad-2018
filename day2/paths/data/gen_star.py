#!/usr/bin/python3

from helper import output, cmdlinearg
import random

n = int(cmdlinearg('n'))
m = n-1
k = int(cmdlinearg('k'))

colors = list(range(k)) + [random.randrange(k) for _ in range(n - k)]
random.shuffle(colors)
colors = colors[:n]
eds = []
for i in range(1,n):
    eds.append((0,i))

output(k, colors, eds)
