/*
** monsters_test.c - Complete Enhanced libft Tester with Visual Effects
** 
** Comprehensive test suite for 42's libft project featuring:
** - All mandatory functions (Part 1 & Part 2)
** - All bonus functions (linked lists)
** - Edge cases and boundary conditions
** - Memory safety and crash prevention
** - Colorized output with ANSI escape codes
** - Animated banner and visual feedback
**
** Usage:
**   cc -Wall -Wextra -Werror monsters_test.c libft.a -o monsters_test
**   ./monsters_test
**
** Memory Testing:
**   valgrind --leak-check=full --show-leak-kinds=all ./monsters_test
**   cc -fsanitize=address -g monsters_test.c libft.a -o test_asan && ./test_asan
*/

#include <stdint.h>
#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

/* ANSI Color Codes */
#define CLR_RESET  "\x1b[0m"
#define CLR_RED    "\x1b[31m"
#define CLR_GREEN  "\x1b[32m"
#define CLR_YELLOW "\x1b[33m"
#define CLR_BLUE   "\x1b[34m"
#define CLR_MAG    "\x1b[35m"
#define CLR_CYAN   "\x1b[36m"
#define CLR_BOLD   "\x1b[1m"

/* Test counters */
static int tests_run = 0;
static int tests_passed = 0;

/* Animated banner */
static void banner(void)
{
    const char *lines[] = {
        " __  __                 _                 _____         _   ",
        "|  \\/  | ___  _ __  ___| |_ ___ _ __ ___|_   _|__  ___| |_ ",
        "| |\\/| |/ _ \\| '_ \\/ __| __/ _ \\ '__/ __|| |/ _ \\/ __| __|",
        "| |  | | (_) | | | \\__ \\ ||  __/ |  \\__ \\| |  __/\\__ \\ |_ ",
        "|_|  |_|\\___/|_| |_|___/\\__\\___|_|  |___/|_|\\___||___/\\__|",
        "                                                            ",
        "         🧪 Comprehensive libft Test Suite 🧪              ",
        NULL
    };
    
    printf("\n%s%s", CLR_MAG, CLR_BOLD);
    for (int i = 0; lines[i]; ++i) {
        printf("%s\n", lines[i]);
        fflush(stdout);
        usleep(60000);
    }
    printf("%s\n", CLR_RESET);
    usleep(150000);
}

/* Section header with visual effect */
static void section(const char *title)
{
    printf("\n%s%s╔═══════════════════════════════════════════════════════════╗%s\n", 
           CLR_CYAN, CLR_BOLD, CLR_RESET);
    printf("%s%s║  %-55s  ║%s\n", CLR_CYAN, CLR_BOLD, title, CLR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════════╝%s\n", 
           CLR_CYAN, CLR_BOLD, CLR_RESET);
    usleep(100000);
}

/* Colored result reporter */
static void report_result(const char *name, int ok)
{
    ++tests_run;
    if (ok) {
        ++tests_passed;
        printf("%s%s✓%s %s\n", CLR_GREEN, CLR_BOLD, CLR_RESET, name);
    } else {
        printf("%s%s✗%s %s\n", CLR_RED, CLR_BOLD, CLR_RESET, name);
    }
}

/* Helper functions */
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
    return (ssize_t)total;
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
    snprintf(buf, sizeof(buf), "%s : exp=\"%s\" got=\"%s\"", title,
             expected ? expected : "(null)", actual ? actual : "(null)");
    report_result(buf, ok);
}

static void expect_mem_eq(const char *title, const void *a, const void *b, size_t n)
{
    int ok = memcmp(a, b, n) == 0;
    char buf[256];
    snprintf(buf, sizeof(buf), "%s", title);
    report_result(buf, ok);
}

static void free_split(char **arr)
{
    if (!arr) return;
    for (size_t i = 0; arr[i]; ++i) free(arr[i]);
    free(arr);
}

/* ========== PART 1: Libc Functions ========== */

