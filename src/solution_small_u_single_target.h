#ifndef COIN_CHANGE_SMALLU_SINGLE
#define COIN_CHANGE_SMALLU_SINGLE
#include <vector>
#include "convolution.h"

//(6) from More on Change-Making and Related Problems
//O(u log u log^2 t) or O(u log^3 u)
//coins should be in sorted order
namespace smallUSingleTarget {
    //w1 has size u+1
     std::vector<int> computeLastFewValuesOfCmt(const std::vector<int>& w1, int m, int t, int u) { // returns C_m^t[t - 4u, ..., t]
        if(u >= t) {
            int p2 = 1;
            while(p2 <= 2*u)
                p2 <<=1;
            auto w1Cp = w1;
            w1Cp.resize(p2);
            auto tmp = solution3::computeCmt(w1Cp,m,p2);

            std::vector<int> res(t-std::max(0,t-4*u)+1);
            for(int i = std::max(0,t-4*u); i <= std::min(u,t);i++)
                res[i - std::max(0,t-4*u)] = tmp[i];
            return res;
        }
        if(m == 0) {
            std::vector<int> res(t-std::max(0,t-4*u)+1,0);
            if(t - 4*u < 1)
                res[0] = 1;
            return res;
        }
        if(m == 1) {
            std::vector<int> res(t-std::max(0,t-4*u)+1);
           
            for(int i = std::max(0,t-4*u); i <= std::min(u,t);i++)
                res[i - std::max(0,t-4*u)] = w1[i];
            
            return res;
        }
        if(m&1) {
            auto c_half = computeLastFewValuesOfCmt(w1,m/2, t/2,u); //[t/2-4u; t/2]
            //assert(c_half.size() == t/2-max(t/2-4*u,0)+1);
            c_half = unallignedCombineCoinSets(c_half,c_half,-1); //[2t/2-8u;2t/2]
            if(t%2 == 1)
                c_half.push_back(0);
            if(t-4*u > 0)
                c_half.assign(c_half.end()-4*u-1, c_half.end());
            
            //assert(c_half.size() == t - max(t-4*u,0)+1);
            c_half = unallignedCombineCoinSets(c_half,w1,-1);
            return std::vector<int>(c_half.begin(), c_half.end() - u);
        }

        auto c_half = computeLastFewValuesOfCmt(w1,m/2-1, t/2,u);
        //assert(c_half.size() == t/2-max(t/2-4*u,0)+1);
        c_half = unallignedCombineCoinSets(c_half,w1,-1); //[t/2-4u; t/2+u]
        //assert(c_half.size() == t/2+u-max(0,t/2-4*u)+1);
        c_half = unallignedCombineCoinSets(c_half,c_half,-1); //[2t/2-8u; 2t/2+2u]
        if(t%2 == 1)
            c_half.push_back(0);
        return std::vector<int>(c_half.end() - std::min((int)c_half.size(),6*u+1), c_half.end()-2*u);
    }

    std::vector<int> getLastFewChanges(int m, const std::vector<int>& coins, int t) {
        std::vector<int> w(coins.back()+1);
        w[0] = 1;
        for(auto &c : coins)
            w[c] = 1;
       
        return computeLastFewValuesOfCmt(w,m,t,coins.back());
    }

    bool canChangeUsingAtMost(int m, const std::vector<int>& coins, int t) {
        return getLastFewChanges(m,coins,t).back();
    }

    int getMinimumCoinNumberFor(std::vector<int> coins, int t) {
        while(coins.back() > t)
            coins.pop_back();

        int additionalMaxCoins = 0;
        if(t > coins.back()*(long long)coins.back()) {
            additionalMaxCoins = (t - coins.back()*(long long)coins.back())/coins.back();
            t -= additionalMaxCoins*coins.back();
        }

        int l=0, r=t, m, res=-1;
        while(l <= r) {
            m = (l+r)/2;
            if(canChangeUsingAtMost(m,coins,t))
                r = m-1, res = m;
            else
                l = m+1;
        }
        if(res == -1)
            return -1;
        return res + additionalMaxCoins;
    }
};

#endif