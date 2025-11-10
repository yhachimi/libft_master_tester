/*
Enhanced monsters_test.c - extended and safer tester

What I changed and added:
- Added missing global counters and report_result().
- Added robust tests for ft_split (basic and multiple delimiters).
- Enhanced ft_lst* tests:
  - Create nodes with separately-allocated content (strdup) to avoid ownership confusion.
  - Test ft_lstmap() normal behavior and edge cases (NULL list, mapper returning NULL).
  - Test ft_lstdelone on a standalone node (correct use).
  - Test ft_lstiter on NULL (should not crash).
  - Test ft_lstclear(NULL, ...) (should not crash).
  - Test ft_lstsize and ft_lstlast.
- Added additional edge-case tests (ft_strmapi/ft_striteri usage, map that intentionally fails).
- Ensured all allocated memory in tests is freed to keep test runner clean.
- Kept FD tests using pipe() and casts where libft prototypes may use non-const.
- Printed instructions for running under Valgrind / ASan.

Usage:
  # from repo root where libft.a is built by parent Makefile:
  cc -Wall -Wextra -Werror monsters_test.c ../libft.a -o monsters_test
  ./monsters_test

Or use the Makefile provided in this repo to build the parent libft and then run the test.

Notes:
- Some behaviors (e.g., ft_split handling of consecutive delimiters, ft_lstmap on mapper-NULL)
  may differ across implementations. Tests try to assert typical libft behavior (skip empty tokens),
  but failures will be reported explicitly so you can inspect.
*/

#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

/* Test counters and reporter */
static int tests_run = 0;
static int tests_passed = 0;

static void report_result(const char *name, int ok)
{
    ++tests_run;
    if (ok) {
        ++tests_passed;
        printf("[OK]    %s\n", name);
    } else {
        printf("[FAIL]  %s\n", name);
    }
}

/* Small helpers */
static ssize_t read_full(int fd, void *buf, size_t n)
{
    size_t total = 0;
    char *p = buf;
    while (total < n) {
        ssize_t r = read(fd, p + total, n - total);
        if (r < 0) return r;
        if (r == 0) break;
        total += (size_t)r;
    }
    return (ssize_t) total;
}

static void expect_int_eq(const char *title, int expected, int actual)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "%s : expected=%d got=%d", title, expected, actual);
    report_result(buf, expected == actual);
}

static void expect_str_eq(const char *title, const char *expected, const char *actual)
{
    char buf[512];
    int ok = (expected == NULL && actual == NULL) ||
             (expected && actual && strcmp(expected, actual) == 0);
    snprintf(buf, sizeof(buf), "%s : expected=\"%s\" got=\"%s\"", title,
             expected ? expected : "(null)", actual ? actual : "(null)");
    report_result(buf, ok);
}

static void expect_mem_eq(const char *title, const void *a, const void *b, size_t n)
{
    char buf[256];
    int ok = memcmp(a, b, n) == 0;
    snprintf(buf, sizeof(buf), "%s : memcmp %s", title, ok ? "equal" : "DIFFER");
    report_result(buf, ok);
}

static void free_split(char **arr)
{
    if (!arr) return;
    for (size_t i = 0; arr[i]; ++i) free(arr[i]);
    free(arr);
}

/* Tests */

static void test_atoi(void)
{
    printf("\n-- ft_atoi --\n");
    expect_int_eq("ft_atoi(\"0\")", atoi("0"), ft_atoi("0"));
    expect_int_eq("ft_atoi(\"42\")", atoi("42"), ft_atoi("42"));
    expect_int_eq("ft_atoi(\"   -42\")", atoi("   -42"), ft_atoi("   -42"));
    expect_int_eq("ft_atoi(\"+123\")", atoi("+123"), ft_atoi("+123"));
    expect_int_eq("ft_atoi(INT_MIN string)", atoi("-2147483648"), ft_atoi("-2147483648"));
    expect_int_eq("ft_atoi(INT_MAX string)", atoi("2147483647"), ft_atoi("2147483647"));
}