static void test_atoi(void)
{
    section("ft_atoi");
    expect_int_eq("ft_atoi(\"0\")", atoi("0"), ft_atoi("0"));
    expect_int_eq("ft_atoi(\"42\")", atoi("42"), ft_atoi("42"));
    expect_int_eq("ft_atoi(\"   -42\")", atoi("   -42"), ft_atoi("   -42"));
    expect_int_eq("ft_atoi(\"+123\")", atoi("+123"), ft_atoi("+123"));
    expect_int_eq("ft_atoi(INT_MIN)", atoi("-2147483648"), ft_atoi("-2147483648"));
    expect_int_eq("ft_atoi(INT_MAX)", atoi("2147483647"), ft_atoi("2147483647"));
    expect_int_eq("ft_atoi(whitespace)", atoi(" \t\n-42"), ft_atoi(" \t\n-42"));
}

static void test_bzero_and_calloc(void)
{
    section("ft_bzero & ft_calloc");
    
    int a = INT_MAX, b = INT_MIN;
    ft_bzero(&a, sizeof(a));
    ft_bzero(&b, sizeof(b));
    report_result("ft_bzero zeros INT_MAX", a == 0);
    report_result("ft_bzero zeros INT_MIN", b == 0);
    
    char arr[10] = "123456789";
    ft_bzero(arr, 5);
    report_result("ft_bzero partial", arr[0] == 0 && arr[4] == 0 && arr[5] == '6');
    
    size_t nm = 5;
    int *s = ft_calloc(nm, sizeof(int));
    int *s2 = calloc(nm, sizeof(int));
    if (!s || !s2) {
        report_result("ft_calloc returned non-NULL", 0);
    } else {
        expect_mem_eq("ft_calloc matches calloc", s, s2, nm * sizeof(int));
    }
    free(s); free(s2);
    
    void *overflow = ft_calloc(SIZE_MAX, SIZE_MAX);
    report_result("ft_calloc overflow protection", overflow == NULL);
    if (overflow) free(overflow);
    
    void *zero1 = ft_calloc(0, 10);
    void *zero2 = ft_calloc(10, 0);
    if (zero1) free(zero1);
    if (zero2) free(zero2);
    report_result("ft_calloc(0, n) doesn't crash", 1);
}

static void test_is_functions(void)
{
    section("ft_is* & ft_toupper/tolower");
    
    int all_match = 1;
    for (int c = -1; c <= 255; ++c) {
        unsigned char uc = (unsigned char)c;
        if ((isalpha(uc) != 0) != (ft_isalpha(uc) != 0)) all_match = 0;
        if ((isdigit(uc) != 0) != (ft_isdigit(uc) != 0)) all_match = 0;
        if ((isalnum(uc) != 0) != (ft_isalnum(uc) != 0)) all_match = 0;
        if (((c >= 0 && c <= 127) != 0) != (ft_isascii(c) != 0)) all_match = 0;
        if ((isprint(uc) != 0) != (ft_isprint(uc) != 0)) all_match = 0;
        if (toupper(uc) != ft_toupper(uc)) all_match = 0;
        if (tolower(uc) != ft_tolower(uc)) all_match = 0;
    }
    report_result("ft_is*/toupper/tolower match libc for -1..255", all_match);
}

static void test_strlen_dup_strl(void)
{
    section("ft_strlen / ft_strdup / ft_strlcpy / ft_strlcat");
    
    const char *cases[] = { "", "a", "hello", "longer string test", NULL };
    for (int i = 0; cases[i]; ++i) {
        if (ft_strlen(cases[i]) != strlen(cases[i])) {
            report_result("ft_strlen matches strlen", 0);
            return;
        }
        char *d = ft_strdup(cases[i]);
        if (!d || strcmp(d, cases[i]) != 0) {
            report_result("ft_strdup works correctly", 0);
            free(d);
            return;
        }
        free(d);
    }
    report_result("ft_strlen matches strlen", 1);
    report_result("ft_strdup works correctly", 1);
    
    char dst[32];
    memset(dst, 'X', sizeof(dst));
    size_t r = ft_strlcpy(dst, "1234567890", sizeof(dst));
    report_result("ft_strlcpy returns src length", r == 10);
    report_result("ft_strlcpy null-terminates", dst[10] == '\0');
    
    strcpy(dst, "Hello");
    r = ft_strlcat(dst, " World", sizeof(dst));
    report_result("ft_strlcat concatenates", strcmp(dst, "Hello World") == 0 && r == 11);
}

