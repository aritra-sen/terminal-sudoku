#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdlib.h>

#define N 3
#define ROWS N*N
#define COLS N*N
#define HCELLS N
#define VCELLS N

struct Pair {
    short row, col;

    Pair() {
        row = 0;
        col = 0;
    }

    Pair(short r, short c) : row(r), col(c) {};
};

bool isValidEntry(short board[][COLS], short r, short c, short val);

// Template implementations must be in header
template<typename T>
void swap(T order[], short i, short j) {
    T temp = order[i];
    order[i] = order[j];
    order[j] = temp;
}

template<typename T>
void randomizeArray(T order[], short len) {
    for(short i = len - 1; i > 0; i--) {
        short ridx = rand() % (i + 1);
        swap<T>(order, i, ridx);
    }
}

bool generateBoardImpl(short board[ROWS][COLS], short r, short c);
void generateSudokuBoard(short board[][COLS]);
bool isUniquelySolvable(short board[ROWS][COLS], short r, short c);
void generateSolvableBoard(short board[][COLS]);

#endif
