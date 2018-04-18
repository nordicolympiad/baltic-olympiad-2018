#!/usr/bin/python3

import math
import sys
import random
import string

def cmdlinearg(name, default=None):
    for arg in sys.argv:
        if arg.startswith(name + "="):
            return arg.split("=")[1]
    assert default is not None, name
    return default

random.seed(int(cmdlinearg('seed', sys.argv[-1])))
n = int(cmdlinearg('n'))
mode = cmdlinearg('mode', 'uniform')
mode, *params = mode.split(',')

# generate parent list for a tree rooted in node 0
def gen_tree(n, mode):
    par = [0]
    assert mode in ['random', 'star', 'line', 'shallow', 'deep', 'deeper']
    for i in range(1, n):
        if mode == 'random':
            pred = random.randrange(i)
        elif mode == 'star':
            pred = 0
        elif mode == 'line':
            pred = i-1
        elif mode == 'shallow':
            pred = int(random.uniform(0, i**0.2) ** 5)
        elif mode == 'deep':
            pred = i-1 - int(random.uniform(0, i**0.1) ** 10)
        elif mode == 'deeper':
            if i < 4:
                pred = random.randrange(i)
            else:
                hi = math.log2(math.log2(i))
                pred = i - int(2 ** 2 ** min(random.uniform(-3, hi), random.uniform(-3, hi), random.uniform(-3, hi)))
        else:
            assert False
        assert 0 <= pred < i
        par.append(pred)
    return par

# ~n/5 cycles of length ~5 each
def small_cycles(n):
    target = []
    while len(target) < n:
        b = len(target)
        a = min(random.randrange(1, 10), n-b)
        target.append(b+a - 1)
        for x in range(b+1, b+a):
            target.append(x-1)
    return target

# Expected length of longest cycle ~ 0.62n, expected number of cycles ~ log n
def rand_cycles(n):
    target = list(range(n))
    random.shuffle(target)
    return target

target = []
if mode == 'uniform':
    target = [random.randrange(n) for _ in range(n)]
elif mode == 'perm':
    target = rand_cycles(n)
elif mode == 'many-cycles':
    target = small_cycles(n)
elif mode == 'one-cycle':
    target = list(range(1, n)) + [0]
elif mode == 'ego' or mode == 'general':
    tree_mode, parts, *params = params
    parts = int(parts)
    splittings = [0] + sorted(random.sample(range(1, n), parts-1))
    for a, b in zip(splittings, splittings[1:] + [n]):
        # nodes a ... b-1
        assert 0 <= a < b <= n
        par = gen_tree(b - a, tree_mode)
        for x in par:
            target.append(a + x)

    if mode == 'general':
        # Bind the individual trees together in cycles
        mode2, = params
        if mode2 == 'perm':
            cyc = rand_cycles(parts)
        elif mode2 == 'many-cycles':
            cyc = small_cycles(parts)
        else:
            assert False, "unknown mode2 " + mode2
        split_targ = [splittings[x] for x in cyc]
        for i in range(len(splittings)):
            assert target[splittings[i]] == splittings[i]
            target[splittings[i]] = split_targ[i]
else:
    assert False, "unknown mode " + mode

assert len(target) == n
def randname():
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(10))

names = [randname() for _ in range(n)]
perm = list(range(n))
random.shuffle(perm)
print(n)
for i in perm:
    print(names[i], names[target[i]])
