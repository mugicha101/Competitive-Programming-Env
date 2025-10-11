using ll = long long;

// Fast modular exponentiation
ll modpow(ll a, ll e, ll mod) {
  ll r = 1;
  a %= mod;
  while (e) {
      if (e & 1) r = (r * a) % mod;
      a = (a * a) % mod;
      e >>= 1;
  }
  return r;
}

// Factorizes m into prime powers
vector<pair<int,int>> factorize(int m) {
  vector<pair<int,int>> factors;
  for (int p = 2; p * p <= m; ++p) {
      if (m % p == 0) {
          int e = 0;
          while (m % p == 0) { m /= p; ++e; }
          factors.push_back({p, e});
      }
  }
  if (m > 1) factors.push_back({m, 1});
  return factors;
}

// nCk % p (p prime), small n,k < p
int nCk_small(int n, int k, int p) {
  if (k < 0 || k > n) return 0;
  int res = 1;
  for (int i = 1; i <= k; ++i) {
      res = (int)((1LL * res * (n - k + i)) % p);
      res = (int)((res * modpow(i, p-2, p)) % p); // Fermat inverse
  }
  return res;
}

// Lucas theorem for prime p
int nCk_lucas(ll n, ll k, int p) {
  int res = 1;
  while (n > 0 || k > 0) {
      int ni = n % p;
      int ki = k % p;
      if (ki > ni) return 0;
      res = (res * nCk_small(ni, ki, p)) % p;
      n /= p;
      k /= p;
  }
  return res;
}

// CRT to combine x ≡ a_i mod m_i for pairwise coprime m_i
int crt(const vector<int>& rem, const vector<int>& mod) {
  ll M = 1;
  for (auto m : mod) M *= m;

  ll ans = 0;
  for (size_t i = 0; i < rem.size(); ++i) {
      ll Mi = M / mod[i];
      ll inv = modpow(Mi, mod[i]-2, mod[i]);
      ans = (ans + Mi * inv % M * rem[i]) % M;
  }
  return (int)ans;
}

// General nCk % m
int nCk_mod(ll n, ll k, int m) {
  if (k < 0 || k > n) return 0;
  auto factors = factorize(m);
  vector<int> rem, mod;
  for (auto [p,e] : factors) {
      int pk = 1;
      for (int i = 0; i < e; ++i) pk *= p;
      int r = nCk_lucas(n, k, pk); // mod prime power can be enhanced later
      rem.push_back(r);
      mod.push_back(pk);
  }
  return crt(rem, mod);
}