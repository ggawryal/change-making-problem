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
#include "src/solution_small_u_single_target.h"
#include "src/test_generators.h"

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

    assert(f({1},255) == 255);
    assert(f({1},256) == 256);
    assert(f({1},257) == 257);
    assert(f({1},1543) == 1543);
    
    assert(f({2},1024) == 512);
    assert(f({2, 31, 33, 39}, 46) == 23);
    assert(f({10, 116, 469, 474, 613},1050) == 48);
}


int main(int argc, char** argv) {
    cout<<"Testing correctness of all solutions"<<endl;
    int iters = (argc >= 2 ? atoi(argv[1]) : 2050);
    cout<<"Testing on some custom examples: "<<flush;
    handTests(classic::getMinimumCoinNumberFor);
    handTests(solution1::getMinimumCoinNumberFor);
    handTests(solution2::getMinimumCoinNumberFor);
    handTests(solution3::getMinimumCoinNumberFor);
    handTests(solution4::getMinimumCoinNumberFor);
    handTests(smallU::getMinimumCoinNumberFor);
    handTests(smallUSingleTarget::getMinimumCoinNumberFor);
    cout<<"passed\n";
    cout<<"Testing on randomly generated testcases:"<<endl;
    for(int k=1;k<iters;k++) {
        pair<vector<int>,int> testcase;
        int n = testGenerators::randInt(1,20);
        int t;
        switch(k%4) {
            case 0:
                t = testGenerators::randInt(n+1,(testGenerators::randInt(0,1) ? 100 : 2000)); 
                testcase = testGenerators::randomTest(n, 1, t-1, t, t);
                break;
            case 1:
                t = testGenerators::randInt(4*(n+1)+30,4000); 
                testcase = testGenerators::randomTest(n, testGenerators::randInt(1,20), t, t, t, -2);
                break; 
            case 2:
                t = testGenerators::randInt(n*n,800); 
                testcase = testGenerators::smallRestsModulo(n,t,t-1, sqrt(t), testGenerators::randInt(1,3,-1));
                break;
            default:
                int restsNumber = testGenerators::randInt(1,3);
                int mod = testGenerators::randInt(16,30);
                t = testGenerators::randInt((n+1)*(mod+1),4*(n+1)*(mod+1)); 
                testcase = testGenerators::difficultRestsModulo(restsNumber,mod,n,t,t-1);
                break;
        }       
        int x0 = classic::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x1 = solution1::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x2 = solution2::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x3 = solution3::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x4 = solution4::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x5 = smallU::getMinimumCoinNumberFor(testcase.first, testcase.second);
        int x6 = smallUSingleTarget::getMinimumCoinNumberFor(testcase.first, testcase.second);
        assert(x0 == x1 && x1 == x2 && x2 == x3 && x3 == x4 && x4 == x5 && x5 == x6);
        if((k&(-k)) == k)
            cerr<<k<<" testcases passed"<<endl;
    }
    cerr<<"all tests passed"<<endl;
    return 0;
}