static void test_strchr_strrchr(void)
{
    section("ft_strchr / ft_strrchr");
    
    const char *s = "hello world";
    report_result("ft_strchr finds first 'o'", strchr(s, 'o') == ft_strchr(s, 'o'));
    report_result("ft_strrchr finds last 'o'", strrchr(s, 'o') == ft_strrchr(s, 'o'));
    report_result("ft_strchr finds \\0", strchr(s, '\0') == ft_strchr(s, '\0'));
    report_result("ft_strchr not found returns NULL", 
                  strchr(s, 'z') == ft_strchr(s, 'z') && ft_strchr(s, 'z') == NULL);
}

static void test_strncmp(void)
{
    section("ft_strncmp");
    
    struct { const char *s1; const char *s2; size_t n; } cases[] = {
        {"abc", "abc", 3}, {"abc", "abd", 3}, {"abc", "ab", 3},
        {"", "", 1}, {"test", "test", 10}, {NULL, NULL, 0}
    };
    
    for (int i = 0; cases[i].s1; i++) {
        int exp = strncmp(cases[i].s1, cases[i].s2, cases[i].n);
        int got = ft_strncmp(cases[i].s1, cases[i].s2, cases[i].n);
        int ok = (exp == 0 && got == 0) || (exp < 0 && got < 0) || (exp > 0 && got > 0);
        if (!ok) {
            report_result("ft_strncmp sign matches", 0);
            return;
        }
    }
    report_result("ft_strncmp sign matches", 1);
}

static void test_strnstr(void)
{
    section("ft_strnstr");
    
    const char *hay = "This is a simple haystack";
    char *found = ft_strnstr(hay, "simple", strlen(hay));
    report_result("ft_strnstr finds substring", 
                  found && strcmp(found, "simple haystack") == 0);
    report_result("ft_strnstr len too short", ft_strnstr(hay, "simple", 10) == NULL);
    report_result("ft_strnstr empty needle", ft_strnstr(hay, "", strlen(hay)) == hay);
}

static void test_mem_functions(void)
{
    section("Memory Functions");
    
    char src[64];
    for (int i = 0; i < (int)sizeof(src); ++i) src[i] = (char)(i & 0xFF);
    
    char a1[32], a2[32];
    memset(a1, 0xAA, sizeof(a1));
    memset(a2, 0xAA, sizeof(a2));
    ft_memset(a1, 0x11, 16);
    memset(a2, 0x11, 16);
    expect_mem_eq("ft_memset matches memset", a1, a2, sizeof(a1));
    
    char b1[32], b2[32];
    memset(b1, 0, sizeof(b1));
    memset(b2, 0, sizeof(b2));
    memcpy(b1, src, 16);
    ft_memcpy(b2, src, 16);
    expect_mem_eq("ft_memcpy matches memcpy", b1, b2, 16);
    
    char c1[64], c2[64];
    for (size_t i = 0; i < sizeof(c1); ++i) c1[i] = c2[i] = (char)i;
    memmove(c1 + 3, c1, 20);
    ft_memmove(c2 + 3, c2, 20);
    expect_mem_eq("ft_memmove overlap forward", c1, c2, sizeof(c1));
    
    report_result("ft_memchr finds char", 
                  memchr("hello", 'o', 5) == ft_memchr("hello", 'o', 5));
    
    int cmp = memcmp("abc", "abd", 3);
    int fcmp = ft_memcmp("abc", "abd", 3);
    report_result("ft_memcmp sign matches", 
                  (cmp == 0 && fcmp == 0) || (cmp < 0 && fcmp < 0) || (cmp > 0 && fcmp > 0));
}

/* ========== PART 2: Additional Functions ========== */

static void test_string_utils(void)
{
    section("Part 2: String Utilities");
    
    char *sub = ft_substr("Hello, world", 7, 5);
    expect_str_eq("ft_substr", "world", sub);
    free(sub);
    
    sub = ft_substr("hello", 10, 5);
    report_result("ft_substr start > len", sub && strcmp(sub, "") == 0);
    free(sub);
    
    char *j = ft_strjoin("foo", "bar");
    expect_str_eq("ft_strjoin", "foobar", j);
    free(j);
    
    char *t = ft_strtrim("  \ttrim me\n  ", " \n\t");
    expect_str_eq("ft_strtrim", "trim me", t);
    free(t);
    
    t = ft_strtrim("xxxhelloxxx", "x");
    expect_str_eq("ft_strtrim custom", "hello", t);
    free(t);
}

