typedef unsigned long long ull;
typedef long long ll;
typedef long double ld;

// correctly computes a * b % c for all 0 ≤ a, b < c < 2^63
ull modmul(ull a, ull b, ull c) {
    ll ret = a * b - c * ull(ld(a) * ld(b) / ld(c));
    return ret + c * (ret < 0) - c * (ret >= (ll)c);
}

// inverse a % p such that (inv(a, p) * a) % p = 1
ull invmod(ull a, ull p) {
    ull ex = p-2, result = 1;
    while (ex > 0) {
        if (ex % 2 == 1) {
            result = (result*a) % p;
        }
        a = (a*a) % p;
        ex /= 2;
    }
    return result;
}

// CRT Chinese Remainder Theorem:
// given pairwise coprime m_1, m_2, ...
// find x mod m_1 * m_2 * ... where x mod m_i = a_i mod m_i
// M = m_1 * m_2 * ...
// M_i = M / m_i
// N_i = modinv(M_i mod m_i)
// x mod M = sum(a_i * M_i * N_i) mod M
