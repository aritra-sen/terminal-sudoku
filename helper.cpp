#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>

#define N 3

struct Pair {
    short row,col;

    Pair() {
        row = 0;
        col = 0;
    }

    Pair(short r, short c) : row(r), col(c) {};
};

// Tells us if a cell insertion is valid.
bool isValidEntry(short board[][N*N], short r, short c, short val);

// Swap 2 Indexs in an array
template<typename T>
void swap(T order[], short i, short j);

// Randomize a given array of length `len`
template <typename T> 
void randomizeArray(T order[], short len);

// Backtracking implementation to generate a random sudoku board.
bool generateBoardImpl(short board[N*N][N*N], short r, short c);

// Helper Prshort function.
void printBoard(short board[][N*N]);

// Generate a N*N sudoku board.
void generateSudokuBoard(short board[][N*N]);

// Check if given sudoku board has only 1 unique solution.
bool isUniquelySolvable(short board[N*N][N*N], short r, short c);

// Given a solved sudoku board, generate a solvable puzzle board.
void generateSolvableBoard(short board[][N*N]);

int main() {
    short board[N*N][N*N];
    for(short r=0;r<N*N;r++) {
        for(short c=0;c<N*N;c++) {
            board[r][c] = 0;
        }
    }

    generateSolvableBoard(board);
    std::cout<<"Problem statement\n";
    printBoard(board);
    return 0;
}


void generateSudokuBoard(short board[][N*N]) {
    generateBoardImpl(board, 0, 0);
    printBoard(board);
}


bool generateBoardImpl(short board[N*N][N*N], short r, short c) {
    if(c == N*N) {
        return generateBoardImpl(board, r + 1, 0);
    }
    if(r == N*N) {
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
            if(generateBoardImpl(board,r,c+1)) {
                return true;
            }
            board[r][c] = 0;
        }
    }
    return false;
}

bool isValidEntry(short board[][N*N], short r, short c, short val) {
    for(short i = 0; i < N*N; i++) {
        if(board[r][i] == val) {
            return false;
        }
    }
    for(short i = 0; i < N*N; i++) {
        if(board[i][c] == val) {
            return false;
        }
    }
    short boxR = r / N;
    short boxC = c / N;
    for(short i = boxR * N ; i < (boxR + 1) * N; i++) {
        for(short j = boxC * N; j < (boxC + 1) * N; j++) {
            if(board[i][j] == val) {
                return false;
            }
        }
    }
    return true;
}

template<typename T>
void swap(T order[], short i, short j) {
    T temp = order[i];
    order[i] = order[j];
    order[j] = temp;
}

template<typename T>
void randomizeArray(T order[], short len) {
    for(short i = len - 1; i > 0 ; i--) {
        short ridx = rand() % (i + 1);
        swap<T>(order, i, ridx);
    }
}

void printBoard(short board[][N*N]) {
    for(short i = 0; i < N*N; i++) {
        for(short j = 0; j < N*N; j++) {
            if(board[i][j] == 0) {
                std::cout<<"- ";                
            } else {
                std::cout<<board[i][j] << " ";
            }
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

bool isUniquelySolvable(short board[N*N][N*N], short r, short c) {
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
        if(isUniquelySolvable(board,r,c+1)) {
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
    srand (time(NULL) + 1000 * getpid()); 

    generateSudokuBoard(board);
    Pair idxs[N*N*N*N];
    short it = 0;
    for(short i = 0; i < N*N; i++) {
        for(short j = 0; j < N*N; j++) {
            idxs[it++] = Pair(i,j);
        }
    }
    randomizeArray<Pair>(idxs, N*N*N*N);
    short count = 0;
    for(Pair idx : idxs) {
        short val = board[idx.row][idx.col];
        board[idx.row][idx.col] = 0;
        if(!isUniquelySolvable(board, 0,0)) {
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