static void test_bzero_and_calloc(void)
{
    printf("\n-- ft_bzero & ft_calloc --\n");
    int a = INT_MAX, b = INT_MIN;
    ft_bzero(&a, sizeof(a));
    ft_bzero(&b, sizeof(b));
    report_result("ft_bzero zeros INT_MAX", a == 0);
    report_result("ft_bzero zeros INT_MIN", b == 0);

    size_t nm = 5;
    int *s = ft_calloc(nm, sizeof(int));
    int *s2 = calloc(nm, sizeof(int));
    if (!s || !s2) {
        perror("calloc");
        report_result("ft_calloc returned non-NULL", 0);
    } else {
        expect_mem_eq("ft_calloc content equals calloc content", s, s2, nm * sizeof(int));
    }
    free(s);
    free(s2);
}

static void test_is_functions(void)
{
    printf("\n-- ft_is* & ft_toupper/tolower --\n");
    for (int c = -1; c <= 255; ++c) {
        unsigned char uc = (unsigned char)c;
        int e_alpha = isalpha(uc) != 0;
        int g_alpha = ft_isalpha(uc) != 0;
        if (e_alpha != g_alpha) {
            char buf[64]; snprintf(buf, sizeof(buf), "ft_isalpha for %d", c);
            report_result(buf, 0); return;
        }
        int e_digit = isdigit(uc) != 0;
        int g_digit = ft_isdigit(uc) != 0;
        if (e_digit != g_digit) {
            char buf[64]; snprintf(buf, sizeof(buf), "ft_isdigit for %d", c);
            report_result(buf, 0); return;
        }
        int e_alnum = isalnum(uc) != 0;
        int g_alnum = ft_isalnum(uc) != 0;
        if (e_alnum != g_alnum) {
            char buf[64]; snprintf(buf, sizeof(buf), "ft_isalnum for %d", c);
            report_result(buf, 0); return;
        }
        int e_ascii = (c >= 0 && c <= 127);
        int g_ascii = ft_isascii(c) != 0;
        if (e_ascii != g_ascii) {
            char buf[64]; snprintf(buf, sizeof(buf), "ft_isascii for %d", c);
            report_result(buf, 0); return;
        }
        int e_print = isprint(uc) != 0;
        int g_print = ft_isprint(uc) != 0;
        if (e_print != g_print) {
            char buf[64]; snprintf(buf, sizeof(buf), "ft_isprint for %d", c);
            report_result(buf, 0); return;
        }
    }
    report_result("ft_is* family matches libc for -1..255", 1);

    for (int c = -1; c <= 255; ++c) {
        unsigned char uc = (unsigned char)c;
        int e_up = toupper(uc);
        int g_up = ft_toupper(uc);
        if (e_up != g_up) {
            char buf[64]; snprintf(buf, sizeof(buf), "ft_toupper for %d", c);
            report_result(buf, 0); return;
        }
        int e_low = tolower(uc);
        int g_low = ft_tolower(uc);
        if (e_low != g_low) {
            char buf[64]; snprintf(buf, sizeof(buf), "ft_tolower for %d", c);
            report_result(buf, 0); return;
        }
    }
    report_result("ft_toupper/tolower match libc", 1);
}

static void test_strlen_dup_strl(void)
{
    printf("\n-- ft_strlen / ft_strdup / ft_strlcpy / ft_strlcat / ft_strnstr --\n");
    const char *cases[] = { "", "a", "hello", "this is a longer string", NULL };
    for (int i = 0; cases[i]; ++i) {
        const char *s = cases[i];
        expect_int_eq("ft_strlen matches strlen", (int)strlen(s), (int)ft_strlen(s));
        char *d = ft_strdup(s);
        if (!d) { report_result("ft_strdup non-NULL", 0); continue; }
        expect_str_eq("ft_strdup equals strdup", s, d);
        free(d);
    }

    char dst[32];
    const char *src = "1234567890";
    memset(dst, 0, sizeof(dst));
    size_t r = ft_strlcpy(dst, src, sizeof(dst));
    report_result("ft_strlcpy returns src length", r == strlen(src));
    report_result("ft_strlcpy writes null-terminated string", dst[sizeof(dst)-1] == '\0');

    strcpy(dst, "pre");
    size_t catret = ft_strlcat(dst, src, sizeof(dst));
    report_result("ft_strlcat returns length >= strlen(initial)", catret >= strlen("pre"));

    const char *hay = "This is a simple haystack";
    const char *needle = "simple";
    char *found = ft_strnstr(hay, needle, strlen(hay));
    report_result("ft_strnstr finds substring", found && strcmp(found, "simple haystack") == 0);
}

