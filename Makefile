CXX      = g++
LDFLAGS  = -lncurses
TARGET   = sudoku
INCLUDES = -Isrc/engine -Isrc/game_logic -Isrc/tui

SRCS = src/main.cpp \
       src/engine/engine.cpp \
       src/game_logic/game_logic.cpp \
       src/tui/tui.cpp

OBJS = $(SRCS:.cpp=.o)

TEST_TARGET  = tests/run_tests
TEST_SRCS    = tests/test_game_logic.cpp \
               tests/test_engine_logic.cpp \
               tests/test_main_constants.cpp \
               src/game_logic/game_logic.cpp \
               src/engine/engine.cpp
TEST_CXXFLAGS = -Isrc/engine -Isrc/game_logic -Itests -DTEST_BUILD

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS)
	$(CXX) $(TEST_CXXFLAGS) $(TEST_SRCS) -o $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(OBJS) $(TEST_TARGET)
