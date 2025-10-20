#include <bits/stdc++.h>

using namespace std;

/**
 * Author: Ulf Lundstrom
 * Date: 2009-02-26
 * License: CC0
 * Source: My head with inspiration from tinyKACTL
 * Description: Class to handle points in the plane.
 * 	T can be e.g. double or long long. (Avoid int.)
 * Status: Works fine, used a lot
 */

/**
 * Author: Johan Sannemo
 * Date: 2017-04-16
 * License: CC0
 * Source: Basic algorithm knowledge
 * Description:
\\\begin{minipage}{75mm}
Returns a vector of indices of the convex hull in counter-clockwise order.
Points on the edge of the hull between two other points are not considered part of the hull.
\end{minipage}
\begin{minipage}{15mm}
\vspace{-6mm}
\includegraphics[width=\textwidth]{../content/geometry/ConvexHull}
\vspace{-6mm}
\end{minipage}
 * Status: tested with Kattis problems convexhull
 * Usage:
 * 	vector<P> ps, hull;
 *  trav(i, convexHull(ps)) hull.push_back(ps[i]);
 * Time: O(n \log n)
*/
#pragma once

typedef long long ll;

template <class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
};

typedef Point<ll> P;
pair<vector<int>, vector<int>> ulHull(const vector<P>& S) {
	vector<int> Q(S.size()), U, L;
	iota(Q.begin(), Q.end(), 0);                                                                                                                                                                         
	sort(Q.begin(), Q.end(), [&S](int a, int b){ return S[a] < S[b]; }); 
	for (auto &it : Q) {
    #define ADDP(C, cmp) while (C.size() > 1 && S[C[C.size()-2]].cross(S[it], S[C.back()]) cmp 0) C.pop_back(); C.push_back(it);
		ADDP(U, <=); ADDP(L, >=);
	}
	return {U, L}; 
}

vector<int> convexHull(const vector<P>& S) {
	vector<int> u, l; tie(u, l) = ulHull(S);
	if (S.size() <= 1) return u;
	if (S[u[0]] == S[u[1]]) return {0};
	l.insert(l.end(), u.rbegin()+1, u.rend()-1);
	return l;
}

// convex hull trick:
// dp[i] = f(i) + max(m(k) * x(i) + b(k))
// x(i) and m(i) both monotonically increase (if x(i) doesn't use Li Chao tree)
// let L(k,i) = m(k) * x(i) + b(k)
// want to maintain hull of lines and max line at current x(i)
// let deque<int> cht be the hull
// when advancing to i, check if L(cht[0],i) < L(cht[1],i) if so cht.pop_front()
// dp[i] = f(i) + max(m(cht[0]) * x(i) + b(cht[0]))
// to ensure hull maintained after adding line L(i,*), need to check that hull lines not dominated by L(i,*)
// this can be done as follows:
// while lineIntersection(L(cht[-2],*),L(cht[-1],*)) >= lineIntersection(L(cht[-2],*),L(i,*)):
//     cht.pop_back() since this means L(cht[-2],*) and L(i,*) dominates L(cht[-1],*)

// template for max cht
// dp[i] = f(i) + max(m(i) * x(i) + b(i))
// for min cht, flip M and B and dp[i] = f(i) - max(m(i) * x(i) + b(i))
auto F = [&](int i) { return _ };
auto M = [&](int k) { return _ };
auto X = [&](int i) { return _ };
auto B = [&](int k) { return _ };
auto L = [&](int k, int i) { return M(k) * X(i) + B(k); };
// get intersection of lines i and j in fraction form
auto lineInt = [&](int i, int j) -> pair<ll,ll> {
		pair<ll,ll> ret = make_pair(B(j) - B(i), M(i) - M(j));
		if (ret.second < 0) {
				ret.first = -ret.first;
				ret.second = -ret.second;
		}
		ll d = gcd(abs(ret.first), abs(ret.second));
		ret.first /= d;
		ret.second /= d;
		return ret;
};
auto fracGeq = [](pair<ll,ll> a, pair<ll,ll> b) {
		return (__int128_t)a.first * (__int128_t)b.second >= (__int128_t)b.first * (__int128_t)a.second;
};
deque<int> cht;
cht.push_back(0);
for (int i = 1; i < n; ++i) {
		while (cht.size() > 1 && L(cht[1],i) >= L(cht[0],i)) cht.pop_front();
		dp[i] = F(i) + L(cht[0],i);

		// given last 2 lines L_a, L_b and new line L_i
		// if L_b < L_a and L_i then pop L_b
		// repeat until false
		while (cht.size() >= 2 && fracGeq(lineInt(cht[cht.size()-2], cht.back()), lineInt(cht[cht.size()-2], i)))
				cht.pop_back();
		cht.push_back(i);
}
cout << dp[n-1] << endl;