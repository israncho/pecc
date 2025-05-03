# Compiler
CC = clang

# Compiler Flags
CFLAGS = -Wall -Wextra -O3 -march=native -fopenmp
LDFLAGS = -fopenmp -lm

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
TESTS_DIR = test

# Source and Object files
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/src/%.o, $(SRCS))

# Test source and object files
TEST_SRCS = $(shell find $(TESTS_DIR) -name '*.c')
TEST_OBJS = $(patsubst $(TESTS_DIR)/%.c, $(BUILD_DIR)/tests/%.o, $(TEST_SRCS))

# Exclude main.o from OBJS for tests
MAIN_OBJ = $(BUILD_DIR)/src/main.o
OBJS_WITHOUT_MAIN = $(filter-out $(MAIN_OBJ), $(OBJS))

# Main executable
TARGET = $(BIN_DIR)/pecc

# Test executable
TEST_TARGET = $(BIN_DIR)/pecc_tests

# Rules
all: $(TARGET)

# Make the main executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking main executable..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Main executable built successfully: $(TARGET)"

# Make the test executable (exclude main.o)
$(TEST_TARGET): $(OBJS_WITHOUT_MAIN) $(TEST_OBJS) | $(BIN_DIR)
	@echo "Linking test executable..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Test executable built successfully: $(TEST_TARGET)"

# Compile every source file (into build/src/)
$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/src
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Compile every test file (into build/tests/)
$(BUILD_DIR)/tests/%.o: $(TESTS_DIR)/%.c | $(BUILD_DIR)/tests
	@echo "Compiling test $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Create directories if they don't exist
$(BUILD_DIR)/src:
	@echo "Creating build/src directory..."
	mkdir -p $(BUILD_DIR)/src

$(BUILD_DIR)/tests:
	@echo "Creating build/tests directory..."
	mkdir -p $(BUILD_DIR)/tests

$(BIN_DIR):
	@echo "Creating bin directory..."
	mkdir -p $(BIN_DIR)

# Clean compiled files
clean:
	@echo "Cleaning build and bin directories..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "All clean."

# Clean and recompile
rebuild: clean all
	@echo "Project rebuilt successfully."

# Run tests
test: $(TEST_TARGET)
	@echo "--------Beginning Test Session-------"
	./$(TEST_TARGET)
	@echo "--------End Test Session-------"

test-with-valgrind: $(TEST_TARGET)
	@echo "--------Beginning Test Session-------"
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=supress_openmp.valgrind --track-origins=yes --error-exitcode=1 --quiet ./$(TEST_TARGET)
	@echo "--------End Test Session-------"

.PHONY: all clean rebuild test