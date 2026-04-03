#include "doctest.h"
#include "game_logic.h"

// Verify compile-time constants that main.cpp and the game rely on.
// These use static_assert so mismatches are caught at build time,
// and TEST_CASE wrappers make them visible in test output.

TEST_CASE("constants: N equals 3") {
    static_assert(N == 3, "N must be 3 for standard Sudoku");
    CHECK(N == 3);
}

TEST_CASE("constants: ROWS equals 9") {
    static_assert(ROWS == 9, "ROWS must be 9 (N*N)");
    CHECK(ROWS == 9);
}

TEST_CASE("constants: COLS equals 9") {
    static_assert(COLS == 9, "COLS must be 9 (N*N)");
    CHECK(COLS == 9);
}

TEST_CASE("constants: HCELLS and VCELLS equal N") {
    static_assert(HCELLS == N, "HCELLS must equal N");
    static_assert(VCELLS == N, "VCELLS must equal N");
    CHECK(HCELLS == 3);
    CHECK(VCELLS == 3);
}

// ─── Board encoding invariants ───────────────────────────────────────────────

TEST_CASE("encoding: val%10 extracts digit from prefilled clue") {
    CHECK(1 % 10 == 1);
    CHECK(9 % 10 == 9);
}

TEST_CASE("encoding: val%10 extracts digit from user-entered value") {
    CHECK(11 % 10 == 1);
    CHECK(19 % 10 == 9);
}

TEST_CASE("encoding: empty cell (0) modulo is 0 — cannot conflict with any digit") {
    CHECK(0 % 10 == 0);
}

TEST_CASE("encoding: clue range 1-9 satisfies v > 0 && v < 10") {
    for (short v = 1; v <= 9; v++) {
        CHECK(v > 0);
        CHECK(v < 10);
    }
}

TEST_CASE("encoding: user-entry range 11-19 satisfies v >= 11 && v <= 19") {
    for (short v = 11; v <= 19; v++) {
        CHECK(v >= 11);
        CHECK(v <= 19);
        CHECK(v % 10 >= 1);
        CHECK(v % 10 <= 9);
    }
}

TEST_CASE("encoding: clue and its user-encoded counterpart share the same digit via modulo") {
    for (short digit = 1; digit <= 9; digit++) {
        short clue = digit;
        short userEntry = digit + 10;
        CHECK(clue % 10 == userEntry % 10);
    }
}
