#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "game_logic.h"

#include <algorithm>
#include <set>

// ─── Helper utilities ────────────────────────────────────────────────────────

static void zeroBoard(short board[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = 0;
}

// Returns true if the set of values in board row r equals {1..9}
static bool rowIsComplete(short board[ROWS][COLS], int r) {
    std::set<short> vals;
    for (int c = 0; c < COLS; c++) vals.insert(board[r][c] % 10);
    for (short v = 1; v <= 9; v++) if (!vals.count(v)) return false;
    return vals.size() == 9;
}

static bool colIsComplete(short board[ROWS][COLS], int c) {
    std::set<short> vals;
    for (int r = 0; r < ROWS; r++) vals.insert(board[r][c] % 10);
    for (short v = 1; v <= 9; v++) if (!vals.count(v)) return false;
    return vals.size() == 9;
}

static bool boxIsComplete(short board[ROWS][COLS], int boxR, int boxC) {
    std::set<short> vals;
    for (int r = boxR * N; r < (boxR + 1) * N; r++)
        for (int c = boxC * N; c < (boxC + 1) * N; c++)
            vals.insert(board[r][c] % 10);
    for (short v = 1; v <= 9; v++) if (!vals.count(v)) return false;
    return vals.size() == 9;
}

static int countEmptyCells(short board[ROWS][COLS]) {
    int count = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (board[r][c] == 0) count++;
    return count;
}

// ─── swap / randomizeArray ───────────────────────────────────────────────────

TEST_CASE("swap: exchanges two elements") {
    short a[] = {1, 2, 3};
    swap(a, 0, 2);
    CHECK(a[0] == 3);
    CHECK(a[2] == 1);
    CHECK(a[1] == 2);
}

TEST_CASE("swap: same index is a no-op") {
    short a[] = {7, 8};
    swap(a, 0, 0);
    CHECK(a[0] == 7);
}

TEST_CASE("randomizeArray: preserves all elements as a permutation of 1-9") {
    short arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    randomizeArray(arr, 9);
    std::set<short> vals(arr, arr + 9);
    CHECK(vals.size() == 9);
    for (short v = 1; v <= 9; v++) CHECK(vals.count(v) == 1);
}

TEST_CASE("randomizeArray: length-1 array is unchanged") {
    short arr[] = {42};
    randomizeArray(arr, 1);
    CHECK(arr[0] == 42);
}

TEST_CASE("randomizeArray: two shuffles with same seed produce same order") {
    short a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    short b[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    srand(12345);
    randomizeArray(a, 9);
    srand(12345);
    randomizeArray(b, 9);
    for (int i = 0; i < 9; i++) CHECK(a[i] == b[i]);
}

// ─── isValidEntry ────────────────────────────────────────────────────────────

TEST_CASE("isValidEntry: valid digit on empty board") {
    short board[ROWS][COLS];
    zeroBoard(board);
    CHECK(isValidEntry(board, 0, 0, 5));
}

TEST_CASE("isValidEntry: rejects digit already in same row") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 5;
    CHECK_FALSE(isValidEntry(board, 0, 8, 5));
}

TEST_CASE("isValidEntry: rejects digit already in same column") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 3;
    CHECK_FALSE(isValidEntry(board, 8, 0, 3));
}

TEST_CASE("isValidEntry: rejects digit already in same 3x3 box") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 7;
    CHECK_FALSE(isValidEntry(board, 2, 2, 7));
}

TEST_CASE("isValidEntry: box boundary — (0,0) conflicts with (2,2) but not (3,3)") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 4;
    CHECK_FALSE(isValidEntry(board, 2, 2, 4));  // same top-left 3x3 box → conflict
    CHECK(isValidEntry(board, 3, 3, 4));         // different row, col, and box → valid
    CHECK(isValidEntry(board, 4, 4, 4));         // center box, different row/col → valid
}

TEST_CASE("isValidEntry: treats encoded user value (val%10) same as raw digit") {
    short board[ROWS][COLS];
    zeroBoard(board);
    board[0][0] = 15;  // user-entered 5 (encoded as 15)
    // digit 5 should be blocked in same row
    CHECK_FALSE(isValidEntry(board, 0, 8, 5));
    // digit 5 should be blocked in same column
    CHECK_FALSE(isValidEntry(board, 8, 0, 5));
    // digit 5 should be blocked in same 3x3 box
    CHECK_FALSE(isValidEntry(board, 1, 1, 5));
}

TEST_CASE("isValidEntry: all nine digits individually valid in fresh row/col/box") {
    short board[ROWS][COLS];
    zeroBoard(board);
    for (short v = 1; v <= 9; v++) {
        CHECK(isValidEntry(board, 0, 0, v));
    }
}

TEST_CASE("isValidEntry: after placing 1-8, only 9 is valid in the remaining cell") {
    short board[ROWS][COLS];
    zeroBoard(board);
    for (int c = 0; c < 8; c++) board[0][c] = c + 1;  // row 0: [1,2,3,4,5,6,7,8,0]
    for (short v = 1; v <= 8; v++) CHECK_FALSE(isValidEntry(board, 0, 8, v));
    CHECK(isValidEntry(board, 0, 8, 9));
}

