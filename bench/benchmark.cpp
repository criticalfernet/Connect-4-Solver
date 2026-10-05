    #include "Board.h"
    #include "alphabeta.h"
    #include "negamax.h"
    #include "alphabetaMO1.h"

    #include <chrono>
    #include <fstream>
    #include <iostream>
    #include <string>
    #include <vector>

    using namespace std;
    using Algorithm = int (*)(const Board&);
    struct TestResult {
        bool alphabetaPassed;
        bool alphabeta1Passed;
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

        int alphaBetaResult;
        int alphaBetaMO1Result;

        long long alphaBetaTotalTime = 0;
        long long alphaBetaMO1TotalTime = 0;

        for (int i = 0; i < RUNS; ++i) {
            alphaBetaTotalTime += benchmark(alphabeta, board, alphaBetaResult);
            alphaBetaMO1TotalTime += benchmark(alphabetaMO1, board, alphaBetaMO1Result);
        } 

        long long alphaBetaTime = alphaBetaTotalTime / RUNS;
        long long alphaBetaMO1Time = alphaBetaMO1TotalTime / RUNS;

        bool alphaBetaPassed = alphaBetaResult == expectedScore;
        bool alphaBetaMO1Passed = alphaBetaMO1Result == expectedScore;

        printStats(test,"alphabeta",alphaBetaPassed,alphaBetaTime);
        if (!alphaBetaPassed) printExpect(expectedScore, alphaBetaResult);
            
        printStats(test,"alphabetaMO1",alphaBetaMO1Passed,alphaBetaMO1Time);
        if (!alphaBetaMO1Passed) printExpect(expectedScore, alphaBetaMO1Result);

        cout << endl;

        saveResult(results, test, "alphabeta", alphaBetaPassed, alphaBetaTime);
        saveResult(results, test, "alphabetaMO1", alphaBetaMO1Passed, alphaBetaMO1Time);

        return {alphaBetaPassed, alphaBetaMO1Passed};
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

        int alphabetaPassed = 0;
        int alphabetaFailed = 0;

        int alphabeta1Passed = 0;
        int alphabeta1Failed = 0;

        for (const auto& [position, expectedScore] : tests) {
            TestResult result = runTest(position, expectedScore, results);

            result.alphabetaPassed
                ? ++alphabetaPassed
                : ++alphabetaFailed;

            result.alphabeta1Passed
                ? ++alphabeta1Passed
                : ++alphabeta1Failed;
        }

        cout << "===============================" << endl;
        cout << "           SUMMARY             " << endl;
        cout << "===============================" << endl;

        cout << "AlphaBeta   : "
            << alphabetaPassed << " passed, "
            << alphabetaFailed << " failed" << endl;

        cout << "AlphaBeta with Move Order : "
            << alphabeta1Passed << " passed, "
            << alphabeta1Failed << " failed" << endl;

        return 0;
    }