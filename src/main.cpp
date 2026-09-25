#include <iostream>
#include "headers/Board.h"
#include "headers/negamax.h"

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

int main() {
    Board board;

    int col;

    while (true) {
        std::cin >> col;

        clearScreen();

        int player = board.currentPlayer;
        bool win = board.isWinningMove(col);

        board.play(col);
        board.print();

        if (win) {
            std::cout << "Player " << player << " wins!" << std::endl;
            break;
        }
        std::cout << "Score: " << negamax(board) << std::endl;
    }

    return 0;
}