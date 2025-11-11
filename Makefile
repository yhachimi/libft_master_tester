# Makefile for libft_master_tester (mandatory + bonus separated)

CC      := cc
MAKE    := $(shell command -v make 2>/dev/null || echo make)
CFLAGS  := -Wall -Wextra -Werror -I..
LIBFT_DIR := ..
LIBFT_LIB := $(LIBFT_DIR)/libft.a

MANDATORY_SRC := monsters_test.c
BONUS_SRC     := monsters_bonus_test.c
MANDATORY_BIN := monsters_test_m
BONUS_BIN     := monsters_test_b

ASAN_M_BIN := monsters_test_m_asan
ASAN_B_BIN := monsters_test_b_asan

.PHONY: all m b build-libft build_m build_b run_m run_b valgrind_m valgrind_b asan_m asan_b clean fclean re

all: m b

#---------------------------------------
#  Mandatory and Bonus test targets
#---------------------------------------

m: build_m run_m
b: build_b run_b

build_m: build-libft $(MANDATORY_BIN)
build_b: build-libft $(BONUS_BIN)

$(MANDATORY_BIN): $(MANDATORY_SRC) $(LIBFT_LIB)
	@echo "🔨 Compiling mandatory tests..."
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) $(MANDATORY_SRC) $(LIBFT_LIB) -o $(MANDATORY_BIN)

$(BONUS_BIN): $(BONUS_SRC) $(LIBFT_LIB)
	@echo "🔨 Compiling bonus tests..."
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) $(BONUS_SRC) $(LIBFT_LIB) -o $(BONUS_BIN)

#---------------------------------------
#  Build parent libft
#---------------------------------------
build-libft:
	@echo "📚 Building libft via its Makefile..."
	@$(MAKE) -C $(LIBFT_DIR)
	@$(MAKE) -C $(LIBFT_DIR) bonus || true
	@if [ ! -f $(LIBFT_LIB) ]; then \
		echo "❌ Error: $(LIBFT_LIB) not found after building $(LIBFT_DIR)"; exit 1; \
	fi

#---------------------------------------
#  Run targets
#---------------------------------------
run_m:
	@echo "🚀 Running mandatory tests..."
	./$(MANDATORY_BIN)

run_b:
	@echo "🚀 Running bonus tests..."
	./$(BONUS_BIN)

#---------------------------------------
#  Valgrind
#---------------------------------------
valgrind_m: build_m
	@echo "🧠 Running mandatory tests under Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(MANDATORY_BIN)

valgrind_b: build_b
	@echo "🧠 Running bonus tests under Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(BONUS_BIN)

#---------------------------------------
#  ASan builds
#---------------------------------------
asan_m:
	@echo "🧩 Building ASan mandatory..."
	@SRCS="$(wildcard $(LIBFT_DIR)/*.c)"; \
	$(CC) $(CFLAGS) -fsanitize=address -g $$SRCS $(MANDATORY_SRC) -o $(ASAN_M_BIN)
	@./$(ASAN_M_BIN)

asan_b:
	@echo "🧩 Building ASan bonus..."
	@SRCS="$(wildcard $(LIBFT_DIR)/*.c)"; \
	$(CC) $(CFLAGS) -fsanitize=address -g $$SRCS $(BONUS_SRC) -o $(ASAN_B_BIN)
	@./$(ASAN_B_BIN)

#---------------------------------------
#  Cleanup
#---------------------------------------
clean:
	@echo "🧹 Cleaning tester binaries..."
	rm -f $(MANDATORY_BIN) $(BONUS_BIN) $(ASAN_M_BIN) $(ASAN_B_BIN) a.out

fclean: clean
	@echo "🧽 Running fclean in libft..."
	-@$(MAKE) -C $(LIBFT_DIR) fclean || true

re: fclean all

