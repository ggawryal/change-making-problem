#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <cassert>
#include <functional>

#include "src/classic.h"
#include "src/solution1.h"
#include "src/solution2.h"
#include "src/solution3.h"
#include "src/solution4.h"
#include "src/solution_for_small_u.h"
using namespace std;

const int RNG_SEED = 5343634;
int randInt(int min, int max) {
    static mt19937 rng(RNG_SEED);
    return uniform_int_distribution<int>(min, max)(rng);
}

vector<int> randomTest(int coins, int min, int max) {
    assert(max-min+1 >= coins);
    set<int> s;
    while((int)s.size() < coins)
        s.insert(randInt(min,max));

    return vector<int>(s.begin(),s.end());
}

void testOnEdgeCases(function<int(vector<int>,int)> f) {
    assert(f({1},1) == 1);
    assert(f({1,2},2) == 1);
    assert(f({1,2},7) == 4);
    assert(f({1,3},4) == 2);
    assert(f({2,4,6},11) == -1);
    assert(f({1,2,3,4},8) == 2);
    assert(f({1,2,3,4},16) == 4);
    assert(f({1,2,4,8,16,32,64,128},255) == 8);
    assert(f({1},1543) == 1543);
    assert(f({2},1024) == 512);

    assert(f({2, 31, 33, 39}, 46) == 23);
    assert(f({10, 116, 469, 474, 613},1050) == 48);
}

void testCorectness() {
    testOnEdgeCases(smallU::getMinimumCoinNumberFor<int>);
    testOnEdgeCases(solution1::getMinimumCoinNumberFor<int>);
    testOnEdgeCases(solution2::getMinimumCoinNumberFor<int>);
    testOnEdgeCases(solution3::getMinimumCoinNumberFor<int>);
    testOnEdgeCases(solution4::getMinimumCoinNumberFor<int>);

    for(int k=0;k<1000;k++) {
        int t = randInt(10,50);
        int c = randInt(1,t/randInt(2,10));
        auto coins = randomTest(c,1,randInt(c,t));
        int x0 = smallU::getAllChangesUpTo(coins,t)[t];
        
        int x1 = solution1::getMinimumCoinNumberFor(coins,t);
        int x2 = solution2::getMinimumCoinNumberFor(coins,t);
        int x3 = solution3::getMinimumCoinNumberFor(coins,t);
        int x4 = solution4::getMinimumCoinNumberFor(coins,t);
    
        assert(x0 == x1 && x1 == x2 && x2 == x3 && x3 == x4);
    }
}

int main() {
    //testCorectness();
    ios::sync_with_stdio(false);
    int n,t;
    cin >> n >> t;
    vector<int> coins(n);
    for(int i=0;i<n;i++)
        cin >> coins[i];
    cout<<solution4::getMinimumCoinNumberFor<int>(coins,t)<<endl;
    return 0;
   
}