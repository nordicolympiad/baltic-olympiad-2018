// Hadamard matrix, plus a single random string that only the target is at the right distance to
#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	bool late = Arg("late", 0);

	vector<vector<bool> > mat = generateMatrix(N), mat2;
	int K = N;
	int D = N / 2;

	randomize(mat);

	vector<bool> nrow(D, 1);
	nrow.resize(N, 0);
	random_shuffle(nrow.begin(), nrow.end());
	int ansrow = late ? N-1 - rand() % (N / 10) : rand() % N;
	for (int i = 0; i < K; i++)
		nrow[i] = nrow[i] ^ mat[ansrow][i];

	mat2.clear();
	for (int i = 0; i < N; i++) {
		if (i == ansrow || dif(mat[i], nrow) != D)
			mat2.push_back(move(mat[i]));
	}
	mat = move(mat2);
	mat.push_back(nrow);
	N = (int)mat.size();

	int ind = rand() % (late ? 5 : N);
	swap(mat[N - 1], mat[N - 1 - ind]);

	print(mat, D);
}
