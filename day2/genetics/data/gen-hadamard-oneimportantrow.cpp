// Hadamard matrix, plus a single random string that only the target is at the right distance to
#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m", -1);
	int A = Arg("a");
	int gena = Arg("gena", A);
	bool late = Arg("late", 0);

	int K;
	matdig mat, mat2;
	tie(mat, K) = generateMatrix(N, gena);
	if (M != -1) {
		assert(M >= (int)mat[0].size());
		for (auto& v : mat) v.resize(M);
	}
	M = (int)mat[0].size();

	randomize(mat, A);

	vdig nrow(K);
	for (dig& x : nrow) x = add(x, randdig_pos(A), A);
	nrow.resize(M);
	random_shuffle(nrow.begin(), nrow.end());
	int ansrow = late ? N-1 - rand() % (N / 20) : rand() % N;
	for (int i = 0; i < M; i++)
		nrow[i] = add(nrow[i], mat[ansrow][i], A);

	mat2.clear();
	for (int i = 0; i < N; i++) {
		if (i == ansrow || dif(mat[i], nrow) != K)
			mat2.push_back(move(mat[i]));
	}
	mat = move(mat2);
	mat.push_back(nrow);
	N = (int)mat.size();

	int ind = rand() % (late ? 5 : N);
	swap(mat[N - 1], mat[N - 1 - ind]);

	print(mat, K, A);
}
