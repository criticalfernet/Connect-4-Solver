#include <iostream>
#include "Board.hpp"
#include "solver.hpp"
#include "Alphabeta.hpp"

using namespace std;

void clearScreen()
{
  cout << "\033[2J\033[H";
}

int main() {
  Board board("6133112551");
  board.print();
  cout << "Score: " << solve(board) << endl;
  int col;

  while (true){
    cin >> col;

    if (!cin || !board.canPlay(col)) {
      cout << "Invalid move..\n";
      continue;
    }

    clearScreen();

    int player = board.getPlayer();
    bool win = board.isWinningMove(col);

    board.play(col);
    board.print();

    if (win){
      cout << "Player " << player << " wins!" << endl;
      break;
    }
    cout << "Score: " << solve(board) << endl;
  }

  return 0;
}