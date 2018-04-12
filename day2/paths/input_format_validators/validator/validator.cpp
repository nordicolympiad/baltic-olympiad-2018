#include "validator.h"

void run() {
	int nlim = Arg("n");
	int n = Int(1, nlim);
	Space();
	int m = Int(1, nlim);
	Space();
	int k = Int(1, Arg("k"));
	Endl();

	SpacedInts(n, 1, k);

	set<pair<int, int>> eds;
	for (int i = 0; i < m; i++) {
		int a = Int(1, n);
		Space();
		int b = Int(1, n);
		Endl();
		assert(a != b);
		if (a > b) swap(a, b);
		eds.insert(make_pair(a, b));
	}
	assert(eds.size() == m);
}
