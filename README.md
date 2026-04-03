# Terminal Sudoku

A Sudoku game that runs in your terminal, built with C++ and ncurses.

## Features

- Randomly generated puzzles with a unique solution guaranteed
- Real-time validation — incorrect entries highlighted in red, correct in green
- Prefilled clues shown in blue and protected from editing
- Keyboard-driven navigation

## Requirements

- C++ compiler (g++)
- [ncurses](https://invisible-island.net/ncurses/) library

**macOS**
```bash
brew install ncurses
```

**Debian/Ubuntu**
```bash
sudo apt install libncurses-dev
```

## Build & Run

```bash
make        # build
make run    # build and run
make test   # build and run unit tests (no ncurses required)
make clean  # remove binary, object files, and test binary
```

## Controls

| Key | Action |
|-----|--------|
| Arrow keys | Move cursor |
| `1`–`9` | Place digit |
| `Backspace` / `Delete` | Clear cell |
| `X` | Quit |

## Project Structure

```
src/
  main.cpp              # Entry point
  engine/
    engine.h/cpp        # Input handling and game state (cursor)
  game_logic/
    game_logic.h/cpp    # Sudoku rules, board generation, solver
  tui/
    tui.h/cpp           # ncurses rendering
tests/
  doctest.h             # Single-header test framework (doctest v2.4.11)
  test_game_logic.cpp   # Tests for board generation, validation, solver
  test_engine_logic.cpp # Tests for cursor math and board state transitions
  test_main_constants.cpp # Static assertions for encoding constants
```

## How It Works

**Board generation** — a complete valid board is built via recursive backtracking with randomized cell ordering, then clues are removed one by one. After each removal the board is checked for unique solvability. Generation stops when 58 clues have been removed (beyond that, uniqueness checking becomes slow).

**Board encoding** — the 9×9 board uses a dual-range integer encoding:
- `1–9`: prefilled clues (immutable)
- `11–19`: user-entered digits (digit = value − 10)
- `0`: empty cell

**Validation** — each user entry is checked against standard Sudoku constraints (row, column, 3×3 box) in real time.
