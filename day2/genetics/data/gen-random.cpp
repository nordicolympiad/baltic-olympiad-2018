#include "gen-bitstrings.h"

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int K = Arg("m");
	int D = Arg("k");
	vector<string> strs;
	string first;
	for (int i = 0; i < K; ++i)
		first += randbit() ? '1' : '0';
	strs.push_back(first);
	for (int i = 0; i < N - 1; ++i) {
		string s = first;
		for (int d = 0; d < D; ++d) {
again:
			int j = (rand() >> 3) % K;
			if (s[j] != first[j])
				goto again;
			s[j] ^= 1;
		}
		strs.push_back(s);
	}
	int targetpos = (rand() >> 3) % N;
	swap(strs[0], strs[targetpos]);
	cerr << targetpos+1 << endl;
	cout << N << ' ' << K << ' ' << D << endl;
	for (int i = 0; i < N; ++i) {
		cout << strs[i] << endl;
	}
	return 0;
}
