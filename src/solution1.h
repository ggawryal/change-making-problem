#ifndef COIN_CHANGE_SOL1
#define COIN_CHANGE_SOL1
#include <vector>
#include <complex>
//from On the Change-Making Problem paper by Timothy M. Chan, Qizheng He
//O(n + t log^3t)
namespace solution1 {
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
    std::vector<int> convolution(std::vector<int> A, std::vector<int> B) {
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

    template<class Num=int>
    bool canChangeUsingAtMost(int m, std::vector<Num> coins, Num t) {
        int p2 = 1;
        while(p2 <= 2*t)
            p2 <<=1;

        std::vector<Num> res(p2),w(p2);
        res[0] = w[0] = 1;
        for(auto &c : coins)
            w[c] = 1;
        
        while(m > 0) {
            if(m&1)
                res = convolution(res,w);
            w = convolution(w,w);
            m>>=1;
        }
        return res[t];
    }

    template<class Num=int>
    Num getMinimumCoinNumberFor(std::vector<Num> coins, Num t) {
        Num l=0, r=t, m, res=-1;
        while(l <= r) {
            m = (l+r)/2;
            if(canChangeUsingAtMost(m,coins,t))
                r = m-1, res = m;
            else
                l = m+1;
        }
        return res;
    }

};

#endif