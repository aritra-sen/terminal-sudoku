#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <ncurses.h>

#define N 3
#define ROWS N*N
#define COLS N*N
#define HCELLS N
#define VCELLS N

#define DEFAULT_COLOR 1
#define PREFILLED_COLOR 2
#define WRONG_COLOR 3
#define RIGHT_COLOR 4


struct Pair {
    short row,col;

    Pair() {
        row = 0;
        col = 0;
    }

    Pair(short r, short c) : row(r), col(c) {};
};

// Current cursor position
Pair curr = Pair(1,1);

// Grid window
WINDOW *win;

// Tells us if a cell insertion is valid.
bool isValidEntry(short board[][COLS], short r, short c, short val);

// Swap 2 Indexs in an array
template<typename T>
void swap(T order[], short i, short j);

// Randomize a given array of length `len`
template <typename T> 
void randomizeArray(T order[], short len);

// Backtracking implementation to generate a random sudoku board.
bool generateBoardImpl(short board[ROWS][COLS], short r, short c);

// Helper Prshort function.
void printBoard(short board[][COLS]);

// Generate a N*N sudoku board.
void generateSudokuBoard(short board[][COLS]);

// Check if given sudoku board has only 1 unique solution.
bool isUniquelySolvable(short board[ROWS][COLS], short r, short c);

// Given a solved sudoku board, generate a solvable puzzle board.
void generateSolvableBoard(short board[][COLS]);

// Handles input from the user.
void handleInput(short board[][COLS]);

int main() {
    initscr();
    start_color();
    keypad(stdscr, TRUE);

    init_color(COLOR_BLACK, 0, 0, 0);

    init_pair(DEFAULT_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(PREFILLED_COLOR, COLOR_BLUE, COLOR_BLACK);
    init_pair(WRONG_COLOR, COLOR_WHITE, COLOR_RED);
    init_pair(RIGHT_COLOR, COLOR_WHITE, COLOR_GREEN);

    bkgd(COLOR_PAIR(DEFAULT_COLOR));
    short board[ROWS][COLS];
    for(short r=0;r<ROWS;r++) {
        for(short c=0;c<COLS;c++) {
            board[r][c] = 0;
        }
    }

    generateSolvableBoard(board);
    printBoard(board);

    handleInput(board);

    endwin();
    return 0;
}


void generateSudokuBoard(short board[][COLS]) {
    generateBoardImpl(board, 0, 0);
}


bool generateBoardImpl(short board[ROWS][COLS], short r, short c) {
    if(c == COLS) {
        return generateBoardImpl(board, r + 1, 0);
    }
    if(r == ROWS) {
        return true;
    }

    // Board has numbers from 1 - N*N.
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

void printBoard(short board[][COLS]) {
    win = newwin(ROWS + VCELLS + 1, COLS + HCELLS + 1, 0,0);
    refresh();
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(DEFAULT_COLOR));

    for(short r = 1; r < VCELLS; r++) {
        mvwaddch(win, r * (N+1), 0, ACS_LTEE);
        for(short c = 1; c <= COLS + HCELLS - 1; c++) {
            mvwaddch(win, r*(N+1), c, ACS_HLINE);
        }
        mvwaddch(win, r * (N+1), COLS + HCELLS, ACS_RTEE);
    }

    for(short c = 1; c < HCELLS; c++) {
        mvwaddch(win, 0, c *(N+1), ACS_TTEE);
        for(short r = 1; r <= ROWS + VCELLS - 1; r++) {
            mvwaddch(win, r, c*(N+1), ACS_VLINE);
        }
        mvwaddch(win, ROWS + VCELLS, c *(N+1), ACS_BTEE);
    }
    for(short r = 1; r < VCELLS; r++) {
        for(short c = 1; c < HCELLS; c++) {
            mvwaddch(win, r*(N+1), c*(N+1), ACS_PLUS);
        }
    }


    for(short i = 1; i <= ROWS; i++) {
        for(short j = 1; j <= COLS; j++) {
            if(board[i-1][j-1] != 0) {
                char digit = (char)('0' + board[i-1][j-1]%10);
                if(board[i-1][j-1] < 10) {
                    wattron(win, A_BOLD);
                    wattron(win, COLOR_PAIR(PREFILLED_COLOR));
                    mvwaddch(win, i + (i-1)/N,j + (j-1)/N, digit);
                    wattroff(win, A_BOLD);
                } else {
                    int num = board[i-1][j-1];
                    board[i-1][j-1] = 0;
                    if(isValidEntry(board, i-1,j-1, num%10)) {
                        wattron(win, COLOR_PAIR(RIGHT_COLOR));
                    } else {
                        wattron(win, COLOR_PAIR(WRONG_COLOR));
                    }
                    mvwaddch(win, i + (i-1)/N,j + (j-1)/N, digit);
                    board[i-1][j-1] = num;
                }
            }
        }
    }
    wmove(win,curr.row,curr.col);
    wrefresh(win);
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
    Pair idxs[ROWS*COLS];
    short it = 0;
    for(short i = 0; i < N*N; i++) {
        for(short j = 0; j < N*N; j++) {
            idxs[it++] = Pair(i,j);
        }
    }
    randomizeArray<Pair>(idxs, ROWS*COLS);
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

void handleInput(short board[][COLS]) {
    bool gameOn = true;
    while(gameOn) {
        int ch = getch();
        switch(ch) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                {
                    int idxR = curr.row - (curr.row/4) - 1, idxC = curr.col - (curr.col/4) - 1;
                    if(board[idxR][idxC] != 0 && board[idxR][idxC] < 10) {
                        clear();
                        printBoard(board);
                        break;
                    }
                    board[idxR][idxC] = ch - '0' + 10;
                    clear();
                    printBoard(board);
                    break;
                }
            case KEY_DC:
            case KEY_BACKSPACE:
                {
                    int idxR = curr.row - (curr.row/4) - 1, idxC = curr.col - (curr.col/4) - 1;
                    if(board[idxR][idxC] != 0 && board[idxR][idxC] < 10) {
                        clear();
                        printBoard(board);
                        break;
                    }
                    board[idxR][idxC] = 0;
                    clear();
                    printBoard(board);
                    break;
                }                
            case KEY_UP:
                if(curr.row > 1) {
                    curr.row--;
                    if(curr.row % 4 == 0 ) {
                        curr.row--;
                    }
                }
                clear();
                printBoard(board);                
                break;
            case KEY_DOWN:
                if(curr.row < ROWS + VCELLS - 1) {
                    curr.row++;
                    if(curr.row % 4 == 0 ) {
                        curr.row++;
                    }
                }
                clear();
                printBoard(board);                
                break;
            case KEY_LEFT:
                if(curr.col > 1) {
                    curr.col--;
                    if(curr.col % 4 == 0 ) {
                        curr.col--;
                    }
                }
                clear();
                printBoard(board);                
                break;
            case KEY_RIGHT:
                if(curr.col < COLS + HCELLS - 1) {
                    curr.col++;
                    if(curr.col % 4 == 0 ) {
                        curr.col++;
                    }
                }
                clear();
                printBoard(board);                
                break;
            case 'X':
                gameOn = false;
                break;
            default:
                clear();
                printBoard(board);
                break;
        }
    }
}