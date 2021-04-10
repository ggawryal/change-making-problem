#ifndef COIN_CHANGE_SOL2
#define COIN_CHANGE_SOL2
#include <vector>
#include "convolution.h"

//solution 2 from On the Change-Making Problem paper by Timothy M. Chan, Qizheng He
//O(n + t log^2t)
namespace solution2 {
    template<class Num=int>
    Num getMinimumCoinNumberFor(std::vector<Num> coins, Num t) {
        int p2 = 1;
        while(p2 <= 2*t)
            p2 <<=1;

        std::vector<std::vector<Num>> w(1,std::vector<Num>(p2));
        w[0][0] = 1;
        for(auto &c : coins)
            w[0][c] = 1;


        while((1<<(Num)w.size()) < p2 && w.back()[t] == 0)
            w.push_back(combineCoinSetsWhereAEqualsB(w.back()));

        if(w.back()[t] == 0)
            return -1;

        std::vector<Num> currentCoinSet(p2);
        currentCoinSet[0] = 1;
        int m = 0;
        for(int i=w.size()-1;i>=0;i--) {
            auto newCoinSet = combineCoinSets(currentCoinSet,w[i]);
            if(newCoinSet[t] == 0) {
                currentCoinSet = newCoinSet;
                m += (1<<i);
            }
        }
        return m+1;
    }

};

#endif