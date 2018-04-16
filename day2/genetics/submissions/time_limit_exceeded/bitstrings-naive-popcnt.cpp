#include <iostream>
#include <cstdlib>
#include <bitset>
#include <string>
#include <vector>
using namespace std;

int main() {
	cin.sync_with_stdio(false);
	int N, K, D;
	cin >> N >> K >> D;
	int K2 = (K + 63) / 64;
	vector<vector<uint64_t>> strings(N, vector<uint64_t>(K2));
	vector<bool> iscandidate(N, true);
	string str;
	for (int i = 0; i < N; ++i) {
		cin >> str;
		for (int j = 0; j < K; ++j)
			strings[i][j / 64] |= (uint64_t)(str[j] == '1') << (j & 63);
	}
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (i == j) continue;
			size_t count = 0;
			for (int k = 0; k < K2; ++k) {
				count += bitset<64>(strings[i][k]^strings[j][k]).count();
			}
			if (count != (size_t)D) {
				// clog << j << ", " << flush;
				goto fail;
			}
		}
		cout << i + 1 << endl;
		return 0;
fail:;
	}
	return 1;
}
