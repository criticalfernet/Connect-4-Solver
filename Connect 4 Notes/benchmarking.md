# Benchmarking

The benchmark score of a position represents the outcome of the game for the **player to move**, assuming both players play perfectly.
The score is based on how many moves are left until the game ends.

## Score

For a winning position, the score is:

```cpp
(Position::WIDTH * Position::HEIGHT + 1 - P.countMoves()) / 2;
```

where:

* `Position::WIDTH` is the board width.
* `Position::HEIGHT` is the board height.
* `P.countMoves()` is the number of moves already played.

For a standard Connect 4 board:

```text
WIDTH  = 7
HEIGHT = 6
```

so the maximum number of moves is:

```text
7 * 6 = 42
```

and the score becomes:

```cpp
(43 - P.countMoves()) / 2;
```

## Meaning

A **positive score** means the player to move can win.

A **negative score** means the player to move will lose with perfect play.

A **zero score** means the game ends in a draw.

The magnitude of the score indicates how quickly the game ends.

### Winning

If the player to move wins, the score is positive.

A larger score means the win happens sooner.

For example:

```text
Score = 1
```

means the player wins with their last possible move.

A larger value means the win occurs earlier.

### Losing

If the player to move cannot win and will eventually lose, the score is negative.

The score is calculated from the opponent's winning position and negated for the current player.

Thus:

```text
Score = -1
```

means the current player loses on the last possible move.

More negative values mean the loss happens sooner.

### Draw

If neither player can force a win:

```text
Score = 0
```

## Example

Consider a position with:

```cpp
P.countMoves() == 10
```

On a 7×6 board:

```cpp
(7 * 6 + 1 - 10) / 2
= (43 - 10) / 2
= 33 / 2
= 16
```

So the winning score for this position is:

```text
16
```

The score is therefore a measure of **how soon the game can be won or lost**, with positive values representing wins, negative values representing losses, and zero representing a draw.
