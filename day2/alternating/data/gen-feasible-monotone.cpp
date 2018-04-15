#include "gen.h"
#include <algorithm>

int main(int argc, char** argv) {
	args.construct(argc, argv);
	int N = Arg("n");
	int M = Arg("m");
    int M1 = args.has_var("m1") ? Arg("m1") : M/2;
    int M2 = args.has_var("m2") ? Arg("m2") : M - M1;
    assert(M1 + M2 <= M);
    int maxMargin = args.has_var("maxmargin") ? Arg("maxmargin") : 0;
    int noiseSegmentMaxLength = args.has_var("noiselength") ? Arg("noiselength") : 0;
	cout << N << ' ' << M << endl;
    vector<pair<int, int> > segments;
    int errors = args.has_var("errors") ? Arg("errors") : 0;
    int e[2];
    e[0] = rand() % (errors+1);
    e[1] = errors - e[0];
    for (int dir = 0; dir < 2; dir++) {
        int cnt = dir ? M2 : M1;
        vector<int> positions;
        set<int> usedPositions;
        assert(N >= cnt);
        usedPositions.insert(0);
        usedPositions.insert(N);
        while((int)usedPositions.size() < cnt+1) {
            usedPositions.insert(rand()%N);
        }
        for (auto x : usedPositions) {
            positions.push_back(x);
        }
        vector<bool> hasError(positions.size());
        for (int i = 0; i < e[dir]; i++) {
            hasError[rand()%hasError.size()] = 1;
        }
        for (size_t i = 0; i+1 < positions.size(); i++) {
            int a = positions[i];
            int b = positions[i+1];
            int len = b-a;
            int newLen = min(N, len + (rand() % (maxMargin+1)));
            if (hasError[i]) {
                newLen = max(1, len-1);
            }
            b = a + newLen;
            if (b > N)
                b = N;
            segments.emplace_back(a+1, b);
        }
    }
    for (int i = 0; i < M - M1 - M2; i++) {
        int a = rand()%N;
        int b = a + (rand()%(noiseSegmentMaxLength+1));
        if (b >= N)
            b = N-1;
        segments.emplace_back(a+1, b+1);
    }
    random_shuffle(segments.begin(), segments.end());
    for (const auto& segment : segments) {
        assert(segment.first <= segment.second);
        cout << segment.first << ' ' << segment.second << endl;
    }
	return 0;
}
