#include "doctest.h"
#include "engine.h"

// ─── Helper ──────────────────────────────────────────────────────────────────

static void zeroBoard(short board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = 0;
}

// ─── screenToBoardIndex ──────────────────────────────────────────────────────
//
// Valid screen rows/cols: 1–11, excluding 4 and 8 (grid lines at multiples of 4)
// Formula: idx = screen - (screen / (N+1)) - 1  where N=3, so N+1=4

TEST_CASE("screenToBoardIndex: top-left (1,1) -> board (0,0)") {
    Pair idx = screenToBoardIndex(Pair(1, 1));
    CHECK(idx.row == 0);
    CHECK(idx.col == 0);
}

TEST_CASE("screenToBoardIndex: top-right (1,11) -> board (0,8)") {
    Pair idx = screenToBoardIndex(Pair(1, 11));
    CHECK(idx.row == 0);
    CHECK(idx.col == 8);
}

TEST_CASE("screenToBoardIndex: bottom-left (11,1) -> board (8,0)") {
    Pair idx = screenToBoardIndex(Pair(11, 1));
    CHECK(idx.row == 8);
    CHECK(idx.col == 0);
}

TEST_CASE("screenToBoardIndex: bottom-right (11,11) -> board (8,8)") {
    Pair idx = screenToBoardIndex(Pair(11, 11));
    CHECK(idx.row == 8);
    CHECK(idx.col == 8);
}

TEST_CASE("screenToBoardIndex: (5,5) -> board (3,3) — first cell after first dividers") {
    Pair idx = screenToBoardIndex(Pair(5, 5));
    CHECK(idx.row == 3);
    CHECK(idx.col == 3);
}

TEST_CASE("screenToBoardIndex: (9,9) -> board (6,6) — first cell after second dividers") {
    Pair idx = screenToBoardIndex(Pair(9, 9));
    CHECK(idx.row == 6);
    CHECK(idx.col == 6);
}

TEST_CASE("screenToBoardIndex: all 81 valid screen positions map within [0,8]") {
    // Valid screen positions: rows/cols in 1..11 excluding 4 and 8
    int valid[] = {1, 2, 3, 5, 6, 7, 9, 10, 11};
    for (int r : valid) {
        for (int c : valid) {
            Pair idx = screenToBoardIndex(Pair(r, c));
            CHECK(idx.row >= 0);
            CHECK(idx.row <= 8);
            CHECK(idx.col >= 0);
            CHECK(idx.col <= 8);
        }
    }
}

// ─── moveCursor ──────────────────────────────────────────────────────────────

TEST_CASE("moveCursor: KEY_UP decrements row, skipping grid line 4") {
    // From row 5, moving up: decrement to 4, but 4%4==0 is a grid line, skip to 3
    Pair result = moveCursor(Pair(5, 5), KEY_UP);
    CHECK(result.row == 3);
    CHECK(result.col == 5);
}

TEST_CASE("moveCursor: KEY_UP skips grid-line row 8") {
    // From row 9, moving up should skip row 8 (8 % 4 == 0) and land on 7
    Pair result = moveCursor(Pair(9, 1), KEY_UP);
    CHECK(result.row == 7);
}

TEST_CASE("moveCursor: KEY_UP clamps at row 1") {
    Pair result = moveCursor(Pair(1, 1), KEY_UP);
    CHECK(result.row == 1);
}

TEST_CASE("moveCursor: KEY_DOWN increments row") {
    Pair result = moveCursor(Pair(2, 1), KEY_DOWN);
    CHECK(result.row == 3);
}

TEST_CASE("moveCursor: KEY_DOWN skips grid-line row 4") {
    // From row 3, moving down should skip row 4 (4 % 4 == 0) and land on 5
    Pair result = moveCursor(Pair(3, 1), KEY_DOWN);
    CHECK(result.row == 5);
}

TEST_CASE("moveCursor: KEY_DOWN skips grid-line row 8") {
    // From row 7, moving down should skip row 8 and land on 9
    Pair result = moveCursor(Pair(7, 1), KEY_DOWN);
    CHECK(result.row == 9);
}

