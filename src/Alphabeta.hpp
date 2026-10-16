#pragma once

#include "Board.hpp"
#include "TranspositionTable.hpp"
#include "MoveSorter.hpp"

constexpr unsigned int log2c(unsigned int n) {
  return n <= 1 ? 0 : log2c((n+1)/2)+1;
}

static TranspositionTable<
  Board::WIDTH*(Board::HEIGHT+1), //key size: 49 bits
  log2c(Board::MAX_SCORE - Board::MIN_SCORE + 1) + 1, //value size: 7 bits to show 37 distinct values
  23
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
  if (int val = tt.get(board.getKey())){
    max = val + Board::MIN_SCORE - 1;
  }

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