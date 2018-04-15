#include <vector>
#include <algorithm>
#include <tuple>
#include <cstring>
#include "validate.h"

using namespace std;

pair<int, vector<pair<int, int>>> read_input(istream &in) {
    int n, m;
    in >> n >> m;
    vector<pair<int, int>> P(m);
    for (auto &v: P) {
        in >> v.first >> v.second;
    }
    return {n, P};
}

vector<bool> read_solution(int M, istream &sol, feedback_function feedback) {
    string S;
    if (!(sol >> S)) {
        feedback("Output file seems to be empty");
    }
    vector<bool> res;
    if (!strcasecmp(S.c_str(), "impossible")) {
        // do nothing, just return empty array
    } else {
        if ((int)S.length() != M) {
            feedback("%s is not a string of length exactly %d", S.c_str(), M);
        }

        res.resize(M);
        for (int i = 0; i < M; ++i) {
            if (S[i] != '0' && S[i] != '1') {
                feedback("%s contains invalid characters (non-01 in position %d)", S.c_str(), i);
            }
            res[i] = (S[i] == '1');
        }
    }
    if (sol >> S) {
        feedback("Trailing output");
    }
    return res;
}

void check_solution(int n, const vector<pair<int, int>> &P, const vector<bool> &Sol, feedback_function feedback) {
    assert((int)P.size() == (int)Sol.size());
    for (int direction = 0; direction < 2; direction++) {
        vector<pair<int, int>> sortedSegments;
        for (size_t i = 0; i < Sol.size(); i++) {
            if (Sol[i] == direction) {
                sortedSegments.push_back(P[i]);
            }
        }
        sort(sortedSegments.begin(), sortedSegments.end(), [n](pair<int, int> a, pair<int, int> b) {
            if (a.first != b.first) {
                return a.first < b.first;
            }
            int lena = (a.second - a.first + n) % n;
            int lenb = (b.second - b.first + n) % n;
            return lena < lenb;
        });
        int hi = 0;
        for (const auto& segment : sortedSegments) {
            int b = segment.second;
            if (segment.first <= segment.second)
                b -= n;
            hi = max(hi, b);
        }
        for (const auto& segment : sortedSegments) {
            int a = segment.first;
            int b = segment.second;
            if (b < a)
                b += n;
            if (a > hi+1) {
                feedback("no wires in direction %d between segments %d and %d", direction, hi+1, a-1);
            }
            hi = max(hi, b);
        }
        if (hi < n) {
            feedback("no wires in direction %d between segments %d and %d", direction, hi+1, n);
        }
    }
}


int main(int argc, char **argv) {
    init_io(argc, argv);

    int n;
    vector<pair<int, int>> P;
    tie(n, P) = read_input(judge_in);
    int m = (int)P.size();
    vector<bool> Out = read_solution(m, cin, wrong_answer);
    vector<bool> Ans = read_solution(m, judge_ans, judge_error);
    if (Out.empty()) {
        if (!Ans.empty()) {
            wrong_answer("'impossible' claimed, but there is a solution");
        }
    } else {
        check_solution(n, P, Out, wrong_answer);
        if (Ans.empty()) {
            judge_error("judge answer says 'impossible', but submission found a valid solution");
        }
    }

    accept();
}
