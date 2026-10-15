#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>

constexpr static uint64_t bottom(int width, int height) {
  return width == 0 ? 0 : bottom(width-1, height) | 1LL << (width-1)*(height+1);
}

class Board {
public:
  static constexpr int WIDTH  = 7;
  static constexpr int HEIGHT = 6;
  static const int MIN_SCORE = -(WIDTH*HEIGHT)/2 + 3;
  static const int MAX_SCORE = (WIDTH*HEIGHT+1)/2 - 3;

  explicit Board(const std::string& moves) {
    for (char c : moves) {
        int col = c - '0';
        if (!canPlay(col)) throw std::invalid_argument("Invalid position");
        play(col);
    }
  }

  unsigned int countMoves() const {return moves;}
  int getPlayer() const {return currentPlayer;}
  int getScore() const {
    return (WIDTH * HEIGHT + 1 - moves) / 2;
  }

  bool canPlay(int col) const {
    if (col < 1 || col > WIDTH)return false;

    --col;
    return (mask & top_mask_col(col)) == 0;
  }

  void play(int col) {
    if (!canPlay(col)) return;

    --col;

    currentPosition ^= mask;
    mask |= mask + bottom_mask_col(col);

    ++moves;
    currentPlayer = 3 - currentPlayer;
  }

  bool isWinningMove(int col) const{
    return winning_position() & possible() & column_mask(col-1);
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

            if (currentPosition & bit)
                piece = (currentPlayer == 1) ? 'x' : 'o';
            else if ((mask ^ currentPosition) & bit)
                piece = (currentPlayer == 1) ? 'o' : 'x';

            std::cout << ' ' << piece << " |";
        }

        std::cout << '\n';
    }

    std::cout << "===============================\n";
  }

  uint64_t getKey() const {
    return currentPosition + mask;
  }

  uint64_t possibleNonLosingMoves() const {
      assert(!canWinNext());

      uint64_t possible_mask = possible();
      uint64_t opponent_win = opponent_winning_position();

      uint64_t forced_moves = possible_mask & opponent_win;

      if (forced_moves) {
          if (forced_moves & (forced_moves - 1))
              return 0;
          else
              possible_mask = forced_moves;
      }

      return possible_mask & ~(opponent_win >> 1);
  }

  bool canWinNext() const{
    return winning_position() & possible();
  }

  static uint64_t column_mask(int col) {
    return ((UINT64_C(1) << HEIGHT)-1) << col*(HEIGHT+1);
  }

private:
  uint64_t currentPosition = 0;
  uint64_t mask = 0;

  unsigned int moves = 0;
  int currentPlayer = 1;

  static uint64_t bottom_mask_col(int col) {
    return UINT64_C(1) << (col * (HEIGHT + 1));
  }

  static uint64_t top_mask_col(int col){
    return (UINT64_C(1) << (HEIGHT - 1)) << col*(HEIGHT+1);
  }

  static uint64_t compute_winning_position(uint64_t position, uint64_t mask) {
    uint64_t r = (position << 1) & (position << 2) & (position << 3);

    uint64_t p = (position << (HEIGHT+1)) & (position << 2*(HEIGHT+1));
    r |= p & (position << 3*(HEIGHT+1));
    r |= p & (position >> (HEIGHT+1));
    p = (position >> (HEIGHT+1)) & (position >> 2*(HEIGHT+1));
    r |= p & (position << (HEIGHT+1));
    r |= p & (position >> 3*(HEIGHT+1));

    p = (position << HEIGHT) & (position << 2*HEIGHT);
    r |= p & (position << 3*HEIGHT);
    r |= p & (position >> HEIGHT);
    p = (position >> HEIGHT) & (position >> 2*HEIGHT);
    r |= p & (position << HEIGHT);
    r |= p & (position >> 3*HEIGHT);

    p = (position << (HEIGHT+2)) & (position << 2*(HEIGHT+2));
    r |= p & (position << 3*(HEIGHT+2));
    r |= p & (position >> (HEIGHT+2));
    p = (position >> (HEIGHT+2)) & (position >> 2*(HEIGHT+2));
    r |= p & (position << (HEIGHT+2));
    r |= p & (position >> 3*(HEIGHT+2));

    return r & (board_mask ^ mask);
  }

  const static uint64_t bottom_mask = bottom(WIDTH, HEIGHT);
  const static uint64_t board_mask = bottom_mask * ((1LL << HEIGHT)-1);

  uint64_t winning_position() const {
    return compute_winning_position(currentPosition, mask);
  }

  uint64_t opponent_winning_position() const {
    return compute_winning_position(currentPosition ^ mask, mask);
  }

  uint64_t possible() const {
    return (mask + bottom_mask) & board_mask;
  }
};