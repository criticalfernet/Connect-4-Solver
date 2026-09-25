#include "Board.h"
#include "alphabeta.h"
#include "negamax.h"

#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using Algorithm = int (*)(const Board&);

long long benchmark(Algorithm algorithm, const Board& board, int& result)
{
    auto start = chrono::high_resolution_clock::now();

    result = algorithm(board);

    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

void printStats(const string& test,string algo, int result, long long time) {
    cout << "| position = " << test
         << " | " << algo << " score = " << result
         << " | time = " << time << " us"
         << endl;
}

void runTest(const string &test)
{
    Board board(test);

    int alphaResult;
    int negamaxResult;

    long long alphaTime = benchmark(alphabeta, board, alphaResult);
    long long negamaxTime = benchmark(negamax, board, negamaxResult);    

    printStats(test,"megamax",negamaxResult,negamaxTime);
    printStats(test,"alphabeta",alphaResult,alphaTime);
    cout << endl;
}

int main()
{
    vector<string> tests = {
        "334455",
        "343434",
        "7422341735647741166133573473242566"};

    for (const auto& test : tests) runTest(test);
}