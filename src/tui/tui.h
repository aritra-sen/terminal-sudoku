#ifndef TUI_H
#define TUI_H

#include <ncurses.h>
#include "game_logic.h"

#define DEFAULT_COLOR 1
#define PREFILLED_COLOR 2
#define WRONG_COLOR 3
#define RIGHT_COLOR 4

void printBoard(short board[][COLS]);

#endif
