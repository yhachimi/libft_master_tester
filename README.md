# 🧠 libft_master_tester

A **complete, professional, and colorized test suite** for **42's libft project** — created with ❤️ by **Younes Hachimi (yhachimi)**.

It builds your `libft.a` via its original Makefile and runs comprehensive verification of all mandatory and bonus functions with extensive edge cases, crash tests, and memory leak detection.

---

## 📜 Overview

This tester is designed to:
- ✅ Build your `libft.a` automatically via your Makefile  
- 🧪 Run **200+ assertions** across all parts (mandatory + bonus)  
- 🎨 Display colorful and animated progress with clear results  
- 🧩 Include **edge cases, overflow checks, null handling**, and memory safety  
- 🧼 Support **Valgrind** and **AddressSanitizer (ASan)** testing modes  
- 💾 Test for memory leaks comprehensively

---

## 🧩 Features

| Category | Covered Functions |
|-----------|------------------|
| **Part 1 (Libc)** | `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`, `ft_strlen`, `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_strchr`, `ft_strrchr`, `ft_strlcpy`, `ft_strlcat`, `ft_strncmp`, `ft_strnstr`, `ft_strdup`, `ft_atoi`, `ft_toupper`, `ft_tolower`, `ft_calloc` |
| **Part 2 (Additional)** | `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_itoa`, `ft_strmapi`, `ft_striteri`, `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd` |
| **Bonus (Linked Lists)** | `ft_lstnew`, `ft_lstadd_front`, `ft_lstsize`, `ft_lstlast`, `ft_lstadd_back`, `ft_lstdelone`, `ft_lstclear`, `ft_lstiter`, `ft_lstmap` |

---

## ⚙️ Installation

Clone or copy this tester **into a subfolder** inside your `libft` repository:

```bash
cd /path/to/your/libft
git clone <this-repo-url> libft_master_tester
# OR create the folder manually and copy the files
```

Your directory structure should look like:

```
libft/
├── ft_*.c
├── libft.h
├── Makefile
└── libft_master_tester/
    ├── Makefile
    ├── monsters_test.c
    ├── monsters_bonus_test.c
    ├── test_utils.h
    └── README.md
```

---

## 🚀 Usage

### Quick Test (Recommended)

Test both mandatory and bonus functions:

```bash
cd libft_master_tester
make
```

This will:
1. Build your `libft.a` 
2. Compile and run mandatory tests
3. Compile and run bonus tests

### Individual Tests

Test only mandatory functions:

```bash
make m
```

Test only bonus functions:

```bash
make b
```

### Memory Leak Detection

#### Using Valgrind (Recommended)

**Mandatory tests:**
```bash
make valgrind_m
```

**Bonus tests:**
```bash
make valgrind_b
```

#### Using AddressSanitizer

**Mandatory tests:**
```bash
make asan_m
```

**Bonus tests:**
```bash
make asan_b
```

### Clean Up

Remove test binaries:

```bash
make clean
```

Remove everything including `libft.a`:

```bash
make fclean
```

Rebuild everything:

```bash
make re
```

---

## 📊 Test Coverage

### Mandatory Tests (120+ tests)

#### Part 1: Character & String Functions
- **Character classification**: All edge cases including negative values and values > 127
- **ft_strlen**: Empty strings, normal strings, very long strings (999 chars)
- **Memory functions**: Overlapping regions, zero length, binary data, unsigned comparisons
- **String search**: NULL terminators, empty strings, not found cases
- **String copy**: Truncation, size 0, size 1 edge cases
- **Conversions**: toupper/tolower with non-alphabetic characters

#### Part 2: Additional Functions
- **ft_substr**: Start beyond string, len > available, empty strings
- **ft_strjoin**: Empty first, empty second, both empty
- **ft_strtrim**: All trimmed, nothing to trim, empty strings
- **ft_split**: Multiple delimiters, only delimiters, no delimiters, empty string
- **ft_itoa**: Zero, INT_MIN, INT_MAX, negative numbers
- **ft_strmapi/striteri**: Empty strings, case transformations
- **Output functions**: Visual verification tests

### Bonus Tests (80+ tests)

