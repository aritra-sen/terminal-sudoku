#ifndef ENGINE_H
#define ENGINE_H

#include "tui.h"      // must come before game_logic.h (ncurses.h must see COLS before our macro)
#include "game_logic.h"

extern Pair curr;

void handleInput(short board[][COLS]);

#endif
