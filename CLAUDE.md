# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Run

```bash
make        # build
make run    # build and run
make clean  # remove binary
```

**Dependency:** `ncurses` library must be installed.

## Architecture

The entire game lives in a single file: `helper.cpp`.

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
Uses ncurses ACS characters for box-drawing. Color pairs are constants at the top of the file:
- `DEFAULT` — empty cells
- `PREFILLED` — blue, clue cells
- `WRONG` — red, incorrect user entry
- `RIGHT` — green, correct user entry

**Input Loop (`handleInput`)**
Handles arrow keys (cursor movement), digit keys 1–9 (place number), Backspace/Delete (clear cell), and `X` (quit). Validates moves against Sudoku rules in real-time.

**`Pair` struct** — tracks cursor position as (row, col).
