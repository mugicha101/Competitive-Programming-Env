// finds closest diff between t and an element in s
ll closestDiff(multiset<ll> &s, ll t) {
    if (s.empty()) return LLONG_MAX; // no values to compare against
    auto l = s.lower_bound(t); // first val >= t
    if (l == s.end()) return abs(*prev(l) - t); // no val >= t, must be last value < t
    if (l == s.begin()) return abs(*l - t); // no val < t, must be first value >= t
    return min(abs(*l - t), abs(*prev(l) - t)); // both val < t and val >= t exist, pick min
}