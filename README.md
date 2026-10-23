# Connect 4

A C++23 terminal implementation of Connect 4 featuring a bitboard-based board representation and an alpha-beta search engine with move ordering and a transposition table.

The project includes an interactive game interface, position scoring, suggested moves, and benchmark tests for evaluating search correctness and performance.

## Features

- Standard **7-column × 6-row** Connect 4 board.
- Compact bitboard representation using 64-bit integers.
- Position notation represented as the sequence of played columns.
- Detection of immediate winning moves and non-losing moves.
- Alpha-beta search with score-window narrowing.
- Transposition table (TT) for reusing information from previously searched positions.
- Move ordering based on potential winning positions.
- Center-first column ordering.
- Search and benchmark statistics, including timing and transposition-table metrics where enabled.
- Interactive terminal interface.

## Requirements

- A C++ compiler supporting C++23.
- GNU Make.
- The provided Makefile uses `g++` with `-Wall -Wextra -std=c++23`.

## Build and Run

The Makefile expects `main.cpp` and the project headers in `src/`.

```sh
make
make run
```

Clean the build output:

```sh
make clean
```

The executable is written to `build/main`.

## Playing

The program initializes the board with the move sequence:

```text
54554
```

After the five-move starting sequence `54554`, the board is displayed in the terminal-style layout below. The top row is shown first; column numbers are shown beneath the grid.

```text
===============================
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   |   | o |   |   |
|   |   |   | x | x |   |   |
|   |   |   | o | x |   |   |
===============================
  1   2   3   4   5   6   7
```

Enter a column number from `1` to `7` to play a move. Invalid input and moves into full columns are rejected.

After a valid move, the board is printed and the program checks whether the move won the game. Otherwise, it prints the position score. When it is Player 2's turn, the program can also suggest a column; the suggestion does not automatically play the move.

- Player 1 is shown as `x`.
- Player 2 is shown as `o`.

The interactive configuration in `main.cpp` currently includes:

```cpp
int constexpr CURRENT = 2;
bool constexpr DEBUG = false;
bool constexpr WEAK = true;
```

`CURRENT` selects the player's turn for which a suggested move is displayed. `DEBUG` enables suggestion output regardless of whose turn it is. `WEAK` selects the bounded `solve(child, WEAK)` mode used when comparing candidate moves.

## Position Notation

A position is recorded as the sequence of columns played, using digits `1` through `7`.

For example, `54554` means:

1. Player 1 plays column 5.
2. Player 2 plays column 4.
3. Player 1 plays column 5.
4. Player 2 plays column 5.
5. Player 1 plays column 4.

## Board Representation

`Board` represents the board with two 64-bit masks:

- `currentPlayerMask` stores the pieces belonging to the player whose turn it is.
- `bothPlayerMask` stores all occupied cells.

Each column uses seven bit positions: six playable cells and one sentinel bit. This layout supports compact bitwise operations for legal moves and winning-position detection.

The board provides operations to:

- Check whether a column can accept a move (`canPlay`).
- Play a column (`play`) or a move represented by a bit mask (`playMove`).
- Check for an immediate winning move (`isWinningMove`).
- Find playable, non-losing moves (`possibleNonLosingMoves`).
- Score a candidate move by counting winning positions it would create (`moveScore`).
- Print the board (`print`).

## Search Engine

### Solver

`solve` determines a position score by making repeated alpha-beta calls with a narrow score window. It narrows the possible score range until the returned score is determined.

The `weak` argument changes the initial score bounds to `-1` and `1`. The interactive program calls `solve(board)` to display the position score and uses `solve(child, WEAK)` when comparing candidate moves. `WEAK` is currently `true`.

Before its search loop, `solve` checks whether the current player has an immediate winning move.

### Alpha-Beta Search

`alphabeta` searches legal non-losing moves and uses alpha-beta cutoffs to avoid exploring branches that cannot improve the result. It also tightens score bounds using the number of moves remaining.

If there are no non-losing moves, it returns a negative score based on the remaining game length. Near the end of the game, it can return `0` directly.

### Move Ordering

`MoveSorter` orders candidate moves by their move scores. The search scores candidates using `Board::moveScore` before exploring them.

The fixed column-order array is:

```cpp
{3, 2, 4, 1, 5, 0, 6}
```

These are zero-based column indices, so the search starts near the center of the board.

Good move ordering can substantially improve alpha-beta performance because strong candidate moves may produce cutoffs earlier, reducing the number of branches that need to be searched.

### Transposition Table

`TranspositionTable` stores keys and search values so that information from previously searched positions can be reused. The position key comes from `Board::getKey()`, and entries store score-bound information that can tighten later alpha-beta windows.

The table is fixed-size and direct-mapped: each key maps to one slot, and a new entry replaces the previous entry occupying that slot. The table is reset when constructed.

## Benchmarks

The three benchmark logs each run the same **100 test positions**. All three versions passed every test, so the expected and actual scores matched for all 100 positions in each run.

