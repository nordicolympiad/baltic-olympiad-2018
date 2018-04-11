#!/usr/bin/python3

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

target = []
if mode == 'normal':
    target = [random.randrange(n) for _ in range(n)]
elif mode == 'perm':
    target = list(range(n))
    random.shuffle(target)
elif mode == 'ego':
    target = [random.randrange(i+1) for i in range(n)]
else:
    assert False

def randname():
    return ''.join(random.choice(string.ascii_lowercase) for _ in range(10))

names = [randname() for _ in range(n)]
perm = list(range(n))
random.shuffle(perm)
print(n)
for i in perm:
    print(names[i], names[target[i]])
