// Hadamard matrix, but with O(log n) random bits flipped
#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int A = Arg("a");
	int gena = Arg("gena", A);
	int its = Arg("its", 0);
	bool late = Arg("late", 0);

	matdig mat, mat2;
	int K;
	tie(mat, K) = generateMatrix(N, gena);
	randomize(mat, A);

	int M = (int)mat[0].size();
	assert(solve(N, M, K, mat) == N);
	assert(N >= 4);

	int cand = N;
	while (cand > 1) {
		mat2 = mat;
		int row = rand() % N;
		if (late)
			row = N-1 - rand() % (N / 4);
		assert(0 <= row && row < N);
		int col1 = rand() % M;
		int col2 = rand() % M;
		dig d = randdig_pos(A);
		mat2[row][col1] = add(mat2[row][col1], d, A);
		mat2[row][col2] = sub(mat2[row][col2], d, A);
		int ncand = solve(N, M, K, mat2);
		clog << ncand << endl;
		if (ncand && ncand < cand) {
			mat.swap(mat2);
			cand = ncand;
			if (--its == 0) break;
		}
	}
	if (cand > 1) {
		while (true) {
			vector<int> cands;
			int r = solve((int)mat.size(), M, K, mat, &cands);
			assert(r > 0);
			if (r == 1) break;
			random_shuffle(cands.begin(), cands.end());
			cands.pop_back();
			sort(cands.begin(), cands.end());
			reverse(cands.begin(), cands.end());
			for (int x : cands) {
				mat.erase(mat.begin() + x);
			}
		}
	}

	print(mat, K, A);
}
