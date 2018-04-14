// Usage: ./validator .in-file .ans-file dir <in >out
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <signal.h>
#include <cassert>
#include <cstring>
#include <cmath>
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

[[noreturn]]
void judge_error2(const char* a, const char* b, const char* c) {
	string msg = string(a) + b + c;
	cerr << msg << endl;
	if (out_dir) {
		string fname = out_dir + string("/judgeerror.txt");
		ofstream fout(fname.c_str());
		fout << msg << endl;
	}
	abort();
}

#undef assert
#define STR2(x) #x
#define STR(x) STR2(x)
#define assert(x) do { if (!(x)) { judge_error2(__FILE__ ":" STR(__LINE__) ": ", __PRETTY_FUNCTION__, ": Assertion `" #x "' failed."); } } while (0)

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

uint64_t rand64() {
	uint64_t ret = rand();
	ret <<= 31;
	ret |= rand();
	return ret;
}

uint64_t rot(uint64_t v, int amt) {
	return (v >> amt) | (v << (64 - amt));
}

uint64_t Hash(uint64_t val, uint64_t seed) {
	val += seed;
	val *= 1236182312312311ULL;
	val = rot(val, 10) ^ rot(val, 40) ^ rot(val, 27);
	val *= 981723ULL;
	return (int)(val >> 35);
}

#define repd(i) for (int i = 0; i < P::DIM; i++)
struct P {
	enum { DIM = 3 };
	array<int, DIM> ar;
	static P K(int x) {
		array<int, DIM> ar;
		repd(i) ar[i] = x;
		return {ar};
	}
	P idiv(int by) const {
		P ret = *this;
		repd(i) ret.ar[i] /= by;
		return ret;
	}
	int count_odd() const {
		int ret = 0;
		repd(i) ret += ar[i]&1;
		return ret;
	}
	int sum() const {
		int sum = 0;
		repd(i) sum += ar[i];
		return sum;
	}
	int dimension() const {
		int dim = 0;
		repd(i) if (ar[i] != 1) dim = i+1;
		return dim;
	}
	uint64_t hash(P dims, uint64_t seed) const {
		uint64_t r = 0;
		repd(i) r = r * dims.ar[i] + ar[i];
		return Hash(r, seed);
	}
};
bool operator<=(const P& a, const P& b) {
	repd(i) if (a.ar[i] > b.ar[i]) return false;
	return true;
}
bool operator==(const P& a, const P& b) {
	repd(i) if (a.ar[i] != b.ar[i]) return false;
	return true;
}
bool operator<(const P& a, const P& b) {
	repd(i) if (a.ar[i] >= b.ar[i]) return false;
	return true;
}
P operator+(const P& a, const P& b) {
	P ret; repd(i) ret.ar[i] = a.ar[i] + b.ar[i]; return ret;
}
P operator-(const P& a, const P& b) {
	P ret; repd(i) ret.ar[i] = a.ar[i] - b.ar[i]; return ret;
}
P operator*(const P& a, int by) {
	P ret; repd(i) ret.ar[i] = a.ar[i] * by; return ret;
}
istream& operator>>(istream& is, P& p) {
	repd(i) is >> p.ar[i];
	return is;
}

struct Strat {
	P dims;
	Strat(P dims) : dims(dims) {
		assert(P::K(1) <= dims);
	}
	virtual ~Strat() {}
	virtual int query(P x) = 0;
	virtual long long maxval() const = 0;
	int oob_query(P x) {
		if (oob(x)) return -1;
		return query(x);
	}
	bool oob(P x) const {
		return !(P::K(0) <= x && x < dims);
	}
};
Strat* readStrat(P dims, istream& cin);

struct AddStrat : Strat {
	Strat* inner;
	int base;
	AddStrat(Strat* other) : Strat(other->dims), inner(other) {
		base = rand() % 1000000 + 1000;
	}
	int query(P x) override {
		return inner->query(x) + base;
	}
	long long maxval() const override { return inner->maxval() + base; }
};

