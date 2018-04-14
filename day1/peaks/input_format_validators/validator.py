import sys
n,m,k,q = map(int, sys.argv[1:5])
parts = raw_input().split()
assert len(parts) >= 5
assert list(map(int, parts[:4])) == [n,m,k,q]
seed = int(parts[4])
assert 0 <= seed

# That's as much as we can do here. The output validator also does some checking.
exit(42)
