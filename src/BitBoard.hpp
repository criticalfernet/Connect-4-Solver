#pragma once
#include <cstdint>

/*
  BitBoard:
  6 13 7n+6 -> buffer row to make bitwise operations simpler (sentinel)
  5 12 7n+5
  4 11 7n+4
  3 10 7n+3
  2 9 7n+2
  1 8 7n+1
  0 7 7n
*/

namespace Bitboard
{
  constexpr static uint64_t bottom()
  {
    uint64_t mask = 0;
    for (int col = 0; col < 7; col++)
      mask |= 1ULL << (col * 7); // set all 7n bits to 1
    return mask;
  }

  static uint64_t column_mask(int col)
  {
    return 63ULL << (col * 7); // sets all playable bits for a column to 1. here 63 since it is 0111111
  }

  static uint64_t bottom_mask_col(int col)
  {
    return bottom() & column_mask(col); // exactly one bit set, the bottom cell of the specified column
  }

  static uint64_t top_mask_col(int col)
  {
    return 32ULL << (col * 7); // exactly one bit set, the top playable cell of the specified column
  }

  static constexpr uint64_t bottom_mask = bottom();
  static constexpr uint64_t sentinel_mask = bottom_mask << 6; // sets all sentinel bits
  static constexpr uint64_t full_mask = (1ULL << 49) - 1;
  static constexpr uint64_t board_mask = full_mask ^ sentinel_mask; // sets the playable bits in each column while leaving the sentinel bit unset.

  static uint64_t possible(uint64_t _bothPlayerMask)
  {
    return (_bothPlayerMask + bottom_mask) & board_mask; // All the cells where a piece can legally be played next
  }

  static uint64_t compute_winning_position(uint64_t _currentPlayerMask, uint64_t _bothPlayerMask){
    // identifies where a winning line could be completed

    uint64_t result = 0;
    uint64_t p;

    // Vertical
    p = (_currentPlayerMask << 1) & (_currentPlayerMask << 2);
    p &= (_currentPlayerMask << 3) | (_currentPlayerMask >> 1);
    result |= p;

    p = (_currentPlayerMask >> 1) & (_currentPlayerMask >> 2);
    p &= (_currentPlayerMask >> 3) | (_currentPlayerMask << 1);
    result |= p;

    // Horizontal
    p = (_currentPlayerMask << 7) & (_currentPlayerMask << 14);
    p &= (_currentPlayerMask << 21) | (_currentPlayerMask >> 7);
    result |= p;

    p = (_currentPlayerMask >> 7) & (_currentPlayerMask >> 14);
    p &= (_currentPlayerMask >> 21) | (_currentPlayerMask << 7);
    result |= p;

    // Diagonal 1
    p = (_currentPlayerMask << 6) & (_currentPlayerMask << 12);
    p &= (_currentPlayerMask << 18) | (_currentPlayerMask >> 6);
    result |= p;

    p = (_currentPlayerMask >> 6) & (_currentPlayerMask >> 12);
    p &= (_currentPlayerMask >> 18) | (_currentPlayerMask << 6);
    result |= p;

    // Diagonal 2
    p = (_currentPlayerMask << 8) & (_currentPlayerMask << 16);
    p &= (_currentPlayerMask << 24) | (_currentPlayerMask >> 8);
    result |= p;

    p = (_currentPlayerMask >> 8) & (_currentPlayerMask >> 16);
    p &= (_currentPlayerMask >> 24) | (_currentPlayerMask << 8);
    result |= p;

    return result & (board_mask ^ _bothPlayerMask); // Keep only empty playable cells
  }


  uint64_t canWinNext(uint64_t _currentPlayerMask, uint64_t _bothPlayerMask){
    return compute_winning_position(_currentPlayerMask,_bothPlayerMask) & possible(_bothPlayerMask);
    // any next legal move is winning
  }

  uint64_t canOpponentWinNext(uint64_t _currentPlayerMask, uint64_t _bothPlayerMask){
    uint64_t _opponentMask = _bothPlayerMask ^ _currentPlayerMask;
    return compute_winning_position(_opponentMask,_bothPlayerMask);
  }
}