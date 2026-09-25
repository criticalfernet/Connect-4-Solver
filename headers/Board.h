#pragma once
#include <string>

class Board {
public:
    static constexpr int WIDTH  = 7;
    static constexpr int HEIGHT = 6;

    explicit Board(const std::string& moves);

    unsigned int countMoves() const;
    int getPlayer() const;
    bool canPlay(int col) const;
    void play(int col);

    bool isWinningMove(int col) const;
    int getScore() const;

    void print() const;

private:
    int board[HEIGHT][WIDTH]{};
    unsigned int moves = 0;
    int currentPlayer = 1;
};