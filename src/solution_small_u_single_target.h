#ifndef COIN_CHANGE_SMALLU_SINGLE
#define COIN_CHANGE_SMALLU_SINGLE
#include <vector>
#include "convolution.h"
using namespace std;
//(6) from More on Change-Making and Related Problems
//O(u log u log t) or O(u log^3 u)
//coins should be in sorted order
namespace smallUSingleTarget {
    template<class Num=int>
    //w1 has size u+1
     std::vector<Num> computeLastFewValuesOfCmt(std::vector<int> w1, int m, int t, int u) { // returns C_m^t[t - 4u, ..., t]
        if(m == 0) {
            vector<int> res(t-max(0,t-4*u)+1,0);
            if(t - 4*u < 1)
                res[0] = 1;
            return res;
        }
        if(m == 1) {
            vector<int> res(t-max(0,t-4*u)+1);
           
            for(int i = max(0,t-4*u); i<=min(u,t);i++)
                res[i - max(0,t-4*u)] = w1[i];
            
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
            return vector<int>(c_half.begin(), c_half.end() - u);
        }

        auto c_half = computeLastFewValuesOfCmt(w1,m/2-1, t/2,u);
        //assert(c_half.size() == t/2-max(t/2-4*u,0)+1);
        c_half = unallignedCombineCoinSets(c_half,w1,-1); //[t/2-4u; t/2+u]
        //assert(c_half.size() == t/2+u-max(0,t/2-4*u)+1);
        c_half = unallignedCombineCoinSets(c_half,c_half,-1); //[2t/2-8u; 2t/2+2u]
        if(t%2 == 1)
            c_half.push_back(0);
        return vector<int>(c_half.end()-min((int)c_half.size(),6*u+1), c_half.end()-2*u);
    }

    template<class Num=int>
    std::vector<int> getLastFewChanges(Num m, const std::vector<Num>& coins, Num t) {
        std::vector<int> w(coins.back()+1);
        w[0] = 1;
        for(auto &c : coins)
            w[c] = 1;
       
        return computeCmt(w,m,t,coins.back());
    }

    template<class Num=int>
    bool canChangeUsingAtMost(Num m, const std::vector<Num>& coins, Num t) {
        return getLastFewChanges(m,coins,t).back();
    }

    template<class Num=int>
    Num getMinimumCoinNumberFor(std::vector<Num> coins, Num t) {
        while(coins.back() > t)
            coins.pop_back();
        int additionalMaxCoins = 0;
        while(t > coins.back()*(long long)coins.back()) {
            t -= coins.back();
            additionalMaxCoins++;
        }
        Num l=0, r=t, m, res=-1;
        while(l <= r) {
            m = (l+r)/2;
            if(canChangeUsingAtMost(m,coins,t))
                r = m-1, res = m;
            else
                l = m+1;
        }
        return res + additionalMaxCoins;
    }
};

#endif