#pragma once

#include "Board.hpp"
#include "TranspositionTable.hpp"
#include "MoveSorter.hpp"
#include <climits>

constexpr unsigned int log2c(unsigned int n) {
  return n <= 1 ? 0 : log2c((n+1)/2)+1;
}

static TranspositionTable<
  Board::WIDTH*(Board::HEIGHT+1), //key size: 49 bits
  log2c(Board::MAX_SCORE - Board::MIN_SCORE + 1) + 1, //value size: 7 bits to show 37 distinct values
  24
> tt;

inline uint64_t nodesSearched = 0;

static int moveOrder[Board::WIDTH] = {3, 2, 4, 1, 5, 0, 6};

int alphabeta(const Board &board, int alpha, int beta)
{
  nodesSearched++;
  assert(alpha < beta);
  assert(!Bitboard::canWinNext(board.currentPlayerMask, board.bothPlayerMask));

  int moves = static_cast<int>(board.countMoves());

  uint64_t next = board.possibleNonLosingMoves();
  if(next == 0) return -(Board::WIDTH*Board::HEIGHT - moves)/2;

  if (board.countMoves() >= Board::WIDTH * Board::HEIGHT - 2)
    return 0;

  int min = -(Board::WIDTH*Board::HEIGHT-2 - moves)/2;
  if(alpha < min) {
    alpha = min;
    if(alpha >= beta) return alpha;
  }

  int max = (Board::WIDTH * Board::HEIGHT - 1 - moves) / 2;

  if (beta > max){
    beta = max;
    if (alpha >= beta) {
      tt.stats.cutoffs++;
      return beta;
    }
  }

  const uint64_t key = board.getKey();
  if(int val = tt.get(key)) {
    if(val > Board::MAX_SCORE - Board::MIN_SCORE + 1) { // we have an lower bound
      min = val + 2*Board::MIN_SCORE - Board::MAX_SCORE - 2;
      if(alpha < min) {
        alpha = min;
        if(alpha >= beta) {
          tt.stats.cutoffs++;
          return alpha;
        }
      }
    }
    else { // we have an upper bound
      max = val + Board::MIN_SCORE - 1;
      if(beta > max) {
        beta = max;
        if (alpha >= beta) {
          tt.stats.cutoffs++;
          return beta;
        }
      }
    }
  }

  { // Prune moves with help of the TT
    int childLowerBounds[Board::WIDTH];
    uint64_t childMoves[Board::WIDTH];
    int n = 0;
 
    for (uint64_t m = next; m; m &= m - 1) { //Iterates through each set bit in next
      const uint64_t mv = m & -m; // mv represents a legal move
      const uint64_t childKey = (board.currentPlayerMask ^ board.bothPlayerMask) + (board.bothPlayerMask | mv);
      int lowerBound = INT_MIN;
 
      if (int v = tt.get(childKey)) {
        if (v <= Board::MAX_SCORE - Board::MIN_SCORE + 1) { //upper bound
          int currentPlayerScore = -(v + Board::MIN_SCORE - 1);
          if (currentPlayerScore >= beta) {
            tt.stats.cutoffs++;
            tt.put(key, currentPlayerScore + Board::MAX_SCORE - 2*Board::MIN_SCORE + 2);
            return currentPlayerScore;
          }
          if (currentPlayerScore > alpha) alpha = currentPlayerScore;
        } else { //                                           lower bound
          lowerBound = v + 2*Board::MIN_SCORE - Board::MAX_SCORE - 2;
        }
      }
      childMoves[n] = mv;
      childLowerBounds[n++] = lowerBound;
    }
 
    for (int k = 0; k < n; k++){
      // Prune moves whose TT-derived upper bound cannot improve alpha.
      if (childLowerBounds[k] != INT_MIN && -childLowerBounds[k] <= alpha) next &= ~childMoves[k];
    }
  }


  MoveSorter movesS;

  for (int i = Board::WIDTH; i--;) {
    if(uint64_t move = next & Bitboard::column_mask(moveOrder[i])) {
       movesS.add(move, board.moveScore(move));
    }
  }

  while(uint64_t next = movesS.getNext()){
      Board b2(board);
      b2.playMove(next);

      int score = -alphabeta(b2, -beta, -alpha);

      if(score >= beta) {
        tt.put(key, score + Board::MAX_SCORE - 2*Board::MIN_SCORE + 2); // save the lower bound of the position
        return score;
      }
      if (score > alpha) alpha = score;
  }

  tt.put(board.getKey(),  alpha - Board::MIN_SCORE + 1);
  return alpha;
}