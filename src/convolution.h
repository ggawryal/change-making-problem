#ifndef COIN_CHANGE_CONVOLUTION
#define COIN_CHANGE_CONVOLUTION
#include <vector>
#include <complex>
#include <fftw3.h>
const double PI = acos(-1);

void FFTW(std::vector<std::complex<double> >&A, int t=1) {
    const int n = A.size();
    fftw_complex *in, *out;
    fftw_plan p;


    in  = fftw_alloc_complex(n);
    out = fftw_alloc_complex(n);

    for(int i=0;i<n;i++) {
        in[i][0] = A[i].real();
        in[i][1] = A[i].imag();
    }

    p = fftw_plan_dft_1d(n, in, out, t, FFTW_ESTIMATE);
    fftw_execute(p);
    
    for(int i=0;i<n;i++) 
        A[i] = {out[i][0], out[i][1]}; 
    
    
    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);
}

void iterativeFFT(std::vector<std::complex<double> >&A, int t=1) {
    typedef std::complex<double> C;

    const int n = A.size();

    std::vector<C> e(n);
    for(int i=0;i<n;i++)
        e[i] = exp(C(0,t*2*PI*i/n));

    for(int i=0;i<n;i++) {
        int j=0; 
        for(int k=1; k<n; k<<=1, j<<=1)
            if(k&i) j++;
        j>>=1; 
        if(i<j) 
            std::swap(A[i], A[j]);
    }

    int k = 0;
    while((1<<k)<n) 
        k++;
    
    for(int m=2;m<=n;m<<=1) {
        k--;
        for(int i=0; i<n; i+=m) {
            for(int j=0; j<m/2; j++) {
                C u = A[i+j];
                C v = e[j<<k]*A[i+j+m/2];
                A[i+j] = u+v;
                A[i+j+m/2] = u-v;
            }
        }
    }
}

void recursiveFFT(std::vector<std::complex<double> >& A, int t=1) {
    typedef std::complex<double> C;
    int n = A.size();
    if(n == 1)
        return;

    std::vector<C> P(n/2),N(n/2);
    for(int i=0;i<n;i+=2) {
        P[i/2] = A[i];
        N[i/2] = A[i+1];
    }
    recursiveFFT(P,t);
    recursiveFFT(N,t);

    C e = exp(C(0,t*2*PI/n)), cur = 1;
    for(int i=0;i<n/2;i++) {
        A[i] = P[i] + cur*N[i];
        A[i+n/2] = P[i] - cur*N[i];
        cur *= e;
    }
}

void FFT(std::vector<std::complex<double> >&A, int t=1) {return FFTW(A,t);}

std::vector<std::complex<double> > toComplexVector(std::vector<int> A) {
    return std::vector<std::complex<double> >(A.begin(),A.end());
}

//A and B should be of lenght 2^k, where 2^k > 2*t
std::vector<int> combineCoinSets(std::vector<int> A, std::vector<int> B, bool removeCoinsAfterHalf = true) {
    auto Ac = toComplexVector(A), Bc = toComplexVector(B);
    FFT(Ac);
    FFT(Bc);
    for(int i=0;i<(int)A.size();i++)
        Ac[i] *= Bc[i];
    FFT(Ac,-1);
    for(int i=0;i<(int)A.size();i++)
        A[i] = (Ac[i].real() >= A.size()/2 ? 1 : 0);

    if(removeCoinsAfterHalf) {
        for(int i=A.size()/2;i<(int)A.size();i++)
            A[i] = 0;
    }
    return A;
}


std::vector<int> unallignedCombineCoinSets(std::vector<int> A, std::vector<int> B, int maxIndex) {
    if(A.size() < B.size())
        swap(A,B);
    int p = 1;
    while(2*p <= A.size())
        p <<=1;
    
    A.resize(p);
    B.resize(p);
    A = combineCoinSets(A,B,false);
    A.resize(maxIndex+1);
    return A;
}

#endif