static void test_split(void)
{
    section("ft_split");
    
    char **res = ft_split("hello,world,test", ',');
    if (res) {
        int ok = (res[0] && strcmp(res[0], "hello") == 0) &&
                 (res[1] && strcmp(res[1], "world") == 0) &&
                 (res[2] && strcmp(res[2], "test") == 0) &&
                 (res[3] == NULL);
        report_result("ft_split basic case", ok);
        free_split(res);
    }
    
    res = ft_split(",,a,,b,,", ',');
    if (res) {
        int ok = (res[0] && strcmp(res[0], "a") == 0) &&
                 (res[1] && strcmp(res[1], "b") == 0) &&
                 (res[2] == NULL);
        report_result("ft_split skips consecutive delimiters", ok);
        free_split(res);
    }
    
    res = ft_split("", ',');
    report_result("ft_split empty string", res && res[0] == NULL);
    free_split(res);
}

static void test_itoa(void)
{
    section("ft_itoa");
    
    struct { int n; const char *exp; } cases[] = {
        {0, "0"}, {42, "42"}, {-42, "-42"},
        {2147483647, "2147483647"}, {-2147483648, "-2147483648"}
    };
    
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        char *result = ft_itoa(cases[i].n);
        char buf[64];
        snprintf(buf, sizeof(buf), "ft_itoa(%d)", cases[i].n);
        expect_str_eq(buf, cases[i].exp, result);
        free(result);
    }
}

static void test_strmapi_striteri(void)
{
    section("ft_strmapi / ft_striteri");
    
    char mapper_upper(unsigned int idx, char c) { 
        (void)idx; 
        return (char)ft_toupper((unsigned char)c); 
    }
    
    char *mapped = ft_strmapi("aBcZ", mapper_upper);
    expect_str_eq("ft_strmapi uppercase", "ABCZ", mapped);
    free(mapped);
    
    void iter_lower(unsigned int idx, char *c) { 
        (void)idx; 
        *c = (char)ft_tolower((unsigned char)*c); 
    }
    
    char mut[] = "HeLLo";
    ft_striteri(mut, iter_lower);
    expect_str_eq("ft_striteri lowercase", "hello", mut);
}

/* ========== File Descriptor Functions ========== */

static void test_put_fd_functions(void)
{
    section("put*_fd Functions");
    
    int p[2];
    if (pipe(p) == -1) {
        report_result("pipe() creation", 0);
        return;
    }
    
    ft_putchar_fd('X', p[1]);
    char out = 0;
    read_full(p[0], &out, 1);
    report_result("ft_putchar_fd", out == 'X');
    
    ft_putstr_fd((char*)"hello", p[1]);
    char buf[64] = {0};
    read_full(p[0], buf, 5);
    report_result("ft_putstr_fd", strcmp(buf, "hello") == 0);
    
    ft_putendl_fd((char*)"line", p[1]);
    memset(buf, 0, sizeof(buf));
    read_full(p[0], buf, 5);
    report_result("ft_putendl_fd", buf[4] == '\n');
    
    ft_putnbr_fd(-12345, p[1]);
    memset(buf, 0, sizeof(buf));
    read_full(p[0], buf, 6);
    report_result("ft_putnbr_fd", strncmp(buf, "-12345", 6) == 0);
    
    close(p[0]);
    close(p[1]);
}

/* ========== BONUS: Linked List Functions ========== */

