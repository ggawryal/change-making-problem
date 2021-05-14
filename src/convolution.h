#ifndef COIN_CHANGE_CONVOLUTION
#define COIN_CHANGE_CONVOLUTION
#include <vector>
#include <complex>
#include <fftw3.h>
const float PI = acos(-1);

//select fft version
#ifndef DONT_USE_FFTW
void FFTW(std::vector<std::complex<float> >&A, int t=1) {
    const int n = A.size();
    if(t == 1) {
        float *in  = fftwf_alloc_real(n);
        fftwf_complex *out = fftwf_alloc_complex(n);
        for(int i=0;i<n;i++)
            in[i] = A[i].real();

        fftwf_plan p = fftwf_plan_dft_r2c_1d(n,in,out,FFTW_ESTIMATE);
        fftwf_execute(p);
        for(int i=0;i<n;i++)
            if(i < n-i)
                A[i] = {out[i][0],-out[i][1]};
            else
                A[i] = {out[n-i][0], out[n-i][1]};
        fftwf_destroy_plan(p);
        fftwf_free(in); fftwf_free(out);
        return;
    }
    else {
        fftwf_complex *in  = fftwf_alloc_complex(n/2+1);
        float *out = fftwf_alloc_real(n);
        for(int i=0;i<=n/2;i++) {
            in[i][0] = A[i].real();
            in[i][1] = -A[i].imag();
        }
        fftwf_plan p = fftwf_plan_dft_c2r_1d(n, in, out, FFTW_ESTIMATE);
        fftwf_execute(p);
        for(int i=0;i<n;i++) 
            A[i] = {out[i], 0}; 
            
        fftwf_destroy_plan(p);
        fftwf_free(in); fftwf_free(out);
    }
}
void FFT(std::vector<std::complex<float> >&A, int t=1) {return FFTW(A,t);}

#else
void iterativeFFT(std::vector<std::complex<float> >&A, int t=1) {
    typedef std::complex<float> C;

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
void FFT(std::vector<std::complex<float> >&A, int t=1) {return iterativeFFT(A,t);}
#endif

std::vector<std::complex<float> > toComplexVector(std::vector<int> A) {
    return std::vector<std::complex<float> >(A.begin(),A.end());
}

std::vector<std::complex<float> > toComplexAndFFT(std::vector<int> A) {
    auto Ac = toComplexVector(A);
    FFT(Ac);
    return Ac;
}

//Ac = FFT(A), Bc = FFT(B)
std::vector<int> combineCoinSetsPart2(std::vector<std::complex<float> > Ac, std::vector<std::complex<float> > Bc, bool removeCoinsAfterHalf) {
    std::vector<int> A(Ac.size());
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

//A and B should be of lenght 2^k, where 2^k > 2*t
std::vector<int> combineCoinSets(std::vector<int> A, std::vector<int> B, bool removeCoinsAfterHalf = true) {
    auto Ac = toComplexAndFFT(A);
    if(A == B)
        return combineCoinSetsPart2(Ac,Ac,removeCoinsAfterHalf);
    else
        return combineCoinSetsPart2(Ac,toComplexAndFFT(B),removeCoinsAfterHalf);
}


std::vector<int> unallignedCombineCoinSets(std::vector<int> A, std::vector<int> B, int maxIndex) {
    if(maxIndex == -1)
        maxIndex = (int)A.size() + (int)B.size()-1;
    if(A.size() < B.size())
        swap(A,B);
    int p = 1;
    while(p < 2*A.size())
        p <<=1;
    
    A.resize(p);
    B.resize(p);
    A = combineCoinSets(A,B,false);
    A.resize(maxIndex);
    return A;
}

#endif