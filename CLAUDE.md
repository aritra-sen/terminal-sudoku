# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
make        # build
make run    # build and run
make test   # build and run unit tests (no ncurses required)
make clean  # remove binary, object files, and test binary
```

**Dependency:** `ncurses` library must be installed (only needed for the game build, not tests).

## Testing

Tests use [doctest](https://github.com/doctest/doctest) v2.4.11 (single header at `tests/doctest.h`).

The test build compiles `game_logic.cpp` and `engine.cpp` only — no `tui.cpp`, no `main.cpp`, no `-lncurses`. The `-DTEST_BUILD` flag:
- Excludes `#include "tui.h"` in `engine.h` (replaced by inline `KEY_*` constant definitions)
- Excludes the `handleInput` event loop in `engine.cpp` (it calls ncurses functions)

**Known gotchas:**
- `isValidEntry(board, r, c, 0)` returns `false` — empty cells have value 0, and `0 % 10 == 0` matches val=0. This is expected; val=0 is never passed in normal game flow.
- `isUniquelySolvable` on an all-zero board will hang — the backtracker explores too many paths before finding 2 solutions. Do not add tests for that case.

## Architecture

The game is split across four source files under `src/`:

```
src/
  main.cpp              # ncurses init, color setup, board init, game loop call
  engine/
    engine.h/cpp        # Input handling (handleInput) and pure-logic helpers
  game_logic/
    game_logic.h/cpp    # Sudoku rules, board generation, solver
  tui/
    tui.h/cpp           # ncurses rendering (printBoard)
```

### Board State Encoding

The 9x9 `board[ROWS][COLS]` uses a dual-range encoding:
- Values **1–9**: prefilled clues (cannot be edited by the user)
- Values **11–19**: user-entered digits (digit = value - 10)
- Value **0**: empty cell

### Major Components

**Board Generation Pipeline**
1. `generateBoardImpl()` — recursive backtracking fills a complete valid board
2. `generateSolvableBoard()` — removes clues from the solved board, checking `isUniquelySolvable()` after each removal; capped at 58 empty cells to keep solution-checking fast
3. `isUniquelySolvable()` — brute-force check that only one solution exists

**Rendering (`printBoard`)**
Uses ncurses ACS characters for box-drawing. Color pair constants are defined in `tui.h`:
- `DEFAULT_COLOR` — empty cells
- `PREFILLED_COLOR` — blue, clue cells
- `WRONG_COLOR` — red, incorrect user entry
- `RIGHT_COLOR` — green, correct user entry

**Input Loop (`handleInput`)**
Handles arrow keys (cursor movement), digit keys 1–9 (place number), Backspace/Delete (clear cell), and `X` (quit). Delegates to four pure-logic helpers (all testable without ncurses):
- `screenToBoardIndex(cursor)` — converts TUI screen coordinates to board array indices
- `applyDigit(board, cursor, digit)` — encodes and places a digit; no-ops on prefilled clues
- `clearCell(board, cursor)` — clears a user-entered cell; no-ops on prefilled clues
- `moveCursor(cursor, direction)` — moves cursor, skipping grid-line rows/cols and clamping to bounds

**`Pair` struct** — tracks cursor position as (row, col).
