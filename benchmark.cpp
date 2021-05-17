#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <cassert>
#include <functional>
#include <chrono>
#include <algorithm>
#include <fstream>

#include "src/classic.h"
#include "src/solution1.h"
#include "src/solution2.h"
#include "src/solution3.h"
#include "src/solution4.h"
#include "src/solution_for_small_u.h"
#include "src/solution_small_u_single_target.h"
#include "test_generators.h"
using namespace std;

typedef function<int(vector<int>, int)>  coinFunc;
typedef pair<vector<int>,int> Test;


double measure(coinFunc solution, Test test) {
    auto start = chrono::high_resolution_clock::now();
    solution(test.first,test.second);
    auto delta = chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(delta).count()/1e3;
}

void tAndTime() {
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"small u", smallU::getMinimumCoinNumberFor},
        {"small u single target", smallUSingleTarget::getMinimumCoinNumberFor},
        {"solution1", solution1::getMinimumCoinNumberFor},
        {"solution2", solution2::getMinimumCoinNumberFor},
        {"solution3", solution3::getMinimumCoinNumberFor},
        {"solution4", solution4::getMinimumCoinNumberFor},
    };
    const vector<int> ts = {10000, 50000, 100000, 200000, 500000, 1000000, 2000000};
    vector<Test> tests;
    for(auto t : ts) {
        int n = min(t/10, 5000);
        tests.push_back({testGenerators::randomTest(n, 1, t, t, t)});
    }
    ofstream out;
    out.open("data/t_time.csv");
    out<<"name";
    for(auto t :ts)
        out<<", "<<t;
    out<<endl;

    for(auto nameAndSolution : solutions) {
        string name = nameAndSolution.first;
        auto solution = nameAndSolution.second;
        cerr<<"measuring "<<name<<endl;
        out<<name;
        for(auto test : tests) {
            double seconds = measure(solution,test);
            out.precision(3);
            out<<", "<<fixed<<seconds<<flush;
        }
        out<<endl;
    }
} 

void smallUvsClassic() {
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"small u", smallU::getMinimumCoinNumberFor}
    };
    const int n = 200, t = 4000000;
    const vector<int> us = {200,500,1000,2000,5000,10000, 15000, 18000, 20000, 25000, 30000, 40000, 50000, 100000};

    vector<Test> tests;
    for(auto u : us)
        tests.push_back({testGenerators::randomTest(n,1,u,t,t)});

    ofstream out;
    out.open("data/smallu_classic.csv");
    out<<"name";
    for(auto u :us)  {
        out<<", "<<u;
    }
    out<<endl;

    for(auto nameAndSolution : solutions) {
        string name = nameAndSolution.first;
        auto solution = nameAndSolution.second;
        cerr<<"measuring "<<name<<endl;
        out<<name;
        for(auto test : tests) {
            double seconds = measure(solution,test);
            out.precision(3);
            out<<", "<<fixed<<seconds<<flush;
        }
        out<<endl;
    }
}

void smallUSingleTargetVsSol() {
    const vector<pair<string,coinFunc> > solutions = {
        {"small u single target", smallUSingleTarget::getMinimumCoinNumberFor},
        {"solution 3", solution3::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor}
    };
    const int n = 200, t = 1000000;
    const vector<int> us = {t/128,t/64,t/32,t/16,t/8,t/4};

    vector<Test> tests;
    for(auto u : us)
        tests.push_back({testGenerators::randomTest(n,1,u,t,t)});

    ofstream out;
    out.open("data/smallu_single_target_sol.csv");
    out<<"name";
    for(auto u :us)  {
        out.precision(5);
        out<<", "<<fixed<<u*1.0/t;
    }
    out<<endl;

    for(auto nameAndSolution : solutions) {
        string name = nameAndSolution.first;
        auto solution = nameAndSolution.second;
        cerr<<"measuring "<<name<<endl;
        out<<name;
        for(auto test : tests) {
            double seconds = measure(solution,test);
            out.precision(3);
            out<<", "<<fixed<<seconds<<flush;
        }
        out<<endl;
    }
}

int main(int argc, char** argv) {  
    tAndTime();
    smallUvsClassic();
    smallUSingleTargetVsSol();
}