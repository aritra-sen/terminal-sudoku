#include <stdio.h>
#include<iostream>
#include<string.h>

#define N 3

bool isValid(int board[][N*N], int r, int c, int val) {
    for(int i = 0; i < N*N; i++) {
        if(board[r][i] == val) {
            return false;
        }
    }
    for(int i = 0; i < N*N; i++) {
        if(board[i][c] == val) {
            return false;
        }
    }
    int boxR = r / N;
    int boxC = c / N;
    for(int i = boxR * N ; i < (boxR + 1) * N; i++) {
        for(int j = boxC * N; j < (boxC + 1) * N; j++) {
            if(board[i][j] == val) {
                return false;
            }
        }
    }
    return true;
}

void swap(int order[], int i, int j) {
    int temp = order[i];
    order[i] = order[j];
    order[j] = temp;
}

void generateRandomOrder(int order[]) {
    for(int i = 0; i < N*N; i++) {
        order[i] = i + 1;
    }

    for(int i = N*N - 1; i > 0 ; i--) {
        int ridx = rand() % (i + 1);
        swap(order, i, ridx);
    }
}

void printBoard(int board[][N*N]) {
    for(int i = 0; i < N*N; i++) {
        for(int j = 0; j < N*N; j++) {
            std::cout<<board[i][j] << " ";
        }
        std::cout<<std::endl;
    }
}

bool generateImpl(int board[N*N][N*N], int r, int c) {
    if(c == N*N) {
        return generateImpl(board, r + 1, 0);
    }
    if(r == N*N) {
        return true;
    }
    //std::cout<<"r = "<<r <<"c = "<<c<<std::endl;
    // printBoard(board);
    int order[N*N]; 
    generateRandomOrder(order);

    for(int i = 0; i < N*N; i++) {
        if(isValid(board, r, c, order[i])) {
            board[r][c] = order[i];
            if(generateImpl(board,r,c+1)) {
                return true;
            }
            board[r][c] = 0;
        }
    }
    return false;
}

void generateSudokuBoard() {
    int board[N*N][N*N];
    for(int r=0;r<N*N;r++) {
        for(int c=0;c<N*N;c++) {
            board[r][c] = 0;
        }
    }

    generateImpl(board, 0, 0);
    printBoard(board);
}

int main() {
    srand (time(NULL)); 
    for(int i = 0; i < 10; i++) {
        generateSudokuBoard();
        std::cout<<"======================================"<<std::endl;
    }
    return 0;
}