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
mode = cmdlinearg('mode', 'normal')

def gen_tree(n, mode):
    eds = [0]    # this makes 0 root, but we could choose some other root
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
        eds.append(pred)
    return eds

target = []
if mode == 'normal':
    target = [random.randrange(n) for _ in range(n)]
elif mode == 'perm':
    target = list(range(n))
    random.shuffle(target)
elif mode == 'loops':
    while len(target) < n:
        b = len(target)
        a = min( random.randrange(1, 10), n-b)
        target.append(b+a - 1)
        for x in range(b+1, b+a):
            target.append(x-1)
else:
    m = int(cmdlinearg('m', 1))
    s = n // m
    for i in range(m):
        eds=gen_tree(s, mode)
        for x in eds:
         target.append( x+i*s )

assert len(target) == n
def randname():
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(10))

names = [randname() for _ in range(n)]
perm = list(range(n))
random.shuffle(perm)
print(n)
for i in perm:
    print(names[i], names[target[i]])
