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

int readnum(const string& line, int& pos, int max) {
	assert(line[pos] == ' ');
	if (pos == (int)line.size() - 1) reject_line("invalid format (too few tokens)", line);
	pos++;
	if (line[pos] == ' ') reject_line("invalid format (empty number)", line);
	int cur = 0;
	while (line[pos] != ' ') {
		int dig = line[pos] - '0';
		if (dig < 0 || dig > 9) reject_line("invalid format (not a digit)", line);
		cur *= 10;
		cur += dig;
		if (cur > max) reject_line("out of bounds (too large)", line);
		pos++;
	}
	if (cur < 1) reject_line("out of bounds (zero)", line);
	return cur;
}

int main(int argc, char** argv) {
	signal(SIGPIPE, SIG_IGN);
	assert(argc >= 2);
	if (argc >= 4) out_dir = argv[3];
	ifstream fin(argv[1]);
	string strat;
	int N_, M_, K_, Q, seed;
	fin >> N_ >> M_ >> K_ >> Q >> seed >> strat;
	srand(seed);

	int RN = N_, RM = M_, RK = K_;
	bool spaced = false;
	if (strat == "spaced") {
		spaced = true;
		RN = (RN+1)/2;
		RM = (RM+1)/2;
		RK = (RK+1)/2;
		fin >> strat;
	}

	auto query_real = [&](int x, int y, int z) {
		if (x < 0 || y < 0 || z < 0 || x >= RN || y >= RM || z >= RK)
			assert(0);
		// TODO space-filling curve thing...
		return x + y + z;
	};

	auto oob_query_real = [&](int x, int y, int z) {
		if (x < 0 || y < 0 || z < 0 || x >= RN || y >= RM || z >= RK)
			return -1;
		return query_real(x, y, z);
	};

	auto query2 = [&](int x, int y, int z) {
		if (x < 0 || y < 0 || z < 0 || x >= N_ || y >= M_ || z >= K_)
			assert(0);
		if (spaced) {
			// For paths that increase by 1 in each step, this puts the path
			// only on even indices and removes shortcuts.
			int odds = x%2 + y%2 + z%2;
			if (odds == 3) return 1;
			if (odds == 2) return 2;
			if (odds == 1) {
				int x1 = x, y1 = y, z1 = z;
				int x2 = x, y2 = y, z2 = z;
				if (x%2) --x1, ++x2;
				if (y%2) --y1, ++y2;
				if (z%2) --z1, ++z2;
				int a = oob_query_real(x1/2, y1/2, z1/2)*2 + 10;
				int b = oob_query_real(x2/2, y2/2, z2/2)*2 + 10;
				assert(a != b);
				if (abs(a - b) != 2) return 3;
				return a + (b - a) / 2;
			}
			return query_real(x/2, y/2, z/2)*2 + 10;
		} else {
			return query_real(x, y, z);
		}
	};

	int base = rand() % 1000000;
	auto query = [&](int x, int y, int z) {
		return query2(x, y, z) + base;
	};

	auto oob_query = [&](int x, int y, int z) {
		if (x < 0 || y < 0 || z < 0 || x >= N_ || y >= M_ || z >= K_)
			return -1;
		return query(x, y, z);
	};

	auto works = [&](int x, int y, int z) {
		int v = query(x, y, z);
		return
			v >= oob_query(x-1,y,z) &&
			v >= oob_query(x+1,y,z) &&
			v >= oob_query(x,y-1,z) &&
			v >= oob_query(x,y+1,z) &&
			v >= oob_query(x,y,z-1) &&
			v >= oob_query(x,y,z+1);
	};

	cout << N_ << ' ' << M_ << ' ' << K_ << ' ' << Q << endl;
	string line;
	for (;;) {
		if (!getline(cin, line)) reject_raw("eof");
		line += ' ';
		if (line[0] != '!' && line[0] != '?') reject_line("invalid format (invalid query type)", line);
		if (line[1] != ' ') reject_line("invalid format (first token too large)", line);
		int pos = 1;
		int x = readnum(line, pos, N_) - 1;
		int y = readnum(line, pos, M_) - 1;
		int z = readnum(line, pos, K_) - 1;
		if (pos != (int)line.size() - 1) reject_line("invalid format (too many tokens)", line);

		if (line[0] == '!') {
			if (works(x, y, z)) accept();
			else reject_raw("not a local maximum");
		}
		cout << query(x, y, z) << endl;
	}
}
