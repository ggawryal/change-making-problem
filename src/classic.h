#ifndef COIN_CHANGE_CLASSIC
#define COIN_CHANGE_CLASSIC
#include <vector>
#include <algorithm>

namespace classic {
    //classic O(nt) dynamic programming solution
    std::vector<int> getAllChangesUpTo(std::vector<int> coins, int t) {
        const int n = coins.size();

        //dp[x] means minimum number of coins needed to change x or -1, if impossible
        std::vector<int> dp(t+1,t+1);
        dp[0] = 0;
        int coinIdx = 0;
        for(int i=0;i<=t;i++) {
            if(coinIdx < n && coins[coinIdx] == i)
                coinIdx++;
            for(int j=coinIdx-1; j>=0; j--)
                dp[i] = std::min(dp[i], dp[i-coins[j]]+1);    
        }
        std::replace_if(dp.begin(), dp.end(), [&](int a) {return a > t;}, -1);
        return dp;
    }

    int getMinimumCoinNumberFor(std::vector<int> coins, int t) {
        return getAllChangesUpTo(coins, t)[t];
    }
};

#endif