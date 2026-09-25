#include <iostream>
#include "Board.h"
#include "alphabeta.h"

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

int main() {
    Board board("");

    int col;

    while (true) {
        std::cin >> col;

        clearScreen();

        int player = board.getPlayer();
        bool win = board.isWinningMove(col);

        board.play(col);
        board.print();

        if (win) {
            std::cout << "Player " << player << " wins!" << std::endl;
            break;
        }
        std::cout << "Score: " << alphabeta(board) << std::endl;
    }

    return 0;
}