#include "tui.h"

extern Pair curr;  // defined in engine.cpp

WINDOW *win;

void printBoard(short board[][COLS]) {
    win = newwin(ROWS + VCELLS + 1, COLS + HCELLS + 1, 0, 0);
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
                    mvwaddch(win, i + (i-1)/N, j + (j-1)/N, digit);
                    wattroff(win, A_BOLD);
                } else {
                    int num = board[i-1][j-1];
                    board[i-1][j-1] = 0;
                    if(isValidEntry(board, i-1, j-1, num%10)) {
                        wattron(win, COLOR_PAIR(RIGHT_COLOR));
                    } else {
                        wattron(win, COLOR_PAIR(WRONG_COLOR));
                    }
                    mvwaddch(win, i + (i-1)/N, j + (j-1)/N, digit);
                    board[i-1][j-1] = num;
                }
            }
        }
    }
    wmove(win, curr.row, curr.col);
    wrefresh(win);
}
