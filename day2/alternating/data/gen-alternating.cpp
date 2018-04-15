#include "gen.h"
#include <algorithm>

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
	int needed = args.has_var("needed") ? Arg("needed") : (M+1);
	cout << N << ' ' << M << endl;
    int maxMargin = args.has_var("maxmargin") ? Arg("maxmargin") : 0;
    int noiseSegmentMaxLength = args.has_var("noiselength") ? Arg("noiselength") : 0;
    int errors = args.has_var("errors") ? Arg("errors") : 0;
    set<int> positions;
    assert(needed <= N);
    while ((int)positions.size() < needed) {
        positions.insert(rand()%N);
    }
    vector<int> v;
    for (int x : positions)
        v.push_back(x);
    int start = rand()%needed;
    vector<bool> hasError(needed);
    for (int i = 0; i < errors; i++) {
        hasError[rand()%hasError.size()] = 1;
    }
    vector<pair<int, int>> initialSegments;
    for (int i = 0; i < needed; i++) {
        int j = 2;
        if (i <= 3) {
            j = 3;
        }
        else if (i == 4) {
            continue;
        }

        int a = v[(i+start)%needed];
        int b = v[(i+j+start)%needed];
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
        b = (a + newLen) % N;
        if (b == 0)
            b = N;
        segments.emplace_back(a+1, b);
    }
    while ((int)segments.size() < M) {
        int a = rand()%N;
        int b = (a + (rand()%(noiseSegmentMaxLength+1)))%N;
        segments.emplace_back(a+1, b+1);
    }
    random_shuffle(segments.begin(), segments.end());
    for (const auto& segment : segments) {
        cout << segment.first << ' ' << segment.second << endl;
    }
	return 0;
}
