#define _USE_MATH_DEFINES
#include <bits/stdc++.h>
 
using namespace std;
 
typedef unsigned int uint;
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
 
const int MOD = 1000000007;

const ld ROT_HALF = 3.14159265358979323846L;
const ld ROT_FULL = ROT_HALF * 2.L;
const ld ROT_QUARTER = ROT_HALF * 0.5L;

template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}
template <typename T,typename U>         
std::pair<T,U> operator-(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first-r.first,l.second-r.second};                                    
}
template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }

using namespace std;

using ll = long long;
const ll INF = LLONG_MAX >> 2;

#define EPS 1e-7
#define all(x) x.begin(), x.end()
#define sz(x) ((int)x.size())
#define each(i, x) for (auto &i : x)
#define vcin(x) for (auto &_e : x) cin >> _e;
#define mp(a,b) make_pair(a,b)

#include <bits/stdc++.h>
using namespace std;

// compressed trie
struct TrieNode {
    struct TrieEdge {
        string s;
        TrieNode *end;
        TrieEdge(string s, TrieNode *end) : s(s), end(end) {}
        ~TrieEdge() {
            // cout << "DELETE EDGE" << endl;
            if (end) delete end;
        }
    };
    unordered_map<char, TrieEdge *> edges;
    bool wordEnd = false;

    ~TrieNode() {
        // cout << "DELETE NODE" << endl;
        for (auto &[c, e] : edges) { if (e) delete e; }
    }
    
    // returns 0 on no match, 1 on prefix, 2 on word match
    int match(const string &s, int i = 0) {
        if (i == (int)s.size()) return wordEnd ? 2 : 1;
        if (!edges.count(s[i])) return 0;

        TrieEdge *e = edges[s[i]];
        if (s.size() - i >= e->s.size()) {
            if (s.substr(i, e->s.size()) != e->s) return 0;

            return e->end->match(s, i + e->s.size());
        }

        return e->s.substr(0, s.size() - i) == s.substr(i) ? 1 : 0;
    }

    void insert(const string &s, int i = 0) {
        if (i == (int)s.size()) {
            wordEnd = true;
            return;
        }

        if (!edges.count(s[i])) {
            TrieNode *n = new TrieNode();
            TrieEdge *e = new TrieEdge(s.substr(i, s.size() - i), n);
            edges[s[i]] = e;
            n->insert(s, s.size());
            return;
        }

        TrieEdge *e = edges[s[i]];
        if (s.size() - i >= e->s.size()) {
            if (s.substr(i, e->s.size()) == e->s) {
                // edge matches next part of string
                e->end->insert(s, i + e->s.size());
                return;
            }
        } else {
            if (e->s.substr(0, s.size() - i) == s.substr(i)) {
                // string matches next part of edge (ends in middle)
                // split edge in middle
                TrieEdge *e2 = new TrieEdge(e->s.substr(s.size() - i), e->end);
                TrieNode *mid = new TrieNode();
                e->end = mid;
                mid->edges[e2->s[0]] = e2;
                e->s.resize(s.size() - i);
                mid->insert(s, s.size());
                return;
            }
        }

        // fork edge
        int j = 0;
        while (e->s[j] == s[i+j]) ++j;
        TrieNode *mid = new TrieNode();
        TrieEdge *ea = new TrieEdge(e->s.substr(j), e->end);
        mid->edges[ea->s[0]] = ea;
        e->s.resize(j);
        e->end = mid;
        mid->insert(s, i + j);
    }

    int traverse(int depth = 0) {
        int sum = 0;
        if (wordEnd) sum += depth;
        for (auto &[c, e] : edges) { sum += e->end->traverse(depth + 1); }
        return sum;
    }
};

int main() {
    // env setup
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    // cout.tie(0); // comment out for baekjoon
    cout << fixed << setprecision(12);

    cout << fixed << setprecision(2);
    int n;
    while (cin >> n) {
        TrieNode *root = new TrieNode();
        string s;
        for (int i = 0; i < n; ++i) {
            cin >> s;
            root->insert(s);
        }
        int strokes = root->traverse();
        cout << ((double)strokes / (double)n) << "\n";
        delete root;
    }
}
