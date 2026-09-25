#include "Board.h"
#include "alphabeta.h"
#include "negamax.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using Algorithm = int (*)(const Board&);
struct TestResult {
    bool negamaxPassed;
    bool alphabetaPassed;
};

long long benchmark(Algorithm algorithm, const Board& board, int& result)
{
    auto start = chrono::high_resolution_clock::now();

    result = algorithm(board);

    auto end = chrono::high_resolution_clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

void printStats(const string& test,string algo, bool result, long long time) {
    cout << "| position = " << test
         << " | " << algo << " = " << (result ? "PASSED" : "FAILED")
         << " | time = " << time << " us"
         << endl;
}

void printExpect(int expected, int result) {
    cout << "| expected = " << expected
         << " | got = " << result << endl;
}

void saveResult(ofstream& file, const string& test, const string& algorithm, bool result, long long time){
    file << test << ","
         << algorithm << ","
         << boolalpha << result << ","
         << time << "\n";
}

TestResult runTest(const string &test, int expectedScore, ofstream& results)
{
    constexpr int RUNS = 20;
    Board board(test);

    int alphaResult;
    int negamaxResult;

    long long alphaTotalTime = 0;
    long long negamaxTotalTime = 0;

    for (int i = 0; i < RUNS; ++i) {
        alphaTotalTime += benchmark(alphabeta, board, alphaResult);
        negamaxTotalTime += benchmark(negamax, board, negamaxResult);
    } 

    long long alphaTime = alphaTotalTime / RUNS;
    long long negamaxTime = negamaxTotalTime / RUNS;
    
    bool negamaxPassed = negamaxResult == expectedScore;
    bool alphabetaPassed = alphaResult == expectedScore;

    printStats(test,"negamax",negamaxPassed,negamaxTime);
    if (!negamaxPassed) printExpect(expectedScore, negamaxResult);
        
    printStats(test,"alphabeta",alphabetaPassed,alphaTime);
    if (!alphabetaPassed) printExpect(expectedScore, alphaResult);

    cout << endl;

    saveResult(results, test, "negamax", negamaxPassed, negamaxTime);
    saveResult(results, test, "alphabeta", alphabetaPassed, alphaTime);

    return {negamaxPassed, alphabetaPassed};
}

vector<pair<string, int>> fetchTests(const string& filename)
{
    ifstream file(filename);

    if (!file) {
        cerr << "Failed to open" << filename << endl;
        return {};
    }

    vector<pair<string, int>> tests;

    string position;
    int expectedScore;

    while (file >> position >> expectedScore) {
        tests.push_back({position, expectedScore});
    }

    return tests;
}

ofstream loadOutFile() {
    ofstream results("results/results.csv", ios::out | ios::trunc);

    if (!results) {
        cerr << "Failed to open results.csv" << endl;
        return {};
    }

    results << "test,algorithm,result,time_us\n";
    return results;
}



int main()
{
    vector<string> testFiles = {
        "tests/Test_L3_R1",
        "tests/Test_L2_R1"
    };
    vector<pair<string, int>> tests;

    for (const auto& file : testFiles){
        auto fileTests = fetchTests(file);

        tests.insert(
            tests.end(),
            fileTests.begin(),
            fileTests.end()
        );
    }

    ofstream results = loadOutFile();
    if (!results) return 1;

    cout << "Tests loaded: " << tests.size() << endl;
    cout << endl;

    int negamaxPassed = 0;
    int negamaxFailed = 0;

    int alphabetaPassed = 0;
    int alphabetaFailed = 0;

    for (const auto& [position, expectedScore] : tests) {
        TestResult result = runTest(position, expectedScore, results);

        result.negamaxPassed
            ? ++negamaxPassed
            : ++negamaxFailed;

        result.alphabetaPassed
            ? ++alphabetaPassed
            : ++alphabetaFailed;
    }

    cout << "===============================" << endl;
    cout << "           SUMMARY             " << endl;
    cout << "===============================" << endl;

    cout << "Negamax   : "
         << negamaxPassed << " passed, "
         << negamaxFailed << " failed" << endl;

    cout << "AlphaBeta : "
         << alphabetaPassed << " passed, "
         << alphabetaFailed << " failed" << endl;

    return 0;
}