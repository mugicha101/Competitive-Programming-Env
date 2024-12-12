#pragma once

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

// extended euclidean algorithm
void eeHelper(ll a, ll b, tuple<ll,ll,ll> &res) {
    if (b == 0) {
        res = {a, 1, 0};
        return;
    }
    
    eeHelper(b, a % b, res);
    swap(get<1>(res), get<2>(res));
    get<2>(res) -= get<1>(res) * (a / b);
}
tuple<ll,ll,ll> extendedEuclid(ll a, ll b) {
    tuple<ll,ll,ll> res;
    eeHelper(a, b, res);
    return res;
}

// returns the inverse of a mod m (-1 if none or multiple exist)
ll invMod(ll a, ll m) {
    auto &&[d, x, y] = extendedEuclid(a, m);
    return d == 1 ? ((x % m) + m) % m : -1;
}