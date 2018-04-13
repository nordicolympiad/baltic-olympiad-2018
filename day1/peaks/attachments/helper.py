import os
import sys

_newline = 10 if sys.version_info[0] == 3 else '\n'

def _write(s):
    written = os.write(1, s)
    assert written != 0
    if written != len(s):
        _write(s[written:])

def _read_line():
    s = os.read(0, 100)
    assert len(s) != 0
    if s[-1] != _newline:
        s += _read_line()
    return s

def query(x, y, z):
    _write(b'? %d %d %d\n' % (x, y, z))
    line = _read_line()[:-1]
    r = int(line)
    if r == -1:
        exit(0)
    return r

def guess(x, y, z):
    _write(b'! %d %d %d\n' % (x, y, z))
    exit(0)


n, m, k, q = map(int, _read_line().split())

# TODO do something smart
query(1, 1, 1)
query(n, m, k)
guess(1, 1, 1)
