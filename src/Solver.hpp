#pragma once

#include "Board.hpp"
#include "Alphabeta.hpp"


int solve(const Board &board, bool weak = false) {
  const int remaining = Board::WIDTH * Board::HEIGHT - static_cast<int>(board.countMoves());

  if(Bitboard::canWinNext(board.currentPlayerMask,board.bothPlayerMask)) 
    return board.getScore();
  
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

    int returnedScore = alphabeta(board, med, med + 1);

    if(returnedScore <= med) max = returnedScore;
    else min = returnedScore;
  }
  return min;
}