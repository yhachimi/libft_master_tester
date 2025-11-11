/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monsters_test.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhachimi <yhachimi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11                                    */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

int tests_run = 0;
int tests_passed = 0;

/* ========== PART 1: Character Classification Tests ========== */

static void test_isalpha(void)
{
    printf("\n%s=== ft_isalpha ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_isalpha('a') && ft_isalpha('Z'))
        result_ok("ft_isalpha: lowercase and uppercase");
    else
        result_ko("ft_isalpha: lowercase and uppercase");
    
    if (!ft_isalpha('0') && !ft_isalpha(' ') && !ft_isalpha('@'))
        result_ok("ft_isalpha: non-alpha chars");
    else
        result_ko("ft_isalpha: non-alpha chars");
    
    if (!ft_isalpha(127) && !ft_isalpha(-1))
        result_ok("ft_isalpha: edge values");
    else
        result_ko("ft_isalpha: edge values");
}

static void test_isdigit(void)
{
    printf("\n%s=== ft_isdigit ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_isdigit('0') && ft_isdigit('5') && ft_isdigit('9'))
        result_ok("ft_isdigit: valid digits");
    else
        result_ko("ft_isdigit: valid digits");
    
    if (!ft_isdigit('a') && !ft_isdigit('/') && !ft_isdigit(':'))
        result_ok("ft_isdigit: non-digits");
    else
        result_ko("ft_isdigit: non-digits");
}

static void test_isalnum(void)
{
    printf("\n%s=== ft_isalnum ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_isalnum('a') && ft_isalnum('Z') && ft_isalnum('5'))
        result_ok("ft_isalnum: alphanumeric");
    else
        result_ko("ft_isalnum: alphanumeric");
    
    if (!ft_isalnum('@') && !ft_isalnum(' ') && !ft_isalnum('\n'))
        result_ok("ft_isalnum: non-alphanumeric");
    else
        result_ko("ft_isalnum: non-alphanumeric");
}

static void test_isascii(void)
{
    printf("\n%s=== ft_isascii ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_isascii(0) && ft_isascii(65) && ft_isascii(127))
        result_ok("ft_isascii: valid ASCII");
    else
        result_ko("ft_isascii: valid ASCII");
    
    if (!ft_isascii(128) && !ft_isascii(-1) && !ft_isascii(300))
        result_ok("ft_isascii: out of range");
    else
        result_ko("ft_isascii: out of range");
}

static void test_isprint(void)
{
    printf("\n%s=== ft_isprint ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_isprint(' ') && ft_isprint('~') && ft_isprint('A'))
        result_ok("ft_isprint: printable chars");
    else
        result_ko("ft_isprint: printable chars");
    
    if (!ft_isprint(31) && !ft_isprint(127) && !ft_isprint('\n'))
        result_ok("ft_isprint: non-printable");
    else
        result_ko("ft_isprint: non-printable");
}

/* ========== String Length Tests ========== */

static void test_strlen(void)
{
    printf("\n%s=== ft_strlen ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_strlen("") == 0)
        result_ok("ft_strlen: empty string");
    else
        result_ko("ft_strlen: empty string");
    
    if (ft_strlen("Hello") == 5)
        result_ok("ft_strlen: normal string");
    else
        result_ko("ft_strlen: normal string");
    
    if (ft_strlen("1337 School") == 11)
        result_ok("ft_strlen: string with space");
    else
        result_ko("ft_strlen: string with space");
    
    char long_str[1000];
    memset(long_str, 'A', 999);
    long_str[999] = '\0';
    if (ft_strlen(long_str) == 999)
        result_ok("ft_strlen: long string");
    else
        result_ko("ft_strlen: long string");
}

/* ========== Memory Functions Tests ========== */

static void test_memset(void)
{
    printf("\n%s=== ft_memset ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char buf[10];
    ft_memset(buf, 'A', 10);
    if (buf[0] == 'A' && buf[9] == 'A')
        result_ok("ft_memset: basic fill");
    else
        result_ko("ft_memset: basic fill");
    
    ft_memset(buf, 0, 10);
    if (buf[0] == 0 && buf[9] == 0)
        result_ok("ft_memset: zero fill");
    else
        result_ko("ft_memset: zero fill");
    
    ft_memset(buf, 255, 5);
    if ((unsigned char)buf[0] == 255 && (unsigned char)buf[4] == 255)
        result_ok("ft_memset: byte value 255");
    else
        result_ko("ft_memset: byte value 255");
}

static void test_bzero(void)
{
    printf("\n%s=== ft_bzero ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char buf[10] = "123456789";
    ft_bzero(buf, 5);
    if (buf[0] == 0 && buf[4] == 0 && buf[5] == '6')
        result_ok("ft_bzero: partial zero");
    else
        result_ko("ft_bzero: partial zero");
    
    ft_bzero(buf, 0);
    if (buf[5] == '6')
        result_ok("ft_bzero: zero length");
    else
        result_ko("ft_bzero: zero length");
}

static void test_memcpy(void)
{
    printf("\n%s=== ft_memcpy ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char src[] = "Hello World";
    char dst[20];
    
    ft_memcpy(dst, src, strlen(src) + 1);
    if (strcmp(dst, "Hello World") == 0)
        result_ok("ft_memcpy: basic copy");
    else
        result_ko("ft_memcpy: basic copy");
    
    char buf[10] = {0};
    ft_memcpy(buf, "test", 0);
    if (buf[0] == 0)
        result_ok("ft_memcpy: zero length");
    else
        result_ko("ft_memcpy: zero length");
    
    unsigned char binary[5] = {0, 1, 255, 128, 42};
    unsigned char binary_dst[5];
    ft_memcpy(binary_dst, binary, 5);
    if (memcmp(binary, binary_dst, 5) == 0)
        result_ok("ft_memcpy: binary data");
    else
        result_ko("ft_memcpy: binary data");
}

static void test_memmove(void)
{
    printf("\n%s=== ft_memmove ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char buf1[20] = "Hello World";
    ft_memmove(buf1 + 6, buf1, 5);
    if (strcmp(buf1, "Hello Hello") == 0)
        result_ok("ft_memmove: overlapping forward");
    else
        result_ko("ft_memmove: overlapping forward");
    
    char buf2[20] = "Hello World";
    ft_memmove(buf2, buf2 + 6, 5);
    if (strncmp(buf2, "World", 5) == 0)
        result_ok("ft_memmove: overlapping backward");
    else
        result_ko("ft_memmove: overlapping backward");
    
    char src[] = "Test";
    char dst[10];
    ft_memmove(dst, src, 5);
    if (strcmp(dst, "Test") == 0)
        result_ok("ft_memmove: non-overlapping");
    else
        result_ko("ft_memmove: non-overlapping");
}

static void test_memchr(void)
{
    printf("\n%s=== ft_memchr ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char str[] = "Hello World";
    if (ft_memchr(str, 'W', strlen(str)) == &str[6])
        result_ok("ft_memchr: char found");
    else
        result_ko("ft_memchr: char found");
    
    if (ft_memchr(str, 'Z', strlen(str)) == NULL)
        result_ok("ft_memchr: char not found");
    else
        result_ko("ft_memchr: char not found");
    
    if (ft_memchr(str, '\0', strlen(str) + 1) == &str[11])
        result_ok("ft_memchr: null terminator");
    else
        result_ko("ft_memchr: null terminator");
    
    char binary[] = {0, 1, 2, 255, 4};
    if (ft_memchr(binary, 255, 5) == &binary[3])
        result_ok("ft_memchr: byte 255");
    else
        result_ko("ft_memchr: byte 255");
}

static void test_memcmp(void)
{
    printf("\n%s=== ft_memcmp ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_memcmp("Hello", "Hello", 5) == 0)
        result_ok("ft_memcmp: equal strings");
    else
        result_ko("ft_memcmp: equal strings");
    
    if (ft_memcmp("Hello", "World", 5) < 0)
        result_ok("ft_memcmp: first < second");
    else
        result_ko("ft_memcmp: first < second");
    
    if (ft_memcmp("World", "Hello", 5) > 0)
        result_ok("ft_memcmp: first > second");
    else
        result_ko("ft_memcmp: first > second");
    
    if (ft_memcmp("test\200", "test\0", 5) > 0)
        result_ok("ft_memcmp: unsigned comparison");
    else
        result_ko("ft_memcmp: unsigned comparison");
    
    if (ft_memcmp("Hello", "Hellx", 0) == 0)
        result_ok("ft_memcmp: zero length");
    else
        result_ko("ft_memcmp: zero length");
}

/* ========== String Search Functions ========== */

static void test_strchr(void)
{
    printf("\n%s=== ft_strchr ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char str[] = "Hello World";
    if (ft_strchr(str, 'W') == &str[6])
        result_ok("ft_strchr: char found");
    else
        result_ko("ft_strchr: char found");
    
    if (ft_strchr(str, 'Z') == NULL)
        result_ok("ft_strchr: char not found");
    else
        result_ko("ft_strchr: char not found");
    
    if (ft_strchr(str, '\0') == &str[11])
        result_ok("ft_strchr: null terminator");
    else
        result_ko("ft_strchr: null terminator");
    
    if (ft_strchr("", 'a') == NULL)
        result_ok("ft_strchr: empty string");
    else
        result_ko("ft_strchr: empty string");
    
    if (ft_strchr("teste", 'e') == strchr("teste", 'e'))
        result_ok("ft_strchr: first occurrence");
    else
        result_ko("ft_strchr: first occurrence");
}

static void test_strrchr(void)
{
    printf("\n%s=== ft_strrchr ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char str[] = "Hello World";
    if (ft_strrchr(str, 'o') == &str[7])
        result_ok("ft_strrchr: last occurrence");
    else
        result_ko("ft_strrchr: last occurrence");
    
    if (ft_strrchr(str, 'Z') == NULL)
        result_ok("ft_strrchr: char not found");
    else
        result_ko("ft_strrchr: char not found");
    
    if (ft_strrchr(str, '\0') == &str[11])
        result_ok("ft_strrchr: null terminator");
    else
        result_ko("ft_strrchr: null terminator");
    
    if (ft_strrchr("", '\0') != NULL)
        result_ok("ft_strrchr: empty string null");
    else
        result_ko("ft_strrchr: empty string null");
}

static void test_strnstr(void)
{
    printf("\n%s=== ft_strnstr ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char haystack[] = "Hello World";
    if (ft_strnstr(haystack, "World", 11) == &haystack[6])
        result_ok("ft_strnstr: substring found");
    else
        result_ko("ft_strnstr: substring found");
    
    if (ft_strnstr(haystack, "World", 8) == NULL)
        result_ok("ft_strnstr: length too short");
    else
        result_ko("ft_strnstr: length too short");
    
    if (ft_strnstr(haystack, "", 11) == haystack)
        result_ok("ft_strnstr: empty needle");
    else
        result_ko("ft_strnstr: empty needle");
    
    if (ft_strnstr("", "test", 5) == NULL)
        result_ok("ft_strnstr: empty haystack");
    else
        result_ko("ft_strnstr: empty haystack");
    
    if (ft_strnstr("lorem ipsum dolor sit amet", "dolor", 15) == NULL)
        result_ok("ft_strnstr: needle beyond len");
    else
        result_ko("ft_strnstr: needle beyond len");
}

static void test_strncmp(void)
{
    printf("\n%s=== ft_strncmp ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_strncmp("Hello", "Hello", 5) == 0)
        result_ok("ft_strncmp: equal strings");
    else
        result_ko("ft_strncmp: equal strings");
    
    if (ft_strncmp("Hello", "Hellx", 4) == 0)
        result_ok("ft_strncmp: partial match");
    else
        result_ko("ft_strncmp: partial match");
    
    if (ft_strncmp("ABC", "ABZ", 3) < 0)
        result_ok("ft_strncmp: first < second");
    else
        result_ko("ft_strncmp: first < second");
    
    if (ft_strncmp("test\200", "test\0", 6) > 0)
        result_ok("ft_strncmp: unsigned comparison");
    else
        result_ko("ft_strncmp: unsigned comparison");
    
    if (ft_strncmp("Hello", "World", 0) == 0)
        result_ok("ft_strncmp: zero length");
    else
        result_ko("ft_strncmp: zero length");
}

/* ========== String Copy Functions ========== */

static void test_strlcpy(void)
{
    printf("\n%s=== ft_strlcpy ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char dst[20];
    size_t ret = ft_strlcpy(dst, "Hello", 20);
    if (ret == 5 && strcmp(dst, "Hello") == 0)
        result_ok("ft_strlcpy: basic copy");
    else
        result_ko("ft_strlcpy: basic copy");
    
    ret = ft_strlcpy(dst, "Hello World", 6);
    if (ret == 11 && strcmp(dst, "Hello") == 0)
        result_ok("ft_strlcpy: truncation");
    else
        result_ko("ft_strlcpy: truncation");
    
    ret = ft_strlcpy(dst, "Test", 1);
    if (ret == 4 && dst[0] == '\0')
        result_ok("ft_strlcpy: size 1");
    else
        result_ko("ft_strlcpy: size 1");
    
    ret = ft_strlcpy(dst, "Test", 0);
    if (ret == 4)
        result_ok("ft_strlcpy: size 0");
    else
        result_ko("ft_strlcpy: size 0");
}

static void test_strlcat(void)
{
    printf("\n%s=== ft_strlcat ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char dst[20] = "Hello ";
    size_t ret = ft_strlcat(dst, "World", 20);
    if (ret == 11 && strcmp(dst, "Hello World") == 0)
        result_ok("ft_strlcat: basic concat");
    else
        result_ko("ft_strlcat: basic concat");
    
    char dst2[10] = "Hello";
    ret = ft_strlcat(dst2, " World", 10);
    if (ret == 11 && strcmp(dst2, "Hello Wor") == 0)
        result_ok("ft_strlcat: truncation");
    else
        result_ko("ft_strlcat: truncation");
    
    char dst3[5] = "Hi";
    ret = ft_strlcat(dst3, "Test", 2);
    if (ret == 6)
        result_ok("ft_strlcat: size <= initial len");
    else
        result_ko("ft_strlcat: size <= initial len");
}

/* ========== Character Conversion ========== */

static void test_toupper(void)
{
    printf("\n%s=== ft_toupper ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_toupper('a') == 'A' && ft_toupper('z') == 'Z')
        result_ok("ft_toupper: lowercase to uppercase");
    else
        result_ko("ft_toupper: lowercase to uppercase");
    
    if (ft_toupper('A') == 'A' && ft_toupper('5') == '5')
        result_ok("ft_toupper: non-lowercase unchanged");
    else
        result_ko("ft_toupper: non-lowercase unchanged");
}

static void test_tolower(void)
{
    printf("\n%s=== ft_tolower ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_tolower('A') == 'a' && ft_tolower('Z') == 'z')
        result_ok("ft_tolower: uppercase to lowercase");
    else
        result_ko("ft_tolower: uppercase to lowercase");
    
    if (ft_tolower('a') == 'a' && ft_tolower('5') == '5')
        result_ok("ft_tolower: non-uppercase unchanged");
    else
        result_ko("ft_tolower: non-uppercase unchanged");
}

/* ========== atoi Tests ========== */

static void test_atoi(void)
{
    printf("\n%s=== ft_atoi ===%s\n", CLR_YELLOW, CLR_RESET);
    
    if (ft_atoi("42") == 42)
        result_ok("ft_atoi: simple positive");
    else
        result_ko("ft_atoi: simple positive");
    
    if (ft_atoi("-42") == -42)
        result_ok("ft_atoi: simple negative");
    else
        result_ko("ft_atoi: simple negative");
    
    if (ft_atoi("   +123") == 123)
        result_ok("ft_atoi: whitespace and +");
    else
        result_ko("ft_atoi: whitespace and +");
    
    if (ft_atoi("0") == 0)
        result_ok("ft_atoi: zero");
    else
        result_ko("ft_atoi: zero");
    
    if (ft_atoi("2147483647") == 2147483647)
        result_ok("ft_atoi: INT_MAX");
    else
        result_ko("ft_atoi: INT_MAX");
    
    if (ft_atoi("-2147483648") == -2147483648)
        result_ok("ft_atoi: INT_MIN");
    else
        result_ko("ft_atoi: INT_MIN");
    
    if (ft_atoi("  \t\n  -123abc") == -123)
        result_ok("ft_atoi: stops at non-digit");
    else
        result_ko("ft_atoi: stops at non-digit");
    
    if (ft_atoi("") == 0)
        result_ok("ft_atoi: empty string");
    else
        result_ko("ft_atoi: empty string");
}

/* ========== calloc Tests ========== */

static void test_calloc(void)
{
    printf("\n%s=== ft_calloc ===%s\n", CLR_YELLOW, CLR_RESET);
    
    int *arr = (int *)ft_calloc(5, sizeof(int));
    if (arr && arr[0] == 0 && arr[4] == 0)
        result_ok("ft_calloc: zeroed memory");
    else
        result_ko("ft_calloc: zeroed memory");
    free(arr);
    
    char *str = (char *)ft_calloc(10, sizeof(char));
    if (str && str[0] == 0 && str[9] == 0)
        result_ok("ft_calloc: char array");
    else
        result_ko("ft_calloc: char array");
    free(str);
    
    void *ptr = ft_calloc(0, 10);
    if (ptr)
        free(ptr);
    result_ok("ft_calloc: zero count");
    
    ptr = ft_calloc(10, 0);
    if (ptr)
        free(ptr);
    result_ok("ft_calloc: zero size");
}

/* ========== strdup Tests ========== */

static void test_strdup(void)
{
    printf("\n%s=== ft_strdup ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *dup = ft_strdup("Hello");
    if (dup && strcmp(dup, "Hello") == 0)
        result_ok("ft_strdup: basic duplicate");
    else
        result_ko("ft_strdup: basic duplicate");
    free(dup);
    
    dup = ft_strdup("");
    if (dup && strcmp(dup, "") == 0)
        result_ok("ft_strdup: empty string");
    else
        result_ko("ft_strdup: empty string");
    free(dup);
    
    char original[] = "1337 School";
    dup = ft_strdup(original);
    if (dup && dup != original && strcmp(dup, original) == 0)
        result_ok("ft_strdup: different address");
    else
        result_ko("ft_strdup: different address");
    free(dup);
}

/* ========== PART 2: Additional Functions ========== */

static void test_substr(void)
{
    printf("\n%s=== ft_substr ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *sub = ft_substr("Hello World", 6, 5);
    if (sub && strcmp(sub, "World") == 0)
        result_ok("ft_substr: basic substring");
    else
        result_ko("ft_substr: basic substring");
    free(sub);
    
    sub = ft_substr("Hello", 0, 3);
    if (sub && strcmp(sub, "Hel") == 0)
        result_ok("ft_substr: from start");
    else
        result_ko("ft_substr: from start");
    free(sub);
    
    sub = ft_substr("Hello", 2, 100);
    if (sub && strcmp(sub, "llo") == 0)
        result_ok("ft_substr: len > available");
    else
        result_ko("ft_substr: len > available");
    free(sub);
    
    sub = ft_substr("Hello", 10, 5);
    if (sub && strcmp(sub, "") == 0)
        result_ok("ft_substr: start beyond string");
    else
        result_ko("ft_substr: start beyond string");
    free(sub);
    
    sub = ft_substr("", 0, 5);
    if (sub && strcmp(sub, "") == 0)
        result_ok("ft_substr: empty string");
    else
        result_ko("ft_substr: empty string");
    free(sub);
}

static void test_strjoin(void)
{
    printf("\n%s=== ft_strjoin ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *join = ft_strjoin("Hello ", "World");
    if (join && strcmp(join, "Hello World") == 0)
        result_ok("ft_strjoin: basic join");
    else
        result_ko("ft_strjoin: basic join");
    free(join);
    
    join = ft_strjoin("", "Test");
    if (join && strcmp(join, "Test") == 0)
        result_ok("ft_strjoin: empty first");
    else
        result_ko("ft_strjoin: empty first");
    free(join);
    
    join = ft_strjoin("Test", "");
    if (join && strcmp(join, "Test") == 0)
        result_ok("ft_strjoin: empty second");
    else
        result_ko("ft_strjoin: empty second");
    free(join);
    
    join = ft_strjoin("", "");
    if (join && strcmp(join, "") == 0)
        result_ok("ft_strjoin: both empty");
    else
        result_ko("ft_strjoin: both empty");
    free(join);
}

static void test_strtrim(void)
{
    printf("\n%s=== ft_strtrim ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *trim = ft_strtrim("   Hello   ", " ");
    if (trim && strcmp(trim, "Hello") == 0)
        result_ok("ft_strtrim: spaces both sides");
    else
        result_ko("ft_strtrim: spaces both sides");
    free(trim);
    
    trim = ft_strtrim("xxxHelloxxx", "x");
    if (trim && strcmp(trim, "Hello") == 0)
        result_ok("ft_strtrim: custom charset");
    else
        result_ko("ft_strtrim: custom charset");
    free(trim);
    
    trim = ft_strtrim("   ", " ");
    if (trim && strcmp(trim, "") == 0)
        result_ok("ft_strtrim: all trimmed");
    else
        result_ko("ft_strtrim: all trimmed");
    free(trim);
    
    trim = ft_strtrim("Hello", "xyz");
    if (trim && strcmp(trim, "Hello") == 0)
        result_ok("ft_strtrim: nothing to trim");
    else
        result_ko("ft_strtrim: nothing to trim");
    free(trim);
    
    trim = ft_strtrim("", " ");
    if (trim && strcmp(trim, "") == 0)
        result_ok("ft_strtrim: empty string");
    else
        result_ko("ft_strtrim: empty string");
    free(trim);
}

static void test_split(void)
{
    printf("\n%s=== ft_split ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char **split = ft_split("Hello World Test", ' ');
    if (split && strcmp(split[0], "Hello") == 0 && 
        strcmp(split[1], "World") == 0 && 
        strcmp(split[2], "Test") == 0 && 
        split[3] == NULL)
        result_ok("ft_split: basic split");
    else
        result_ko("ft_split: basic split");
    for (int i = 0; split && split[i]; i++)
        free(split[i]);
    free(split);
    
    split = ft_split("   Hello   World   ", ' ');
    if (split && strcmp(split[0], "Hello") == 0 && 
        strcmp(split[1], "World") == 0 && 
        split[2] == NULL)
        result_ok("ft_split: multiple delimiters");
    else
        result_ko("ft_split: multiple delimiters");
    for (int i = 0; split && split[i]; i++)
        free(split[i]);
    free(split);
    
    split = ft_split("", ' ');
    if (split && split[0] == NULL)
        result_ok("ft_split: empty string");
    else
        result_ko("ft_split: empty string");
    free(split);
    
    split = ft_split("Hello", ' ');
    if (split && strcmp(split[0], "Hello") == 0 && split[1] == NULL)
        result_ok("ft_split: no delimiter");
    else
        result_ko("ft_split: no delimiter");
    for (int i = 0; split && split[i]; i++)
        free(split[i]);
    free(split);
    
    split = ft_split("   ", ' ');
    if (split && split[0] == NULL)
        result_ok("ft_split: only delimiters");
    else
        result_ko("ft_split: only delimiters");
    free(split);
}

static void test_itoa(void)
{
    printf("\n%s=== ft_itoa ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *str = ft_itoa(42);
    if (str && strcmp(str, "42") == 0)
        result_ok("ft_itoa: positive number");
    else
        result_ko("ft_itoa: positive number");
    free(str);
    
    str = ft_itoa(-42);
    if (str && strcmp(str, "-42") == 0)
        result_ok("ft_itoa: negative number");
    else
        result_ko("ft_itoa: negative number");
    free(str);
    
    str = ft_itoa(0);
    if (str && strcmp(str, "0") == 0)
        result_ok("ft_itoa: zero");
    else
        result_ko("ft_itoa: zero");
    free(str);
    
    str = ft_itoa(2147483647);
    if (str && strcmp(str, "2147483647") == 0)
        result_ok("ft_itoa: INT_MAX");
    else
        result_ko("ft_itoa: INT_MAX");
    free(str);
    
    str = ft_itoa(-2147483648);
    if (str && strcmp(str, "-2147483648") == 0)
        result_ok("ft_itoa: INT_MIN");
    else
        result_ko("ft_itoa: INT_MIN");
    free(str);
}

static char test_map_func(unsigned int i, char c)
{
    (void)i;
    if (c >= 'a' && c <= 'z')
        return c - 32;
    return c;
}

static void test_strmapi(void)
{
    printf("\n%s=== ft_strmapi ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *result = ft_strmapi("hello", test_map_func);
    if (result && strcmp(result, "HELLO") == 0)
        result_ok("ft_strmapi: uppercase mapping");
    else
        result_ko("ft_strmapi: uppercase mapping");
    free(result);
    
    result = ft_strmapi("", test_map_func);
    if (result && strcmp(result, "") == 0)
        result_ok("ft_strmapi: empty string");
    else
        result_ko("ft_strmapi: empty string");
    free(result);
}

static void test_iter_func(unsigned int i, char *c)
{
    (void)i;
    if (*c >= 'a' && *c <= 'z')
        *c = *c - 32;
}

static void test_striteri(void)
{
    printf("\n%s=== ft_striteri ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char str[] = "hello";
    ft_striteri(str, test_iter_func);
    if (strcmp(str, "HELLO") == 0)
        result_ok("ft_striteri: uppercase in place");
    else
        result_ko("ft_striteri: uppercase in place");
    
    char empty[] = "";
    ft_striteri(empty, test_iter_func);
    if (strcmp(empty, "") == 0)
        result_ok("ft_striteri: empty string");
    else
        result_ko("ft_striteri: empty string");
}

static void test_putchar_fd(void)
{
    printf("\n%s=== ft_putchar_fd ===%s\n", CLR_YELLOW, CLR_RESET);
    
    printf("Testing ft_putchar_fd (visual check): ");
    ft_putchar_fd('A', 1);
    ft_putchar_fd('B', 1);
    ft_putchar_fd('\n', 1);
    result_ok("ft_putchar_fd: executed");
}

static void test_putstr_fd(void)
{
    printf("\n%s=== ft_putstr_fd ===%s\n", CLR_YELLOW, CLR_RESET);
    
    printf("Testing ft_putstr_fd (visual check): ");
    ft_putstr_fd("Hello World", 1);
    printf("\n");
    result_ok("ft_putstr_fd: executed");
    
    ft_putstr_fd("", 1);
    result_ok("ft_putstr_fd: empty string");
}

static void test_putendl_fd(void)
{
    printf("\n%s=== ft_putendl_fd ===%s\n", CLR_YELLOW, CLR_RESET);
    
    printf("Testing ft_putendl_fd (visual check): ");
    ft_putendl_fd("Hello World", 1);
    result_ok("ft_putendl_fd: executed");
    
    ft_putendl_fd("", 1);
    result_ok("ft_putendl_fd: empty string");
}

static void test_putnbr_fd(void)
{
    printf("\n%s=== ft_putnbr_fd ===%s\n", CLR_YELLOW, CLR_RESET);
    
    printf("Testing ft_putnbr_fd (visual check): ");
    ft_putnbr_fd(42, 1);
    printf(" ");
    ft_putnbr_fd(-42, 1);
    printf(" ");
    ft_putnbr_fd(0, 1);
    printf("\n");
    result_ok("ft_putnbr_fd: basic numbers");
    
    printf("INT_MIN and INT_MAX: ");
    ft_putnbr_fd(-2147483648, 1);
    printf(" ");
    ft_putnbr_fd(2147483647, 1);
    printf("\n");
    result_ok("ft_putnbr_fd: edge values");
}

/* ========== Main Test Runner ========== */

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    banner();
    
    printf("\n%s%s╔════════════════════════════════════════════════╗%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    printf("%s%s║    PART 1: Libc Functions                      ║%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    printf("%s%s╚════════════════════════════════════════════════╝%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    
    test_isalpha();
    test_isdigit();
    test_isalnum();
    test_isascii();
    test_isprint();
    test_strlen();
    test_memset();
    test_bzero();
    test_memcpy();
    test_memmove();
    test_memchr();
    test_memcmp();
    test_strchr();
    test_strrchr();
    test_strnstr();
    test_strncmp();
    test_strlcpy();
    test_strlcat();
    test_toupper();
    test_tolower();
    test_atoi();
    test_calloc();
    test_strdup();
    
    printf("\n%s%s╔════════════════════════════════════════════════╗%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    printf("%s%s║    PART 2: Additional Functions                ║%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    printf("%s%s╚════════════════════════════════════════════════╝%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    
    test_substr();
    test_strjoin();
    test_strtrim();
    test_split();
    test_itoa();
    test_strmapi();
    test_striteri();
    test_putchar_fd();
    test_putstr_fd();
    test_putendl_fd();
    test_putnbr_fd();
    
    summary();
    
    if (tests_run == tests_passed)
    {
        printf("\n%s%s🎉 ALL MANDATORY TESTS PASSED! 🎉%s\n\n", 
               CLR_BOLD, CLR_GREEN, CLR_RESET);
        return (0);
    }
    else
    {
        printf("\n%s%s❌ SOME TESTS FAILED ❌%s\n\n", 
               CLR_BOLD, CLR_RED, CLR_RESET);
        return (1);
    }
}
