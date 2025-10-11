#pragma once

#define _USE_MATH_DEFINES
#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef complex<double> cnum;

// get nth root of unity to the pth power
cnum inline getW(int n, double p) {
    return pow(cnum{M_E, 0}, cnum{0, M_PI * 2.0 * p / n});
}

// precondition: n power of 2
vector<cnum> fftHelper(const vector<cnum>& xArr, cnum w, int offset, int spacing) {
    // setup
    int n = (int)xArr.size() >> spacing;
    int h = n >> 1;
    int d = 1 << spacing;

    // base case
    if (n == 1)
        return {xArr[offset]};

    // recurse
    vector<cnum> yEven = fftHelper(xArr, w * w, offset, spacing + 1);
    vector<cnum> yOdd = fftHelper(xArr, w * w, offset + d, spacing + 1);

    // merge
    vector<cnum> yArr(n);
    cnum wk = 1;
    for (int k = 0; k < h; ++k) {
        yArr[k] = yEven[k] + wk * yOdd[k];
        yArr[k + h] = yEven[k] - wk * yOdd[k];
        wk *= w;
    }
    return yArr;
}

// precondition: n power of 2
vector<cnum> fft(const vector<cnum>& xArr) {
    int n = (int)xArr.size();
    if (n ^ (n & (-n)))
        throw exception();
    return fftHelper(xArr, getW((int)xArr.size(), -1), 0, 0);
}

// precondition: n power of 2
vector<cnum> ifft(const vector<cnum>& yArr) {
    int n = (int)yArr.size();
    if (n ^ (n & (-n)))
        throw exception();
    vector<cnum> output = fftHelper(yArr, getW((int)yArr.size(), 1), 0, 0);
    cnum divN = cnum{1.0/yArr.size(), 0};
    for (cnum& y : output)
        y *= divN;
    return output;
}

// ensure arr length is power of 2 (not needed if fft package used)
void pad(vector<cnum>& arr) {
    int s = (int)arr.size();
    while (s ^ (s & (-s))) {
        arr.emplace_back(0,0);
        ++s;
    }
}

// multiplies polynomials represented by the coefficient arrays A and B
// returns a coefficient array
vector<cnum> multiplyPolynomials(vector<cnum> A, vector<cnum> B) {
    // pad length of A and B to account for larger output C
    unsigned int outputSize = A.size() + B.size();
    while (A.size() < outputSize)
        A.emplace_back(0);
    while (B.size() < outputSize)
        B.emplace_back(0);

    // apply dft (O(nlgn))
    pad(A);
    pad(B);
    vector<cnum> dftA = fft(A);
    vector<cnum> dftB = fft(B);

    // point-wise multiply to get dft of A * B (O(n))
    vector<cnum> dftC;
    dftC.reserve(dftA.size());
    for (int i = 0; i < dftA.size(); ++i)
        dftC.push_back(dftA[i] * dftB[i]);

    // inverse dft
    vector<cnum> C = ifft(dftC);

    // trim until matches outputSize
    while (C.size() > outputSize)
        C.pop_back();
    return C;
}

// calculates {a + b | a in A and b in B}
// preconditions: A.size() == B.size() == n, 0 <= A[i], B[i] <= 10n, all elements in A and B are distinct integers
vector<int> cartesianSum(const vector<int>& A, const vector<int>& B) {
    int n = (int)A.size();

    // get input freq arrays (O(n))
    vector<cnum> freqA(n * 10 + 1);
    vector<cnum> freqB(n * 10 + 1);
    for (int a : A)
        freqA[a] += 1;
    for (int b : B)
        freqB[b] += 1;

    // multiply polynomials (O(nlgn))
    vector<cnum> freqC = multiplyPolynomials(freqA, freqB);

    // convert to output array (O(n))
    vector<int> C;
    for (int c = 0; c < freqC.size(); ++c) {
        int amount = (int)round(freqC[c].real());
        for (int i = 0; i < amount; ++i)
            C.push_back(c);
    }
    return C;
}

// driver code
int main() {
    ios_base::sync_with_stdio(false);

    // define A and B
    const int n = 1000;
    vector<int> A;
    vector<int> B;
    random_device rd;
    mt19937 rGen(rd());
    uniform_int_distribution<int> rDistr(0, n * 10);
    while (A.size() < n) {
        A.push_back(rDistr(rGen));
        B.push_back(rDistr(rGen));
    }

    // calculate cartesian sum of A and B
    auto calcStart = chrono::high_resolution_clock::now();
    vector<int> C = cartesianSum(A, B);
    auto calcEnd = chrono::high_resolution_clock::now();

    // output result
    for (int c : C)
        cout << c << " ";
    cout << endl << "time in ms: " << (calcEnd - calcStart).count() / 1000 << endl;
}