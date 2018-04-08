// Hadamard matrix, but with O(log n) random bits flipped
#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	bool late = Arg("late", 0);

	vector<vector<bool>> mat = generateMatrix(N), mat2;
	randomize(mat);

	int K = N;
	int D = N / 2;
	assert(solve(N, K, D, mat) == N);
	assert(N >= 4);

	int cand = N;
	while (cand > 1) {
		mat2 = mat;
		int row = rand() % N;
		if (late)
			row = N - rand() % (N / 4);
		int col1 = rand() % K;
		int col2 = rand() % K;
		mat2[row][col1] = mat2[row][col1] ^ 1;
		mat2[row][col2] = mat2[row][col2] ^ 1;
		int ncand = solve(N, K, D, mat2);
		clog << ncand << endl;
		if (ncand && ncand < cand) {
			mat.swap(mat2);
			cand = ncand;
		}
	}

	print(mat, D);
}
