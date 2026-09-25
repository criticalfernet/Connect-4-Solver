# Connect 4

A small C++ implementation of a Connect 4 board and a Negamax search.

The project is currently focused on getting the board representation and
search working. The program lets you enter moves, displays the board,
checks for wins, and runs a depth-limited Negamax search after each
move.

## Current State

Right now the program contains:

-   A `Board` class for representing a 7×6 Connect 4 board.
-   Move validation and move execution.
-   Win detection.
-   A simple terminal board display.
-   Position notation based on the sequence of played columns.
-   A Negamax search with a maximum depth of 5.
-   A score based on how many moves remain when a win is found.

The search is **not yet a complete Connect 4 solver**. If the search
reaches depth `0`, it currently returns `0`.

------------------------------------------------------------------------

## Board

The board is represented as a 7-column by 6-row Connect 4 board.

Columns are entered using numbers from `1` to `7`.

The `Board` class keeps track of:

-   The pieces on the board.
-   The current player.
-   The number of moves played.

### Playing a Move

`Board::play(int col)` first checks whether the column can be played.

If it can, the piece is placed in the lowest empty position in that
column. The current player is then switched:

``` cpp
currentPlayer = 3 - currentPlayer;
```

The number of moves is also incremented.

Invalid moves are simply ignored.

### Checking for a Win

`Board::isWinningMove(int col)` checks whether playing in a given column
would create four pieces in a row.

It checks four directions:

-   Horizontal
-   Vertical
-   Diagonal `/`
-   Diagonal `\`

The check looks in both directions from the piece that would be played.

------------------------------------------------------------------------

## Display

The board can be printed using `Board::print()`.

Player `1` is displayed as:

``` text
x
```

Player `2` is displayed as:

``` text
o
```

For example:

``` text
===============================
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   |   |   |   |   |
|   |   |   | o |   |   |   |
|   |   |   | x |   |   |   |
|   |   |   | o | x | x |   |
===============================
```

------------------------------------------------------------------------

## Position Notation

A position is represented by the sequence of columns that were played.

For example:

``` text
434353
```

means:

``` text
Player 1 → column 4
Player 2 → column 3
Player 1 → column 4
Player 2 → column 3
Player 1 → column 5
Player 2 → column 3
```

The order matters, so different move sequences can produce different
positions.

An empty position is represented by:

``` text
""
```

More details are documented in [Position's
notation.md](Position’s%20notation.md).

------------------------------------------------------------------------

## Negamax

The search uses the Negamax variant of Minimax.

The basic idea is that the score of a position from the opponent's point
of view is the negative of the score from the current player's point of
view.

So instead of separately implementing maximizing and minimizing players,
the search recursively does:

``` cpp
int score = -negamax(b2, depth - 1);
```

The best score is then selected from all legal moves.

The current implementation starts with:

``` cpp
int bestScore = -Board::WIDTH * Board::HEIGHT;
```

and searches every playable column.

### Search Depth

The default maximum search depth is:

``` cpp
int MAX_DEPTH = 5;
```

So:

``` cpp
negamax(board)
```

is equivalent to:

``` cpp
negamax(board, 5);
```

The depth-limited search is intentional for now. Exploring the entire
Connect 4 game tree grows very quickly.

------------------------------------------------------------------------

## Current Terminal Conditions

The current Negamax implementation stops searching when one of these
conditions is reached.

### Depth reaches zero

``` cpp
if (depth == 0) return 0;
```

At the moment, the program does not have a positional evaluation
function for positions that are simply reached because the search depth
ran out.

Therefore, those positions receive a score of `0`.

### Board is full

If all 42 cells are occupied:

``` cpp
if(board.countMoves() == Board::WIDTH * Board::HEIGHT)
    return 0;
```

the game is treated as a draw.

### A winning move exists

Before searching deeper, the program checks every playable column.

If the current player can win immediately:

``` cpp
if(board.canPlay(x) && board.isWinningMove(x)) {
    return board.getScore();
}
```

the position receives the current score.

------------------------------------------------------------------------

## Score

The score for a winning position is calculated by:

``` cpp
(Board::WIDTH * Board::HEIGHT + 1 - this->countMoves()) / 2;
```

For a standard 7×6 board this becomes:

``` cpp
(43 - countMoves()) / 2;
```

The score is therefore based on how many moves have already been played.

The current scoring idea is:

-   Positive score → a win was found.
-   Larger positive score → the win occurs earlier.
-   `0` → no win was found at that point, including depth exhaustion or
    a full-board draw.

Because the search is currently depth-limited and returns `0` at depth
`0`, a `0` from the program should **not** yet be interpreted as a
proven draw.

The scoring details are documented in
[benchmarking.md](benchmarking.md).

------------------------------------------------------------------------

## Program Flow

The current `main()` loop works roughly like this:

1.  Read a column number from the user.
2.  Clear the terminal.
3.  Check whether that move is a winning move.
4.  Play the move.
5.  Print the board.
6.  If the move was winning, announce the winner and stop.
7.  Otherwise, run Negamax and print its score.
8.  Repeat.

The relevant flow is:

``` cpp
int player = board.currentPlayer;
bool win = board.isWinningMove(col);

board.play(col);
board.print();

if (win) {
    std::cout << "Player " << player << " wins!" << std::endl;
    break;
}

std::cout << "Score: " << negamax(board) << std::endl;
```

------------------------------------------------------------------------

## Files

The main source files currently used by the project are:

``` text
src/
├── main.cpp
├── Board.cpp
└── negamax.cpp
```

with their corresponding headers in:

``` text
headers/
├── Board.h
└── negamax.h
```

The repository also contains notes documenting the algorithms and
notation.

------------------------------------------------------------------------

## Notes

-   [Position's notation.md](Position’s%20notation.md) --- describes how
    positions are represented as move sequences.
-   [MINIMAX algorithm.md](MINIMAX%20algorithm.md) --- notes on Minimax
    and the Negamax variant.
-   [benchmarking.md](benchmarking.md) --- documents the current
    scoring/benchmarking idea.

------------------------------------------------------------------------

## What Comes Next

The current implementation is intentionally simple.

The next improvements can build on the existing board and Negamax
implementation, such as adding a better evaluation for positions where
the search reaches depth `0`, increasing search depth, and eventually
working toward a complete Connect 4 solver.
