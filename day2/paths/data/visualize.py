#!/usr/bin/env python3
# usage: ./visualize.py <input | neato -Tpng -o out.png && xdg-open out.png
n, m, k = map(int, input().split())
cols = list(map(int, input().split()))
colnames = ['red', 'blue', 'green', 'yellow', 'purple']
print("graph {")
for i in range(m):
    a,b = map(int, input().split())
    print("{} -- {}".format(a,b))
for i in range(n):
    print("{} [style=filled, fillcolor={}]".format(i+1, colnames[cols[i]-1]))
print("}")
