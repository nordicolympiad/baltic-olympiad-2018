// Usage: ./validator .in-file .ans-file dir <in >out
#include <iostream>
#include <string>
#include <vector>
#include <signal.h>
using namespace std;

const char* out_dir = nullptr;

[[noreturn]]
void reject_raw(const string& msg) {
	cerr << msg << endl;
	if (out_dir) {
		string fname = out_dir + string("/judgemessage.txt");
		ofstream fout(fname.c_str());
		fout << msg << endl;
	}
	exit(43);
}

[[noreturn]]
void reject_line(const char* msg, string line) {
	if (line.size() > 1000) line = line.substr(0, 1000) + "...";
	cout << "-1" << endl;
	reject_raw(msg + string(". input: ") + line);
}

[[noreturn]]
void accept() {
	exit(42);
}

int readnum(const char* str, int max, const string& line) {
	if (!str[0]) reject_line("invalid format (empty token)", line);
	int cur = 0;
	while (*str) {
		int dig = *str++ - '0';
		if (dig < 0 || dig > 9) reject_line("invalid format (not a digit)", line);
		cur *= 10;
		cur += dig;
		if (cur > max) reject_line("out of bounds (too large)", line);
	}
	if (cur < 1) reject_line("out of bounds (zero)", line);
	return cur;
}

struct Strat {
	int N, M, K;
	Strat(int N, int M, int K) : N(N), M(M), K(K) {}
	virtual ~Strat() {}
	virtual int query(int x, int y, int z) = 0;
	virtual long long maxval() const = 0;
	int oob_query(int x, int y, int z) {
		if (oob(x,y,z)) return -1;
		return query(x, y, z);
	}
	bool oob(int x, int y, int z) const {
		return (x < 0 || y < 0 || z < 0 || x >= N || y >= M || z >= K);
	}
};

struct AddStrat : Strat {
	Strat* inner;
	int base;
	AddStrat(Strat* other) : Strat(other->N, other->M, other->K), inner(other) {
		base = rand() % 1000000 + 1000;
	}
	int query(int x, int y, int z) override {
		return inner->query(x, y, z) + base;
	}
	long long maxval() const override { return inner->maxval() + base; }
};

// For paths that increase by 1 in each step, this wrapper puts the path only
// on even indices and removes shortcuts.
struct SpacedPathStrat : Strat {
	Strat* inner;
	SpacedPathStrat(Strat* other, int N, int M, int K) : Strat(N, M, K), inner(other) {}
	int query(int x, int y, int z) override {
		assert(!oob(x,y,z));
		int odds = x%2 + y%2 + z%2;
		if (odds == 3) return 1;
		if (odds == 2) return 2;
		if (odds == 1) {
			int x1 = x, y1 = y, z1 = z;
			int x2 = x, y2 = y, z2 = z;
			if (x%2) --x1, ++x2;
			if (y%2) --y1, ++y2;
			if (z%2) --z1, ++z2;
			int a = inner->oob_query(x1/2, y1/2, z1/2)*2 + 10;
			int b = inner->oob_query(x2/2, y2/2, z2/2)*2 + 10;
			assert(a != b); // TODO: remove this later. better weak test data than judge error
			if (abs(a - b) != 2) return 3;
			return a + (b - a) / 2;
		}
		return inner->query(x/2, y/2, z/2)*2 + 10;
	}
	long long maxval() const override { return inner->maxval() * 2 + 10; }
};

struct RandomStrat : Strat {
	uint64_t seed;
	RandomStrat(int N, int M, int K) : Strat(N, M, K) {
		seed = rand();
		seed <<= 30;
		seed |= rand();
	}
	int query(int x, int y, int z) override {
		uint64_t val = x * M * K + y * K + z;
		val += seed;
		val *= 1236182312312311ULL;
		val = rot(val, 10) ^ rot(val, 40) ^ rot(val, 27);
		val *= 981723ULL;
		return (int)(val >> 35);
	}
	long long maxval() const override { return 1 << 29; }
	uint64_t rot(uint64_t v, int amt) {
		return (v >> amt) | (v << (64 - amt));
	}
};

struct SpaceFillStrat : Strat {
	uint64_t seed;
	SpaceFillStrat(int N, int M, int K) : Strat(N, M, K) {
		seed = rand();
		seed <<= 30;
		seed |= rand();
	}
	int query(int x, int y, int z) override {
		// TODO
		return x + y + z;
	}
	long long maxval() const override { return N+M+K; }
};

Strat* readStrat(istream& in, int N, int M, int K) {
	string str;
	in >> str;
	if (str == "random") return new RandomStrat(N, M, K);
	if (str == "spacefill") return new SpaceFillStrat(N, M, K);
	if (str == "spaced") {
		int N2 = (N+1)/2;
		int M2 = (M+1)/2;
		int K2 = (K+1)/2;
		return new SpacedPathStrat(readStrat(in, N2, M2, K2), N, M, K);
	}
	assert(0 && "unknown strategy");
	abort();
}

int main(int argc, char** argv) {
	signal(SIGPIPE, SIG_IGN);
	assert(argc >= 2);
	if (argc >= 4) out_dir = argv[3];
	ifstream fin(argv[1]);
	int N, M, K, Q, seed;
	fin >> N >> M >> K >> Q >> seed;
	srand(seed);

	Strat* strat = readStrat(fin, N, M, K);
	strat = new AddStrat(strat);
	assert(strat->maxval() < 1000000000);

	auto works = [&](int x, int y, int z) {
		int v = strat->query(x, y, z);
		return
			v >= strat->oob_query(x-1,y,z) &&
			v >= strat->oob_query(x+1,y,z) &&
			v >= strat->oob_query(x,y-1,z) &&
			v >= strat->oob_query(x,y+1,z) &&
			v >= strat->oob_query(x,y,z-1) &&
			v >= strat->oob_query(x,y,z+1);
	};

	cout << N << ' ' << M << ' ' << K << ' ' << Q << endl;
	string line;
	vector<const char*> tokens;
	for (;;) {
		if (!getline(cin, line)) reject_raw("eof");

		// Split on whitespace
		line += ' ';
		tokens.clear();
		size_t ind = 0;
		while (ind != (int)line.size()) {
			size_t ind2 = line.find(' ', ind);
			assert(ind2 != string::npos);
			line[ind2] = '\0';
			tokens.push_back(line.data() + ind);
			ind = ind2 + 1;
		}

		if (tokens.size() != 4) reject_line("invalid format (must have 4 tokens)", line);
		const char *type = tokens[0];
		if (strcmp(type, "?") && strcmp(type, "!")) reject_line("invalid format (invalid query type)", line);
		int x = readnum(tokens[1], N, line) - 1;
		int y = readnum(tokens[2], M, line) - 1;
		int z = readnum(tokens[3], K, line) - 1;

		if (type[0] == '!') {
			if (works(x, y, z)) accept();
			else reject_raw("not a local maximum");
		}
		int r = strat->query(x, y, z) + 1;
		assert(1 <= r && r <= 1000000000);
		cout << r << endl;
	}
}