#### Linked List Operations
- **ft_lstnew**: NULL content, different data types, pointer preservation
- **ft_lstadd_front/back**: Empty list, NULL nodes, multiple additions
- **ft_lstsize**: Empty list, 1 node, 5 nodes, 1000 nodes
- **ft_lstlast**: Empty list, single node, multiple nodes
- **ft_lstdelone**: With/without content deletion, NULL nodes
- **ft_lstclear**: Empty list, single node, multiple nodes, with/without del function
- **ft_lstiter**: Empty list, single node, multiple nodes, content modification
- **ft_lstmap**: Empty list, single node, multiple nodes, proper duplication

#### Edge Cases & Stress Tests
- **Large lists**: 1000 nodes creation, traversal, and deletion
- **Alternating operations**: Mixed front/back additions
- **NULL content lists**: Lists with NULL data pointers
- **Memory safety**: 100+ allocation/free cycles, map/clear cycles

---

## 🔍 What Makes This Tester Special?

### Comprehensive Coverage
- **200+ test cases** covering normal usage, edge cases, and extreme values
- Tests for **buffer overflows**, **null pointers**, **empty strings**, **zero lengths**
- **Integer limits** (INT_MIN, INT_MAX) testing
- **Unsigned comparisons** for memory functions

### Memory Safety
- **Memory leak detection** through Valgrind integration
- **AddressSanitizer** support for real-time memory error detection
- Tests specifically designed to catch:
  - Memory leaks
  - Double frees
  - Use after free
  - Buffer overflows
  - Invalid reads/writes

### Real-World Scenarios
- Tests based on common **student mistakes**
- Edge cases that **moulinette tests**
- **Overlapping memory** tests for memmove
- **NULL terminator** handling
- **Off-by-one** error detection

---

## 🐛 Common Issues & Solutions

### Issue: "No rule to make target 'test_mandatory.c'"

**Solution**: Your test files might have different names. The Makefile expects:
- `monsters_test.c` (mandatory tests)
- `monsters_bonus_test.c` (bonus tests)

Rename your files accordingly or update the Makefile variables:
```makefile
MANDATORY_SRC := your_mandatory_test_file.c
BONUS_SRC     := your_bonus_test_file.c
```

### Issue: "libft.a not found"

**Solution**: Make sure:
1. You're running from inside the `libft_master_tester` directory
2. Your parent `libft` directory has a proper Makefile
3. Your Makefile has `all` and `bonus` rules

### Issue: Valgrind not installed

**Solution**: Install Valgrind:
```bash
# Ubuntu/Debian
sudo apt-get install valgrind

# macOS (using Homebrew)
brew install valgrind
```

### Issue: "Multiple definition of tests_run/tests_passed"

**Solution**: Make sure each `.c` test file has these lines:
```c
int tests_run = 0;
int tests_passed = 0;
```

And `test_utils.h` declares them as `extern`:
```c
extern int tests_run;
extern int tests_passed;
```

---

## 📝 Interpreting Results

### Color Codes
- 🟢 **Green ✓**: Test passed
- 🔴 **Red ✗**: Test failed
- 🟡 **Yellow**: Test section header

### Success Rate
- **100%**: Perfect! All tests passed ✅
- **90-99%**: Very good, but check failed tests
- **< 90%**: Several issues need attention

### Memory Leaks
When running Valgrind:
- **"All heap blocks were freed"** = No leaks ✅
- **"definitely lost"** = Critical memory leak ❌
- **"still reachable"** = Usually OK (cached memory)

---

## 🎯 Tips for Success

1. **Fix mandatory tests first** before starting bonus
2. **Read error messages carefully** - they tell you exactly what failed
3. **Test with Valgrind regularly** - catch leaks early
4. **Pay attention to**:
   - NULL pointer handling
   - Empty string cases
   - Zero length parameters
   - Integer overflow/underflow
5. **Use ASan during development** for immediate feedback

---

## 📚 Additional Resources

- [42 Cursus libft subject](https://github.com/binary-breadcrumb/libft/blob/main/en.subject.pdf)
- [Valgrind Quick Start](http://valgrind.org/docs/manual/quick-start.html)
- [AddressSanitizer Documentation](https://github.com/google/sanitizers/wiki/AddressSanitizer)

---

## 🤝 Contributing

Found a bug or have a suggestion? Feel free to:
- Open an issue
- Submit a pull request
- Contact: yhachimi@student.1337.ma

---

## 📜 License

This project is free to use for educational purposes at 42 School.

---

## ⭐ Acknowledgments

Created with 💜 for the 42 community by **Younes Hachimi (yhachimi)**

If this tester helped you, consider giving it a ⭐!

---

**Good luck with your libft! May your tests be green and your leaks be zero! 🚀**
