#!/usr/bin/env python3

import random
import sys

lim_a = int(sys.argv[1])
lim_b = int(sys.argv[2])
lim_c = int(sys.argv[3])
random.seed(int(sys.argv[-1]))

def rand(lim):
    if lim > 0:
        return random.randint(1, lim)
    return -lim

print("{} {} {}".format(rand(lim_a), rand(lim_b), rand(lim_c)))
