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
    double res = 0;
    auto start = chrono::high_resolution_clock::now();
    solution(test.first,test.second);
    auto delta = chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
}


template<class T>
void save_to_csv(const string& name, const vector<T>& xPoints, const vector<pair<string,coinFunc> >& solutions, const vector<vector<double> >& times, const string& saveDirectory) {
    ofstream out;
    out.open(saveDirectory + name + ".csv");
    if(out.bad()) {
        cerr<<"file opening failed"<<endl;
        exit(1);
    }
    out<<"name";
    out.precision(3);
    for(auto x :xPoints) 
        out<<", "<<x;
    out<<endl;

    for(int i=0;i<(int)solutions.size();i++) {
        out<<solutions[i].first;
        for(auto milllis : times[i]) {
            out<<", "<<fixed<<milllis/1000.0<<flush;
        }
        out<<endl;
    }
                
}


void tAndTime(int runs, string saveDirectory) {
    cerr<<"preparing data for t, time graph"<<endl;
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"small u", smallU::getMinimumCoinNumberFor},
        {"small u single target", smallUSingleTarget::getMinimumCoinNumberFor},
        {"solution 1", solution1::getMinimumCoinNumberFor},
        {"solution 2", solution2::getMinimumCoinNumberFor},
        {"solution 3", solution3::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor},
    };
    const vector<int> ts = {50000, 100000, 200000, 500000, 1000000};

    vector<vector<double> > times(solutions.size(), vector<double>(ts.size()));
    for(int run=0;run < runs; run++) {
        vector<Test> tests;
        int test_index = 0;
        for(auto t : ts) {
            auto test = run % 2 ? testGenerators::randomTest(4000,1,t/4,t,t) : testGenerators::smallModulos(min(t*2/100, 4000),t-1,100,3);
            int solution_index = 0;
            for(auto nameAndSolution : solutions) {
                times[solution_index][test_index] += measure(nameAndSolution.second,test)/runs;
                solution_index++;
            }
            test_index++;
        }
    }
    save_to_csv("t_time",ts,solutions,times, saveDirectory);
}

void smallUvsClassic(int runs, string saveDirectory) {
    cerr<<"preparing data for classic vs all targets u graph"<<endl;
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"small u", smallU::getMinimumCoinNumberFor}
    };
    const int n = 200, t = 4000000;
    const vector<int> us = {300,500,1000,2000,5000,10000, 15000, 18000, 20000, 25000, 30000, 40000, 50000, 100000};

    vector<vector<double> > times(solutions.size(), vector<double>(us.size()));
    for(int run=0;run < runs; run++) {
        vector<Test> tests;
        int test_index = 0;
        for(auto u : us) {
            auto test = testGenerators::randomTest(n,1,u,t,t);
            int solution_index = 0;
            for(auto nameAndSolution : solutions) {
                times[solution_index][test_index] += measure(nameAndSolution.second,test)/runs;
                solution_index++;
            }
            test_index++;
        }
    }
    save_to_csv("smallu_classic",us,solutions,times, saveDirectory);
}

void smallUSingleTargetVsSol(int runs, string saveDirectory) {
    const vector<pair<string,coinFunc> > solutions = {
        {"small u single target", smallUSingleTarget::getMinimumCoinNumberFor},
        {"solution 3", solution3::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor}
    };
    const int n = 200, t = 1000000;
    const vector<int> us = {t/128,t/64,t/32,t/16,t/8,t/4,t/2};
    vector<float> usFractions;
    for(auto u : us)
        usFractions.push_back(u/(float)t);

    vector<vector<double> > times(solutions.size(), vector<double>(us.size()));
    for(int run=0;run < runs; run++) {
        vector<Test> tests;
        int test_index = 0;
        for(auto u : us) {
            auto test = testGenerators::randomTest(n,1,u,t,t);
            int solution_index = 0;
            for(auto nameAndSolution : solutions) {
                times[solution_index][test_index] += measure(nameAndSolution.second,test)/runs;
                solution_index++;
            }
            test_index++;
        }
    }
    save_to_csv("smallusingletarget_sol",usFractions,solutions,times, saveDirectory);
}

void nAndTime(int runs, string saveDirectory) {
    cerr<<"preparing data for n, time graph"<<endl;
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor}
    };
    vector<int> ns = {200,300,400,500,600,700,800,1000};
    const int t = 500000;
    for(int i=1200;i<=4000; i += 400)
        ns.push_back(i);

    vector<vector<double> > times(solutions.size(), vector<double>(ns.size()));
    for(int run=0;run < runs; run++) {
        vector<Test> tests;
        int test_index = 0;
        for(auto n : ns) {
            auto test = run % 2 ? testGenerators::randomTest(n,1,t,t,t) : testGenerators::smallModulos(n,t-1,100,5);
            int solution_index = 0;
            for(auto nameAndSolution : solutions) {
                times[solution_index][test_index] += measure(nameAndSolution.second,test)/runs;
                solution_index++;
            }
            test_index++;
        }
    }
    save_to_csv("n_time",ns,solutions,times, saveDirectory);
}

int main(int argc, char** argv) {
    int runs = 1;
    if(argc >= 2)
        runs = atoi(argv[1]);
    string saveDirectory = "data/";
    if(argc >= 3)
        saveDirectory = string(argv[2]);
    tAndTime(runs,saveDirectory);
    smallUvsClassic(runs,saveDirectory);
    smallUSingleTargetVsSol(runs,saveDirectory);
    nAndTime(runs,saveDirectory);
}