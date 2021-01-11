#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <cassert>
#include "src/classic.h"
#include "src/solution1.h"
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

int main() {
    int t = 80;
    for(int k=0;k<1000;k++) {
        auto coins = randomTest(5,1,t/4);
        int x1 = classic::getAllChangesUpTo(coins,t)[t];
        int x2 = solution1::getMinimumCoinNumberFor(coins,t);
        assert(x1 == x2);
    }
}