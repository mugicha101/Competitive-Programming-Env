#include <bits/stdc++.h>

// TODO: rewrite segtrees using array

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

// all primes necessary for the prime factorization of a number <= 100,000
const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 
    137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 
    227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313};

bool prime(ll a) { if (a==1) return 0; ll cap = sqrt(a); for (int i=2;i<=cap;++i) if (a%i==0) return 0; return 1; }

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const { return hash<T1>{}(p.first) ^ hash<T2>{}(p.second); }
};

typedef vector<vector<int>> AdjList;

// POW 2 GENERAL SEG TREE
// inclusive queries
struct SegTree {
    typedef int T;
    SegTree* left;
    SegTree* right;
    int size;
    int half;
    T val = 0;

    // interchangable op
    inline T op(T a, T b) const {
        return a + b;
    }

    SegTree() {}
    SegTree(int size) {
        // size needs to be power of 2
        this->size = size;
        half = this->size >> 1;
        if (half) {
            left = new SegTree(half);
            right = new SegTree(half);
        }
    }

    void update(int i, T v) {
        if (half) {
            (i < half ? left : right)->update(i & (half-1), v);
            val = op(left->val, right->val);
        } else val = v; // depending on what you want can be op(val, v)
    }

    T query(int l, int r) {
        if (r < 0 || l >= size) return 0; // depends on op
        if (l <= 0 && r+1 >= size) return val;
        return op(left->query(l, r), right->query(l-half, r-half));
    }
};

// STANDARD SEG TREE
// array based
// inclusive queries
class SegTree {
    // config
    using T = int; // type
    const T ZRV = 0; // value to return if range is 0
    const T IV = 0; // initial value of elements
    inline T op(T l, T r) const { return l + r; } // range query operator
    // config end

    vector<T> nodes;

    inline void asn(int i) {
        nodes[i] = op(nodes[left(i)], nodes[right(i)]);
    }

    inline int left(int i) const {
        return (i << 1) + 1;
    }

    inline int right(int i) const {
        return (i << 1) + 2;
    }

    inline int parent(int i) const {
        return (i - 1) >> 1;
    }

    inline int elem(int i) const {
        return (nodes.size() >> 1) + i;
    }

    inline bool leaf(int i) const {
        return i >= elem(0);
    }

    T query_helper(int index, int size, int first, int last) const {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) return nodes[index];
        size >>= 1;
        return op(query_helper(left(index), size, first, last), query_helper(right(index), size, first - size, last - size));
    }

    void dump_helper(int index, int indent) {
        cout << string(indent * 2, ' ') << "(" <<  nodes[index] << ")" << endl;
        if (leaf(index)) return;

        dump_helper(left(index), indent+1);
        dump_helper(right(index), indent+1);
    }

public:

    SegTree(int size) {
        --size;
        int n = 1;
        while (size) {
            n <<= 1;
            size >>= 1;
        }
        nodes.resize(n * 2 - 1, IV);
        for (int i = n-2; i >= 0; --i) {
            asn(i);
        }
    }

    T get(int index) const {
        return nodes[elem(index)];
    }

    void update(int index, T value) {
        index = elem(index);
        nodes[index] = value;
        while (index) {
            index = parent(index);
            asn(index);
        }
    }

    T query(int first, int last) const {
        return query_helper(0, (nodes.size() + 1) >> 1, first, last);
    }

    void dump() {
        dump_helper(0, 0);
    }
};

// LAZY PROP RANGE ADDITION QUERY SEG TREE
// TODO: array based
// modify for sum, prod, xor, etc.
struct SegTree {
    typedef int T;
    SegTree* left = nullptr;
    SegTree* right = nullptr;
    int size;
    T val = 0;
    T lazy = 0;
    SegTree(int size) : size(size) {
        if (size == 1) return;
        left = new SegTree(size >> 1);
        right = new SegTree((size + 1) >> 1);
    }

    void init(const vector<T>& arr, int offset = 0) {
        if (size == 1) {
            val = arr[offset];
            return;
        }
        left->init(arr, offset);
        right->init(arr, offset + (size >> 1));
        val = left->val + right->val;
    }
    
    ~SegTree() {
        if (size == 1) return;
        delete left;
        delete right;
    }

    void add(int first, int last, T value) {
        if (first >= size || last < 0) return;
        if (first <= 0 && last >= size-1) {
            (size == 1 ? val : lazy) += value;
            return;
        }
        left->add(first, last, value);
        right->add(first - (size >> 1), last - (size >> 1), value);
        val = left->val + right->val;
    }

    void add(int index, T value) {
        if (size == 1) {
            val += value;
            return;
        }
        val += value;
        if (index < (size >> 1)) left->update(index, value);
        else right->update(index - (size >> 1), value);
    }

    void update(int index, T value) {
        if (size == 1) {
            val = value;
            return;
        }
        if (index < (size >> 1)) left->update(index, value);
        else right->update(index - (size >> 1), value);
        val = left->val + right->val; // mayb need to include lazy need to test
    }

    T query(int first, int last) {
        if (first >= size || last < 0) return 0;
        if (first <= 0 && last >= size-1) return lazy * size + val;
        int itv = last + 1 - first;
        return lazy * itv + left->query(first, last) + right->query(first - (size >> 1), last - (size >> 1));
    }
};

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