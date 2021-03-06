#ifndef TEST_GENERATORS_COIN_CHANGE
#define TEST_GENERATORS_COIN_CHANGE
#include <vector>
#include <set>
#include <map>
#include <random>
#include <queue>

namespace testGenerators {
    int RNG_SEED = 5343634;
    std::mt19937 rng(RNG_SEED);
    int randInt(int min, int max, int w  = 0) {
        if(w == 0)
            return std::uniform_int_distribution<int>(min, max)(rng);
        else if(w > 0)
            return std::max(std::uniform_int_distribution<int>(min, max)(rng), randInt(min,max,w-1));
        else 
            return std::min(std::uniform_int_distribution<int>(min, max)(rng), randInt(min,max,w+1));
    }

    void shuffle(std::vector<int>& v) {
        std::shuffle(v.begin(), v.end(),rng);
    }

    std::pair<std::vector<int>, int> randomTest(int coins, int min, int max, int minT, int maxT, int w=0) {
        assert(max-min+1 >= coins);
        std::set<int> s;
        while((int)s.size() < coins)
            s.insert(randInt(min,max,w));

        return {std::vector<int>(s.begin(),s.end()), randInt(minT, maxT)};
    }

    std::pair<std::vector<int>,int> smallRestsModulo(int n, int t, int u, int modulo, int biggestRest = -1) {
        if(biggestRest == -1)
            biggestRest = std::max(1,(int)sqrt(modulo));
        std::vector<int> availableCoins;
        for(int r=0;r<modulo;r++) {
            for(int i=0;i<=u;i+=modulo)
                availableCoins.push_back(i+r);
            if(r >= biggestRest && availableCoins.size() >= n)
                break;
        }
        availableCoins.erase(availableCoins.begin()); // remove coin 0
        assert(availableCoins.size() >= n);
        shuffle(availableCoins);
        availableCoins.resize(n);
        std::sort(availableCoins.begin(),availableCoins.end());
        return {availableCoins,t};
    }

    std::pair<std::vector<int>,int> difficultRestsModulo(int m, int M, int n, int minT, int u) {    
        assert(M > 1);
        m = std::min(m,M-1);  
        auto modularCoins = [&](std::vector<int> coins, int t) {
            const int n = coins.size();
            std::vector<int> dp(t,t+1);
            dp[0] = 0;
            for(int j=0; j<n;j++) {
                for(bool upgr = true; upgr; ) {
                    upgr = false;
                    for(int i=0;i<t;i++) {
                        if(dp[(i+coins[j])%(t)] > dp[i]+1) {
                            upgr = true;
                            dp[(i+coins[j])%(t)] = dp[i]+1;
                        }
                    }
                }
            }
            for(auto&a : dp)
                if(a > t)
                    a = -1;
            return dp;
        };

        std::pair<int,std::vector<int> > ans;
        for(int t=0;t<100; t++) {
            auto rs = randomTest(m,1,M-1,M-1,M-1,randInt(0,2)-1).first;
            auto rs2 = modularCoins(rs,M);
            int w = *std::max_element(rs2.begin(),rs2.end());
            ans = std::max(ans,{w,rs});
        }
        auto rests = ans.second;
        std::pair<int,int> hardRest = {0,0};
        int i = 0;
        for(auto a:modularCoins(rests,M)) {
            hardRest = std::max(hardRest,{a,i});
            i++;
        }

        int t = minT + (M-minT%M) + hardRest.second;
        std::vector<int> availableCoins;
        for(int i=0;i<=u;i+=M)
            for(auto j : rests)
                if(i+j <= u)
                    availableCoins.push_back(i+j);
        if(std::find(availableCoins.begin(),availableCoins.end(),0) != availableCoins.end())
            availableCoins.erase(std::find(availableCoins.begin(),availableCoins.end(),0));
        if(availableCoins.size() < n) {
            assert(m < M);
            return difficultRestsModulo(std::min(M,2*m),M,n,minT,u);
        }
        shuffle(availableCoins);
        availableCoins.resize(n);
        std::sort(availableCoins.begin(),availableCoins.end());
        return {availableCoins,t};
    }
}



