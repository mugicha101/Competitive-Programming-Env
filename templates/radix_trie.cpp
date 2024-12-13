#define _USE_MATH_DEFINES
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
};
