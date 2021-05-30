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
    return std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count()/1e6;
}


template<class T>
void save_to_csv(const string& name, const vector<T>& xPoints, const string& xPointsLabel, const vector<pair<string,coinFunc> >& solutions, const vector<vector<double> >& times, const vector<vector<double> >& stddev, const string& saveDirectory) {
    ofstream out;
    out.open(saveDirectory + name + ".csv");
    if(out.bad()) {
        cerr<<"file opening failed"<<endl;
        exit(1);
    }
    out<<xPointsLabel;
    out.precision(5);
    for(auto x :xPoints) 
        out<<", "<<x;
    out<<endl;

    for(int i=0;i<(int)solutions.size();i++) {
        out<<"avg_"<<solutions[i].first;
        for(auto milllis : times[i]) {
            out<<", "<<fixed<<milllis/1000.0<<flush;
        }
        out<<endl;
    }

    for(int i=0;i<(int)solutions.size();i++) {
        out<<"stddev_"<<solutions[i].first;
        for(auto x : stddev[i]) {
            out<<", "<<fixed<<x/1000.0<<flush;
        }
        out<<endl;
    }
                
}

pair<vector<vector<double> >, vector<vector<double> > > measureTimes(vector<pair<string,coinFunc> > solutions, vector<int> xPoints, function<Test(int)> testGenerator, int runs) {
    vector<vector<double> > avgTimes(solutions.size(), vector<double>(xPoints.size()));
    auto stddev = avgTimes;
    for(int run=0;run < runs; run++) {
        vector<Test> tests;
        int test_index = 0;
        for(auto x : xPoints) {
            auto test =  testGenerator(x);
            int solution_index = 0;
            for(auto nameAndSolution : solutions) {
                double millis = measure(nameAndSolution.second,test);
                avgTimes[solution_index][test_index] += millis/runs;
                stddev[solution_index][test_index] += millis*millis/runs;
                solution_index++;
            }
            test_index++;
        }
    }
    for(int i=0;i<stddev.size();i++)
        for(int j=0;j<stddev[i].size();j++)
            stddev[i][j] = sqrt(stddev[i][j] - avgTimes[i][j]*avgTimes[i][j]);
    return {avgTimes, stddev};
}


void tAndTime(int runs, string saveDirectory) {
    cerr<<"preparing data for t, time graph"<<endl;
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"solution 1", solution1::getMinimumCoinNumberFor},
        {"solution 2", solution2::getMinimumCoinNumberFor},
        {"solution 3", solution3::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor},
        {"small u single target", smallUSingleTarget::getMinimumCoinNumberFor},
    };
    const vector<int> ts = {50000, 100000, 200000, 500000, 1000000};
    auto testGen = [&](int t) {return testGenerators::randomTest(4000,1,t/4,t,t);};
    auto avgTimeAndStddev = measureTimes(solutions, ts, testGen, runs);

    save_to_csv("t_time",ts,"reszta do wydania",solutions, avgTimeAndStddev.first, avgTimeAndStddev.second, saveDirectory);
}


void smallUvsClassic(int runs, string saveDirectory) {
    cerr<<"preparing data for classic vs all targets u graph"<<endl;
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"small u", smallU::getMinimumCoinNumberFor}
    };
    const int n = 200, t = 4000000;
    const vector<int> us = {300,500,1000,2000,5000,10000, 15000, 18000, 20000, 25000, 30000, 40000, 50000, 100000};
    auto testGen = [&](int u) {return testGenerators::randomTest(n,1,u,t,t);};
    auto avgTimeAndStddev = measureTimes(solutions, us, testGen, runs);
    save_to_csv("smallu_classic",us,"wielkość największej monety",solutions,avgTimeAndStddev.first, avgTimeAndStddev.second, saveDirectory);
}

void smallUSingleTargetVsSol(int runs, string saveDirectory) {
    const vector<pair<string,coinFunc> > solutions = {
        {"small u single target", smallUSingleTarget::getMinimumCoinNumberFor},
        {"solution 2", solution2::getMinimumCoinNumberFor},
        {"solution 3", solution3::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor}
    };
    const int n = 200, t = 1000000;
    const vector<int> us = {t/128,t/64,t/32,t/16,t/8,t/4,t/2};
    vector<float> usFractions;
    for(auto u : us)
        usFractions.push_back(u/(float)t);
    auto testGen = [&](int u) {return testGenerators::randomTest(n,1,u,t,t);};
    auto avgTimeAndStddev = measureTimes(solutions, us, testGen, runs);
    save_to_csv("smallusingletarget_sol",usFractions,"największa moneta $/$ reszta do wydania",solutions,avgTimeAndStddev.first, avgTimeAndStddev.second, saveDirectory);
}

void nAndTime(int runs, string saveDirectory) {
    cerr<<"preparing data for n, time graph"<<endl;
    const vector<pair<string,coinFunc> > solutions = {
        {"classic", classic::getMinimumCoinNumberFor},
        {"solution 2", solution2::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor}
    };
    vector<int> ns = {200,300,400,500,600,700,800,1000};
    const int t = 500000;
    for(int i=1200;i<=4000; i += 400)
        ns.push_back(i);

    auto testGen = [&](int n) {return testGenerators::randomTest(n,1,t,t,t);};
    auto avgTimeAndStddev = measureTimes(solutions, ns, testGen, runs);
    save_to_csv("n_time",ns,"liczba monet",solutions,avgTimeAndStddev.first, avgTimeAndStddev.second, saveDirectory);
}

void sol2Vs4OnBigAnswerTests(int runs, string saveDirectory) {
    cerr<<"preparing data for sol 2 vs sol 4"<<endl;
    const vector<pair<string,coinFunc> > solutions = {
        {"solution 2", solution2::getMinimumCoinNumberFor},
        {"solution 4", solution4::getMinimumCoinNumberFor},
    };
    
    vector<int> logW(20);
    iota(logW.begin(), logW.end(),1);
    vector<int> w = logW;
    for(auto &x : w)
        x = (1<<x);

    auto testGen = [&](int l) {
        const int M = (1<<(20-l));
        return testGenerators::randomTest(M,1,M, (1<<19)+5, (1<<20)-5);
    };
    auto avgTimeAndStddev = measureTimes(solutions, logW, testGen, runs);
    save_to_csv("sol2_vs_4_big_answer",w,"wynik",solutions,avgTimeAndStddev.first, avgTimeAndStddev.second, saveDirectory);
}

int main(int argc, char** argv) {
    int runs = 1;
    if(argc >= 2)
        runs = atoi(argv[1]);
    string saveDirectory = "data/";
    if(argc >= 3)
        saveDirectory = string(argv[2]);
    tAndTime(runs,saveDirectory);
    sol2Vs4OnBigAnswerTests(runs, saveDirectory);
    nAndTime(runs,saveDirectory);
    smallUSingleTargetVsSol(runs,saveDirectory);
    smallUvsClassic(runs,saveDirectory);
}