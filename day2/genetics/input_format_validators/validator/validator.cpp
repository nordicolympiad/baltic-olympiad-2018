#include "validator.h"

void run() {
	int nlim = Arg("n");
	int n = Int(2, nlim);
	Space();
	int m = Int(2, nlim);
	Space();
	Int(1, m);
	Endl();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			char ch = Char();
			assert(ch == '0' || ch == '1');
		}
		Endl();
	}
}
