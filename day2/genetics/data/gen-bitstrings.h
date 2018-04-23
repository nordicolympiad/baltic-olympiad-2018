#include "gen.h"
#include <algorithm>

static const int Iterations = 3; // a guess of the number of iterations required to solve a test-case

enum dig : int {};
typedef vector<dig> vdig;
typedef vector<vdig> matdig;

dig randdig(int A) {
	return (dig)((rand() >> 8) % A);
}

dig randdig_pos(int A) {
	return (dig)(1 + (rand() >> 8) % (A-1));
}

int solve(int N, int K, int D, const matdig& strings, vector<int>* out = nullptr) {
	assert(N == (int)strings.size());
	assert(K == (int)strings[0].size());
	vector<bool> iscandidate(N, true);
	int ncand = N;
	for (int c = 0; c < Iterations; ++c) {
		vector<unsigned> weight(N);
		unsigned int totweight = 0;
		for (int i = 0; i < N; ++i) {
			weight[i] = rand();
			totweight += weight[i];
		}
		vector<unsigned> count[4];
		count[0].assign(K, 0);
		count[1].assign(K, 0);
		count[2].assign(K, 0);
		count[3].assign(K, 0);
		for (int i = 0; i < N; ++i) {
			unsigned w = weight[i];
			for (int j = 0; j < K; ++j) {
				count[strings[i][j]][j] += w;
			}
		}
		for (int i = 0; i < N; ++i) {
			if (!iscandidate[i]) continue;
			unsigned sumdif = 0;
			for (int j = 0; j < K; ++j)
				sumdif += totweight - count[strings[i][j]][j];
			unsigned expectedsumdif = (totweight - weight[i]) * D;
			if (sumdif != expectedsumdif) {
				iscandidate[i] = false;
				--ncand;
			}
		}
		if (ncand == 0) break;
	}
	if (out != nullptr) {
		out->clear();
		for (int i = 0; i < N; i++) {
			if (iscandidate[i]) out->push_back(i);
		}
	}
	return ncand;
}

int dif(const vdig& a, const vdig& b) {
	int N = (int)a.size();
	assert(N == (int)b.size());
	int res = 0;
	for (int i = 0; i < N; i++) {
		res += a[i] != b[i];
	}
	return res;
}

dig add(dig a, dig b, int A) {
	if (A == 2 || A == 4) return (dig)(a ^ b);
	if (A == 3) return (dig)((a + b) % 3);
	assert(0);
}

dig sub(dig a, dig b, int A) {
	if (A == 2 || A == 4) return (dig)(a ^ b);
	if (A == 3) return (dig)((a - b + 3) % 3);
	assert(0);
}

static const int lut4[4][4] = {
	{0,0,0,0},
	{0,1,2,3},
	{0,2,3,1},
	{0,3,1,2},
};
int mul(int a, int b, int A) {
	if (A == 2) return a & b;
	if (A == 3) return a * b % 3;
	if (A == 4) return lut4[a][b];
	assert(0);
}

matdig tensor(matdig A, matdig B, int Av) {
	assert(A.size() == A[0].size());
	assert(B.size() == B[0].size());
	matdig C(A.size() * B.size(), vdig(A.size() * B.size()));
	for (size_t i1 = 0; i1 < A.size(); i1++)
	for (size_t i2 = 0; i2 < B.size(); i2++)
	for (size_t j1 = 0; j1 < A.size(); j1++)
	for (size_t j2 = 0; j2 < B.size(); j2++) {
		C[i1 * B.size() + i2][j1 * B.size() + j2] = add(A[i1][j1], B[i2][j2], Av);
	}
	return C;
}

pair<matdig, int> generateI(int N, int A) {
	matdig ret(N, vdig(N));
	for (int i = 0; i < N; i++) ret[i][i] = randdig_pos(A);
	return {ret, 2};
}

