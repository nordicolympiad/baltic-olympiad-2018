#include "gen.h"
#include <algorithm>

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
	cout << N << ' ' << M << endl;
	int numparents = args.has_var("numparents") ? Arg("numparents") : M/4;
    int maxMargin = args.has_var("maxmargin") ? Arg("maxmargin") : 0;
    int noiseSegmentMaxLength = args.has_var("noiselength") ? Arg("noiselength") : 0;
    int errors = args.has_var("errors") ? Arg("errors") : 0;
    bool monotone = args.has_var("monotone") ? Arg("monotone") : 0;
    set<int> positions;
    if (monotone)
        positions.insert(0);
    while ((int)positions.size() < numparents) {
        positions.insert(rand()%N);
    }
    vector<int> v;
    for (int x : positions)
        v.push_back(x);
    int start = rand()%numparents;
    if (monotone)
        start = 0;
    vector<bool> hasError(numparents);
    for (int i = 0; i < errors; i++) {
        hasError[rand()%hasError.size()] = 1;
    }
    vector<pair<int, int>> initialSegments;
    for (int i = 0; i < numparents; i++) {
        int a = v[(i+start)%numparents];
        int b = v[(i+1+start)%numparents];
        initialSegments.emplace_back(a, b);
    }
    vector<pair<int, int>> segments;
    for (size_t i = 0; i < initialSegments.size(); i++) {
        const auto& it = initialSegments[i];
        int a = it.first;
        int b = it.second;
        int len = (b-a+N)%N;
        int newLen = min(N, len + (rand() % (maxMargin+1)));
        if (hasError[i]) {
            newLen = max(1, len-1);
        }
        if (monotone && a+newLen >= N) {
            newLen = N-a-1;
        }
        b = (a + newLen) % N;
        if (b == 0)
            b = N;
        if (monotone)
            assert(b >= a+1);
        segments.emplace_back(a+1, b);
        segments.emplace_back(a+1, a+1);
        int c = (a+2)%N;
        if (c == 0)
            c = N;
        int d = (a + newLen - 1)%N;
        if (d == 0)
            d = N;
        if (monotone && d < c) {
            d = max(b, c);
        }
        segments.emplace_back(c, d);
        segments.emplace_back(b, b);
    }
    while ((int)segments.size() < M) {
        int a = rand()%N;
        int b = (a + (rand()%(noiseSegmentMaxLength+1)))%N;
        if (monotone && b < a)
            b = a;
        segments.emplace_back(a+1, b+1);
    }
    random_shuffle(segments.begin(), segments.end());
    for (const auto& segment : segments) {
        cout << segment.first << ' ' << segment.second << endl;
    }
	return 0;
}
