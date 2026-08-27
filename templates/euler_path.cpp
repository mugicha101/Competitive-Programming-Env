// valid degrees for directed euler tour:
// - all nodes have indeg = outdeg: euler cycle
// - all but 2 nodes have indeg = outdeg, one node has indeg = outdeg + 1, other node has outdeg = indeg + 1: euler path
//   must start at node with outdeg > indeg and end at node with indeg > outdeg
// valid degrees for undirected euler tour:
// - all nodes have even degree: euler cycle
// - all but 2 nodes have even degree: euler path, must start and end at node with odd degree
// regardless of directedness: all edges must also be in same weak component

// hierholzer algorithm:
// - start at src
// - visit edge with successor node having outdeg > 0
// - if remaining edges, traverse from there and insert them into existing traversal
// time complexity: O(N + M)

// requires union find
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

// DIRECTED GRAPH

void dEulerPathDFS(vector<int> &up, vector<vector<int>> &adj, int curr) {
    while (!adj[curr].empty()) {
        int next = adj[curr].back();
        adj[curr].pop_back();
        dEulerPathDFS(up, adj, next);
    }
    up.push_back(curr);
}
vector<int> dEulerPath(int n, const vector<pair<int,int>> &edges, bool cycle) {
    vector<vector<int>> adj(n);
    vector<int> indeg(n);
    UnionFind uf(n);
    int m = edges.size();
    for (auto &[a,b] : edges) {
        adj[a].push_back(b);
        uf.merge(a,b);
        ++indeg[b];
    }

    int r = -1;
    for (int i = 0; i < n; ++i) {
        if (adj[i].empty() && indeg[i] == 0) continue;

        int x = uf.find(i);
        if (r == -1) r = x;
        else if (r != x) return {};
    }
    int src = -1;
    int dst = -1;
    for (int i = 0; i < n; ++i) {
        int deg = indeg[i] - (int)adj[i].size();
        if (abs(deg) > 1) return {};
        else if (deg == -1) {
            if (src != -1) return {};
            src = i;
        } else if (deg == 1) {
            if (dst != -1) return {};
            dst = i;
        }
    }
    if ((cycle && src != -1) || (src == -1) ^ (dst == -1)) return {};

    if (src == -1) while (adj[++src].empty());
    vector<int> up;
    up.reserve(m);
    dEulerPathDFS(up, adj, src);
    reverse(up.begin(), up.end());
    return up;
}

// UNDIRECTED GRAPH

void udEulerPathDFS(vector<int> &up, vector<vector<pair<int,int>>> &adj, int curr, int prev) {
    while (!adj[curr].empty()) {
        auto [next, rev] = adj[curr].back();
        adj[curr].pop_back();
        if (next == -1) continue;

        adj[next][rev].first = -1;
        udEulerPathDFS(up, adj, next, curr);
    }
    up.push_back(curr);
}
vector<int> udEulerPath(int n, const vector<pair<int,int>> &edges, bool cycle) {
    vector<vector<pair<int,int>>> adj(n);
    UnionFind uf(n);
    int m = edges.size();
    for (auto &[a,b] : edges) {
        adj[a].emplace_back(b, adj[b].size());
        adj[b].emplace_back(a, adj[a].size()-1);
        uf.merge(a,b);
    }

    int r = -1;
    for (int i = 0; i < n; ++i) {
        if (adj[i].empty()) continue;

        int x = uf.find(i);
        if (r == -1) r = x;
        else if (r != x) return {};
    }
    int src = -1;
    int dst = -1;
    for (int i = 0; i < n; ++i) {
        if ((adj[i].size() & 1) == 0) continue;

        if (src != -1) {
            if (dst != -1) return {};
            dst = i;
        } else {
            src = i;
        }
    }
    if ((cycle && src != -1) || (src == -1) ^ (dst == -1)) return {};

    if (src == -1) while (adj[++src].empty());
    vector<int> up;
    up.reserve(m);
    udEulerPathDFS(up, adj, src, -1);
    return up;
}