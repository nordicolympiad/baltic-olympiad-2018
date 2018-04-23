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
	vector<vector<bool> > strings(N, vector<bool>(K));
	vector<bool> iscandidate(N, true);
	int ncand = N;
	string str;
	for (int i = 0; i < N; ++i) {
		cin >> str;
		for (int j = 0; j < K; ++j)
			strings[i][j] = (str[j] == '1');
	}
	while (ncand > 1) {
cerr << ncand << endl;
		vector<unsigned> weight(N);
		unsigned int totweight = 0;
		for (int i = 0; i < N; ++i) {
			weight[i] = rand();
			totweight += weight[i];
		}
		vector<unsigned> count(K, 0);
		for (int i = 0; i < N; ++i) {
			unsigned w = weight[i];
			for (int j = 0; j < K; ++j) {
				count[j] += (unsigned)strings[i][j] * w;
			}
		}
		for (int i = 0; i < N; ++i) {
			if (!iscandidate[i]) continue;
			unsigned sumdif = 0;
			for (int j = 0; j < K; ++j)
				sumdif += (strings[i][j] ? totweight - count[j] : count[j]);
			unsigned expectedsumdif = (totweight - weight[i]) * D;
			if (sumdif != expectedsumdif) {
				iscandidate[i] = false;
				--ncand;
			}
		}
	}
	int ans = -1;
	for (int i = 0; i < N; ++i) {
		if (iscandidate[i]) {
			ans = i;
		}
	}
	assert(ans != -1);
	for (int i = 0; i < N; ++i) if (i != ans) {
		int dif = 0;
		for (int j = 0; j < K; ++j)
			dif += (strings[i][j] != strings[ans][j]);
		assert(dif == D);
	}
	cout << ans + 1 << endl;
	return 0;
}
