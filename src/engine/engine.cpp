#include "engine.h"

Pair curr = Pair(1, 1);

Pair screenToBoardIndex(Pair cursor) {
    short idxR = cursor.row - (cursor.row / (N + 1)) - 1;
    short idxC = cursor.col - (cursor.col / (N + 1)) - 1;
    return Pair(idxR, idxC);
}

bool applyDigit(short board[][COLS], Pair cursor, short digit) {
    Pair idx = screenToBoardIndex(cursor);
    if (board[idx.row][idx.col] != 0 && board[idx.row][idx.col] < 10) {
        return false;
    }
    board[idx.row][idx.col] = digit + 10;
    return true;
}

bool clearCell(short board[][COLS], Pair cursor) {
    Pair idx = screenToBoardIndex(cursor);
    if (board[idx.row][idx.col] != 0 && board[idx.row][idx.col] < 10) {
        return false;
    }
    board[idx.row][idx.col] = 0;
    return true;
}

Pair moveCursor(Pair cursor, int direction) {
    switch (direction) {
        case KEY_UP:
            if (cursor.row > 1) {
                cursor.row--;
                if (cursor.row % (N + 1) == 0) cursor.row--;
            }
            break;
        case KEY_DOWN:
            if (cursor.row < ROWS + VCELLS - 1) {
                cursor.row++;
                if (cursor.row % (N + 1) == 0) cursor.row++;
            }
            break;
        case KEY_LEFT:
            if (cursor.col > 1) {
                cursor.col--;
                if (cursor.col % (N + 1) == 0) cursor.col--;
            }
            break;
        case KEY_RIGHT:
            if (cursor.col < COLS + HCELLS - 1) {
                cursor.col++;
                if (cursor.col % (N + 1) == 0) cursor.col++;
            }
            break;
    }
    return cursor;
}

#ifndef TEST_BUILD
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
                applyDigit(board, curr, ch - '0');
                clear();
                printBoard(board);
                break;
            case KEY_DC:
            case KEY_BACKSPACE:
            case '\b':   // ASCII 8: Ctrl+H / some terminals' backspace
            case 127:    // ASCII 127 (DEL): macOS terminals send this for Backspace key
                clearCell(board, curr);
                clear();
                printBoard(board);
                break;
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                curr = moveCursor(curr, ch);
                clear();
                printBoard(board);
                break;
            case 'X':
            case 'x':
                gameOn = false;
                break;
            default:
                clear();
                printBoard(board);
                break;
        }
    }
}
#endif
