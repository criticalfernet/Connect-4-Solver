#include <iostream>
#include "Board.hpp"
#include "Solver.hpp"
#include "Alphabeta.hpp"
#include "Benchmark.hpp"

using namespace std;

int constexpr CURRENT = 1;
bool constexpr DEBUG = false;
bool constexpr WEAK = true;

void clearScreen(){
  cout << "\033[2J\033[H";
}

int getBestMove(Board &b){
  int bestMove = 0;
  int bestScore = Board::MIN_SCORE;

  for (int col = 1; col <= Board::WIDTH; col++) {
      if (!b.canPlay(col))continue;

      Board child(b);
      child.play(col);

      int score = -solve(child,WEAK);

      if (score > bestScore) {
          bestScore = score;
          bestMove = col;
      }
  }

  return bestMove;
}

void printStats(Board& board) {
  cout << "Score: " << solve(board) << endl;
  if(board.getPlayer() == CURRENT || DEBUG) 
    cout << "Best Move: " << getBestMove(board) << endl;
}

int play() {
  Board board("");
  board.print();
  printStats(board);
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
    printStats(board);
  }

  return 0;
}


int main(int argc, char* argv[])
{
    if (argc > 1 && string(argv[1]) == "bench")
      return runBenchmark();

    if (argc > 1 && string(argv[1]) == "benchTT")
      return runBenchmark(true);

    return play();
}