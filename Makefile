CXX      = g++
LDFLAGS  = -lncurses
TARGET   = sudoku
INCLUDES = -Isrc/engine -Isrc/game_logic -Isrc/tui

SRCS = src/main.cpp \
       src/engine/engine.cpp \
       src/game_logic/game_logic.cpp \
       src/tui/tui.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)
