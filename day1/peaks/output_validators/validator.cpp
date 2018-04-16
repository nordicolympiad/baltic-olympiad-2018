// Usage: ./validator .in-file .ans-file dir <in >out
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <climits>
#include <signal.h>
using namespace std;

const char* out_dir = nullptr;

void judge_message(const string& msg) {
	cerr << msg << endl;
	if (out_dir) {
		string fname = out_dir + string("/judgemessage.txt");
		ofstream fout(fname.c_str());
		fout << msg << endl;
	}
}

[[noreturn]]
void reject_raw(const string& msg) {
	judge_message(msg);
	exit(43);
}

[[noreturn]]
void reject_line(const char* msg, string line) {
	for (char& c : line) if (c == '\0') c = ' ';
	if (line.size() > 0 && line[line.size()-1] == ' ') line = line.substr(0, line.size()-1);
	if (line.size() > 1000) line = line.substr(0, 1000) + "...";
	cout << "-1" << endl;
	reject_raw(msg + string(". input: ") + line);
}

[[noreturn]]
void accept(const string& msg) {
	judge_message(msg);
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

bool initialization_done = false;
int my_rand() {
	assert(!initialization_done);
	return rand();
}
#define rand my_rand

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
	static P Z;
	int operator[](int d) const { return ar[d]; }
	int& operator[](int d) { return ar[d]; }
	P idiv(int by) const {
		P ret = *this;
		repd(i) ret[i] /= by;
		return ret;
	}
	int count_odd() const {
		int ret = 0;
		repd(i) ret += ar[i]&1;
		return ret;
	}
	int abssum() const {
		int sum = 0;
		repd(i) sum += abs(ar[i]);
		return sum;
	}
	int sum() const {
		int sum = 0;
		repd(i) sum += ar[i];
		return sum;
	}
	int prod() const {
		int prod = 1;
		repd(i) prod *= ar[i];
		return prod;
	}
	int dimension() const {
		int dim = 0;
		repd(i) if (ar[i] != 1) dim = i+1;
		return dim;
	}
	uint64_t repr(P dims) const {
		uint64_t r = 0;
		repd(i) r = r * dims.ar[i] + ar[i];
		return r;
	}
	int irepr(P dims) const { return (int)repr(dims); }
	uint64_t hash(P dims, uint64_t seed) const {
		return Hash(repr(dims), seed);
	}
};
P P::Z = P::K(0);

inline bool operator<=(const P& a, const P& b) {
	repd(i) if (a[i] > b[i]) return false;
	return true;
}
inline bool operator==(const P& a, const P& b) {
	repd(i) if (a[i] != b[i]) return false;
	return true;
}
inline bool operator<(const P& a, const P& b) {
	repd(i) if (a[i] >= b[i]) return false;
	return true;
}
inline P operator+(const P& a, const P& b) {
	P ret; repd(i) ret[i] = a[i] + b[i]; return ret;
}
inline P operator-(const P& a, const P& b) {
	P ret; repd(i) ret[i] = a[i] - b[i]; return ret;
}
inline P operator*(const P& a, int by) {
	P ret; repd(i) ret[i] = a[i] * by; return ret;
}
istream& operator>>(istream& is, P& p) {
	repd(i) is >> p[i];
	return is;
}
ostream& operator<<(ostream& os, const P& p) {
	repd(i) os << p[i] << ' ';
	return os;
}

// Dense matrix
struct Mat {
	vector<vector<vector<int>>> m;
	Mat(P dims) : m(dims[0], vector<vector<int>>(dims[1], vector<int>(dims[2]))) {}
	int get(P x) const { return m[x[0]][x[1]][x[2]]; }
	void set(P x, int val) { m[x[0]][x[1]][x[2]] = val; }
	void unset(P x) { set(x, 0); }
	bool has(P x) const { return get(x) != 0; }
	template<class F>
	void each(F f) { for (auto& a : m) for (auto& b : a) for (auto& c : b) f(c); }
};

// Sparse matrix
struct HashMat {
	unordered_map<int, int> m;
	P dims;
	HashMat(P dims) : m(dims.prod() / dims[0] * 10), dims(dims) {
		assert(dims.prod() <= INT_MAX);
	}
	int get(P x) const { return m.at(x.irepr(dims)); }
	void set(P x, int val) { m[x.irepr(dims)] = val; }
	void unset(P x) { m.erase(x.irepr(dims)); }
	bool has(P x) const { return m.count(x.irepr(dims)); }
};

