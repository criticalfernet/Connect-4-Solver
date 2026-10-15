#pragma once

#include "Board.hpp"
#include "Alphabeta.hpp"

int solve(const Board &board, bool weak = false) {
  const int remaining = Board::WIDTH * Board::HEIGHT - static_cast<int>(board.countMoves());

  if(board.canWinNext())  return (Board::WIDTH  *Board::HEIGHT + 1 - board.countMoves())/2;
  
  int min = -(remaining / 2);
  int max = (remaining + 1) / 2;

  if(weak) {
    min = -1;
    max = 1;
  }

  while(min < max) {
    int med = min + (max - min)/2;

    if(med <= 0 && min/2 < med) med = min/2;
    else if(med >= 0 && max/2 > med) med = max/2;

    int r = alphabeta(board, med, med + 1);

    if(r <= med) max = r;
    else min = r;
  }
  return min;
}