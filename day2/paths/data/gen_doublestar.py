#!/usr/bin/python3

from helper import output, cmdlinearg

#Only works for k=5
#m <= 1e5 for n <= 75000

n = int(cmdlinearg('n'))
k = 5
endsize = n//3
midsize = n - 2*endsize - 2
star_a = n-2
star_b = n-1

colors = []
eds = []

left = list(range(0, endsize))
mid = list(range(endsize, endsize + midsize))
right = list(range(endsize + midsize, endsize*2 + midsize))

for i in left:
    colors.append(0)
    eds.append((i, star_a))

for i in mid:
    colors.append(1)
    eds.append((i, star_a))
    eds.append((i, star_b))

for i in right:
    colors.append(2)
    eds.append((i, star_b))

colors.append(3)
colors.append(4)

output(k, colors, eds)
