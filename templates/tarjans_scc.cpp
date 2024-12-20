/**
 * Author: Lukas Polacek
 * Date: 2009-10-28
 * License: CC0
 * Source: Czech graph algorithms book, by Demel. (Tarjan's algorithm)
 * Description: Finds strongly connected components in a
 * directed graph. If vertices $u, v$ belong to the same component,
 * we can reach $u$ from $v$ and vice versa.
 * Usage: scc(graph, [\&](vi\& v) { ... }) visits all components
 * in reverse topological order. comp[i] holds the component
 * index of a node (a component only has edges to components with
 * lower index). ncomps will contain the number of components.
 * Time: O(E + V)
 * Status: Bruteforce-tested for N <= 5
 */
#pragma once

#include <bits/stdc++.h>
using namespace std;

vector<int> val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int tarjans_dfs(int j, G& g, F& f) {
  int low = val[j] = ++Time, x; z.push_back(j);
  for (auto e : g[j]) if (comp[e] < 0)
    low = min(low, val[e] ?: tarjans_dfs(e,g,f));

  if (low == val[j]) {
    do {
      x = z.back(); z.pop_back();
      comp[x] = ncomps;
      cont.push_back(x);
    } while (x != j);
    f(cont); cont.clear();
    ncomps++;
  }
  return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
  int n = g.size();
  val.assign(n, 0); comp.assign(n, -1);
  Time = ncomps = 0;
  for (int i = 0; i < n; ++i) if (comp[i] < 0) tarjans_dfs(i, g, f);
}