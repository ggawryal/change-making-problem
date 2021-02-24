#ifndef COIN_CHANGE_SMALL_U
#define COIN_CHANGE_SMALL_U
#include <vector>
#include <algorithm>

namespace smallU {
    //O(t + u^2 log u) solution
    //Algorithm 1 (All-targets change-making) from More on Change-Making and Related Problems paper by Timothy M. Chan and Qizheng He
    template<class Num=int>
    std::vector<Num> getAllChangesUpTo(std::vector<Num> coins, Num t) {
        const int n = coins.size();
        const int u = coins.back();
        //dp[x] means minimum number of coins needed to change x or -1, if impossible
        std::vector<Num> dp(t+1,t+1);
        dp[0] = 0;
        for(int i=1;i<=t;i++) {
            for(int j=n-1; j >= max(0LL,n-1-u*(long long)u/i); j--) {
                if(coins[j] <= i)
                    dp[i] = std::min(dp[i],dp[i-coins[j]]+1);    
            }
        }
        std::replace_if(dp.begin(), dp.end(), [&](int a) {return a > t;}, -1);
        return dp;
    }
    template<class Num=int>
    int getMinimumCoinNumberFor(std::vector<Num> coins, Num t) {
        return getAllChangesUpTo(coins, t)[t];
    }
};

#endif