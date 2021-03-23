#ifndef TEST_GENERATORS_COIN_CHANGE
#define TEST_GENERATORS_COIN_CHANGE
#include <vector>
#include <set>
#include <random>

namespace testStatisticsMeasurement {
    template<class Num=int>
    std::vector<int> countHowManyDifferentCoinsAreUsedForEach(std::vector<Num> coins, Num t) {
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
        std::vector<int> res(t+1,-1);
        for(int y=0;y<=t;y++) {
            int x = y;
            if(dp[x] == -1)
                continue;

            std::vector<int> used;
            while(x > 0){
                used.push_back(x - comeFrom[x]);
                x = comeFrom[x];
            }
            std::sort(used.begin(),used.end());
            used.erase(std::unique(used.begin(),used.end()),used.end());
            res[y] = used.size();
        }
        return res;
    } 
};

namespace testGenerators {
    int RNG_SEED = 5343634;
    int randInt(int min, int max, int w  = 0) {
        static mt19937 rng(RNG_SEED);
        if(w == 0)
            return uniform_int_distribution<int>(min, max)(rng);
        else if(w > 0)
            return std::max(uniform_int_distribution<int>(min, max)(rng), randInt(min,max,w-1));
        else 
            return std::min(uniform_int_distribution<int>(min, max)(rng), randInt(min,max,w+1));
    }

    pair<vector<int>, int> randomTest(int coins, int min, int max, int minT, int maxT, int w=0) {
        assert(max-min+1 >= coins);
        set<int> s;
        while((int)s.size() < coins)
            s.insert(randInt(min,max,w));

        return {vector<int>(s.begin(),s.end()), randInt(minT, maxT)};
    }

    pair<vector<int>,int> smallModulos(int n, int t, int modulo, int biggestRest=-1) {
        if(biggestRest == -1)
            biggestRest = max(1,(int)sqrt(modulo));
        set<int> coins;
        while(coins.size() < n) {
            int r = randInt(0,biggestRest,-2);
            int k = randInt(0,t/modulo-1,0);
            int coin = k*modulo+r;
            if(coin != 0 && coin < t)
                coins.insert(coin);
        }
        return {vector<int>(coins.begin(),coins.end()),t};
    }

    pair<vector<int>,int> hardModularRests(int m, int M, int n, int minT = 1e6) {        
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

        pair<int,vector<int> > ans;
        for(int t=0;t<100; t++) {
            auto rs = randomTest(m,1,M-1,M-1,M-1,randInt(0,2)-1).first;
            auto rs2 = modularCoins(rs,M);
            int w = *max_element(rs2.begin(),rs2.end());
            ans = max(ans,{w,rs});
        }
        auto rests = ans.second;
        pair<int,int> hardRest = {0,0};
        int i = 0;
        for(auto a:modularCoins(rests,M)) {
            hardRest = max(hardRest,{a,i});
            i++;
        }
        //cerr<<"\nhardness = "<<hardRest.second<<" "<<hardRest.first<<endl;

        int t = minT - minT%M + hardRest.second;
        set<int> coins;
        while(coins.size() < n) {
            int r = randInt(0,m,-2);
            int k = randInt(0,t/M,0);

            int coin = k*M+rests[r];
            if(coin < t && coin > 0)
                coins.insert(coin);
        }
        return {vector<int>(coins.begin(),coins.end()),t};
    }
}

#endif