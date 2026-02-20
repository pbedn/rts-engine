CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -Llib -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

SRC = $(wildcard src/**/*.c) src/*.c

TEST_SRC = \
	tests/test_pathfinding.c

GAME_TARGET = build/rts
TEST_TARGET = test_runner

# --- Default target ---
all: $(GAME_TARGET)

# --- Game build ---
$(GAME_TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(GAME_TARGET)

# --- Run game ---
run: $(GAME_TARGET)
	./$(GAME_TARGET)

# --- Test build ---
$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

# --- Run tests ---
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# --- Clean ---
clean:
	rm -f $(GAME_TARGET) $(TEST_TARGET)
# 	find src -name "*.o" -delete
# 	find src -name "*.d" -delete