### Version comparison

| Metric | Version 1 | Version 2 | Version 3 |
|---|---:|---:|---:|
| Tests passed | 100 / 100 | 100 / 100 | 100 / 100 |
| Tests failed | 0 | 0 | 0 |
| Total nodes searched | 6,607,707,235 | 4,011,913,709 | 3,539,859,017 |
| Total elapsed search time | 1,095.88 s | 797.06 s | 625.72 s |
| Overall throughput | 6.03 million nodes/s | 5.03 million nodes/s | 5.66 million nodes/s |
| Mean per-position TT hit rate | 24.10% | 12.45% | 13.24% |
| Mean per-position cutoff rate | 18.18% | 8.89% | 8.88% |
| Mean per-position collision rate | 37.94% | 38.79% | 26.95% |
| Total TT probes | 10,714,041,644 | 10,929,475,320 | 9,654,048,944 |
| Total TT stores | 4,336,553,461 | 3,254,225,841 | 2,867,719,055 |
| Slowest single test | 140.15 s | 93.44 s | 72.21 s |

**How the versions differ**

- **Version 1:** Baseline benchmark.
- **Version 2:** Added a function that prunes moves using information from the transposition table.
- **Version 3:** Removed `strPosition` to avoid string copies and doubled the transposition-table size.

### Improvements across versions

| Comparison | Total-time reduction | Speedup | Nodes reduction |
|---|---:|---:|---:|
| Version 1 → Version 2 | 27.27% | 1.375× | 39.28% |
| Version 2 → Version 3 | 21.50% | 1.274× | 11.77% |
| Version 1 → Version 3 | 42.90% | 1.751× | 46.43% |

Across the complete 100-position suite, **Version 3 finishes in about 57.1% of Version 1's total time**, while searching about **46.4% fewer nodes**. That is a 1.75× speedup over the baseline for these runs.

Overall throughput is calculated as total nodes divided by total elapsed time. The TT hit, cutoff, and collision rates in the table are the arithmetic means of the 100 per-position percentages printed by the benchmark. They are not ratios calculated from summed counters.

### Understanding the metrics

| Metric | Meaning |
|---|---|
| Position | Sequence of played columns, using digits `1` through `7`. |
| Expected / Actual | Expected score and score returned by the solver. |
| Nodes | Search nodes visited for that position. |
| Time (µs) / Time (s) | Elapsed search time in microseconds and seconds. |
| Nodes/sec | Search throughput for that individual position. |
| TT Probes | Number of transposition-table lookups. |
| Hit Rate | Percentage of probes counted as transposition-table hits by the implementation. |
| Cutoffs | Percentage of search events counted as cutoffs in this benchmark's output. |
| TT Stores | Number of transposition-table stores. |
| Coll. Rate | Collision percentage reported by the benchmark implementation. |
| Result | Whether the actual score matched the expected score. |

### What the results suggest

- **Correctness remained stable:** all 300 test executions across the three versions passed.
- **Search work decreased:** total nodes fell from 6.61 billion in Version 1 to 3.54 billion in Version 3.
- **Version 3 was fastest overall:** its total elapsed time was 470.16 seconds lower than Version 1's.
- **Node count and runtime tell different stories:** Version 2 visited fewer nodes than Version 1, but its overall nodes-per-second rate was also lower. This is why both node count and elapsed time matter.
- **The larger TT appears to help reduce collisions in these runs:** the mean reported collision rate dropped from 38.79% in Version 2 to 26.95% in Version 3. This comparison reflects both Version 3 changes together, so it does not isolate the effect of table size alone.

These measurements are the totals and per-position statistics in the supplied log files. Runtime can vary with compiler flags, machine load, and other environment differences; rerunning the versions under the same conditions is the best way to confirm the size of the improvement.

## Source Files

| File | Purpose |
|---|---|
| `main.cpp` | Runs the interactive game loop, prints search statistics, and selects a suggested move. |
| `Board.hpp` | Board state, move handling, board display, and position-related helpers. |
| `BitBoard.hpp` | Bit masks and bitwise routines for legal moves and winning positions. |
| `Solver.hpp` | Score search built around repeated alpha-beta calls. |
| `Alphabeta.hpp` | Alpha-beta search, pruning, move ordering, and transposition-table use. |
| `MoveSorter.hpp` | Orders candidate moves by score. |
| `TranspositionTable.hpp` | Fixed-size storage for position keys and search values. |
| `OpeningBook.hpp` | Opening-book functionality, if enabled in the current build. |
| `Makefile` | Build, run, and clean targets. |

## Project Goals

The main engineering goals are to preserve correct Connect 4 scores while improving search performance through:

- Bitboard operations instead of a cell-by-cell board representation.
- Alpha-beta pruning and tighter score bounds.
- Effective move ordering.
- Transposition-table reuse.
- Reproducible correctness and performance benchmarks.

Performance should be evaluated across a representative test suite, since individual positions can differ greatly in search difficulty.
