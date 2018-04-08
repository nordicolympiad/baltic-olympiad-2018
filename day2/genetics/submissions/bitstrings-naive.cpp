#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
using namespace std;

int main() {
	cin.sync_with_stdio(false);
	int N, K, D;
	cin >> N >> K >> D;
	vector<vector<bool> > strings(N, vector<bool>(K));
	vector<bool> iscandidate(N, true);
	string str;
	for (int i = 0; i < N; ++i) {
		cin >> str;
		for (int j = 0; j < K; ++j)
			strings[i][j] = (str[j] == '1');
	}
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (i == j) continue;
			int count = 0;
			for (int k = 0; k < K; ++k) {
				count += (strings[i][k] != strings[j][k]);
			}
			if (count != D) {
				clog << j << ", " << flush;
				goto fail;
			}
		}
		cout << i + 1 << endl;
		// return 0;
fail:;
	}
	return 1;
}
