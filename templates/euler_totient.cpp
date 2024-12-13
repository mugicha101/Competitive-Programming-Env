  #pragma once

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
#define sz(x) ((int)x.size())

ll totient(ll n) {
    // prime decomp
    ll n; cin >> n;
    queue<ll> q;
    q.push(n);
    unordered_set<ll> primeFactors;
    unordered_set<ll> visited;
    while (!q.empty()) {
        ll curr = q.front();
        q.pop();
        if (curr == 1) continue;

        ll d = 2;
        while (d * d <= curr && curr % d) ++d;
        if (d * d > curr) {
            primeFactors.insert(curr);
        } else {
            ll e = curr / d;
            if (visited.insert(d).second) q.push(d);
            if (visited.insert(e).second) q.push(e);
        }
    }

    // euler totient
    __int128_t num = n;
    __int128_t den = 1;
    for (ll p : primeFactors) {
        num *= p - 1;
        den *= p;
    }
    ll res = num / den;
    return res;
}