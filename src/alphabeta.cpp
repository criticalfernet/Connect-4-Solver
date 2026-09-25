#include "alphabeta.h"

constexpr int MAX_DEPTH = 30;

int alphabeta(const Board &board, int depth, int alpha, int beta)
{
    if (depth == 0) return 0;
    if (board.countMoves() == Board::WIDTH * Board::HEIGHT) return 0;

    for (int x = 1; x <= Board::WIDTH; x++) {
        if (board.canPlay(x) && board.isWinningMove(x)) {
            return board.getScore();
        }
    }

    int bestScore = -Board::WIDTH * Board::HEIGHT;

    for (int x = 1; x <= Board::WIDTH; x++) {
        if (board.canPlay(x)) {
            Board b2(board);
            b2.play(x);

            int score = -alphabeta(b2, depth - 1, -beta, -alpha);

            if (score > bestScore) bestScore = score;
            if (score > alpha) alpha = score;

            if (alpha >= beta) break;
        }
    }
    return bestScore;
}


int alphabeta(const Board &board)
{
    return alphabeta(
        board,
        MAX_DEPTH,
        -Board::WIDTH * Board::HEIGHT,
        Board::WIDTH * Board::HEIGHT
    );
}