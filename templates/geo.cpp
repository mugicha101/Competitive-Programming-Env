// compares 2 vectors, returns true if a has smaller angle than b (0 angle = x+ axis)
// when used in sort, orders clockwise (closest first when colinear)
template<typename T>
bool angleCmp(pair<T,T> a, pair<T,T> b) {
    auto [ax, ay] = a;
    auto [bx, by] = b;
    bool aHalf = (ay > 0) || (ay == 0 && ax >= 0);
    bool bHalf = (by > 0) || (by == 0 && bx >= 0);
    T cross = ax * by - ay * bx;

    // aHalf != bHalf: upper half < lower half
    // cross != 0: ccw < cw
    // cross == 0: colinear, choose closest first
    return aHalf == bHalf ? cross == 0 ? ax * ax + ay * ay < bx * bx + by * by : cross > 0 : aHalf;
}

// get value v representing separation (    t actual separation, but can use for comparisons)
// v = a dot b / (mag(a) * mag(b))
template<typename T>
bool sepRank(pair<T,T> a, pair<T,T> b) {
    auto [ax, ay] = a;
    auto [bx, by] = b;
    return (long double)(ax * bx + ay * by) / sqrt((long double)((ax * ax + ay * ay) * (bx * bx + by * by)));
}


// dist squared from point to closest point on edge of simple poly (negative if inside poly)
// poly given ccw or cw
// T = input integer type, CT = calculation int type (should be double T), F = floating point type
template<typename T, typename CT = i128, typename F = ld>
F pip(const vector<pair<T,T>> &poly, pair<T,T> p) {
    F res = numeric_limits<F>::max();
    int w = 0;
    for (int i = 0; i < poly.size(); ++i) {
        const auto &a = i ? poly[i-1] : poly.back();
        const auto &b = poly[i];

        // translate by -a
        CT dx = (CT)b.first - a.first, dy = (CT)b.second - a.second;
        CT px = (CT)p.first - a.first, py = (CT)p.second - a.second;
        CT ds = dx * dx + dy * dy;
        if (ds == 0) continue; // duplicate point

        // figure out side point is on
        CT cross = dx * py - dy * px;
        CT dot = px * dx + py * dy;
        if (cross == 0 && dot >= 0 && dot <= ds) return 0.L; // on polygon

        // update winding number using pre-translated a,b,p
        w += (cross > 0 && a.second <= p.second && p.second < b.second);
        w -= (cross < 0 && b.second <= p.second && p.second < a.second);

        // get dist to projection point q (floating point)
        F qm = (F)clamp(dot, (CT)0, ds) / ds;
        F rx = qm * dx - px;
        F ry = qm * dy - py;
        res = min(res, rx * rx + ry * ry);
    }
    return w ? -res : res;
}

// KACTL

/**
 * Author: Ulf Lundstrom
 * Date: 2009-02-26
 * License: CC0
 * Source: My head with inspiration from tinyKACTL
 * Description: Class to handle points in the plane.
 * 	T can be e.g. double or long long. (Avoid int.)
 * Status: Works fine, used a lot
 */

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
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
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

/**
 * Author: chilli, Takanori MAEHARA
 * Date: 2019-10-31
 * License: CC0
 * Source: https://github.com/spaghetti-source/algorithm/blob/master/geometry/_geom.cc#L744
 * Description: Returns the area of the intersection of a circle with a
 * ccw polygon.
 * Time: O(n)
 * Status: Tested on GNYR 2019 Gerrymandering, stress-tested
 */

typedef Point<double> P;
#define arg(p, q) atan2(p.cross(q), p.dot(q))
double circlePoly(P c, double r, vector<P> ps) {
	auto tri = [&](P p, P q) {
		auto r2 = r * r / 2;
		P d = q - p;
		auto a = d.dot(p)/d.dist2(), b = (p.dist2()-r*r)/d.dist2();
		auto det = a * a - b;
		if (det <= 0) return arg(p, q) * r2;
		auto s = max(0., -a-sqrt(det)), t = min(1., -a+sqrt(det));
		if (t < 0 || 1 <= s) return arg(p, q) * r2;
		P u = p + d * s, v = q + d * (t-1);
		return arg(p,u) * r2 + u.cross(v)/2 + arg(v,q) * r2;
	};
	auto sum = 0.0;
	rep(i,0,sz(ps))
		sum += tri(ps[i] - c, ps[(i + 1) % sz(ps)] - c);
	return sum;
}