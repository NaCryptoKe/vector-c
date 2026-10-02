CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -Wpointer-arith -std=c17

TARGET = app

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET_PATH = $(BIN_DIR)/$(TARGET)


.PHONY: all clean test


all: $(TARGET_PATH)


$(TARGET_PATH): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -MMD -MP -c $< -o $@


-include $(OBJECTS:.o=.d)


# Test suite lives outside SRC_DIR so it is never linked into the library.
TEST_DIR  = tests
TEST_SRC  = $(wildcard $(TEST_DIR)/*.c)
TEST_NAME = test_vector
TEST_PATH = $(BIN_DIR)/$(TEST_NAME)

# Library objects only -- main.o provides its own main().
LIB_OBJECTS = $(filter-out $(OBJ_DIR)/main.o, $(OBJECTS))

test: $(TEST_PATH)
	./$(TEST_PATH)

$(TEST_PATH): $(TEST_SRC) $(LIB_OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -Iinclude $(TEST_SRC) $(LIB_OBJECTS) -o $@


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)