static void test_bonus_linked_list(void)
{
    section("BONUS: Linked List Functions");
    
    char *c1 = strdup("one");
    char *c2 = strdup("two");
    char *c3 = strdup("three");
    
    if (!c1 || !c2 || !c3) {
        free(c1); free(c2); free(c3);
        report_result("list content allocation", 0);
        return;
    }
    
    t_list *n1 = ft_lstnew(c1);
    t_list *n2 = ft_lstnew(c2);
    t_list *n3 = ft_lstnew(c3);
    
    if (!n1 || !n2 || !n3) {
        report_result("ft_lstnew", 0);
        return;
    }
    
    ft_lstadd_back(&n1, n2);
    ft_lstadd_back(&n1, n3);
    report_result("ft_lstsize returns 3", ft_lstsize(n1) == 3);
    
    t_list *last = ft_lstlast(n1);
    report_result("ft_lstlast points to last", last == n3);
    
    char *front_c = strdup("front");
    t_list *front = ft_lstnew(front_c);
    ft_lstadd_front(&n1, front);
    report_result("ft_lstadd_front", n1 == front);
    
    void *mapf(void *c) {
        char *s = (char *)c;
        char *out = malloc(strlen(s) + 3);
        if (!out) return NULL;
        strcpy(out, s);
        strcat(out, "_m");
        return out;
    }
    
    t_list *mapped = ft_lstmap(n1, mapf, free);
    report_result("ft_lstmap creates new list", mapped != NULL && ft_lstsize(mapped) == 4);
    
    ft_lstclear(&n1, free);
    ft_lstclear(&mapped, free);
    report_result("ft_lstclear nulls head", n1 == NULL && mapped == NULL);
    
    ft_lstiter(NULL, NULL);
    ft_lstclear(NULL, free);
    report_result("list functions handle NULL", 1);
}

/* ========== Summary & Results ========== */

static void print_summary(void)
{
    printf("\n");
    printf("%s%s╔═══════════════════════════════════════════════════════════╗%s\n", 
           CLR_YELLOW, CLR_BOLD, CLR_RESET);
    printf("%s%s║                      TEST SUMMARY                         ║%s\n", 
           CLR_YELLOW, CLR_BOLD, CLR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════════╝%s\n", 
           CLR_YELLOW, CLR_BOLD, CLR_RESET);
    
    printf("\n  Total tests:  %s%d%s\n", CLR_BOLD, tests_run, CLR_RESET);
    printf("  Passed:       %s%s%d%s\n", CLR_GREEN, CLR_BOLD, tests_passed, CLR_RESET);
    printf("  Failed:       %s%s%d%s\n", 
           tests_run == tests_passed ? CLR_GREEN : CLR_RED, 
           CLR_BOLD, tests_run - tests_passed, CLR_RESET);
    
    if (tests_run == tests_passed) {
        printf("\n%s%s", CLR_GREEN, CLR_BOLD);
        for (int i = 0; i < 3; ++i) {
            printf("\r  🎉 ALL TESTS PASSED! 🎉  ");
            fflush(stdout);
            usleep(300000);
            printf("\r                              \r");
            fflush(stdout);
            usleep(150000);
        }
        printf("  🎉 ALL TESTS PASSED! 🎉  \n%s", CLR_RESET);
    } else {
        printf("\n%s%s  ❌ SOME TESTS FAILED ❌  %s\n", CLR_RED, CLR_BOLD, CLR_RESET);
    }
}

static void print_memory_tips(void)
{
    printf("\n%s%s╔═══════════════════════════════════════════════════════════╗%s\n", 
           CLR_CYAN, CLR_BOLD, CLR_RESET);
    printf("%s%s║              MEMORY TESTING RECOMMENDATIONS               ║%s\n", 
           CLR_CYAN, CLR_BOLD, CLR_RESET);
    printf("%s%s╚═══════════════════════════════════════════════════════════╝%s\n", 
           CLR_CYAN, CLR_BOLD, CLR_RESET);
    
    printf("\n%sValgrind (leak detection):%s\n", CLR_CYAN, CLR_RESET);
    printf("  valgrind --leak-check=full --show-leak-kinds=all ./monsters_test\n");
    
    printf("\n%sAddressSanitizer (runtime errors):%s\n", CLR_CYAN, CLR_RESET);
    printf("  cc -fsanitize=address -g monsters_test.c libft.a -o test_asan\n");
    printf("  ./test_asan\n\n");
}

/* ========== Main Runner ========== */

int main(void)
{
    banner();
    
    test_atoi();
    test_bzero_and_calloc();
    test_is_functions();
    test_strlen_dup_strl();
    test_strchr_strrchr();
    test_strncmp();
    test_strnstr();
    test_mem_functions();
    test_string_utils();
    test_split();
    test_itoa();
    test_strmapi_striteri();
    test_put_fd_functions();
    test_bonus_linked_list();
    
    print_summary();
    print_memory_tips();
    
    return (tests_run == tests_passed) ? 0 : 1;
}
