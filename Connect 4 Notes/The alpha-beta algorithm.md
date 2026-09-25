# Alpha-Beta Pruning

Alpha-beta pruning is an optimization of the **Minimax algorithm**.

It produces the **same result as Minimax**, but avoids searching parts of the game tree that cannot affect the final decision.

---

## 1. The problem with Minimax

In a game like Connect 4, every position can lead to several possible positions.

For example:

```text
                Position
             /    /  |  \    \
            /    /   |   \    \
           P1   P2   P3   P4   P5 ...
```

Minimax recursively explores these positions.

At every level:

* The current player tries to get the **highest possible score**.
* The opponent tries to get the **lowest possible score**.

The problem is that the number of positions grows very quickly.

Connect 4 has up to 7 possible moves from a position, so a rough estimate of the number of positions at depth `d` is:

$$
7^d
$$

For example:

$$
7^{10} = 282,475,249
$$

So searching everything can become expensive very quickly.

---

# 2. The key observation

Minimax does not always need to examine every possible position.

Sometimes, while searching a branch, we can already determine that:

> This branch cannot possibly affect the final decision.

If that happens, we can **stop searching that branch**.

This is called **pruning**.

Alpha-beta pruning is the method used to do this.

---

# 3. A simple example

Imagine we are choosing between two moves:

```text
                 MAX
                /   \
               A     B
```

We examine move `A` first.

Suppose we discover:

```text
A → +10
```

So we currently know:

```text
Best move found so far = A
Best score found so far = +10
```

Now we start examining move `B`.

Suppose that while examining `B`, we discover that the opponent can force a result of:

```text
B → +5
```

Since we are maximizing:

```text
A → +10
B → +5
```

We already know that `A` is better than `B`.

There is no reason to continue examining the remaining possibilities under `B`.

Those positions cannot make `B` better than `A` in a way that changes our decision.

So we **prune** the rest of the branch.

---

# 4. Alpha and Beta

Alpha-beta pruning uses two values:

* **Alpha**
* **Beta**

They represent bounds on the scores that can still matter.

## Alpha

Alpha represents the **best score the maximizing player has already found**.

In other words:

> "I already know I can get at least this much."

If we already have:

```text
alpha = +10
```

then finding something that is only `+5` is not useful to the maximizing player.

---

## Beta

Beta represents the **best score the minimizing player has already found**.

In other words:

> "I already know I can force the result to be at most this much."

If:

```text
beta = +5
```

then the maximizing player cannot make this branch relevant if they already have a guaranteed alternative of `+10`.

---

# 5. The pruning condition

The important condition is:

$$
\boxed{\alpha \geq \beta}
$$

When this happens, we can stop searching the current branch.

Why?

Because the two players' bounds have crossed.

There is no remaining result in this branch that can affect the decision.

So:

```text
if alpha >= beta
    prune the branch
```

---

# 6. Understanding alpha with an example

Suppose we are the maximizing player.

We have already searched two moves:

```text
Move A → +3
Move B → +7
```

The best result we currently have is:

```text
alpha = +7
```

Now we investigate another move:

```text
Move C
```

Suppose we discover that the opponent can force this branch to be no better than:

```text
+4
```

Then:

```text
Move A → +3
Move B → +7
Move C → ≤ +4
```

We already have:

```text
+7
```

So Move C cannot become our preferred move.

The rest of the branch can therefore be ignored.

---

# 7. Understanding beta with an example

Now imagine that we are deeper in the tree and it is the minimizing player's turn.

Suppose the minimizing player has already found:

```text
Move A → +2
```

Since they are minimizing, they already know they can get the result down to `+2`.

So their current bound is:

```text
beta = +2
```

Now suppose another branch starts producing a result of:

```text
+5
```

The minimizing player doesn't care about that result.

They already have a way to get:

```text
+2
```

So there is no reason to continue exploring a branch that cannot improve their decision.

---

# 8. A complete small tree

Consider this tree:

```text
                    MAX
                  /     \
                A         B
              /   \     /   \
             3     5   2     ?
```

MAX wants the highest score.

First, we evaluate `A`:

```text
A → min(3, 5)
  → 3
```

So MAX now knows:

```text
alpha = 3
```

Now we investigate `B`.

The first result we find is:

```text
B → 2
```

Since `B` is controlled by MIN, MIN already has a result of `2`.

Therefore:

```text
B ≤ 2
```

But MAX already has:

```text
A = 3
```

So:

```text
A = 3
B ≤ 2
```

MAX will choose `A`.

The `?` under `B` does not need to be examined.

We can prune it.

```text
                    MAX
                  /     \
                A         B
              /   \     /   \
             3     5   2     X
                          ↑
                       pruned
```

Notice that we did **not** need to know what `?` actually contained.

---

# 9. Alpha-beta does not change Minimax

This is one of the most important properties of alpha-beta pruning.

Alpha-beta pruning does **not** change the result.

If Minimax says:

```text
Best move = Column 4
Score = +1
```

then Alpha-Beta should also say:

```text
Best move = Column 4
Score = +1
```

The difference is that Alpha-Beta may examine far fewer positions.

So:

```text
Minimax
    ↓
Search everything
    ↓
Get the answer
```

while:

```text
Alpha-Beta
    ↓
Search only relevant branches
    ↓
Prune irrelevant branches
    ↓
Get the same answer
```


