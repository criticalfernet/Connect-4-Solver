#include "alphabetaMO1.h"

constexpr int MAX_DEPTH = 10;

int alphabetaMO1(const Board &board, int depth, int alpha, int beta)
{
    if (depth == 0) return 100;
    if (board.countMoves() == Board::WIDTH * Board::HEIGHT) return 0;

    for (int x = 1; x <= Board::WIDTH; x++) {
        if (board.canPlay(x) && board.isWinningMove(x)) {
            return board.getScore();
        }
    }

    int bestScore = -Board::WIDTH * Board::HEIGHT;

    const int moveOrder[Board::WIDTH] = {4, 3, 5, 2, 6, 1, 7};

    for (int i = 0; i < Board::WIDTH; i++) {
        int x = moveOrder[i];
        if (board.canPlay(x)) {
            Board b2(board);
            b2.play(x);

            int score = -alphabetaMO1(b2, depth - 1, -beta, -alpha);

            if (score > bestScore) bestScore = score;
            if (score > alpha) alpha = score;

            if (alpha >= beta) break;
        }
    }
    return bestScore;
}


int alphabetaMO1(const Board &board)
{
    return alphabetaMO1(
        board,
        MAX_DEPTH,
        -Board::WIDTH * Board::HEIGHT,
        Board::WIDTH * Board::HEIGHT
    );
}