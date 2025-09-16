#pragma once

#include <bits/stdc++.h>
using namespace std;

// TODO: DEBUG
// max matching on bipartite graph (max size set of non-touching edges)
// maximum independent set size = # of vertices - |max matching size| (max number of nonadjacent nodes)
// O(VE)
struct Khun {
    int n, k;

    // reading output: // if mt[u] = v != -1 indicates edge(u in U, v in V) is in the max matching
    vector<vector<int>> g;
    vector<int> mt;
    vector<bool> used;

    // note: g is bipartite graph U,V where g[u in U] = subset of V that u is adjacent to
    // uses 0 indexed nodes
    Khun(vector<vector<int>> g) : n(g.size()), g(g) {}

    bool try_kuhn(int v) {
        if (used[v])
            return false;
        used[v] = true;
        for (int to : g[v]) {
            if (mt[to] == -1 || try_kuhn(mt[to])) {
                mt[to] = v;
                return true;
            }
        }
        return false;
    }

    // convert standard adjacency list to bipartite graph
    // return {} if impossible
    static vector<vector<int>> bipartite(const vector<vector<int>> &adj) {
        // bipartite + re-index so that V is lower indices and U is higher indices
        int n = (int)adj.size();
        vector<int> color(n, -1); // color
        queue<int> q;
        array<vector<int>,2> b;
        for (int root = 0; root < n; ++root) {
            if (color[root] != -1) continue;

            color[root] = 0;
            b[0].push_back(0);
            q.push(root);
            int c = 0;
            while (!q.empty()) {
                c = 1 - c;
                for (int qi = (int)q.size(); qi > 0; --qi) {
                    int curr = q.front();
                    q.pop();
                    for (int next : adj[curr]) {
                        if (color[next] != -1) {
                            if (color[next] != c) return {};
                            continue;
                        }

                        color[next] = c;
                        b[c].push_back(next);
                        q.push(next);
                    }
                }
            }
        }
        vector<int> nidx(n); // old index -> new index
        vector<int> oidx(n); // new index -> old index
        for (int i = 0; i < (int)b[0].size(); ++i) {
            int j = b[0][i];
            nidx[j] = i;
            oidx[i] = j;
        }
        int o = (int)b[0].size();
        for (int i = 0; i < (int)b[1].size(); ++i) {
            int j = b[1][i];
            oidx[j] = o + i;
            nidx[o + i] = j;
        }
        vector<vector<int>> g(b[0].size());
        for (int i = 0; i < (int)g.size(); ++i) {
            g[i] = adj[oidx[i]];
            for (auto &j : g[i]) j = nidx[j];
        }
        return g;
    }

    void solve() {
        mt.assign(k, -1);
        for (int v = 0; v < n; ++v) {
            used.assign(n, false);
            try_kuhn(v);
        }
    }
};