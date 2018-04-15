#include "gen.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
	cout << N << ' ' << M << endl;
    assert(2*M <= N);
	for (int i = 0; i < M; ++i) {
        int a = i;
        if (i == 1)
            a = 0;
        int b = i+M;
        cout << a+1 << ' ' << b+1 << endl;
	}
	return 0;
}
