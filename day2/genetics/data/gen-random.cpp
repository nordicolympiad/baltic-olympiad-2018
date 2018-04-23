#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
	int K = Arg("k");
	int A = Arg("a");

	matdig strs;
	vdig first;
	for (int i = 0; i < M; ++i)
		first.push_back(randdig(A));
	strs.push_back(first);
	for (int i = 0; i < N - 1; ++i) {
		vdig s = first;
		for (int k = 0; k < K; ++k) {
again:
			int j = (rand() >> 3) % M;
			if (s[j] != first[j])
				goto again;
			s[j] = add(s[j], randdig_pos(A), A);
		}
		strs.push_back(s);
	}
	int targetpos = (rand() >> 3) % N;
	swap(strs[0], strs[targetpos]);
	cerr << targetpos+1 << endl;
	print(strs, K, A);
	return 0;
}
