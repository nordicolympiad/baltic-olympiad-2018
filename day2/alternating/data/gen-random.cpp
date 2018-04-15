#include "gen.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
	cout << N << ' ' << M << endl;
	for (int i = 0; i < M; ++i) {
        int a = rand() % N;
        int b = rand() % N;
        cout << a+1 << ' ' << b+1 << endl;
	}
	return 0;
}
