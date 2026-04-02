#include "engine.h"

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
    for(short r = 0; r < ROWS; r++) {
        for(short c = 0; c < COLS; c++) {
            board[r][c] = 0;
        }
    }

    generateSolvableBoard(board);
    printBoard(board);

    handleInput(board);

    endwin();
    return 0;
}
