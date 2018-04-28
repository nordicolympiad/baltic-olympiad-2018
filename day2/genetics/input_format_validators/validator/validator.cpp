#include "validator.h"

void run() {
	int nlim = Arg("n");
	int a = Arg("a");
	assert(a == 2 || a == 4);
	int n = Int(3, nlim);
	Space();
	int m = Int(3, nlim);
	Space();
	Int(1, m);
	Endl();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			char ch = Char();
			assert(ch == 'A' || ch == 'C' || ch == 'G' || ch == 'T');
			if (a == 2) assert(ch == 'A' || ch == 'C');
		}
		Endl();
	}
}
