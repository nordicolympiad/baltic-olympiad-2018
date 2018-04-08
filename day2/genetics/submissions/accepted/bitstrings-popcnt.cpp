// This is faster than bitstrings.cpp, but only by a factor 2... Turns out the
// main algorithm is about as slow as matrix transposition/bitpacking...
// With template code it wouldn't be impossible to make a task out of the last
// ounce of optimization, but... no.
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>
using namespace std;

int main() {
	cin.sync_with_stdio(false);
	int N, K, D;
	cin >> N >> K >> D;
	int N2 = (N + 63) / 64;
	vector<vector<uint64_t>> strings(K, vector<uint64_t>(N2));
	vector<bool> iscandidate(N, true);
	vector<int> rc(N);
	vector<vector<int>> mat(N, vector<int>(K));
	int ncand = N;
	string str;
#ifdef HEX
	getline(cin, str);
	assert(K % 4 == 0);
	for (int i = 0; i < N; ++i) {
		getline(cin, str);
		int co = 0;
		for (int j = 0; j < K / 4; ++j) {
			int v = ('0' <= str[j] && str[j] <= '9' ? str[j] - '0' : str[j] - 'a' + 10);
			int v0 = v >> 0 & 1;
			int v1 = v >> 1 & 1;
			int v2 = v >> 2 & 1;
			int v3 = v >> 3 & 1;
			mat[i][4*j+0] = 1 - 2 * v0;
			mat[i][4*j+1] = 1 - 2 * v1;
			mat[i][4*j+2] = 1 - 2 * v2;
			mat[i][4*j+3] = 1 - 2 * v3;
			co += v0 + v1 + v2 + v3;
			strings[4*j+0][i / 64] |= uint64_t(v0) << (i & 63);
			strings[4*j+1][i / 64] |= uint64_t(v1) << (i & 63);
			strings[4*j+2][i / 64] |= uint64_t(v2) << (i & 63);
			strings[4*j+3][i / 64] |= uint64_t(v3) << (i & 63);
		}
		rc[i] = co;
	}
#else
	for (int i = 0; i < N; ++i) {
		cin >> str;
		int co = 0;
		for (int j = 0; j < K; ++j) {
			int v = (str[j] == '1');
			strings[j][i / 64] |= uint64_t(v) << (i & 63);
			mat[i][j] = 1 - 2 * v;
			co += v;
		}
		rc[i] = co;
	}
#endif

	vector<bool> included(N);
	vector<uint64_t> bincluded(N2);
	vector<int> count(K, 0);
	while (ncand > 1) {
		unsigned int nincluded = 0;
		bincluded.assign(N2, 0);
		for (int i = 0; i < N; ++i) {
			included[i] = (rand() >> 14) & 1;
			bincluded[i / 64] |= uint64_t(included[i]) << (i & 63);
			nincluded += included[i] ? 1 : 0;
		}
		for (int j = 0; j < K; ++j) {
			int co = 0;
			for (int i = 0; i < N2; ++i)
				co += __builtin_popcountll(strings[j][i] & bincluded[i]);
			count[j] = co;
		}
		for (int i = 0; i < N; ++i) {
			if (!iscandidate[i]) continue;
			unsigned int sumdif = 0;
			for (int j = 0; j < K; ++j)
				sumdif += count[j] * mat[i][j];
			sumdif += nincluded * rc[i];
			unsigned int expectedsumdif = nincluded * D - (included[i] ? D : 0);
			if (sumdif != expectedsumdif) {
				iscandidate[i] = false;
				--ncand;
			}
		}
	}
	for (int i = 0; i < N; ++i) {
		if (iscandidate[i]) {
			cout << i + 1 << endl;
			return 0;
		}
	}
	cout << "nothing!" << endl;
	return 1;
}
