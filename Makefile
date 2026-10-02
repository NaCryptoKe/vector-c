CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -Wpointer-arith -std=c17

TARGET = app

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET_PATH = $(BIN_DIR)/$(TARGET)


.PHONY: all clean test asan asan-app check


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


# ---------------------------------------------------------------------------
# Sanitizers
#
# Manual memory management is this library's whole job, so ASan/UBSan are
# part of the definition of "done" rather than an optional extra. These
# targets rebuild from source so the instrumented objects never mix with the
# plain $(OBJECTS) produced by `all`.
# ---------------------------------------------------------------------------

SAN_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer -g

# Runtime settings: leak detection on, and any UB/overflow stops the run so
# a real defect surfaces as a non-zero exit instead of a warning line.
ASAN_ENV  = ASAN_OPTIONS=detect_leaks=1:strict_string_checks=1
UBSAN_ENV = UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1

LIB_SOURCES = $(filter-out $(SRC_DIR)/main.c, $(SOURCES))

ASAN_TEST_PATH = $(BIN_DIR)/$(TEST_NAME)_asan
ASAN_APP_PATH  = $(BIN_DIR)/$(TARGET)_asan

# Run the full test suite under AddressSanitizer + UndefinedBehaviorSanitizer.
asan: $(ASAN_TEST_PATH)
	$(ASAN_ENV) $(UBSAN_ENV) ./$(ASAN_TEST_PATH)

$(ASAN_TEST_PATH): $(TEST_SRC) $(LIB_SOURCES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SAN_FLAGS) -Iinclude $(TEST_SRC) $(LIB_SOURCES) -o $@

# Same, for the example/smoke application.
asan-app: $(ASAN_APP_PATH)
	$(ASAN_ENV) $(UBSAN_ENV) ./$(ASAN_APP_PATH)

$(ASAN_APP_PATH): $(SOURCES)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SAN_FLAGS) -Iinclude $(SOURCES) -o $@

# Everything a change must pass before it is considered done.
check: all test asan asan-app


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)