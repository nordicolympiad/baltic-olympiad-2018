// Identity matrix, with most rows doubled
#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m", N);

	assert(N % 2 == 1);
	assert((N+1)/2 <= M);

	vector<vector<bool>> mat(N, vector<bool>(M));
	for (int i = 0; i < N/2; i++) {
		mat[2*i+0][i] = 1;
		mat[2*i+1][i] = 1;
	}
	mat[N-1][N/2] = 1;
	randomize(mat);
	print(mat, 2);
}