struct Strat {
	P dims;
	Strat(P dims) : dims(dims) {
		assert(P::K(1) <= dims);
	}
	virtual ~Strat() {}
	virtual int do_query(P x) = 0;
	virtual long long maxval() const = 0;
	int oob_query(P x) {
		if (oob(x)) return -1;
		return do_query(x);
	}
	int query(P x) {
		assert(!oob(x));
		return do_query(x);
	}
	bool oob(P x) const {
		return !(P::Z <= x && x < dims);
	}
};
Strat* readStrat(P dims, istream& cin);

// Wraps another strategy, and adds a constant to each cell. Doesn't hurt, I guess?
struct AddStrat : Strat {
	Strat* inner;
	int base;
	AddStrat(Strat* other) : Strat(other->dims), inner(other) {
		base = rand() % 1000000 + 1000;
	}
	int do_query(P x) override {
		return inner->query(x) + base;
	}
	long long maxval() const override { return inner->maxval() + base; }
};

// For paths that increase by 1 in each step, this wrapper puts the path only
// on even indices and removes shortcuts. It then applies a 1d strategy to the
// created path.
struct SpacedStrat : Strat {
	Strat *inner, *inner1d;
	SpacedStrat(P dims, istream& cin) : Strat(dims) {
		assert(dims.count_odd() == P::DIM);
		P innerDim = (dims+P::K(2)).idiv(2);
		inner = readStrat(innerDim, cin);
		assert(inner->maxval() == innerDim.prod() - 1);
		P dim2 = P::K(1);
		dim2[0] = innerDim.prod() * 2 - 1;
		inner1d = readStrat(dim2, cin);
	}
	int do_query(P p) override {
		assert(!oob(p));
		int odds = p.count_odd();
		if (odds > 1) return P::DIM - odds;
		if (odds == 1) {
			P A = p.idiv(2), B = (p + P::K(1)).idiv(2);
			int a = inner->query(A)*2;
			int b = inner->query(B)*2;
			if (abs(a - b) > 2) return P::DIM - odds;
			return tr(a + (b - a) / 2) + P::DIM;
		}
		return tr(inner->query(p.idiv(2))*2) + P::DIM;
	}
	int tr(int val) {
		P p = P::Z;
		p[0] = val;
		return inner1d->query(p);
	}
	long long maxval() const override { return inner1d->maxval() + P::DIM; }
};

// This puts very small values at the edges, making only the middle relevant.
struct PadStrat : Strat {
	Strat* inner;
	P lo, hi;
	struct SpacedT{} static Spaced;
	PadStrat(P dims, istream& cin) : Strat(dims) {
		cin >> lo >> hi;
		inner = readStrat(dims - lo - hi, cin);
	}
	PadStrat(P dims, istream& cin, SpacedT) : Strat(dims) {
		hi = P::Z;
		lo = P::Z;
		repd(i) if (dims[i] % 2 == 0) {
			lo[i]++;
		}
		inner = new SpacedStrat(dims - lo - hi, cin);
	}
	int do_query(P x) override {
		assert(!oob(x));
		int sum = 0;
		repd(i) sum += ed(x[i], lo[i], hi[i], dims[i]);
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
	int do_query(P x) override {
		return (int)(x.hash(dims, seed) >> 35);
	}
	long long maxval() const override { return 1 << 29; }
};

// Space-filling curve, going from the origin to a random point. Combine with
// SpacedStrat for best results.
struct SpaceFillStrat : Strat {
	uint64_t seed;
	vector<P> pcands;
	SpaceFillStrat(P dims, istream& cin) : Strat(dims) {
		seed = rand64();
	}
	int do_query(P x) override {
		int res = 0;
		bool r = rec(res, x, P::Z, P::Z, P::Z, dims, false);
		assert(r);
		return res;
	}
	bool rec(int& res, P want, P corner1, P corner2, P top, P bottom, bool hasCorner2) {
		assert(top <= corner1 && corner1 <= bottom);
		P dims = bottom - top;
		if (!(top <= want && want < bottom)) {
			res += dims.prod();
			return false;
		}
		if (dims == P::K(1)) return true;
		uint64_t rnd = top.hash(this->dims, bottom.hash(this->dims, seed));

		auto pa = pickPivot(corner1, corner2, dims, top, hasCorner2, 0, rnd);
		P corner3 = pa.first;
		int dim = pa.second;
		assert(corner3[dim] != top[dim]);
		assert(corner3[dim] != bottom[dim]);
		P top1 = top, bottom1 = bottom;
		P top2 = top, bottom2 = bottom;
		if (corner1[dim] < corner3[dim]) {
			assert(corner1[dim] == top[dim]);
			top1[dim] = top[dim];
			bottom1[dim] = corner3[dim];
			top2[dim] = corner3[dim];
			bottom2[dim] = bottom[dim];
		}
		else {
			assert(corner1[dim] == bottom[dim]);
			top2[dim] = top[dim];
			bottom2[dim] = corner3[dim];
			top1[dim] = corner3[dim];
			bottom1[dim] = bottom[dim];
		}
		if (rec(res, want, corner1, corner3, top1, bottom1, true)) return true;
		if (rec(res, want, corner3, corner2, top2, bottom2, hasCorner2)) return true;
		assert(false);
		abort();
	}

