#include "validator.h"

void run() {
	int nlim = Arg("n");
	int n = Int(2, nlim);
	Space();
	int m = Int(2, nlim);
	Endl();

	int nowrap = Arg("nowrap", 0);

	for (int i = 0; i < m; i++) {
		int a = Int(1, n);
		Space();
		int b = Int(1, n);
		Endl();
		if (nowrap) assert(a <= b);
	}
}