static void test_mem_functions(void)
{
    printf("\n-- mem functions --\n");
    char src[64]; for (int i = 0; i < (int)sizeof(src); ++i) src[i] = (char)(i & 0xFF);

    char a1[32], a2[32];
    memset(a1, 0xAA, sizeof(a1));
    memset(a2, 0xAA, sizeof(a2));
    ft_memset(a1, 0x11, 16);
    memset(a2, 0x11, 16);
    expect_mem_eq("ft_memset matches memset on prefix", a1, a2, sizeof(a1));

    char b1[32], b2[32];
    memset(b1, 0, sizeof(b1));
    memset(b2, 0, sizeof(b2));
    memcpy(b1, src, 16);
    ft_memcpy(b2, src, 16);
    expect_mem_eq("ft_memcpy matches memcpy non-overlap", b1, b2, 16);

    char c1[64], c2[64];
    for (size_t i = 0; i < sizeof(c1); ++i) c1[i] = c2[i] = (char)i;
    memmove(c1 + 3, c1, 20);
    ft_memmove(c2 + 3, c2, 20);
    expect_mem_eq("ft_memmove matches memmove (overlap)", c1, c2, sizeof(c1));

    void *p1 = memchr("hello world", 'o', 11);
    void *p2 = ft_memchr("hello world", 'o', 11);
    report_result("ft_memchr finds same location", (p1 == p2));

    int cmp = memcmp("abc", "abd", 3);
    int fcmp = ft_memcmp("abc", "abd", 3);
    report_result("ft_memcmp sign matches memcmp", ( (cmp == 0 && fcmp == 0) || (cmp < 0 && fcmp < 0) || (cmp > 0 && fcmp > 0) ));
}

static void test_string_utils(void)
{
    printf("\n-- substr / join / trim / split / itoa / strmapi / striteri --\n");
    char *sub = ft_substr("Hello, world", 7, 5);
    expect_str_eq("ft_substr \"world\"", "world", sub);
    free(sub);

    char *j = ft_strjoin("foo", "bar");
    expect_str_eq("ft_strjoin foobar", "foobar", j);
    free(j);

    char *t = ft_strtrim("  \ttrim me\n  ", " \n\t");
    expect_str_eq("ft_strtrim trims", "trim me", t);
    free(t);

    /* ft_split tests - typical libft behavior: consecutive delimiters are skipped */
    char **res = ft_split("hello,world,test", ',');
    if (res) {
        int ok = (res[0] && strcmp(res[0], "hello") == 0) &&
                 (res[1] && strcmp(res[1], "world") == 0) &&
                 (res[2] && strcmp(res[2], "test") == 0) &&
                 (res[3] == NULL);
        report_result("ft_split basic case", ok);
        for (size_t i = 0; res[i]; ++i) free(res[i]);
        free(res);
    } else {
        report_result("ft_split basic case", 0);
    }

    res = ft_split(",,a,,b,,", ',');
    if (res) {
        /* typical libft: expect {"a","b",NULL} (skip empty tokens) */
        int ok = (res[0] && strcmp(res[0], "a") == 0) &&
                 (res[1] && strcmp(res[1], "b") == 0) &&
                 (res[2] == NULL);
        report_result("ft_split skips consecutive delimiters", ok);
        for (size_t i = 0; res[i]; ++i) free(res[i]);
        free(res);
    } else {
        report_result("ft_split skips consecutive delimiters", 0);
    }

    char *i0 = ft_itoa(0);
    char *i1 = ft_itoa(-2147483648);
    expect_str_eq("ft_itoa 0", "0", i0);
    expect_str_eq("ft_itoa INT_MIN", "-2147483648", i1);
    free(i0); free(i1);

    /* ft_strmapi: uppercase mapping */
    char mapper(unsigned int idx, char c) { (void)idx; return (char)ft_toupper((unsigned char)c); }
    char *mapped = ft_strmapi("aBcZ", mapper);
    expect_str_eq("ft_strmapi upper", "ABCZ", mapped);
    free(mapped);

    void iter(unsigned int idx, char *c) { (void)idx; *c = (char)ft_tolower((unsigned char)*c); }
    char mut[] = "HeLLo";
    ft_striteri(mut, iter);
    expect_str_eq("ft_striteri lower", "hello", mut);
}

