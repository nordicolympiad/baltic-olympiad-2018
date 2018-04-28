// Identity matrix, with most rows doubled
#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m", -1);
	int A = Arg("a");
	int realn = Arg("realn", N);
	int gena = Arg("gena", A);
	string type = Arg("type");
	string adjoin = Arg("adjoin", "");

	assert(N % 2 == 1);
	assert(realn % 2 == 1);
	int N2 = (realn+1)/2;

	int K;
	matdig mat, mat2;
	bool hadamard = (type == "hadamard");
	if (type == "id") tie(mat, K) = generateI(N2, gena);
	else if (hadamard) tie(mat, K) = generateMatrix(N2, gena);
	else assert(0);

	for (int i = 0; i < N2; i++) {
		mat2.push_back(mat[i]);
		if (hadamard) {
			dig d = randdig(gena);
			for (dig& d2 : mat[i]) d2 = add(d2, d, gena);
		}
		mat2.push_back(mat[i]);
	}
	mat2.resize(N+1);
	mat2.pop_back();
	randomize(mat2, A);
	adjoinMat(mat2, adjoin, &K, gena);
	randomize(mat2, A);
	if (M != -1) {
		assert((int)mat2[0].size() <= M);
		for (auto& v : mat2)
			v.resize(M);
	}
	randomize(mat2, A);
	print(mat2, K, A);
}