TEST_CASE("moveCursor: KEY_DOWN clamps at row 11 (ROWS+VCELLS-1)") {
    // Max row = ROWS + VCELLS - 1 = 9 + 3 - 1 = 11
    Pair result = moveCursor(Pair(11, 1), KEY_DOWN);
    CHECK(result.row == 11);
}

TEST_CASE("moveCursor: KEY_LEFT decrements col") {
    Pair result = moveCursor(Pair(1, 2), KEY_LEFT);
    CHECK(result.col == 1);
}

TEST_CASE("moveCursor: KEY_LEFT skips grid-line col 4") {
    // From col 5, moving left should skip col 4 and land on 3
    Pair result = moveCursor(Pair(1, 5), KEY_LEFT);
    CHECK(result.col == 3);
}

TEST_CASE("moveCursor: KEY_LEFT clamps at col 1") {
    Pair result = moveCursor(Pair(1, 1), KEY_LEFT);
    CHECK(result.col == 1);
}

TEST_CASE("moveCursor: KEY_RIGHT increments col") {
    Pair result = moveCursor(Pair(1, 2), KEY_RIGHT);
    CHECK(result.col == 3);
}

TEST_CASE("moveCursor: KEY_RIGHT skips grid-line col 8") {
    Pair result = moveCursor(Pair(1, 7), KEY_RIGHT);
    CHECK(result.col == 9);
}

TEST_CASE("moveCursor: KEY_RIGHT clamps at col 11 (COLS+HCELLS-1)") {
    Pair result = moveCursor(Pair(1, 11), KEY_RIGHT);
    CHECK(result.col == 11);
}

TEST_CASE("moveCursor: does not change unrelated axis") {
    Pair result = moveCursor(Pair(5, 6), KEY_UP);
    CHECK(result.col == 6);
    result = moveCursor(Pair(5, 6), KEY_LEFT);
    CHECK(result.row == 5);
}

// ─── applyDigit ──────────────────────────────────────────────────────────────

TEST_CASE("applyDigit: writes digit+10 to empty cell") {
    short board[ROWS][COLS];
    zeroBoard(board);
    bool ok = applyDigit(board, Pair(1, 1), 5);
    CHECK(ok);
    CHECK(board[0][0] == 15);
}

TEST_CASE("applyDigit: digit 1 encodes to 11") {
    short board[ROWS][COLS];
    zeroBoard(board);
    applyDigit(board, Pair(1, 1), 1);
    CHECK(board[0][0] == 11);
}

TEST_CASE("applyDigit: digit 9 encodes to 19") {
    short board[ROWS][COLS];
    zeroBoard(board);
    applyDigit(board, Pair(1, 1), 9);
    CHECK(board[0][0] == 19);
}

TEST_CASE("applyDigit: overwrites existing user-entered value") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 13;  // user-entered 3
    bool ok = applyDigit(board, Pair(1, 1), 7);
    CHECK(ok);
    CHECK(board[0][0] == 17);
}

TEST_CASE("applyDigit: refuses to overwrite prefilled clue (value 1-9)") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 5;  // prefilled clue
    bool ok = applyDigit(board, Pair(1, 1), 3);
    CHECK_FALSE(ok);
    CHECK(board[0][0] == 5);  // unchanged
}

// ─── clearCell ───────────────────────────────────────────────────────────────

TEST_CASE("clearCell: sets user-entered cell to 0") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 14;
    bool ok = clearCell(board, Pair(1, 1));
    CHECK(ok);
    CHECK(board[0][0] == 0);
}

TEST_CASE("clearCell: refuses to clear prefilled clue") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 6;  // prefilled clue
    bool ok = clearCell(board, Pair(1, 1));
    CHECK_FALSE(ok);
    CHECK(board[0][0] == 6);  // unchanged
}

TEST_CASE("clearCell: no-op on already-empty cell") {
    short board[ROWS][COLS];
    zeroBoard(board);
    bool ok = clearCell(board, Pair(1, 1));
    CHECK(ok);
    CHECK(board[0][0] == 0);
}
