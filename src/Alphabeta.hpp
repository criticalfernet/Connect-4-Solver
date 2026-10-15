#pragma once

#include "Board.hpp"
#include "TranspositionTable.hpp"

// constexpr int SCORE_OFFSET = 43;
static TranspositionTable tt(1u << 20);

static int moveOrder[Board::WIDTH] = {4, 3, 5, 2, 6, 1, 7};

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

  for (int i = 0; i < Board::WIDTH; i++)
  {
    int x = moveOrder[i];
    if(next & Board::column_mask(x-1)){
      Board b2(board);
      b2.play(x);

      int score = -alphabeta(b2, -beta, -alpha);

      if (score >= beta) return score;
      if (score > alpha) alpha = score;
    }
  }

  tt.put(board.getKey(),  alpha - Board::MIN_SCORE + 1);
  return alpha;
}