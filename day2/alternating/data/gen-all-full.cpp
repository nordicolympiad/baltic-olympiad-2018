#include "gen.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
	cout << N << ' ' << M << endl;
	for (int i = 0; i < M; ++i) {
        cout << "1 " << N << endl;
	}
	return 0;
}
