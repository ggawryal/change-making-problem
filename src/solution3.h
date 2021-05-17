#ifndef COIN_CHANGE_SOL3
#define COIN_CHANGE_SOL3
#include <vector>
#include "convolution.h"

//solution 3 from On the Change-Making Problem paper by Timothy M. Chan, Qizheng He
//O(n + t log^2t), decision version in O(n + t log t)
//coins should be in sorted order
namespace solution3 {
    std::vector<int> computeCmt(std::vector<int> w1, int m, int t) {
        if(m == 0) {
            for(int i=0;i<(int)w1.size();i++)
                w1[i] = (i == 0 ? 1 : 0);
            return w1;
        }
        if(m == 1)
            return w1;
        
        auto halfW1 = w1;
        halfW1.resize(t/2);
        for(int i=t/4;i<halfW1.size();i++)
            halfW1[i] = 0;
        
        if(m&1) {
            halfW1 = computeCmt(halfW1,m/2, t/2);
            halfW1.resize(t);
            halfW1 = combineCoinSets(halfW1,halfW1);
            return combineCoinSets(w1,halfW1);
        }

        halfW1 = computeCmt(halfW1,m/2-1, t/2);
        halfW1.resize(t);

        halfW1 = combineCoinSets(halfW1,w1);
        return combineCoinSets(halfW1,halfW1);
    }

    std::vector<int> getAllChangesUsingAtMost(int m, const std::vector<int>& coins, int t) {
        int p2 = 1;
        while(p2 <= 2*t)
            p2 <<=1;

        std::vector<int> w(p2);
        w[0] = 1;
        for(auto &c : coins) {
            if(c > t)
                break;
            w[c] = 1;
        }
       
        return computeCmt(w,m,p2);
    }

    bool canChangeUsingAtMost(int m, const std::vector<int>& coins, int t) {
        return getAllChangesUsingAtMost(m,coins,t)[t];
    }

    int getMinimumCoinNumberFor(const std::vector<int>& coins, int t) {
        int l=0, r=t, m, res=-1;
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