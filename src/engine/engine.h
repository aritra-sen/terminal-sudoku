#ifndef ENGINE_H
#define ENGINE_H

#ifndef TEST_BUILD
#include "tui.h"      // must come before game_logic.h (ncurses.h must see COLS before our macro)
#else
// Provide ncurses key constants for test builds (octal values matching ncurses)
#define KEY_UP    0403
#define KEY_DOWN  0402
#define KEY_LEFT  0404
#define KEY_RIGHT 0405
#endif

#include "game_logic.h"

extern Pair curr;

Pair screenToBoardIndex(Pair cursor);
bool applyDigit(short board[][COLS], Pair cursor, short digit);
bool clearCell(short board[][COLS], Pair cursor);
Pair moveCursor(Pair cursor, int direction);

#ifndef TEST_BUILD
void handleInput(short board[][COLS]);
#endif

#endif
