#include "game_logic.h"
#include <unistd.h>
#include <time.h>

bool isValidEntry(short board[][COLS], short r, short c, short val) {
    for(short i = 0; i < COLS; i++) {
        if(board[r][i]%10 == val) {
            return false;
        }
    }
    for(short i = 0; i < ROWS; i++) {
        if(board[i][c]%10 == val) {
            return false;
        }
    }
    short boxR = r / N;
    short boxC = c / N;
    for(short i = boxR * N ; i < (boxR + 1) * N; i++) {
        for(short j = boxC * N; j < (boxC + 1) * N; j++) {
            if(board[i][j]%10 == val) {
                return false;
            }
        }
    }
    return true;
}

bool generateBoardImpl(short board[ROWS][COLS], short r, short c) {
    if(c == COLS) {
        return generateBoardImpl(board, r + 1, 0);
    }
    if(r == ROWS) {
        return true;
    }

    short order[N*N];
    for(short i = 0; i < N*N; i++) {
        order[i] = i + 1;
    }
    randomizeArray<short>(order, N*N);

    for(short i = 0; i < N*N; i++) {
        if(isValidEntry(board, r, c, order[i])) {
            board[r][c] = order[i];
            if(generateBoardImpl(board, r, c+1)) {
                return true;
            }
            board[r][c] = 0;
        }
    }
    return false;
}

void generateSudokuBoard(short board[][COLS]) {
    generateBoardImpl(board, 0, 0);
}

bool isUniquelySolvable(short board[ROWS][COLS], short r, short c) {
    if(c == N*N) {
        return isUniquelySolvable(board, r + 1, 0);
    }
    if(r == N*N) {
        return true;
    }
    if(board[r][c] != 0) return isUniquelySolvable(board, r, c+1);

    short order[N*N];
    for(short i = 0; i < N*N; i++) {
        order[i] = i + 1;
    }
    randomizeArray<short>(order, N*N);
    short sols = 0;
    for(short i = 0; i < N*N; i++) {
        if(!isValidEntry(board, r, c, order[i])) {
            continue;
        }
        board[r][c] = order[i];
        if(isUniquelySolvable(board, r, c+1)) {
            sols++;
            if(sols > 1) {
                break;
            }
        }
    }
    board[r][c] = 0;
    return sols == 1;
}

void generateSolvableBoard(short board[][N*N]) {
    srand(time(NULL) + 1000 * getpid());

    generateSudokuBoard(board);
    Pair idxs[ROWS*COLS];
    short it = 0;
    for(short i = 0; i < N*N; i++) {
        for(short j = 0; j < N*N; j++) {
            idxs[it++] = Pair(i, j);
        }
    }
    randomizeArray<Pair>(idxs, ROWS*COLS);
    short count = 0;
    for(Pair idx : idxs) {
        short val = board[idx.row][idx.col];
        board[idx.row][idx.col] = 0;
        if(!isUniquelySolvable(board, 0, 0)) {
            board[idx.row][idx.col] = val;
        } else {
            count++;
        }
        // capping it at 58, since the code gets slower after this.
        if(count == 58) {
            return;
        }
    }
}
