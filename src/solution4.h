#ifndef COIN_CHANGE_SOL4
#define COIN_CHANGE_SOL4
#include <vector>
#include "convolution.h"
#include "solution3.h"

//solution 4 from On the Change-Making Problem paper by Timothy M. Chan, Qizheng He
//O(n + t log(t) loglog(t))
namespace solution4 {
    int getMinimumCoinNumberFor(std::vector<int> coins, int t) {
        int p2 = 1;
        int k = 0;
        while(p2 <= 2*t) {
            p2 <<=1;
            k++;
        }

        //binary search 2^l such that m* is in (2^l, 2^(l+1)]
        int low=0,high=k,mid,l=-1;
        while(low <= high) {
            mid = (low+high)/2;
            if(solution3::canChangeUsingAtMost(1<<mid,coins,t))
                high = mid-1;
            else {
                low = mid+1;
                l = mid;
            }
        }
        if(l == -1) 
            return t != 0;
        if(l == k)
            return -1;
        
        std::vector<int> C = solution3::getAllChangesUsingAtMost((1<<l),coins,t);
        C.resize(t+1);
        int res = (1<<l);
        int indexShift = 0;
        for(int i=l;i>=0;i--) {
            if(i < l-1) {
                std::vector<int> C2((t>>(l-i-1))+1);
                for(int j=0;j<C2.size();j++)
                    C2[C2.size()-j-1] = C[C.size()-j-1];
                indexShift += C.size()-C2.size();
                C = C2;
            }
            auto Cr = solution3::getAllChangesUsingAtMost(1<<i, coins, (t>>(l-i)));
            auto Cw = unallignedCombineCoinSets(C,Cr,t-indexShift+1);
            if(Cw[t-indexShift] == 0) {
                C = Cw;
                res += (1<<i);
            }
        }
        return res+1;
    }
};

#endif