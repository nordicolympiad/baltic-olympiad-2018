#include "gen.h"
#include <algorithm>

static const int Iterations = 50; // a guess of the number of iterations required to solve a test-case

int solve(int N, int K, int D, const vector<vector<bool> >& strings, vector<int>* out = nullptr) {
	assert(N == (int)strings.size());
	assert(K == (int)strings[0].size());
	vector<bool> iscandidate(N, true);
	int ncand = N;
	for (int c = 0; c < Iterations; ++c) {
		vector<bool> included(N);
		unsigned int nincluded = 0;
		for (int i = 0; i < N; ++i) {
			included[i] = (rand() >> 14) & 1;
			nincluded += included[i] ? 1 : 0;
		}
		vector<int> count(K, 0);
		for (int i = 0; i < N; ++i) {
			if (!included[i]) continue;
			for (int j = 0; j < K; ++j) {
				count[j] += strings[i][j];
			}
		}
		for (int i = 0; i < N; ++i) {
			if (!iscandidate[i]) continue;
			unsigned int sumdif = 0;
			for (int j = 0; j < K; ++j)
				sumdif += (strings[i][j] ? nincluded - count[j] : count[j]);
			unsigned int expectedsumdif = nincluded * D - (included[i] ? D : 0);
			if (sumdif != expectedsumdif) {
				iscandidate[i] = false;
				--ncand;
				if (ncand == 0) return 0;
			}
		}
	}
	if (out != nullptr) {
		out->clear();
		for (int i = 0; i < N; i++) {
			if (iscandidate[i]) out->push_back(i);
		}
	}
	return ncand;
}

int dif(const vector<bool>& a, const vector<bool>& b) {
	int N = (int)a.size();
	assert(N == (int)b.size());
	int res = 0;
	for (int i = 0; i < N; i++) {
		res += a[i] != b[i];
	}
	return res;
}

typedef vector<vector<bool>> matbool;
matbool tensor(matbool A, matbool B) {
	assert(A.size() == A[0].size());
	assert(B.size() == B[0].size());
	matbool C(A.size() * B.size(), vector<bool>(A.size() * B.size()));
	for (size_t i1 = 0; i1 < A.size(); i1++)
	for (size_t i2 = 0; i2 < B.size(); i2++)
	for (size_t j1 = 0; j1 < A.size(); j1++)
	for (size_t j2 = 0; j2 < B.size(); j2++) {
		C[i1 * B.size() + i2][j1 * B.size() + j2] = A[i1][j1] ^ B[i2][j2];
	}
	return C;
}

vector<vector<bool>> generateI(int N) {
	vector<vector<bool>> ret(N, vector<bool>(N));
	for (int i = 0; i < N; i++) ret[i][i] = 1;
	return ret;
}

vector<vector<bool>> generateMatrix(int N) {
	vector<vector<bool>> basis2 = {
		{1, 0},
		{1, 1}
	}, basis8 = {
		{1,1,1,0,1,0,0,0},
		{1,1,0,1,0,0,0,1},
		{1,0,1,0,0,0,1,1},
		{1,1,0,0,0,1,1,0},
		{1,0,0,0,1,1,0,1},
		{1,0,0,1,1,0,1,0},
		{1,0,1,1,0,1,0,0},
		{1,1,1,1,1,1,1,1},
	}, basis12 = {
		{1,1,0,1,1,1,0,0,0,1,0,0},
		{1,0,1,1,1,0,0,0,1,0,0,1},
		{1,1,1,1,0,0,0,1,0,0,1,0},
		{1,1,1,0,0,0,1,0,0,1,0,1},
		{1,1,0,0,0,1,0,0,1,0,1,1},
		{1,0,0,0,1,0,0,1,0,1,1,1},
		{1,0,0,1,0,0,1,0,1,1,1,0},
		{1,0,1,0,0,1,0,1,1,1,0,0},
		{1,1,0,0,1,0,1,1,1,0,0,0},
		{1,0,0,1,0,1,1,1,0,0,0,1},
		{1,0,1,0,1,1,1,0,0,0,1,0},
		{1,1,1,1,1,1,1,1,1,1,1,1},
	};

	vector<vector<bool>> ret(1, vector<bool>{1});

	int orign = N;
	assert(N > 0);
	while (N % 3 == 0) {
		N /= 3;
		if (N % 4 != 0) die(to_string(orign) + " cannot be written as 12^a * 2^b");
		N /= 4;
		ret = tensor(ret, basis12);
	}
	if (N & (N-1)) die(to_string(orign) + " cannot be written as 12^a * 2^b");
	while (N % 8 == 0) {
		N /= 8;
		ret = tensor(ret, basis8);
	}
	while (N % 2 == 0) {
		N /= 2;
		ret = tensor(ret, basis2);
	}
	assert(N == 1);

	for (int i = 0; i < orign; i++) if (randbit()) {
		for (int j = 0; j < orign; j++)
			ret[i][j] = ret[i][j] ^ 1;
	}

	return ret;
}

void randomize(matbool& mat) {
	random_shuffle(mat.begin(), mat.end());
	int N = (int)mat.size(), M = (int)mat[0].size();
	vector<bool> bits(M);
	vector<int> perm(M);
	for (int j = 0; j < M; ++j) {
		perm[j] = j;
		bits[j] = randbit();
	}
	random_shuffle(perm.begin(), perm.end());
	matbool out(N, vector<bool>(M));
	for (int i = 0; i < N; ++i)
	for (int j = 0; j < M; ++j) {
		out[i][j] = mat[i][perm[j]] ^ bits[j];
	}
	mat = out;
}

void adjoinMat(matbool& mat, const string& adjoin, int* K) {
	matbool mat2;
	if (adjoin == "hadamard") {
		int size = (int)mat.size();
		while (size & (size-1)) size++;
		mat2 = generateMatrix(size);
		randomize(mat2);
		mat2.erase(mat2.begin() + mat.size(), mat2.end());
		*K += size/2;
	}
	else if (adjoin == "id") {
		mat2 = generateI((int)mat.size());
		*K += 2;
	}
	else {
		assert(adjoin.empty());
		return;
	}

	assert(mat2.size() == mat.size());
	int si = (int)mat.size();
	for (int i = 0; i < si; i++) {
		mat[i].insert(mat[i].end(), mat2[i].begin(), mat2[i].end());
	}
}

#ifdef HEX
void print(matbool& mat, int K) {
	int N = (int)mat.size(), M = (int)mat[0].size();
	int M2 = (M+3)/4 * 4;
	vector<string> out(N, string(M2/4, 'X'));
	for (int i = 0; i < N; ++i) {
		// pad with zeroes; doesn't change anything
		for (int i = M; i < M2; ++i)
			mat[i].push_back(false);
		for (int j = 0; j < M2; j += 4) {
			int v = (mat[i][j + 3] << 3) | (mat[i][j + 2] << 2) | (mat[i][j + 1] << 1) | mat[i][j];
			out[i][j / 4] = (char)(v < 10 ? '0' + v : 'a' + v - 10);
		}
	}
	cout << N << ' ' << M2 << ' ' << K << endl;
	for (int i = 0; i < N; ++i) {
		cout << out[i] << endl;
	}
}
#else
void print(matbool& mat, int K) {
	int N = (int)mat.size(), M = (int)mat[0].size();
	vector<string> out(N, string(M, 'X'));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
			out[i][j] = mat[i][j] ? '1' : '0';
	cout << N << ' ' << M << ' ' << K << endl;
	for (int i = 0; i < N; ++i) {
		cout << out[i] << endl;
	}
}
#endif
