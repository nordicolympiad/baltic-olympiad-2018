// Identity matrix, with most rows doubled
#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m", -1);
	string type = Arg("type");
	string adjoin = Arg("adjoin", "");

	assert(N % 2 == 1);
	int N2 = (N+1)/2;

	int K;
	matbool mat, mat2;
	if (type == "id") mat = generateI(N2), K = 2;
	else if (type == "hadamard") mat = generateMatrix(N2), K = N2 / 2;
	else assert(0);

	for (int i = 0; i < N2; i++) {
		mat2.push_back(mat[i]);
		mat2.push_back(mat[i]);
	}
	mat2.pop_back();
	randomize(mat2);
	adjoinMat(mat2, adjoin, &K);
	randomize(mat2);
	if (M != -1) {
		assert((int)mat2[0].size() <= M);
		for (auto& v : mat2)
			v.resize(M);
	}
	randomize(mat2);
	print(mat2, K);
}
