#ifndef COIN_CHANGE_SOL1
#define COIN_CHANGE_SOL1
#include <vector>
#include "convolution.h"

//solution 1 from On the Change-Making Problem paper by Timothy M. Chan, Qizheng He
//O(n + t log^3t)
namespace solution1 {
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
                res = combineCoinSets(res,w);
            w = combineCoinSetsWhereAEqualsB(w);
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