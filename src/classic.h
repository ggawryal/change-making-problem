#ifndef COIN_CHANGE_CLASSIC
#define COIN_CHANGE_CLASSIC
#include <vector>

namespace classic {
    //classic O(nt) dynamic programming solution
    std::vector<int> getAllChangesUpTo(std::vector<int> coins, int t) {
        const int n = coins.size();

        //dp[x] means minimum number of coins needed to change x or -1, if impossible
        std::vector<int> dp(t+1,t+1);
        dp[0] = 0;
        for(int i=0;i<=t;i++) {
            if(dp[i] > t) {
                dp[i] = -1;
                continue;
            }
            for(int j=0; j<n && i + coins[j] <= t;j++)
                dp[i+coins[j]] = std::min(dp[i+coins[j]],dp[i]+1);    
        }
        return dp;
    }

    int getMinimumCoinNumberFor(std::vector<int> coins, int t) {
        return getAllChangesUpTo(coins, t)[t];
    }
};

#endif