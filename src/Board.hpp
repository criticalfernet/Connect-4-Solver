#pragma once
#include <cstdint>
#include <string>
#include <cassert>
#include <bit>
#include "BitBoard.hpp"

using namespace Bitboard;

class Board {
public:
  static constexpr int WIDTH  = 7;
  static constexpr int HEIGHT = 6;
  static constexpr int MIN_SCORE = -(WIDTH*HEIGHT)/2 + 3;
  static constexpr int MAX_SCORE = (WIDTH*HEIGHT+1)/2 - 3;
  std::string strPosition = "";

  uint64_t currentPlayerMask = 0; // mask for pieces of active player
  uint64_t bothPlayerMask = 0; // mask for pieces of both players
  
  Board(const std::string& moves) {
    for (char c : moves) {
        int col = c - '0';
        if (!canPlay(col)) throw std::invalid_argument("Invalid position");
        play(col);
    }
  }

public:
  unsigned int countMoves() const {return moves;}
  int getPlayer() const {return currentPlayer;}
  int getScore() const {
    return (WIDTH * HEIGHT + 1 - moves) / 2;
  }

  bool canPlay(int col) const {
    if (col < 1 || col > WIDTH) return false;
    col--;

    return (bothPlayerMask & top_mask_col(col)) == 0; // checks if the top cell of the column is unoccupied
  }

  void play(int col) {
    if (!canPlay(col)) return;
    strPosition += char('0' + col);
    col--;

    currentPlayerMask ^= bothPlayerMask; // Switch to player making the move
    currentPlayer = 3 - currentPlayer;

    uint64_t column = column_mask(col);
    uint64_t emptyCell = (bothPlayerMask + bottom_mask_col(col)) & column;  // Find the lowest empty cell in this column
    bothPlayerMask |= emptyCell;

    moves++;
  }

  void playMove(uint64_t move) {
    currentPlayerMask ^= bothPlayerMask;
    bothPlayerMask |= move;

    ++moves;
    currentPlayer = 3 - currentPlayer;
  }

  bool isWinningMove(int col) const{
    return canWinNext(currentPlayerMask, bothPlayerMask) & column_mask(col-1);
  }

  char symbolPlayer() const{
    return (currentPlayer == 1) ? 'x' : 'o';
  }

  void print() const {
    std::cout << '\n';
    std::cout <<"Player '" << symbolPlayer()<< "' to play."<< std::endl;
    std::cout << "===============================\n";

    for (int row = HEIGHT - 1; row >= 0; --row)
    {
        std::cout << "|";

        for (int col = 0; col < WIDTH; ++col)
        {
            int bitIndex = col * (HEIGHT + 1) + row;
            uint64_t bit = UINT64_C(1) << bitIndex;

            char piece = ' ';

            if (currentPlayerMask & bit)
                piece = (currentPlayer == 1) ? 'x' : 'o';
            else if ((bothPlayerMask ^ currentPlayerMask) & bit)
                piece = (currentPlayer == 1) ? 'o' : 'x';

            std::cout << ' ' << piece << " |";
        }

        std::cout << '\n';
    }

    std::cout << "===============================\n";
  }

  uint64_t getKey() const {
    return currentPlayerMask + bothPlayerMask;
  }

  uint64_t possibleNonLosingMoves() const {
      assert(!canWinNext(currentPlayerMask, bothPlayerMask));

      uint64_t possible_mask = possible(bothPlayerMask);
      uint64_t opponent_win = canOpponentWinNext(currentPlayerMask, bothPlayerMask);

      uint64_t forced_moves = possible_mask & opponent_win;

      if (forced_moves) {
          if (forced_moves & (forced_moves - 1))
              return 0;
          else
              possible_mask = forced_moves;
      }

      return possible_mask & ~(opponent_win >> 1); // >> 1 to ensure case of playing empty and making opponent win
  }

  int moveScore(uint64_t move) const {
    uint64_t newPlayerMask = currentPlayerMask | move;
    return std::popcount(compute_winning_position(newPlayerMask, bothPlayerMask));
  }



private:
  unsigned int moves = 0;
  int currentPlayer = 1;
};