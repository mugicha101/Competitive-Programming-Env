// hirschberg's algorithm following https://dl.acm.org/doi/10.1145/360825.360861
// given n x m matrix V, finds a path in V from (0,0) to (n-1,m-1) with min/max sum
// given that edges of form (i,j) -> max/min{(i,j+1), (i+1,j), (i,j) + V[i][j]}
// O(nm) time, O(min(n,m)) space, but searches less area than standard 2D DP via divide and conquer

// pseudocode:
// w.l.o.g. use max instead of max/min
// let MSP(M) be max sum path on matrix M
// let MS(M) be sum of max sum path on matrix M
// halve M into T=M[0..<h], B=M[h..]
// choose j s.t. MS(T[0..<j]) + MS(B[j..]) is maximized using basic MS algorithm
// MSP(M) = MSP(T[0..<j]) + MSP(B[j..])

struct HirschbergLCS {
    using T = uint16_t;
    
    inline T trans(T l, T u, T d, char ac, char bc) const {
        return ac == bc ? d + 1 : max(l,u);
    }
    
    vector<T> fdp, rdp;
    void msp(string &res, const string &a, const string &b, int si, int ei, int sj, int ej) {
        if (si >= ei|| sj >= ej) return;
        if (si + 1 == ei) {
            if (find(b.begin() + sj, b.begin() + ej, a[si]) != b.begin() + ej) {
                res += a[si];
            }
            return;
        }
        if (sj + 1 == ej) {
            if (find(a.begin() + si, a.begin() + ei, b[sj]) != a.begin() + ei) {
                res += b[sj];
            }
            return;
        }

        int hi = (si + ei) >> 1;
        memset(&fdp[sj], 0, sizeof(T) * (ej+1-sj));
        memset(&rdp[sj], 0, sizeof(T) * (ej+1-sj));
        
        // forward LCS
        for (int i = si+1; i <= hi; ++i) {
            T d = fdp[sj];
            for (int j = sj+1; j <= ej; ++j) {
                T t = fdp[j];
                fdp[j] = trans(fdp[j-1], fdp[j], d, a[i-1], b[j-1]);
                d = t;
            }
        }

        // reverse LCS
        for (int i = ei-1; i >= hi; --i) {
            T d = rdp[ej];
            for (int j = ej-1; j >= sj; --j) {
                T t = rdp[j];
                rdp[j] = trans(rdp[j+1], rdp[j], d, a[i], b[j]);
                d = t;
            }
        }

        // find optimal column split
        int k = sj;
        T kv = fdp[sj] + rdp[sj];
        for (int j = sj+1; j <= ej; ++j) {
            T jv = fdp[j] + rdp[j];
            if (jv <= kv) continue;

            k = j;
            kv = jv;
        }

        msp(res, a, b, si, hi, sj, k);
        msp(res, a, b, hi, ei, k, ej);
    }

    string calc(const string &a, const string &b) {
        if (a.size() < b.size()) return calc(b, a);

        fdp.resize(b.size() + 1);
        rdp.resize(b.size() + 1);
        string res;
        res.reserve(b.size());
        msp(res, a, b, 0, a.size(), 0, b.size());
        return res;
    }
};
