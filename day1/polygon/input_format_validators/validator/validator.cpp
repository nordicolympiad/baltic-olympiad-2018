#include "validator.h"

void run() {
	int n = Int(2, Arg("n"));
	Endl();
	vector<string> names, loves;
	auto assert_valid = [](const string& name) {
		if (name.size() > 10) die_line("Name " + name + " is too long");
		if (name.size() == 0) die_line("Empty name");
		for (char c : name) {
			if (!('a' <= c && c <= 'z')) die_line("Name " + name + " contains invalid characters");
		}
	};
	for (int i = 0; i < n; i++) {
		string line = Line();
		size_t sp = line.find(' ');
		assert(sp != string::npos);
		string a = line.substr(0, sp);
		string b = line.substr(sp + 1);
		assert_valid(a);
		assert_valid(b);
		names.push_back(a);
		loves.push_back(b);
	}

	AssertUnique(names);
	map<string, int> ren;
	for (int i = 0; i < n; i++)
		ren[names[i]] = i;

	vector<int> target(n);
	for (int i = 0; i < n; i++) {
		if (!ren.count(loves[i]))
			die("Target name " + loves[i] + " not found");
		target[i] = ren[loves[i]];
	}

	string spe = Arg("special", "");
	if (spe == "all_loved") {
		AssertUnique(loves);
	}
	else if (spe == "no_cycles") {
		vector<int> color(n);
		for (int i = 0; i < n; i++) {
			int x = i;
			while (!color[x]) {
				color[x] = 1;
				x = target[x];
			}
			if (color[x] == 1 && target[x] != x) die("Cycle!");
			x = i;
			while (color[x] == 1) {
				color[x] = 2;
				x = target[x];
			}
		}
	}
	else assert(spe == "");
}
