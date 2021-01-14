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
    assert(f({1,3},4) == 2);
    assert(f({2,4,6},11) == -1);
    assert(f({1,2,3,4},8) == 2);
    assert(f({1,2,3,4},16) == 4);
    assert(f({1,2,4,8,16,32,64,128},255) == 8);
    assert(f({1},1543) == 1543);
    assert(f({2},1024) == 512);
}

int main() {
    testOnEdgeCases(solution1::getMinimumCoinNumberFor<int>);
    testOnEdgeCases(solution2::getMinimumCoinNumberFor<int>);
    testOnEdgeCases(solution3::getMinimumCoinNumberFor<int>);

    int t = 80;
    for(int k=0;k<1000;k++) {
        auto coins = randomTest(5,1,t/4);
        int x1 = classic::getAllChangesUpTo(coins,t)[t];
        int x2 = solution1::getMinimumCoinNumberFor(coins,t);
        int x3 = solution2::getMinimumCoinNumberFor(coins,t);
        int x4 = solution3::getMinimumCoinNumberFor(coins,t);
        assert(x1 == x2 && x2 == x3 && x3 == x4);
    }
}