#pragma once

#include "Board.hpp"
#include "TranspositionTable.hpp"
#include "MoveSorter.hpp"

constexpr unsigned int log2c(unsigned int n) {
  return n <= 1 ? 0 : log2c(n/2)+1;
}

static TranspositionTable<Board::WIDTH*(Board::HEIGHT+1),
  log2c(Board::MAX_SCORE - Board::MIN_SCORE + 1) + 1,
  23
> tt;

static int moveOrder[Board::WIDTH] = {3, 2, 4, 1, 5, 0, 6};

int alphabeta(const Board &board, int alpha, int beta)
{
  assert(alpha < beta);
  assert(!board.canWinNext());

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
    if (alpha >= beta) return beta;
  }

  MoveSorter movesS;

  for (int i = Board::WIDTH; i--;) {
    if(uint64_t move = next & Board::column_mask(moveOrder[i])) {
       movesS.add(move, board.moveScore(move));
    }
  }

  while(uint64_t next = movesS.getNext()){
      Board b2(board);
      b2.playMove(next);

      int score = -alphabeta(b2, -beta, -alpha);

      if (score >= beta) return score;
      if (score > alpha) alpha = score;
  }

  tt.put(board.getKey(),  alpha - Board::MIN_SCORE + 1);
  return alpha;
}