// For paths that increase by 1 in each step, this wrapper puts the path only
// on even indices and removes shortcuts.
struct SpacedPathStrat : Strat {
	Strat* inner;
	SpacedPathStrat(P dims, istream& cin) : Strat(dims) {
		inner = readStrat((dims+P::K(2)).idiv(2), cin);
	}
	int query(P p) override {
		assert(!oob(p));
		int odds = p.count_odd();
		if (odds > 1) return P::DIM+1 - odds;
		if (odds == 1) {
			P A = p.idiv(2), B = (p + P::K(1)).idiv(2);
			int a = inner->query(A)*2 + 10;
			int b = inner->query(B)*2 + 10;
			if (abs(a - b) > 2) return P::DIM+1 - odds;
			return a + (b - a) / 2;
		}
		return inner->query(p.idiv(2))*2 + 10;
	}
	long long maxval() const override { return inner->maxval() * 2 + 10; }
};

// This puts very small values at the edges, making only the middle relevant.
struct PadStrat : Strat {
	Strat* inner;
	P lo, hi;
	PadStrat(P dims, istream& cin) : Strat(dims) {
		cin >> lo >> hi;
		inner = readStrat(dims - lo - hi, cin);
	}
	int query(P x) override {
		assert(!oob(x));
		int sum = 0;
		repd(i) sum += ed(x.ar[i], lo.ar[i], hi.ar[i], dims.ar[i]);
		if (sum) return 1000000 - sum;
		return inner->query(x - lo) + 1000000;
	}
	int ed(int x, int x0, int x1, int w) { // distance away from the middle
		if (x < x0) return x0 - x;
		if (x >= w-x1) return x - (w-x1) + 1;
		return 0;
	}
	long long maxval() const override { return inner->maxval() + 1000000; }
};

// Random function.
struct RandomStrat : Strat {
	uint64_t seed;
	RandomStrat(P dims) : Strat(dims) {
		seed = rand64();
	}
	int query(P x) override {
		return (int)(x.hash(dims, seed) >> 35);
	}
	long long maxval() const override { return 1 << 29; }
};

struct SpaceFillStrat : Strat {
	uint64_t seed;
	SpaceFillStrat(P dims) : Strat(dims) {
		seed = rand64();
	}
	int query(P x) override {
		// TODO
		return x.sum();
	}
	long long maxval() const override { return dims.sum(); }
};

// Unimodal function, which is piecewise linear and discontinuous at its peak.
struct OneDimPeakStrat : Strat {
	int pivot, leftBase, rightBase, N;
	const int MAX_BASE = 100000000;
	const int PIVOT_VAL = 200000000;
	OneDimPeakStrat(P dims, istream& cin) : Strat(dims) {
		assert(dims.dimension() == 1);
		N = dims.ar[0];
		int same;
		cin >> same;
		pivot = rand() % N;
		leftBase = rand() % MAX_BASE;
		rightBase = same ? leftBase : rand() % MAX_BASE;
	}
	int query(P p) override {
		int x = p.ar[0];
		if (x == pivot) return PIVOT_VAL;
		if (x < pivot) return leftBase + x * 100;
		return rightBase + (N - x) * 100;
	}
	long long maxval() const override { return max(PIVOT_VAL, MAX_BASE + 100*dims.sum()); }
};

// Continuous unimodal function.
struct OneDimPeakStrat2 : Strat {
	int pivot, N;
	const int PIVOT_VAL = 200000000, SUB_VAL = 190000000;
	enum class Type {
		sqrt, pw
	} type;
	OneDimPeakStrat2(P dims, istream& cin) : Strat(dims) {
		assert(dims.dimension() == 1);
		N = dims.ar[0];
		pivot = rand() % N;
		string stype;
		cin >> stype;
		if (stype == "sqrt") type = Type::sqrt;
		else if (stype == "pw") type = Type::pw;
		else assert(0);
	}
	int query(P p) override {
		int x = p.ar[0];
		double v;
		if (x == pivot) v = 0;
		else if (x < pivot) v = f(1 - x / (double)pivot);
		else v = f((x - pivot) / (double)(N - pivot));
		v = min(v, 1.0);
		v = max(v, 0.0);
		return PIVOT_VAL - int(SUB_VAL * v) - (x != pivot);
	}
	double f(double x) {
		assert(0 < x);
		assert(x <= 1);
		if (type == Type::sqrt) return sqrt(x);
		else if (type == Type::pw) return x * sqrt(x);
		else assert(0);
	}
	long long maxval() const override { return PIVOT_VAL; }
};

