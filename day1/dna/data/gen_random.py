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
r = int(cmdlinearg('r', k))

s = list(range(k)) + [random.randrange(k) for _ in range(n - k)]
random.shuffle(s)
start = random.randrange(n+1)
end = random.randrange(n+1)
if end < start:
    start,end = end,start
if start == end:
    start = max(start-1, 0)
    end = min(end+1, n)

co = Counter()
for i in range(start, end):
    co[s[i]] += 1

li = []
while not li:
    for occ in co:
        c = random.randrange(co[occ]+1)
        if c:
            li.append((occ, c))
    random.shuffle(li)

assert 1 <= len(li) <= k
if len(li) > r:
    li = li[:r]
else:
    r = len(li)

print(n, k, r)
print(*s)
for (w, c) in li:
    print(w, c)
