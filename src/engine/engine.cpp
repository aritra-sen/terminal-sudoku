#include "engine.h"

Pair curr = Pair(1, 1);

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
                    int idxR = curr.row - (curr.row/(N+1)) - 1, idxC = curr.col - (curr.col/(N+1)) - 1;
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
            case '\b':   // ASCII 8: Ctrl+H / some terminals' backspace
            case 127:    // ASCII 127 (DEL): macOS terminals send this for Backspace key
                {
                    int idxR = curr.row - (curr.row/(N+1)) - 1, idxC = curr.col - (curr.col/(N+1)) - 1;
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
                    if(curr.row % (N+1) == 0) {
                        curr.row--;
                    }
                }
                clear();
                printBoard(board);
                break;
            case KEY_DOWN:
                if(curr.row < ROWS + VCELLS - 1) {
                    curr.row++;
                    if(curr.row % (N+1) == 0) {
                        curr.row++;
                    }
                }
                clear();
                printBoard(board);
                break;
            case KEY_LEFT:
                if(curr.col > 1) {
                    curr.col--;
                    if(curr.col % (N+1) == 0) {
                        curr.col--;
                    }
                }
                clear();
                printBoard(board);
                break;
            case KEY_RIGHT:
                if(curr.col < COLS + HCELLS - 1) {
                    curr.col++;
                    if(curr.col % (N+1) == 0) {
                        curr.col++;
                    }
                }
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
