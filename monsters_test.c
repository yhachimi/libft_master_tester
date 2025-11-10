/* -------------------------------------------------------------MONSTRES TEST BY yhachimi------------------------------------------------------------------------------------------------------------------------*/
/*
Usage:
  cc monsters_test.c libft.a   # or link with your libft build command
  ./a.out

For leak/crash detection:
  valgrind --leak-check=full ./a.out
or compile with ASan:
  cc -fsanitize=address -g monsters_test.c libft.a && ./a.out
*/

#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

/* Test counters */
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

/* Read up to n bytes (handles short reads) */
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

/* Basic helpers used by many tests */
static void expect_int_eq(const char *title, int expected, int actual)
{
    char buf[256];
    snprintf(buf, sizeof(buf), "%s : expected=%d got=%d", title, expected, actual);
    report_result(buf, expected == actual);
}

static void expect_str_eq(const char *title, const char *expected, const char *actual)
{
    char buf[256];
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

/* Test functions */

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

static void test_strlen_and_dup_and_strl(void)
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

    /* ft_strlcpy and ft_strlcat: basic checks */
    char dst1[20];
    const char *src = "123456789012345";
    memset(dst1, 0, sizeof(dst1));
    size_t r1 = ft_strlcpy(dst1, src, sizeof(dst1));
    report_result("ft_strlcpy returns src length", r1 == strlen(src));
    report_result("ft_strlcpy writes null-terminated string", dst1[sizeof(dst1)-1] == '\0');

    strcpy(dst1, "pre");
    size_t catret = ft_strlcat(dst1, src, sizeof(dst1));
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

    char **parts = ft_split("a--b--c", '-');
    if (parts) {
        report_result("ft_split returned non-NULL", parts[0] != NULL);
        for (size_t i = 0; parts[i]; ++i) free(parts[i]);
        free(parts);
    } else {
        report_result("ft_split returned non-NULL", 0);
    }

    char *i0 = ft_itoa(0);
    char *i1 = ft_itoa(-2147483648);
    expect_str_eq("ft_itoa 0", "0", i0);
    expect_str_eq("ft_itoa INT_MIN", "-2147483648", i1);
    free(i0); free(i1);

    /* ft_strmapi with proper mapper */
    char mapper(unsigned int idx, char c) { (void)idx; return (char)ft_toupper((unsigned char)c); }
    char *mapped = ft_strmapi("aBcZ", mapper);
    expect_str_eq("ft_strmapi upper", "ABCZ", mapped);
    free(mapped);

    void iter(unsigned int idx, char *c) { (void)idx; *c = (char)ft_tolower((unsigned char)*c); }
    char mut[] = "HeLLo";
    ft_striteri(mut, iter);
    expect_str_eq("ft_striteri lower", "hello", mut);
}

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
    ft_putstr_fd((char*)s, p[1]); /* cast to match possible non-const prototype */
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

static void test_bonus_linked_list(void)
{
    printf("\n-- bonus linked list functions --\n");
    /* Create nodes using your bonus-named constructors if they have *_bonus suffix */
    t_list *n1 = ft_lstnew((void *)strdup("one"));
    t_list *n2 = ft_lstnew((void *)strdup("two"));
    t_list *n3 = ft_lstnew((void *)strdup("three"));
    if (!n1 || !n2 || !n3) {
        report_result("ft_lstnew_bonus returns non-NULL", 0);
        free(n1); free(n2); free(n3);
        return;
    }

    ft_lstadd_back(&n1, n2);
    ft_lstadd_back(&n1, n3);
    report_result("ft_lstadd_back produced size 3", ft_lstsize(n1) == 3);
    t_list *last = ft_lstlast(n1);
    report_result("ft_lstlast points to last node", last == n3);

    /* test ft_lstdelone_bonus on a standalone node (correct usage) */
    t_list *solo = ft_lstnew(strdup("solo"));
    if (!solo) {
        report_result("ft_lstnew_bonus(single) non-NULL", 0);
    } else {
        ft_lstdelone(solo, free); /* okay: solo wasn't part of any list */
        /* cannot check solo == NULL because lstdelone doesn't set caller pointer to NULL */
        report_result("ft_lstdelone_bonus(single node) didn't crash", 1);
    }

    /* test lstiter: run through list without crash */
    void iter_noop(void *content) { (void)content; }
    ft_lstiter(n1, iter_noop);

    /* test lstmap - create a map that strdup content with suffix */
    void *mapf(void *c) {
        char *s = (char *)c;
        size_t L = strlen(s);
        char *out = malloc(L + 3);
        if (!out) return NULL;
        strcpy(out, s);
        strcat(out, "_m");
        return out;
    }
    t_list *mapped = ft_lstmap(n1, mapf, free);
    report_result("ft_lstmap_bonus produced a new list", mapped != NULL);

    /* Now clear the original list and the mapped list safely (no double-free) */
    ft_lstclear(&n1, free);
    ft_lstclear(&mapped, free);
    report_result("ft_lstclear_bonus nulled head", n1 == NULL && mapped == NULL);
}

static void test_alloc_heuristics(void)
{
    printf("\n-- allocation heuristics & crash-safety hints --\n");
    char *s = ft_strdup("leak-check");
    if (!s) { report_result("ft_strdup non-NULL", 0); }
    else { free(s); report_result("ft_strdup alloc/free", 1); }

    char *it = ft_itoa(12345);
    if (!it) report_result("ft_itoa non-NULL", 0);
    else { free(it); report_result("ft_itoa alloc/free", 1); }
}

int main(void)
{
    printf("=== MONSTERS TEST RUN ===\n");
    test_atoi();
    test_bzero_and_calloc();
    test_is_functions();
    test_strlen_and_dup_and_strl();
    test_mem_functions();
    test_string_utils();
    test_put_fd_functions();
    test_bonus_linked_list();
    test_alloc_heuristics();

    printf("\nSummary: tests run = %d, passed = %d, failed = %d\n",
           tests_run, tests_passed, tests_run - tests_passed);

    printf("\nNotes on memory-leak and crash testing:\n"
           " - This runner performs many allocations and frees and checks correctness, but it cannot\n"
           "   detect memory leaks by itself. To detect leaks run the binary under Valgrind:\n"
           "     valgrind --leak-check=full ./a.out\n"
           " - To catch heap-buffer-overflow and many crashes at runtime, compile and run with AddressSanitizer:\n"
           "     cc -fsanitize=address -g monsters_test.c libft.a && ./a.out\n");

    return (tests_run == tests_passed) ? 0 : 1;
}