/* put*_fd tests using pipe() */
static void test_put_fd_functions(void)
{
    printf("\n-- put*_fd functions --\n");
    int p[2];
    if (pipe(p) == -1) {
        perror("pipe");
        report_result("pipe()", 0);
        return;
    }

    /* ft_putchar_fd */
    char ch = 'X';
    ft_putchar_fd(ch, p[1]);
    char out = 0;
    ssize_t r = read_full(p[0], &out, 1);
    report_result("ft_putchar_fd wrote 1 byte & matches", (r == 1 && out == ch));

    /* ft_putstr_fd */
    const char *s = "hello";
    ft_putstr_fd((char*)s, p[1]); /* cast in case prototype is non-const */
    char buf[64] = {0};
    r = read_full(p[0], buf, strlen(s));
    report_result("ft_putstr_fd wrote expected string", (r == (ssize_t)strlen(s) && strncmp(buf, s, strlen(s)) == 0));

    /* ft_putendl_fd */
    const char *ln = "line";
    ft_putendl_fd((char*)ln, p[1]);
    memset(buf, 0, sizeof(buf));
    r = read_full(p[0], buf, strlen(ln) + 1);
    report_result("ft_putendl_fd wrote line + newline", (r == (ssize_t)(strlen(ln) + 1) && buf[strlen(ln)] == '\n'));

    /* ft_putnbr_fd */
    ft_putnbr_fd(-12345, p[1]);
    const char *exp = "-12345";
    memset(buf, 0, sizeof(buf));
    r = read_full(p[0], buf, strlen(exp));
    report_result("ft_putnbr_fd wrote -12345", (r == (ssize_t)strlen(exp) && strncmp(buf, exp, strlen(exp)) == 0));

    close(p[0]);
    close(p[1]);
}

