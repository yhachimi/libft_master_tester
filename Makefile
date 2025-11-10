# Makefile for libft_master_tester
# Builds the parent libft (via its Makefile), then compiles and runs monsters_test.
# Place this file in the libft_master_tester directory.
#
# Usage:
#   make        # build parent libft, compile test, run test
#   make build  # build parent libft and compile test (no run)
#   make run    # run the test binary (after build)
#   make valgrind
#   make asan
#   make clean
#   make fclean
#
CC ?= cc
CFLAGS ?= -Wall -Wextra -Werror -I..
LIBFT_DIR := ..
LIBFT_LIB := $(LIBFT_DIR)/libft.a
TEST_SRC := monsters_test.c
TEST_BIN := monsters_test
ASAN_BIN := monsters_test_asan

.PHONY: all build build-libft build-test run valgrind asan clean fclean re

all: build run

build: build-libft build-test

# Build the parent libft using its own Makefile
build-libft:
	@echo "Building libft in $(LIBFT_DIR) via its Makefile..."
	@make  -C $(LIBFT_DIR)
	@make bonus -C ../
	@if [ ! -f $(LIBFT_LIB) ]; then \
		echo "Error: $(LIBFT_LIB) not found after building $(LIBFT_DIR)"; exit 1; \
	fi

# Compile the test and link with the libft created by the parent Makefile
build-test: $(LIBFT_LIB) $(TEST_SRC)
	@echo "Compiling $(TEST_BIN) and linking with $(LIBFT_LIB)..."
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIBFT_LIB) -o $(TEST_BIN)

# Run the test binary
run: $(TEST_BIN)
	@echo "Running $(TEST_BIN)..."
	./$(TEST_BIN)

# Run under Valgrind (requires valgrind installed)
valgrind: build
	@if ! command -v valgrind >/dev/null 2>&1; then \
		echo "valgrind not found; install valgrind to use this target"; exit 1; \
	fi
	@echo "Running $(TEST_BIN) under Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_BIN)

# Build ASan-instrumented binary by compiling libft sources + test together,
# so ASan also instruments libft code. This does not call the parent Makefile.
asan:
	@echo "Building ASan-instrumented binary $(ASAN_BIN)..."
	$(CC) $(CFLAGS) -fsanitize=address -g $(wildcard $(LIBFT_DIR)/*.c) $(TEST_SRC) -o $(ASAN_BIN)
	@echo "Running $(ASAN_BIN)..."
	./$(ASAN_BIN)

clean:
	@echo "Cleaning test binaries"
	-rm -f $(TEST_BIN) $(ASAN_BIN) a.out

# Call parent libft's fclean if available, and remove local test binaries
fclean: clean
	@echo "Calling fclean in $(LIBFT_DIR) (if present)"
	-@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
