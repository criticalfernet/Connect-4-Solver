#pragma once

class Board {
public:
    static constexpr int WIDTH = 7;
    static constexpr int HEIGHT = 6;

    unsigned int countMoves() const;
    bool canPlay(int col) const;
    void play(int col);
    bool isWinningMove(int col) const;
    void print() const;
    
    int getScore() const;
    int currentPlayer = 1;

private:
    int board[HEIGHT][WIDTH] = {};
    unsigned int moves = 0;
};