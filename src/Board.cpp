#include "headers/Board.h"
#include "iostream"

bool Board::canPlay(int col) const {
    if (col < 1 || col > WIDTH)
        return false;

    return board[HEIGHT - 1][col - 1] == 0;
}

void Board::play(int col) {
    if (!canPlay(col))
        return;

    for (int row = 0; row < HEIGHT; ++row) {
        if (board[row][col - 1] == 0) {
            board[row][col - 1] = currentPlayer;
            currentPlayer = 3 - currentPlayer;
            moves++;
            return;
        }
    }
}

bool Board::isWinningMove(int col) const {
    if (!canPlay(col))
        return false;

    int c = col - 1;

    int row = 0;
    while (row < HEIGHT && board[row][c] != 0)
        ++row;

    int player = currentPlayer;

    const int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    for (const auto& direction : directions) {
        int count = 1;

        for (int i = 1; i < 4; ++i) {
            int r = row + direction[1] * i;
            int c2 = c + direction[0] * i;

            if (r < 0 || r >= HEIGHT || c2 < 0 || c2 >= WIDTH)
                break;

            if (board[r][c2] != player)
                break;

            ++count;
        }

        for (int i = 1; i < 4; ++i) {
            int r = row - direction[1] * i;
            int c2 = c - direction[0] * i;

            if (r < 0 || r >= HEIGHT || c2 < 0 || c2 >= WIDTH)
                break;

            if (board[r][c2] != player)
                break;

            ++count;
        }

        if (count >= 4)
            return true;
    }

    return false;
}

void Board::print() const {
    std::cout << std::endl;
    std::cout << "===============================\n";

    for (int row = HEIGHT - 1; row >= 0; --row) {
        std::cout << "|";

        for (int col = 0; col < WIDTH; ++col) {
            char piece = ' ';

            if (board[row][col] == 1)
                piece = 'x';
            else if (board[row][col] == 2)
                piece = 'o';

            std::cout << " " << piece << " |";
        }

        std::cout << "\n";
    }

    std::cout << "===============================\n";
}

unsigned int Board::countMoves() const {
    return moves;
} 


int Board::getScore() const {
    return (Board::WIDTH*Board::HEIGHT + 1 - this->countMoves())/2;
}