namespace testStatisticsMeasurement {
    template<class Num=int>
    std::pair<std::vector<Num>, std::vector<Num> > calcDP(std::vector<Num> coins, Num t) {
        const int n = coins.size();
        std::vector<Num> dp(t+1,t+1);
        std::vector<Num> comeFrom(t+1,t+1);
        dp[0] = 0;
        for(int i=0;i<=t;i++) {
            if(dp[i] > t) {
                dp[i] = -1;
                continue;
            }
            for(int j=0; j<n && i + coins[j] <= t;j++)
                if(dp[i+coins[j]] > dp[i]+1) {    
                    dp[i+coins[j]] = std::min(dp[i+coins[j]],dp[i]+1);
                    comeFrom[i+coins[j]] = i;
                }
        }
        return {dp,comeFrom};
    }

    std::vector<int> getCoins(int x, const std::vector<int>& comeFrom) {
        std::vector<int> used;
        while(x > 0){
            used.push_back(x - comeFrom[x]);
            x = comeFrom[x];
        }
        return used;
    }
    std::vector<std::vector<int> > getAllPossibleCoinSetsForTarget(std::vector<int> coins, int t, int limit) {
        auto dp = calcDP(coins,t).first;
        if(dp[t] == -1)
            return {};
        std::queue<std::pair<std::pair<int,int>, std::vector<int> > > q; //{t,c},S:  rest t; don't use coins >= coins[c], coin set S
        q.push({{t,coins.size()},{}});
        std::vector<std::vector<int> > ans;
        while(!q.empty()) {
            int r = q.front().first.first, c = q.front().first.second;
            std::vector<int> S = q.front().second;
            q.pop();
            if(r == 0) {
                ans.push_back(S);
                continue;
            }
            for(int i=c-1;i>=0;i--) {
                int c = coins[i];
                if(r-c >= 0 && dp[r-c]+1 == dp[r]) {
                    S.push_back(c);
                    if(q.size() + ans.size() <= limit)
                        q.push({{r-c,i+1},S});
                    S.pop_back();
                }
            }
        }
        return ans;
    }


    template<class Num=int>
    std::vector<int> getResultsChangesAfterRemovingRandomCoins(std::vector<int> coins, int t, double fractionOfCoinsToRemove) { //doesn't remove biggest coin
        auto dp1 = calcDP(coins,t).first;
        std::vector<int> coinAfterRemoval(coins.begin(), coins.end()-1);

        testGenerators::shuffle(coinAfterRemoval);
        for(int i=0;i<(coins.size()-1)*fractionOfCoinsToRemove; i++)
            coinAfterRemoval.pop_back();

        coinAfterRemoval.push_back(coins.back());
        std::sort(coinAfterRemoval.begin(), coinAfterRemoval.end());
        auto dp2 = calcDP(coinAfterRemoval,t).first;

        std::vector<int> res;
        for(int i=0;i<=t;i++)
            if(dp1[i] != dp2[i])
                res.push_back(i);
        return res;
    } 

    template<class Num=int>
    std::vector<int> restsForWhichGreedyWorks(std::vector<Num> coins, Num t) {
        auto dp = calcDP(coins,t).first;
        std::vector<int> greedyRes(t+1);
        std::vector<int> res;
        for(int i=0;i<=t;i++) {
            int t2 = i;
            for(int j=coins.size()-1;j>=0;j--) {
                if(coins[j] <= t2) {
                    greedyRes[i] += t2 / coins[j];
                    t2 %= coins[j];
                }
            }
            if(t2 != 0)
                greedyRes[i] = -1;
            if(greedyRes[i] == dp[i])
                res.push_back(i);
        }
        return res;
    }
};

#endif