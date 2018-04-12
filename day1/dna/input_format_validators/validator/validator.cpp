#include "validator.h"

void run() {
	int n = Int(2, Arg("n"));
	Space();
	int k = Int(1, n);
	Space();
	int r = Int(1, k);
	Endl();

	assert(r <= (int)Arg("r", n));

	vector<int> v = SpacedInts(n, 0, k-1);
	assert(set<int>(v.begin(), v.end()).size() == k);

	vector<int> bs;
	for (int i = 0; i < r; i++) {
		int b = Int(0, k-1);
		Space();
		Int(1, n);
		Endl();
		bs.push_back(b);
	}
	AssertUnique(bs);
}
