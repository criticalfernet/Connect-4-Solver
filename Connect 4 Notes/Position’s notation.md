# Position Notation

A Connect 4 position is represented by the sequence of columns played.

Each number represents the column where a player dropped their piece.

For example:

```text
434353
```

means:

```text
Player 1 → column 4
Player 2 → column 3
Player 1 → column 4
Player 2 → column 3
Player 1 → column 5
Player 2 → column 3
```

The board after these moves is:

```text
. . . . . . .
. . . . . . .
. . . . . . .
. . O . . . .
. . X . . . .
. . O X X . .
```

## Column Numbers

Columns are numbered from left to right:

```text
1 2 3 4 5 6 7
```

Therefore, a move is always a number from `1` to `7`.

For example:

```text
4
```

means the player played in the fourth column.

## Position String

The position can be written as one continuous string:

```text
434353
```

The order is important. For example:

```text
434353
```

and

```text
443353
```

are different positions because the moves were played in a different order.

## Empty Position

An empty board has no moves:

```text
""
```

## Example

Move sequence:

```text
443322
```

means:

```text
P1 → 4
P2 → 4
P1 → 3
P2 → 3
P1 → 2
P2 → 2

board:
. . . . . . .
. . . . . . .
. . . . . . .
. . . . . . .
. O O O . . .
. X X X . . .
```