// Sawtooth function with 1000 teeth.
struct OneDimBlocksStrat : Strat {
	uint64_t seed;
	int N;
	OneDimBlocksStrat(P dims, istream& cin) : Strat(dims) {
		assert(dims.dimension() == 1);
		N = dims.ar[0];
		assert(N == 1000000);
		seed = rand64();
	}
	int query(P p) override {
		int x = p.ar[0];
		if (x >= N-2400) return N-x;
		const int BS = 1000;
		int block = x / BS, ind = x % BS;
		return Hash(block, seed) % 100000000 + 30000 * ind;
	}
	long long maxval() const override { return 100000000 + 30000 * 1000; }
};

// Constant function!
struct ConstStrat : Strat {
	using Strat::Strat;
	int query(P) override {
		return 4; // chosen by a fair dice roll.
	}
	long long maxval() const override { return 4; }
};

// Increases monotonically towards one corner.
struct CornerStrat : Strat {
	string plusminus;
	CornerStrat(P dims, istream& cin) : Strat(dims) {
		cin >> plusminus;
		assert(plusminus.size() == P::DIM);
		for (char c : plusminus) {
			assert(c == '-' || c == '+');
		}
	}
	int query(P x) override {
		repd(i) {
			if (plusminus[i] == '-') x.ar[i] = dims.ar[i]-1 - x.ar[i];
		}
		return x.sum();
	}
	long long maxval() const override { return dims.sum(); }
};

Strat* readStrat(P dims, istream& cin) {
	string str;
	cin >> str;
	if (str == "random") return new RandomStrat(dims);
	if (str == "spacefill") return new SpaceFillStrat(dims);
	if (str == "spaced") return new SpacedPathStrat(dims, cin);
	if (str == "pad") return new PadStrat(dims, cin);
	if (str == "const") return new ConstStrat(dims);
	if (str == "1d-peak") return new OneDimPeakStrat(dims, cin);
	if (str == "1d-peak2") return new OneDimPeakStrat2(dims, cin);
	if (str == "1d-block") return new OneDimBlocksStrat(dims, cin);
	if (str == "corner") return new CornerStrat(dims, cin);
	assert(0 && "unknown strategy");
	abort();
}

int main(int argc, char** argv) {
	signal(SIGPIPE, SIG_IGN);
	assert(argc >= 2);
	if (argc >= 4) out_dir = argv[3];
	ifstream fin(argv[1]);
	P dims;
	int Q, seed;
	fin >> dims >> Q >> seed;
	srand(seed);

	Strat* strat = readStrat(dims, fin);
	strat = new AddStrat(strat);
	assert(strat->maxval() < 1000000000);
	assert(strat->dims == dims);
	assert(fin);
	string dummy;
	assert(!(fin >> dummy));

	auto works = [&](P x) {
		int v = strat->query(x);
		repd(i) {
			P y = x, z = x;
			--y.ar[i];
			++z.ar[i];
			if (v < strat->oob_query(y)) return false;
			if (v < strat->oob_query(z)) return false;
		}
		return true;
	};

	repd(i) cout << dims.ar[i] << ' ';
	cout << Q << endl;
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

		if (tokens.size() != 1 + P::DIM) reject_line("invalid format (must have DIMS+1 tokens)", line);
		const char *type = tokens[0];
		if (strcmp(type, "?") && strcmp(type, "!")) reject_line("invalid format (invalid query type)", line);
		P x;
		repd(i) {
			x.ar[i] = readnum(tokens[i+1], dims.ar[i], line) - 1;
		}

		if (type[0] == '!') {
			if (works(x)) accept();
			else reject_raw("not a local maximum");
		}

		if (!Q--) reject_line("too many queries", line);
		int r = strat->query(x) + 1;
		assert(1 <= r && r <= 1000000000);
		cout << r << endl;
	}
}
