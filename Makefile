CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -Wpointer-arith -std=c17

TARGET = app

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET_PATH = $(BIN_DIR)/$(TARGET)


.PHONY: all clean


all: $(TARGET_PATH)


$(TARGET_PATH): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -Iinclude -MMD -MP -c $< -o $@


-include $(OBJECTS:.o=.d)


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)