#ifndef COIN_CHANGE_CONVOLUTION
#define COIN_CHANGE_CONVOLUTION
#include <vector>
#include <complex>

void FFT(std::vector<std::complex<double> >& A, int t=1) {
    typedef std::complex<double> C;
    int n = A.size();
    if(n == 1)
        return;

    std::vector<C> P(n/2),N(n/2);
    for(int i=0;i<n;i+=2) {
        P[i/2] = A[i];
        N[i/2] = A[i+1];
    }
    FFT(P,t);
    FFT(N,t);
    const double PI = acos(-1);
    C e = exp(C(0,t*2*PI/n)), cur = 1;
    for(int i=0;i<n/2;i++) {
        A[i] = P[i] + cur*N[i];
        A[i+n/2] = P[i] - cur*N[i];
        cur *= e;
    }
}

std::vector<std::complex<double> > toComplexVector(std::vector<int> A) {
    return std::vector<std::complex<double> >(A.begin(),A.end());
}

//A and B should be of lenght 2^k, where 2^k > 2*t
std::vector<int> combineCoinSets(std::vector<int> A, std::vector<int> B) {
    auto Ac = toComplexVector(A), Bc = toComplexVector(B);
    FFT(Ac);
    FFT(Bc);
    for(int i=0;i<(int)A.size();i++)
        Ac[i] *= Bc[i];
    FFT(Ac,-1);
    for(int i=0;i<(int)A.size();i++)
        A[i] = (i < (int)A.size()/2 && Ac[i].real() >= A.size()/2 ? 1 : 0);
    return A;
}

#endif