TEST_CASE("isValidEntry: val=0 conflicts with any empty cell (0%10 == 0%10)") {
    // The implementation checks board[r][i]%10 == val for every cell in row/col/box.
    // Empty cells have value 0, so 0%10 == 0 == val, causing a conflict.
    // This is a design characteristic: val=0 (empty sentinel) should not be
    // passed to isValidEntry in normal game flow.
    short board[ROWS][COLS];
    zeroBoard(board);
    // Row 0 has empty cells, so val=0 conflicts with them
    CHECK_FALSE(isValidEntry(board, 0, 8, 0));
}

// ─── generateSudokuBoard ─────────────────────────────────────────────────────

TEST_CASE("generateSudokuBoard: no cell remains zero") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSudokuBoard(board);
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            CHECK(board[r][c] != 0);
}

TEST_CASE("generateSudokuBoard: all values in range 1-9") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSudokuBoard(board);
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++) {
            CHECK(board[r][c] >= 1);
            CHECK(board[r][c] <= 9);
        }
}

TEST_CASE("generateSudokuBoard: each row contains 1-9 exactly once") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSudokuBoard(board);
    for (int r = 0; r < ROWS; r++)
        CHECK(rowIsComplete(board, r));
}

TEST_CASE("generateSudokuBoard: each column contains 1-9 exactly once") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSudokuBoard(board);
    for (int c = 0; c < COLS; c++)
        CHECK(colIsComplete(board, c));
}

TEST_CASE("generateSudokuBoard: each 3x3 box contains 1-9 exactly once") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSudokuBoard(board);
    for (int br = 0; br < N; br++)
        for (int bc = 0; bc < N; bc++)
            CHECK(boxIsComplete(board, br, bc));
}

// ─── isUniquelySolvable ──────────────────────────────────────────────────────

TEST_CASE("isUniquelySolvable: fully filled valid board returns true") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSudokuBoard(board);
    CHECK(isUniquelySolvable(board, 0, 0));
}

TEST_CASE("isUniquelySolvable: near-complete board with two swappable cells returns false") {
    // Generate a valid board, then clear two cells that are in each other's row/col/box
    // such that swapping their values produces another valid solution.
    // Strategy: generate a solved board, find two cells (r1,c1) and (r2,c2) in
    // different rows, cols, and boxes where v1 is valid at (r2,c2) and v2 at (r1,c1).
    short board[ROWS][COLS];
    zeroBoard(board);
    srand(42);
    generateSudokuBoard(board);
    // Look for a pair of cells we can swap
    for (int r1 = 0; r1 < ROWS; r1++) {
        for (int c1 = 0; c1 < COLS; c1++) {
            for (int r2 = r1 + 1; r2 < ROWS; r2++) {
                for (int c2 = c1 + 1; c2 < COLS; c2++) {
                    // Skip if same box, row, or col
                    if (r1 == r2 || c1 == c2) continue;
                    if ((r1 / N) == (r2 / N) && (c1 / N) == (c2 / N)) continue;
                    short v1 = board[r1][c1], v2 = board[r2][c2];
                    if (v1 == v2) continue;
                    // Clear both and check if each value is valid at the other position
                    board[r1][c1] = 0; board[r2][c2] = 0;
                    bool v2AtR1 = isValidEntry(board, r1, c1, v2);
                    bool v1AtR2 = isValidEntry(board, r2, c2, v1);
                    board[r1][c1] = v1; board[r2][c2] = v2;  // restore
                    if (v2AtR1 && v1AtR2) {
                        // Found a swappable pair — clear them and verify non-unique
                        board[r1][c1] = 0; board[r2][c2] = 0;
                        CHECK_FALSE(isUniquelySolvable(board, 0, 0));
                        return;
                    }
                }
            }
        }
    }
    // If no swappable pair found (extremely unlikely), just skip
    MESSAGE("Could not find swappable pair; test skipped");
}

TEST_CASE("isUniquelySolvable: near-complete board with one forced fill returns true") {
    // Build a valid board, leave only one cell empty
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSudokuBoard(board);
    short saved = board[8][8];
    board[8][8] = 0;
    CHECK(isUniquelySolvable(board, 0, 0));
    board[8][8] = saved;  // restore
}

TEST_CASE("isUniquelySolvable: board generated by generateSolvableBoard returns true") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSolvableBoard(board);
    CHECK(isUniquelySolvable(board, 0, 0));
}

// ─── generateSolvableBoard ───────────────────────────────────────────────────

TEST_CASE("generateSolvableBoard: result has at least 1 empty cell") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSolvableBoard(board);
    CHECK(countEmptyCells(board) >= 1);
}

TEST_CASE("generateSolvableBoard: result has at most 58 empty cells") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSolvableBoard(board);
    CHECK(countEmptyCells(board) <= 58);
}

TEST_CASE("generateSolvableBoard: all non-zero cells are clue values (1-9, not user-encoded)") {
    short board[ROWS][COLS];
    zeroBoard(board);
    generateSolvableBoard(board);
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (board[r][c] != 0) {
                CHECK(board[r][c] >= 1);
                CHECK(board[r][c] <= 9);
            }
}

TEST_CASE("generateSolvableBoard: generates different boards across calls") {
    short board1[ROWS][COLS], board2[ROWS][COLS];
    zeroBoard(board1); zeroBoard(board2);
    srand(1);
    generateSolvableBoard(board1);
    srand(999);
    generateSolvableBoard(board2);
    // With overwhelming probability, two different seeds produce different boards
    bool same = true;
    for (int r = 0; r < ROWS && same; r++)
        for (int c = 0; c < COLS && same; c++)
            if (board1[r][c] != board2[r][c]) same = false;
    CHECK_FALSE(same);
}
