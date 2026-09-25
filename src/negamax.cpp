#include "headers/negamax.h"

int negamax(const Board &board, int depth)
{

    if (depth == 0) return 0;
    if(board.countMoves() == Board::WIDTH*Board::HEIGHT) return 0;

    for(int x = 0; x < Board::WIDTH; x++) {
        if(board.canPlay(x) && board.isWinningMove(x)) {
            return board.getScore();
        }
    }

    int bestScore = -Board::WIDTH * Board::HEIGHT;

    for(int x = 0; x < Board::WIDTH; x++) {
        if(board.canPlay(x)) {
            Board b2(board);
            b2.play(x);
            int score = -negamax(b2, depth-1);
            if(score > bestScore) bestScore = score;
        }
    }

    return bestScore;
}


int MAX_DEPTH = 5;

int negamax(const Board &board)
{
    return negamax(board, MAX_DEPTH);
}