/* Bonus linked-list tests */
static void test_bonus_linked_list(void)
{
    printf("\n-- bonus linked list functions --\n");
    /* Allocate unique content strings and create nodes */
    char *c1 = strdup("one");
    char *c2 = strdup("two");
    char *c3 = strdup("three");
    if (!c1 || !c2 || !c3) {
        free(c1); free(c2); free(c3);
        report_result("allocating list content", 0);
        return;
    }

    t_list *n1 = ft_lstnew(c1);
    t_list *n2 = ft_lstnew(c2);
    t_list *n3 = ft_lstnew(c3);
    if (!n1 || !n2 || !n3) {
        /* free content if nodes weren't created */
        free(c1); free(c2); free(c3);
        if (n1) { free(n1); }
        if (n2) { free(n2); }
        if (n3) { free(n3); }
        report_result("ft_lstnew non-NULL", 0);
        return;
    }

    ft_lstadd_back(&n1, n2);
    ft_lstadd_back(&n1, n3);
    report_result("ft_lstadd_back produced size 3", ft_lstsize(n1) == 3);
    t_list *last = ft_lstlast(n1);
    report_result("ft_lstlast points to last node", last == n3);

    /* ft_lstdelone on a standalone node (correct use) */
    t_list *solo = ft_lstnew(strdup("solo"));
    if (!solo) {
        report_result("ft_lstnew(single) non-NULL", 0);
    } else {
        ft_lstdelone(solo, free); /* safe: solo not in any list */
        report_result("ft_lstdelone(single node) didn't crash", 1);
    }

    /* ft_lstiter on NULL should not crash */
    ft_lstiter(NULL, NULL);
    report_result("ft_lstiter(NULL) didn't crash", 1);

    /* ft_lstmap: normal mapper */
    void *mapf_ok(void *c) {
        char *s = (char *)c;
        char *out = malloc(strlen(s) + 3);
        if (!out) return NULL;
        strcpy(out, s);
        strcat(out, "_m");
        return out;
    }
    t_list *mapped = ft_lstmap(n1, mapf_ok, free);
    if (mapped) {
        report_result("ft_lstmap produced new mapped list", ft_lstsize(mapped) == 3);
        /* verify original unchanged: none of original contents should contain "_m" */
        int orig_intact = 1;
        for (t_list *it = n1; it; it = it->next) {
            if (strstr((char *)it->content, "_m")) { orig_intact = 0; break; }
        }
        report_result("ft_lstmap doesn't modify original", orig_intact);
    } else {
        report_result("ft_lstmap produced new mapped list", 0);
    }

    /* ft_lstmap with a mapper that fails for one element -> expected behavior: returns NULL or partial cleared list */
    void *mapf_fail_for_two(void *c) {
        char *s = (char *)c;
        if (strcmp(s, "two") == 0) return NULL; /* simulate allocation/map failure */
        char *out = malloc(strlen(s) + 2);
        if (!out) return NULL;
        strcpy(out, s);
        return out;
    }
    t_list *mapped_fail = ft_lstmap(n1, mapf_fail_for_two, free);
    /* Accept either NULL (common libft behavior) or a non-NULL list (implementation-specific) -> we report accordingly */
    if (mapped_fail == NULL) {
        report_result("ft_lstmap returns NULL on mapper failure", 1);
    } else {
        report_result("ft_lstmap returns NULL on mapper failure", 0);
        /* If non-NULL, free it to avoid leaks */
        ft_lstclear(&mapped_fail, free);
    }

    /* ft_lstmap with NULL inputs */
    t_list *map_null = ft_lstmap(NULL, mapf_ok, free);
    report_result("ft_lstmap(NULL, ...) returns NULL", map_null == NULL);

    /* ft_lstclear on original and mapped lists (safe cleanup) */
    ft_lstclear(&n1, free);
    ft_lstclear(&mapped, free);
    report_result("ft_lstclear nulled head", n1 == NULL && mapped == NULL);
}

/* Allocation & crash-safety hints */
static void test_alloc_heuristics(void)
{
    printf("\n-- allocation heuristics & crash-safety hints --\n");
    char *s = ft_strdup("leak-check");
    if (!s) { report_result("ft_strdup non-NULL", 0); }
    else { free(s); report_result("ft_strdup alloc/free", 1); }

    char *it = ft_itoa(12345);
    if (!it) report_result("ft_itoa non-NULL", 0);
    else { free(it); report_result("ft_itoa alloc/free", 1); }

    /* sanity checks that should not crash */
    ft_lstclear(NULL, free);
    report_result("ft_lstclear(NULL) didn't crash", 1);
}

/* Runner */
int main(void)
{
    printf("=== MONSTERS TEST RUN ===\n");

    test_atoi();
    test_bzero_and_calloc();
    test_is_functions();
    test_strlen_dup_strl();
    test_mem_functions();
    test_string_utils();
    test_put_fd_functions();
    test_bonus_linked_list();
    test_alloc_heuristics();

    printf("\nSummary: tests run = %d, passed = %d, failed = %d\n",
           tests_run, tests_passed, tests_run - tests_passed);

    printf("\nMemory-leak and crash testing recommendations:\n"
           " - Run under Valgrind to find leaks:\n"
           "     valgrind --leak-check=full --show-leak-kinds=all ./monsters_test\n"
           " - To catch heap-buffer-overflow and other runtime errors, build with ASan:\n"
           "     cc -fsanitize=address -g monsters_test.c ../libft.a -o monsters_test_asan && ./monsters_test_asan\n");

    return (tests_run == tests_passed) ? 0 : 1;
}