pair<matdig, int> generateMatrix(int N, int A) {
	assert(N % A == 0);
	matdig ret(1, vdig{1});

	if (A == 2) {
		dig a = (dig)0, b = (dig)1;
		matdig basis2 = {
			{a, b},
			{a, a}
		}, basis8 = {
			{b,b,b,a,b,a,a,a},
			{b,b,a,b,a,a,a,b},
			{b,a,b,a,a,a,b,b},
			{b,b,a,a,a,b,b,a},
			{b,a,a,a,b,b,a,b},
			{b,a,a,b,b,a,b,a},
			{b,a,b,b,a,b,a,a},
			{b,b,b,b,b,b,b,b},
		}, basis12 = {
			{b,b,a,b,b,b,a,a,a,b,a,a},
			{b,a,b,b,b,a,a,a,b,a,a,b},
			{b,b,b,b,a,a,a,b,a,a,b,a},
			{b,b,b,a,a,a,b,a,a,b,a,b},
			{b,b,a,a,a,b,a,a,b,a,b,b},
			{b,a,a,a,b,a,a,b,a,b,b,b},
			{b,a,a,b,a,a,b,a,b,b,b,a},
			{b,a,b,a,a,b,a,b,b,b,a,a},
			{b,b,a,a,b,a,b,b,b,a,a,a},
			{b,a,a,b,a,b,b,b,a,a,a,b},
			{b,a,b,a,b,b,b,a,a,a,b,a},
			{b,b,b,b,b,b,b,b,b,b,b,b},
		};

		int orign = N;
		assert(N > 0);
		while (N % 3 == 0) {
			N /= 3;
			if (N % 4 != 0) die(to_string(orign) + " cannot be written as 12^a * 2^b");
			N /= 4;
			ret = tensor(ret, basis12, 2);
		}
		if (N & (N-1)) die(to_string(orign) + " cannot be written as 12^a * 2^b");
		while (N % 8 == 0) {
			N /= 8;
			ret = tensor(ret, basis8, 2);
		}
		while (N % 2 == 0) {
			N /= 2;
			ret = tensor(ret, basis2, 2);
		}
		assert(N == 1);
		N = orign;
	}
	else if (A == 4) {
		dig a = (dig)0, b = (dig)1, c = (dig)2, d = (dig)3;
		matdig basis4 = {
			{a,a,a,a},
			{a,b,c,d},
			{a,c,d,b},
			{a,d,b,c},
		};

		int orign = N;
		assert(N > 0);
		while (N > 1) {
			if (N % 4 != 0) die(to_string(orign) + " cannot be written as 4^k");
			N /= 4;
			ret = tensor(ret, basis4, 4);
		}
		assert(N == 1);
		N = orign;
	}
	else if (A == 3) {
		dig a = (dig)0, b = (dig)1, c = (dig)2;
		matdig basis3 = {
			{a,a,a},
			{a,b,c},
			{a,c,b},
		};

		int orign = N;
		assert(N > 0);
		while (N > 1) {
			if (N % 3 != 0) die(to_string(orign) + " cannot be written as 3^k");
			N /= 3;
			ret = tensor(ret, basis3, 3);
		}
		assert(N == 1);
		N = orign;
	}
	else assert(0);

	for (int i = 0; i < N; i++) {
		dig val = randdig(A);
		if (!val) continue;
		for (int j = 0; j < N; j++)
			ret[i][j] = add(ret[i][j], val, A);
	}

	return {ret, N - N / A};
}

void randomize(matdig& mat, int A) {
	random_shuffle(mat.begin(), mat.end());
	int N = (int)mat.size(), M = (int)mat[0].size();
	vector<dig> bits(M);
	vector<int> perm(M);
	for (int j = 0; j < M; ++j) {
		perm[j] = j;
		bits[j] = randdig(A);
	}
	random_shuffle(perm.begin(), perm.end());
	matdig out(N, vdig(M));
	for (int i = 0; i < N; ++i)
	for (int j = 0; j < M; ++j) {
		out[i][j] = add(mat[i][perm[j]], bits[j], A);
	}
	mat = out;
}

void adjoinMat(matdig& mat, const string& adjoin, int* K, int A) {
	matdig mat2;
	int K2;
	if (adjoin == "hadamard") {
		int size = (int)mat.size();
		while (size & (size-1)) size++;
		tie(mat2, K2) = generateMatrix(size, A);
		randomize(mat2, A);
		mat2.erase(mat2.begin() + mat.size(), mat2.end());
	}
	else if (adjoin == "id") {
		tie(mat2, K2) = generateI((int)mat.size(), A);
	}
	else {
		assert(adjoin.empty());
		return;
	}
	*K += K2;

	assert(mat2.size() == mat.size());
	int si = (int)mat.size();
	for (int i = 0; i < si; i++) {
		mat[i].insert(mat[i].end(), mat2[i].begin(), mat2[i].end());
	}
}

char toch(dig d, int A) {
	assert(0 <= d && d < A);
	return "ACGT"[d];
}

void print(matdig& mat, int K, int A) {
	int N = (int)mat.size(), M = (int)mat[0].size();
	vector<string> out(N, string(M, 'X'));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
			out[i][j] = toch(mat[i][j], A);
	cout << N << ' ' << M << ' ' << K << endl;
	for (int i = 0; i < N; ++i) {
		cout << out[i] << endl;
	}
}
