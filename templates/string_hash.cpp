using h_t = uint64_t;
uint64_t seed;
hash<std::string> hasher;
unordered_map<uint64_t, string> col;

// set seed
void shseed() {
  mt19937_64 rng(time(0));
  seed = rng();
}

// hash string
uint64_t sh(const string &s) {
    uint64_t h = 3ULL | (seed ^ (uint64_t)hasher(s));
    
    // uncomment to check for collisions
    // auto it = col.find(h);
    // if (it == col.end()) col[h] = s;
    // else assert(it->second == s);

    return h;
}

// suffix array on hash type
typedef pair<ll, int> pli;
using vi = vector<int>;
struct SuffixArray {
	vi a;
	vector<h_t> s;
	SuffixArray(const vector<h_t>& _s) : s(_s) {
        s.push_back(0);
		int N = sz(s);
		vector<pli> b(N);
		a.resize(N);
		for(int i = 0; i < N; ++i) {
			b[i].first = s[i];
			b[i].second = i;
		}

		int q = 8;
		while ((1 << q) < N) q++;
		for (int moc = 0;; moc++) {
			sort(all(b)); // sort(all(b)) can be used as well
			a[b[0].second] = 0;
			for(int i = 1; i < N; ++i)
				a[b[i].second] = a[b[i - 1].second] +
					(b[i - 1].first != b[i].first);

			if ((1 << moc) >= N) break;
			for (int i = 0; i < N; ++i) {
				b[i].first = (ll)a[i] << q;
				if (i + (1 << moc) < N)
					b[i].first += a[i + (1 << moc)];
				b[i].second = i;
			}
		}
		for(int i = 0; i < sz(a); ++i) a[i] = b[i].second;
	}
	vi lcp() {
		// longest common prefixes: res[i] = lcp(a[i], a[i-1])
		int n = sz(a), h = 0;
		vi inv(n), res(n);
		for (int i = 0; i < n; ++i) inv[a[i]] = i;
		for (int i = 0; i < n; ++i) if (inv[i] > 0) {
			int p0 = a[inv[i] - 1];
            while (s[i + h] == s[p0 + h]) h++;
			res[inv[i]] = h;
			if(h > 0) h--;
		}
		return res;
	}
};