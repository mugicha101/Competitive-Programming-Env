#include <bits/stdc++.h>

using namespace std;
#define INPUT true

typedef long long ll;
typedef long double ld;

#define ssort(x) sort(x.begin(), x.end())
#define flip(x) reverse(x.begin(), x.end())
#define iseq(x) iota(x.begin(), x.end(), 0)

#define forn(i,n) for (int i = 0; i < n; ++i)
#define rforn(i,n) for (int i = n-1; i >= 0; --i)
#define fora(a) for(auto x : a)
#define forr(a) for(auto& x : a)

template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> ostream& operator<<(ostream &os, const T_container &v) { os << '{'; string sep; for (const T &x : v) os << sep << x, sep = ", "; return os << '}'; }
#define len(x) ((int)x.size())
#define all(a) (a).begin(), (a).end()
#define getx(a) a.first
#define gety(a) a.second
#define MOD 1000000007
#define INF 1e9
#define EPS 1e-9

bool prime(ll a) { if (a==1) return 0; ll cap = sqrt(a); for (int i=2;i<=cap;++i) if (a%i==0) return 0; return 1; }

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const { return hash<T1>{}(p.first) ^ hash<T2>{}(p.second); }
};

typedef vector<vector<int>> AdjList;

int main(int argc, char **argv) {
    // env setup
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cout << setprecision(12);
    #if INPUT
    freopen("./input.txt","r",stdin);
    #endif
    
    // input
}