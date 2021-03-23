#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <cassert>
#include <functional>
#include <chrono>
#include <algorithm>

#include "src/classic.h"
#include "src/solution1.h"
#include "src/solution2.h"
#include "src/solution3.h"
#include "src/solution4.h"
#include "src/solution_for_small_u.h"

#include "test_generators.h"
using namespace std;


int main(int argc, char** argv) {
    const int runs = argc >= 2 ? atoi(argv[1]) : 1;
    
    for(int t = 1e6; t<=1e7; t += 1e6) {
        long double avgTime = 0;
        long double stddev = 0;
        
        //auto test = testGenerators::randomTest(10000, 1, t/10, t, t);
        auto test = testGenerators::smallModulos(10000, t-1, 100, 10);
        
        for(int i=0; i<runs; i++) {
            auto start = chrono::high_resolution_clock::now();

            solution4::getMinimumCoinNumberFor(test.first,test.second);

            auto delta = chrono::high_resolution_clock::now() - start;
            long double ms = std::chrono::duration_cast<std::chrono::microseconds>(delta).count()/1e6;
            cerr<<"time = "<<ms<<endl;
            avgTime += ms/runs;
            stddev += ms*ms/runs;
        }
        cout.precision(3);
        cout<<"average time = "<<fixed<<avgTime<<"s, stddev = "<<fixed<<sqrt(stddev - avgTime*avgTime)<<"\n";
    }
}