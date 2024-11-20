# pragma once

#include <bits/stdc++.h>
using namespace std;

// z algorithm
vector<int> zarr(string s) {
    int n = s.size();
    vector<int> z(n);
    z[0] = n;
    int l = 0, r = 0;
    for (int i = 1; i < n; ++i) {
        if (i >= r) {
            l = i;
            r = i;
            while (r < n && s[r] == s[r-l])
                ++r;
            z[i] = r - l;
        } else if (i + z[i-l] >= r) {
            l = i;
            while (r < n && s[r] == s[r-l])
                ++r;
            z[i] = r-l;
        } else
            z[i] = z[i-l];
    }
    return z;
}