	pair<P, int> pickPivot(P corner1, P corner2, P dims, P base, bool hasCorner2, int avoidDims, uint64_t rnd) {
		int nonzerodims = 0;
		repd(i) if (dims[i] != 1) nonzerodims++;
		assert(nonzerodims > 0);

		int cands = 0;
		repd(i) if ((!hasCorner2 || corner1[i] != corner2[i]) && !(avoidDims & (1 << i))) {
			cands += dims[i] - 1;
		}

		assert(cands > 0);
		int pos = (int)(rnd % (unsigned)cands);
		rnd >>= 32;

		int dim = -1;
		repd(i) if ((!hasCorner2 || corner1[i] != corner2[i]) && !(avoidDims & (1 << i))) {
			if (pos < dims[i] - 1) {
				dim = i;
				break;
			}
			pos -= dims[i] - 1;
		}
		assert(dim != -1);
		pos++;
		assert(0 < pos && pos < dims[dim]);
		assert(pcands.empty());

		for (int it = 0; it < 2; ++it) {
			P pivot = base;
			pivot[dim] += (base[dim] == corner1[dim] ? pos : dims[dim] - pos);
			auto same = [&](P a, P b) -> bool {
				repd(i) {
					if (abs(a[i] - b[i]) > 1) return false;
				}
				return true;
			};
			auto test = [&]() -> bool {
				if (nonzerodims == 1) return true;
				if (same(pivot, corner1)) return false;
				if (hasCorner2 && same(pivot, corner2)) return false;
				int area = 1;
				int parity = 0;
				repd(i) {
					if (i != dim) area *= dims[i];
					if (corner1[i] != pivot[i]) {
						parity ^= corner1[i] ^ pivot[i] ^ 1;
					}
				}
				parity ^= area;
				return parity & 1;
			};
			function<void(int)> rec = [&](int ind) -> void {
				if (ind == dim) {
					rec(ind+1);
				}
				else if (ind == P::DIM) {
					if (test()) pcands.push_back(pivot);
				}
				else {
					pivot[ind] = base[ind];
					rec(ind+1);
					pivot[ind] = base[ind] + dims[ind];
					rec(ind+1);
				}
			};
			rec(0);

			if (pcands.empty()) {
				if (pos > 1) --pos;
				else if (pos < dims[dim]-1) ++pos;
				else return pickPivot(corner1, corner2, dims, base, hasCorner2, avoidDims | (1 << dim), rnd);
			}
		}
		assert(!pcands.empty());
		P pivot = pcands[rnd % (int)pcands.size()];
		pcands.clear();
		return {pivot, dim};
	}

	long long maxval() const override { return dims.prod() - 1; }
};

// Create a random walk towards the end, and everything else leading up to the
// start of the walk. Params: noiseq (~0.3, controls jitter in walk),
// its (~N^(D-2), number of segments).
struct RandomWalkStrat : Strat {
	HashMat mat;
	P startPoint;
	int startPointVal;
	const int MAX_VAL = 400000000;
	double noiseq;
	int its;
	RandomWalkStrat(P dims, istream& cin) : Strat(dims), mat(dims) {
		assert(dims.dimension() >= 3);
		assert(MAX_VAL > dims.prod());
		cin >> noiseq >> its;
		walk();
	}
	int do_query(P x) override {
		if (mat.has(x)) return mat.get(x);
		return startPointVal - (x - startPoint).abssum();
	}
	long long maxval() const override { return MAX_VAL; }

