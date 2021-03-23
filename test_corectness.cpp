#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <cassert>
#include <functional>
#include <chrono>

#include "src/classic.h"
#include "src/solution1.h"
#include "src/solution2.h"
#include "src/solution3.h"
#include "src/solution4.h"
#include "src/solution_for_small_u.h"
#include "test_generators.h"

using namespace std;

void handTests(function<int(vector<int>,int)> f) {
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




int main(int argc, char** argv) {
    int iters = (argc >= 2 ? atoi(argv[1]) : 1024);
    handTests(smallU::getMinimumCoinNumberFor<int>);
    handTests(solution1::getMinimumCoinNumberFor<int>);
    handTests(solution2::getMinimumCoinNumberFor<int>);
    handTests(solution3::getMinimumCoinNumberFor<int>);
    handTests(solution4::getMinimumCoinNumberFor<int>);

    for(int k=0;k<iters;k++) {
        if((k&(-k)) == k)
            cerr<<"iteration "<<k<<endl;
        pair<vector<int>,int> testcase;
        int n = testGenerators::randInt(1,20);
        int t = testGenerators::randInt(n+1,100); 
        if(k%3 == 0) 
            testcase = testGenerators::randomTest(n, 1, t-1, t, t);
        else if(k%3 == 1){
            t = testGenerators::randInt(n*n,800); 
            testcase = testGenerators::smallModulos(n,t, sqrt(t), testGenerators::randInt(1,3,-1));
        }
        else {
            int restsNumber = testGenerators::randInt(1,3);
            int mod = testGenerators::randInt(16,30);
            t = testGenerators::randInt((n+1)*(mod+1),4*(n+1)*(mod+1)); 
            testcase = testGenerators::hardModularRests(restsNumber,mod,n,t);
        }
        int x0 = smallU::getAllChangesUpTo(testcase.first, testcase.second)[testcase.second];
        int x1 = solution1::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x2 = solution2::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x3 = solution3::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x4 = solution4::getMinimumCoinNumberFor(testcase.first, testcase.second);
        assert(x0 == x1 && x1 == x2 && x2 == x3 && x3 == x4);
    }
    return 0;
}