#include "gen.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
    bool feasible = args.has_var("feasible") ? Arg("feasible") : 0;
	cout << N << ' ' << M << endl;
	for (int i = 0; i < M - 2*feasible; ++i) {
        int a = i;
        int b = N-i-1;
        cout << a+1 << ' ' << b+1 << endl;
	}
    if (feasible) {
        cout << "1 1" << endl;
        cout << N << " " << N << endl;
    }
	return 0;
}