	int adj(P p) const {
		int res = 0;
		repd(i) for (int by = -1; by <= 1; by += 2) {
			P q = p;
			q[i] += by;
			if (mat.has(q)) res++;
		}
		return res;
	}

	void walk() {
		P p = P::Z;
		repd(i) p[i] = rand() % dims[i];
		mat.set(p, MAX_VAL);
		const int noise = (int)(dims[0] * noiseq);
		int reverts = 0;
		vector<int> seg;
		vector<P> history;
		history.push_back(p);
		for (int i = 0; i < its; i++) {
			P q = P::Z;
			repd(i) q[i] = rand() % dims[i];
			seg.clear();
			repd(i) {
				int dif = q[i] - p[i];
				for (int j = 0; j < abs(dif); j++) {
					if (dif > 0) seg.push_back(i);
					else seg.push_back(~i);
				}
				for (int j = 0; j < noise; j++) {
					seg.push_back(i);
					seg.push_back(~i);
				}
			}
			random_shuffle(seg.begin(), seg.end(), [](int x) { return rand() % x; });
			for (int mv : seg) {
				int by = (mv >= 0 ? 1 : -1);
				mv = (mv >= 0 ? mv : ~mv);
				P p2 = p;
				p2[mv] += by;
				if (oob(p2) || mat.has(p2) || adj(p2) > 1) {
					// Revert one step even further back, so we don't end up
					// trapped in a simple corner. (We could extend this to
					// more complex corners by reverting more than one step.
					// Or by adding connectivity checks, but that's annoying.)
					if ((int)history.size() > 1) {
						assert(history.back() == p);
						mat.unset(p);
						history.pop_back();
						p = history.back();
					}
					reverts++;
					continue;
				}
				p = p2;
				mat.set(p, MAX_VAL - (int)history.size());
				history.push_back(p);
			}
		}

		assert(mat.has(p));
		startPoint = p;
		startPointVal = mat.get(p);
		cerr << "generated path of length " << history.size() << ", " << reverts << " reverts" << endl;
	}
};

// Create a spiral, with a maximum somewhere on it. Use together with SpacedStrat.
struct SpiralStrat : Strat {
	SpiralStrat(P dims, istream& cin) : Strat(dims) {
		assert(dims.dimension() == 2);
		assert(dims[0] == dims[1]);
	}
	int do_query(P x) override {
		int N = dims[0];
		int layer = min({x[0], N-1 - x[0], x[1], N-1 - x[1]});
		int sqs = N - layer * 2;
		int outer = N*N - sqs*sqs;
		--sqs;
		int ind;
		if (x[0] == layer) ind = x[1];
		else if (N-1 - x[1] == layer) ind = x[0] + sqs;
		else if (N-1 - x[0] == layer) ind = N-1 - x[1] + 2*sqs;
		else ind = N-1 - x[0] + 3*sqs;
		ind += outer - layer;
		return ind;
	}
	long long maxval() const override { return dims.prod() - 1; }
};

// Unimodal function, which is piecewise linear and discontinuous at its peak.
struct OneDimPeakStrat : Strat {
	int pivot, scale, leftBase, rightBase, N;
	const int MAX_BASE = 100000000;
	const int PIVOT_VAL = 200000000;
	OneDimPeakStrat(P dims, istream& cin) : Strat(dims) {
		assert(dims.dimension() == 1);
		N = dims[0];
		int same;
		cin >> same;
		pivot = rand() % N;
		scale = MAX_BASE / N;
		assert(scale > 0);
		leftBase = rand() % MAX_BASE;
		rightBase = same ? leftBase : rand() % MAX_BASE;
	}
	int do_query(P p) override {
		int x = p[0];
		if (x == pivot) return PIVOT_VAL;
		if (x < pivot) return leftBase + x * scale;
		return rightBase + (N - x) * scale;
	}
	long long maxval() const override {
		return max((long long)PIVOT_VAL, MAX_BASE + (long long)scale*dims.sum());
	}
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
		N = dims[0];
		pivot = rand() % N;
		string stype;
		cin >> stype;
		if (stype == "sqrt") type = Type::sqrt;
		else if (stype == "pw") type = Type::pw;
		else assert(0);
	}
	int do_query(P p) override {
		int x = p[0];
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
		N = dims[0];
		assert(N == 1000000);
		seed = rand64();
	}
	int do_query(P p) override {
		int x = p[0];
		if (x >= N-2400) return N-x;
		const int BS = 1000;
		int block = x / BS, ind = x % BS;
		return (int)(Hash(block, seed) % 100000000U) + 30000 * ind;
	}
	long long maxval() const override { return 100000000 + 30000 * 1000; }
};

