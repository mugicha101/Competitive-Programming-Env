#include <bits/stdc++.h>
using namespace std;

#define all(a) (a).begin(), (a).end()

struct SuffixArray {
    vector<vector<int>> c; // suffix equivalence classes
    vector<int> p; // final level permutation
    int lvls = 0;
    SuffixArray(string s) {
        int ogSize = (int)s.size();
        while ((1 << lvls) <= ogSize) ++lvls;
        s += '\0';
        int n = s.size();
        p.resize(n);
        c.resize(lvls + 1, vector<int>(n));

        // sort single characters
        iota(all(p), 0);
        sort(all(p), [&](int i, int j) {
            return s[i] < s[j];
        });
        c[0][p[0]] = 0;
        for (int i = 1; i < n; ++i) {
            c[0][p[i]] = c[0][p[i-1]] + (s[p[i-1]] != s[p[i]]);
        }

        // build up to full suffixes
        auto modIdx = [&](int i) {
            return i >= n ? i - n : i;
        };
        for (int l = 1; l <= lvls; ++l) {
            int k = 1 << (l - 1);
            auto cmp = [&](int i, int j) {
                return c[l-1][i] == c[l-1][j] ? c[l-1][modIdx(i + k)] < c[l-1][modIdx(j + k)] : c[l-1][i] < c[l-1][j];
            };
            sort(all(p), cmp);
            c[l][p[0]] = 0;
            for (int i = 1; i < n; ++i) {
                c[l][p[i]] = c[l][p[i-1]] + cmp(p[i-1], p[i]);
            }
        }
    }

    // lcp (lazy version, no lcp array)
    int lcp(int i, int j) {
        int ans = 0;
        int n = p.size();
        for (int l = lvls; l >= 0; --l) {
            if (c[l][i % n] == c[l][j % n]) {
                int k = 1 << l;
                ans += k;
                i += k;
                j += k;
            }
        }
        return ans;
    }

    // get kth suffix (start index) in alphabetical order
    int kthSuffix(int k) {
        return p[k + 1];
    }
};