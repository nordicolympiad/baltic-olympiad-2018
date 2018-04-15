#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <cstdlib>
#include <climits>
#include <cassert>
using namespace std;

[[noreturn]]
void die(const string& msg) {
	cerr << msg << endl;
	_Exit(43);
}

struct ArgType {
	string _name, _x;
	ArgType(const string& x) : _x(x) {}
	operator string() const { return _x; }
	operator long long() const;
	operator int() const {
		long long val = (long long)*this;
		if (val < INT_MIN || val > INT_MAX)
			die("number " + to_string(val) + " is too large for an int for parameter " + _name);
		return (int)val;
	}
	operator bool() const {
		long long val = (long long)*this;
		if (val != 0 && val != 1)
			die("number " + to_string(val) + " is too large for a bool");
		return (bool)val;
	}
};

struct IntType {
	long long _x;
	IntType(long long x) : _x(x) {}
	operator long long() const { return _x; }
	operator int() const {
		if (_x < INT_MIN || _x > INT_MAX)
			die("number " + to_string(_x) + " is too large for an int");
		return (int)_x;
	}
	operator bool() const {
		if (_x != 0 && _x != 1)
			die("number " + to_string(_x) + " is too large for a bool");
		return (bool)_x;
	}
};

ArgType Arg(const string& name);

void destroyargs();

struct Args {
	bool initialized = false;
	map<string, string> params;
	set<string> used_params;
	int seed = 0;

	void construct(int argc, char** argv) {
		initialized = true;
		for (int i = 1; i < argc; i++) {
			string s = argv[i];
			size_t ind = s.find('=');
			if (ind == string::npos) {
				if (i == argc - 1)
					seed = (int)ArgType{s};
				continue;
			}
			auto before = s.substr(0, ind), after = s.substr(ind + 1);
			if (params.count(before))
				die("Duplicate parameter " + before);
			params[before] = after;
		}
		if (has_var("seed")) seed = Arg("seed");
		srand(seed);
		atexit(destroyargs);
	}

	void destroy() {
		assert(initialized);
		initialized = false;
		if (!params.empty()) {
			string name = params.begin()->first;
			die("Unused parameter " + name);
		}
	}

	bool has_var(const string& name) {
		if (!initialized) die("Must not read variables before main");
		return params.count(name) || used_params.count(name);
	}

	string get_var(const string& name) {
		if (!initialized) die("Must not read variables before main");
		if (used_params.count(name)) die("Must not read parameter " + name + " twice (either typo or slow)");
		if (!params.count(name)) die("No parameter " + name);
		string res = params.at(name);
		params.erase(name);
		used_params.insert(name);
		return res;
	}
} args;

void destroyargs() { args.destroy(); }

ArgType::operator long long() const {
	string dummy;
	{
		long long num;
		istringstream iss(_x);
		iss >> num;
		if (iss && !(iss >> dummy)) return num;
	}
	{
		// We also allow scientific notation, for clarity
		long double num;
		istringstream iss(_x);
		iss >> num;
		if (iss && !(iss >> dummy)) return (long long)num;
	}
	die("Unable to parse value " + _x + " for parameter " + _name);
}

ArgType Arg(const string& name) {
	return {args.get_var(name)};
}

IntType Arg(const string& name, long long _default) {
	if (!args.has_var(name))
		return _default;
	return {(long long)Arg(name)};
}

string Arg(const string& name, const string& _default) {
	if (!args.has_var(name))
		return _default;
	return (string)Arg(name);
}

// TODO reimplement rand, random_shuffle

int randbit() {
	return (rand() >> 15) & 1;
}