// Constant function!
struct ConstStrat : Strat {
	using Strat::Strat;
	int do_query(P) override {
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
	int do_query(P x) override {
		repd(i) {
			if (plusminus[i] == '-') x[i] = dims[i]-1 - x[i];
		}
		return x.sum();
	}
	long long maxval() const override { return dims.sum(); }
};

// For debugging.
struct PrintStrat : Strat {
	Strat* inner;
	PrintStrat(P dims, istream& cin) : Strat(dims) {
		inner = readStrat(dims, cin);
		static_assert(P::DIM == 3, "");
		P p;
		long long mv = inner->maxval();
		bool isSpaced = (dynamic_cast<SpacedStrat*>(inner) != nullptr);
		int &x = p[0], &y = p[1], &z = p[2];
		for (x = 0; x < dims[0]; x++) {
			for (y = 0; y < dims[1]; y++) {
				for (z = 0; z < dims[2]; z++) {
					int v = inner->query(p);
					assert(0 <= v);
					assert(v <= mv);
					if (isSpaced) {
						if (v >= P::DIM) cout << "█";
						else cout << " ";
					}
					else {
						cout << v << ' ';
					}
				}
				if (dims.dimension() >= 3) cout << endl;
			}
			if (dims.dimension() >= 2) cout << endl;
		}
		cout << endl;
		exit(0);
	}
	int do_query(P) override { assert(0); }
	long long maxval() const override { assert(0); }
};

Strat* readStrat(P dims, istream& cin) {
	string str;
	cin >> str;
	if (str == "const") return new ConstStrat(dims);
	if (str == "random") return new RandomStrat(dims);
	if (str == "print") return new PrintStrat(dims, cin);
	if (str == "spacefill") return new SpaceFillStrat(dims, cin);
	if (str == "spaced") return new SpacedStrat(dims, cin);
	if (str == "pad") return new PadStrat(dims, cin);
	if (str == "spiral") return new SpiralStrat(dims, cin);
	if (str == "walk") return new RandomWalkStrat(dims, cin);
	if (str == "1d-peak") return new OneDimPeakStrat(dims, cin);
	if (str == "1d-peak2") return new OneDimPeakStrat2(dims, cin);
	if (str == "1d-block") return new OneDimBlocksStrat(dims, cin);
	if (str == "corner") return new CornerStrat(dims, cin);
	if (str == "pspaced") return new PadStrat(dims, cin, PadStrat::Spaced);
	assert(0 && "unknown strategy");
	abort();
}

int main(int argc, char** argv) {
	signal(SIGPIPE, SIG_IGN);
	assert(argc >= 2);
	if (argc >= 4) out_dir = argv[3];
	ifstream fin(argv[1]);
	fin.exceptions(ios_base::failbit | ios_base::eofbit | ios_base::badbit);
	P dims;
	int Q, seed;
	fin >> dims >> Q >> seed;
	srand(seed);

	Strat* strat = readStrat(dims, fin);
	strat = new AddStrat(strat);
	assert(strat->maxval() < 1000000000);
	assert(strat->dims == dims);
	assert(fin);
	fin.exceptions(ios_base::goodbit);
	string dummy;
	assert(!(fin >> dummy));
	initialization_done = true;

	auto works = [&](P x) {
		int v = strat->query(x);
		repd(i) {
			P y = x, z = x;
			--y[i];
			++z[i];
			if (v < strat->oob_query(y)) return false;
			if (v < strat->oob_query(z)) return false;
		}
		return true;
	};

	repd(i) cout << dims[i] << ' ';
	cout << Q << endl;
	string line;
	vector<const char*> tokens;
	int usedq = 0;
	for (;;) {
		if (!getline(cin, line)) reject_raw("eof");

		// Split on whitespace
		line += ' ';
		tokens.clear();
		size_t ind = 0;
		while (ind != line.size()) {
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
			x[i] = readnum(tokens[i+1], dims[i], line) - 1;
		}

		if (type[0] == '!') {
			if (works(x)) accept("ok, " + to_string(usedq) + "/" + to_string(Q) + " queries");
			else reject_raw("not a local maximum");
		}

		if (++usedq > Q) reject_line("too many queries", line);
		int r = strat->query(x) + 1;
		assert(1 <= r && r <= 1000000000);
		cout << r << endl;
	}
}
