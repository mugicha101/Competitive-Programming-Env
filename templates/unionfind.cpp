#pragma once

#include <bits/stdc++.h>
using namespace std;

// MINE
struct UnionFind {
    vector<int> uf;
    vector<int> rank;
    int comps;
    UnionFind(int size) : uf(size), rank(size), comps(size) {
        iota(uf.begin(), uf.end(), 0);
    }

    int find(int x) {
        if (uf[x] != x) uf[x] = find(uf[x]);
        return uf[x];
    }

    bool merge(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) return false;

        --comps;
        if (ra > rb) {
            uf[rb] = ra;
        } else {
            uf[ra] = rb;
            rank[rb] += rank[ra] == rank[rb];
        }
        return true;
    }
};

// KACTL
/**
 * Author: Lukas Polacek
 * Date: 2009-10-26
 * License: CC0
 * Source: folklore
 * Description: Disjoint-set data structure.
 * Time: $O(\alpha(N))$
 */
struct UnionFind {
    vector<int> e;
    int comps;
    UnionFind(int n) : e(n, -1), comps(n) {}
    bool sameSet(int a, int b) { return find(a) == find(b); }
    int size(int x) { return -e[find(x)]; }
    int find(int x) { return e[x] < 0 ? x : e[x] = find(e[x]); }
    bool join(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (e[a] > e[b]) swap(a, b);
        e[a] += e[b]; e[b] = a;
        --comps;
        return true;
    }
};

/**
 * Author: Lukas Polacek, Simon Lindholm
 * Date: 2019-12-26
 * License: CC0
 * Source: folklore
 * Description: Disjoint-set data structure with undo.
 * If undo is not needed, skip st, time() and rollback().
 * Usage: int t = uf.time(); ...; uf.rollback(t);
 * Time: $O(\log(N))$
 * Status: tested as part of DirectedMST.h
 */
#pragma once

struct RollbackUF {
	vi e; vector<pii> st;
	RollbackUF(int n) : e(n, -1) {}
	int size(int x) { return -e[find(x)]; }
	int find(int x) { return e[x] < 0 ? x : find(e[x]); }
	int time() { return sz(st); }
	void rollback(int t) {
		for (int i = time(); i --> t;)
			e[st[i].first] = st[i].second;
		st.resize(t);
	}
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		st.push_back({a, e[a]});
		st.push_back({b, e[b]});
		e[a] += e[b]; e[b] = a;
		return true;
	}
};