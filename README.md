# Connect 4

A C++23 terminal implementation of Connect 4 with a bitboard-based board representation and a search engine built around alpha-beta pruning.

The program starts from a predefined position, lets you enter columns, prints the board after each move, reports the position score, and displays a suggested move when it is Player 2's turn (or when debug output is enabled).

## Features

- 7-column × 6-row Connect 4 board.
- Bitboard representation using 64-bit integers.
- Position notation stored as the sequence of played columns.
- Detection of immediate winning moves and non-losing moves.
- Alpha-beta search with score-window narrowing.
- Transposition table for reusing information about previously searched positions.
- Move ordering based on potential winning positions.
- Terminal interface for entering moves and displaying the board.

## Requirements

- A C++ compiler that supports C++23.
- GNU Make, for the provided Makefile.

The Makefile uses `g++` with `-Wall -Wextra -std=c++23`.

## Build and Run

The Makefile expects the source files and headers to be in a `src/` directory, with `main.cpp` at `src/main.cpp` and the headers under `src/`.

Build the program:

```sh
make
```

Build and run:

```sh
make run
```

Remove the build output:

```sh
make clean
```

The executable is written to `build/main`.

## Playing

The program initializes the board with this move sequence:

```text
54554
```

Enter a column number from `1` to `7` to play a move. A move is rejected if the input is invalid or the selected column is full.

After a valid move, the board is printed and the program checks whether that move won the game. If it did, the winner is announced and the game ends. Otherwise, the program prints the position score.

When it is Player 2's turn, the program also prints a suggested column. This is a recommendation only: the program still waits for you to enter the next move.

Player 1 is shown as `x`, and Player 2 as `o`.

## Position Notation

A position is stored as the sequence of columns played, using digits `1` through `7`.

For example:

```text
54554
```

represents five moves, in order:

1. Player 1 plays column 5.
2. Player 2 plays column 4.
3. Player 1 plays column 5.
4. Player 2 plays column 5.
5. Player 1 plays column 4.

The sequence is kept in `Board::strPosition`.

## Board Representation

`Board` represents the board using two 64-bit masks:

- `currentPlayerMask` stores the pieces belonging to the player whose turn it is.
- `bothPlayerMask` stores all occupied cells.

Each of the seven columns uses seven bit positions: six playable cells and one sentinel bit. This layout supports compact bitwise operations for legal moves and winning-position detection.

The board provides methods to:

- Check whether a column can accept a move (`canPlay`).
- Play a column (`play`) or a move represented as a bit mask (`playMove`).
- Check for an immediate winning move (`isWinningMove`).
- Find playable, non-losing moves (`possibleNonLosingMoves`).
- Score a candidate move by counting winning positions it would create (`moveScore`).
- Print the board (`print`).

## Search

### Solver

`solve` searches for a score using repeated alpha-beta calls with a narrow score window. It narrows the possible score range until it determines the returned score.

The `weak` argument changes the initial score bounds to `-1` and `1`. The interactive program calls `solve(board)` to display the position score and uses `solve(child, WEAK)` when comparing candidate moves; `WEAK` is currently set to `true`.

Before the search loop, `solve` checks whether the current player has an immediate winning move.

### Alpha-Beta Search

`alphabeta` searches legal non-losing moves and uses alpha-beta cutoffs to avoid exploring branches that cannot improve the result. It also tightens the score bounds using the number of moves remaining.

If there are no non-losing moves, it returns a negative score based on the remaining game length. Near the end of the game, it can return `0` directly.

### Move Ordering

`MoveSorter` keeps candidate moves ordered by their move scores. `alphabeta` scores candidate moves using `Board::moveScore` before searching them.

The search also uses a fixed column-order array:

```cpp
{3, 2, 4, 1, 5, 0, 6}
```

These are zero-based column indices, so the order begins near the center of the board.

### Transposition Table

`TranspositionTable` stores a key and value for searched positions. The search uses `Board::getKey()` as its position key and stores score-bound information so later searches can tighten their alpha-beta window.

The table is a fixed-size, direct-mapped table. A key is mapped to one slot; storing another key in that slot replaces the previous entry. The table is reset when constructed.

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
| `Makefile` | Build, run, and clean targets. |

## Configuration in `main.cpp`

The interactive program currently defines:

```cpp
int constexpr CURRENT = 2;
bool constexpr DEBUG = false;
bool constexpr WEAK = true;
```

`CURRENT` controls which player's turn displays the suggested move. `DEBUG` makes the suggestion display regardless of whose turn it is. `WEAK` selects the bounded `solve(child, WEAK)` mode used while comparing candidate moves.

