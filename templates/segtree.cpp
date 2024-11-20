#pragma once

#include <bits/stdc++.h>
using namespace std;
template<typename A, typename B> ostream& operator<<(ostream &os, const pair<A, B> &p) { return os << '(' << p.first << ", " << p.second << ')'; }

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

    void assign(int index, T value) {
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

// LAZY SEG TREE
// array based
// inclusive queries
class LazySegTree {
    // config
    using T = pair<int,int>; // type
    using L = int; // lazy type (must be numeric)
    const T ZRV = make_pair(INT_MAX, INT_MIN); // value to return if range is 0
    const T IV = make_pair(INT_MAX, INT_MIN); // initial value of elements
    inline T op(T l, T r) const { return make_pair(min(l.first, r.first), max(l.second, r.second)); } // range query operator
    inline T lop(T v, L l) const { return make_pair(v.first + l, v.second + l); } // lazy update operator
    // config end

    vector<L> lazy;
    vector<T> nodes;

    inline void asn(int i) {
        nodes[i] = op(at(left(i)), at(right(i)));
    }

    inline T at(int i) const {
        return lop(nodes[i], lazy[i]);
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

    T update_helper(int index, int size, int first, int last, L value) {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) {
            lazy[index] += value;
            return at(index);
        }
        size >>= 1;
        update_helper(left(index), size, first, last, value);
        update_helper(right(index), size, first - size, last - size, value);
        asn(index);
        return at(index);
    }

    T query_helper(int index, int size, int first, int last) const {
        if (first >= size || last < 0) return ZRV;
        if (first <= 0 && last >= size-1) return at(index);
        size >>= 1;
        return lop(op(query_helper(left(index), size, first, last), query_helper(right(index), size, first - size, last - size)), lazy[index]);
    }

    void dump_helper(int index, int indent) {
        cout << string(indent * 2, ' ') << "(" <<  nodes[index] << " + " << lazy[index] << ")" << endl;
        if (leaf(index)) return;

        dump_helper(left(index), indent+1);
        dump_helper(right(index), indent+1);
    }

public:
    
    LazySegTree(int size) {
        --size;
        int n = 1;
        while (size) {
            n <<= 1;
            size >>= 1;
        }
        nodes.resize(n * 2 - 1, IV);
        lazy.resize(n * 2 - 1, 0);
        for (int i = n-2; i >= 0; --i) {
            asn(i);
        }
    }

    T get(int index) const {
        return nodes[elem(index)];
    }

    void assign(int index, T value) {
        index = elem(index);
        nodes[index] = value;
        lazy[index] = 0;
        while (index) {
            index = parent(index);
            asn(index);
        }
    }

    void update(int first, int last, L value) {
        update_helper(0, (nodes.size() + 1) >> 1, first, last, value);
    }

    T query(int first, int last) const {
        return query_helper(0, (nodes.size() + 1) >> 1, first, last);
    }

    void dump() {
        dump_helper(0, 0);
    }
};
