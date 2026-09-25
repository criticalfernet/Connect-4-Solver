# Minimax

Solving Connect 4 can be seen as finding the best path in a decision tree where each node is a `Position`.

At each node, the current player chooses one move, leading to one of the possible next positions.

The score of a position is determined recursively:

* when it is the current player's turn, the score is the **maximum** score of all possible next positions
* when it is the opponent's turn, the score is the **minimum** score of all possible next positions

This allows the engine to choose the move that leads to the best possible outcome assuming the opponent also plays optimally.

## Search

The search continues through the possible positions until a terminal position is reached.

A terminal position can be:

* a win
* a loss
* a draw

For a winning position, the score is based on the number of moves played:

```cpp
(Position::WIDTH * Position::HEIGHT + 1 - P.countMoves()) / 2;
```

The score is used to prefer wins that occur sooner and losses that occur later.

## Recursive Definition

For a position `P`, Minimax can be described as:

```text
score(P) = max(score(child))
```

when it is our turn, and:

```text
score(P) = min(score(child))
```

when it is the opponent's turn.

Each `child` is a position resulting from one legal move.

## Example

Suppose the current position has three possible moves with scores:

```text
Move 1 →  5
Move 2 →  2
Move 3 → -1
```

If it is our turn, Minimax chooses:

```text
max(5, 2, -1) = 5
```

If it is the opponent's turn, the opponent chooses the move that gives us the lowest score:

```text
min(5, 2, -1) = -1
```

## Tree Size

The number of positions grows rapidly as the search depth increases.

Exploring the entire game tree is therefore generally impractical.

Minimax can instead search to a specified depth, evaluating the positions reached at that depth.

This gives a trade-off between search depth and the amount of computation required.
