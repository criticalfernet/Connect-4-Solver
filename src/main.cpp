#include <iostream>
#include "Board.h"
#include "alphabetaMO1.h"

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

int main() {
    Board board("676352563513445344436141267136571");
    board.print();
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
        std::cout << "Score: " << alphabetaMO1(board) << std::endl;
    }

    return 0;
}