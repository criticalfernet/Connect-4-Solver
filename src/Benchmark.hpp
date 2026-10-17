
#pragma once

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include "BenchmarkData.hpp"
#include "Solver.hpp"

using namespace std;
int constexpr MAX_TESTS = 100;

void showTTStatsFunc(uint64_t nodes) {
  const auto& s = tt.stats;
  double hitRate = s.probes
    ? 100.0 * s.hits / s.probes
    : 0.0;

  double collRate = s.probes
    ? 100.0 * s.collisions / s.probes
    : 0.0;

  double cutoffRate = 100.0 * s.cutoffs / nodes;

  cout << setw(12) << s.probes
    << setw(10) << fixed << setprecision(2) << hitRate << "%"
    << setw(12) << fixed << setprecision(2) << cutoffRate << "%"
    << setw(12) << s.stores
    << setw(12) << fixed << setprecision(2) << collRate << "%";
}

inline int runBenchmark(bool showTTStats = false)
{
  using namespace std::chrono;

  uint64_t totalNodes = 0;
  int passed = 0;
  int failed = 0;

  std::cout << left
    << setw(15) << "Position"
    << right
    << setw(10)  << "Expected"
    << setw(10)  << "Actual"
    << setw(12) << "Nodes"
    << setw(12) << "Time (us)"
    << setw(10) << "Time (s)"
    << setw(12) << "Nodes/sec";

  if (showTTStats) {
    std::cout << setw(12) << "TT Probes"
      << setw(10) << "Hit Rate"
      << setw(12) << "Cutoffs"
      << setw(12) << "TT Stores"
      << setw(12) << "Coll. Rate";
  }

  std::cout<< "  Result\n";
  std::cout << string(showTTStats ? 150 : 90, '-') << '\n';

  int testCount = 0;

  for (const auto& [position, expected] : benchmarks)
  {
    if (++testCount > MAX_TESTS) break;
    Board board(position);

    tt.reset();
    nodesSearched = 0;

    auto start = steady_clock::now();
    int actual = solve(board);
    auto end = steady_clock::now();

    auto elapsed =
      duration_cast<microseconds>(end - start).count();

    uint64_t nodes = nodesSearched;
    totalNodes += nodes;

    bool correct = (actual == expected);

    if (correct) passed++;
    else failed++;

    double nps = elapsed > 0
      ? static_cast<double>(nodes) * 1'000'000.0 / elapsed
      : 0.0;

    std::cout << left << setw(15) << position
      << right
      << setw(10) << expected
      << setw(10) << actual
      << setw(12) << nodes
      << setw(12) << elapsed
      << setw(10) << fixed << setprecision(2) << (elapsed / 1'000'000.0)
      << setw(12) << static_cast<uint64_t>(nps);

    if (showTTStats) showTTStatsFunc(nodes);

    std::cout << "  " << (correct ? "PASS" : "FAIL") << '\n';
  }

  std::cout << string(showTTStats ? 150 : 90, '-') << '\n';
  std::cout << "Tests: " << passed + failed
    << " | Passed: " << passed
    << " | Failed: " << failed << '\n';

  std::cout << "Total nodes: " << totalNodes << '\n';

  return failed == 0 ? 0 : 1;
}
