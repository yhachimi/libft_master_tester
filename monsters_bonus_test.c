/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monsters_bonus_test.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhachimi <yhachimi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11                                    */
/*                                                                            */
/* ************************************************************************** */

#include "test_utils.h"

int tests_run = 0;
int tests_passed = 0;

/* ========== Helper Functions for Testing ========== */

static int list_length(t_list *lst)
{
    int len = 0;
    while (lst)
    {
        len++;
        lst = lst->next;
    }
    return len;
}

static void free_list(t_list *lst)
{
    t_list *tmp;
    while (lst)
    {
        tmp = lst->next;
        free(lst);
        lst = tmp;
    }
}

static void free_list_with_content(t_list *lst)
{
    t_list *tmp;
    while (lst)
    {
        tmp = lst->next;
        free(lst->content);
        free(lst);
        lst = tmp;
    }
}

static void del_content(void *content)
{
    free(content);
}

static void iter_upper(void *content)
{
    char *str = (char *)content;
    while (*str)
    {
        if (*str >= 'a' && *str <= 'z')
            *str = *str - 32;
        str++;
    }
}

static void *map_dup(void *content)
{
    char *str = (char *)content;
    return strdup(str);
}

/* ========== ft_lstnew Tests ========== */

static void test_lstnew(void)
{
    printf("\n%s=== ft_lstnew ===%s\n", CLR_YELLOW, CLR_RESET);
    
    t_list *node = ft_lstnew("1337");
    if (node && strcmp(node->content, "1337") == 0 && node->next == NULL)
        result_ok("ft_lstnew: basic creation");
    else
        result_ko("ft_lstnew: basic creation");
    free(node);
    
    node = ft_lstnew(NULL);
    if (node && node->content == NULL && node->next == NULL)
        result_ok("ft_lstnew: NULL content");
    else
        result_ko("ft_lstnew: NULL content");
    free(node);
    
    int value = 42;
    node = ft_lstnew(&value);
    if (node && *(int *)node->content == 42)
        result_ok("ft_lstnew: integer content");
    else
        result_ko("ft_lstnew: integer content");
    free(node);
    
    char *str = strdup("test");
    node = ft_lstnew(str);
    if (node && node->content == str)
        result_ok("ft_lstnew: pointer preservation");
    else
        result_ko("ft_lstnew: pointer preservation");
    free(str);
    free(node);
}

/* ========== ft_lstadd_front Tests ========== */

static void test_lstadd_front(void)
{
    printf("\n%s=== ft_lstadd_front ===%s\n", CLR_YELLOW, CLR_RESET);
    
    t_list *lst = NULL;
    t_list *node1 = ft_lstnew("first");
    ft_lstadd_front(&lst, node1);
    if (lst == node1 && strcmp(lst->content, "first") == 0)
        result_ok("ft_lstadd_front: add to empty list");
    else
        result_ko("ft_lstadd_front: add to empty list");
    
    t_list *node2 = ft_lstnew("second");
    ft_lstadd_front(&lst, node2);
    if (lst == node2 && strcmp(lst->content, "second") == 0 && 
        lst->next == node1 && strcmp(lst->next->content, "first") == 0)
        result_ok("ft_lstadd_front: add to existing list");
    else
        result_ko("ft_lstadd_front: add to existing list");
    
    t_list *node3 = ft_lstnew("third");
    ft_lstadd_front(&lst, node3);
    if (lst == node3 && list_length(lst) == 3)
        result_ok("ft_lstadd_front: multiple additions");
    else
        result_ko("ft_lstadd_front: multiple additions");
    
    free_list(lst);
    
    lst = NULL;
    ft_lstadd_front(&lst, NULL);
    if (lst == NULL)
        result_ok("ft_lstadd_front: NULL node");
    else
        result_ko("ft_lstadd_front: NULL node");
}

/* ========== ft_lstsize Tests ========== */

static void test_lstsize(void)
{
    printf("\n%s=== ft_lstsize ===%s\n", CLR_YELLOW, CLR_RESET);
    
    t_list *lst = NULL;
    if (ft_lstsize(lst) == 0)
        result_ok("ft_lstsize: empty list");
    else
        result_ko("ft_lstsize: empty list");
    
    t_list *node1 = ft_lstnew("1");
    lst = node1;
    if (ft_lstsize(lst) == 1)
        result_ok("ft_lstsize: one node");
    else
        result_ko("ft_lstsize: one node");
    
    t_list *node2 = ft_lstnew("2");
    t_list *node3 = ft_lstnew("3");
    t_list *node4 = ft_lstnew("4");
    t_list *node5 = ft_lstnew("5");
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node5;
    
    if (ft_lstsize(lst) == 5)
        result_ok("ft_lstsize: five nodes");
    else
        result_ko("ft_lstsize: five nodes");
    
    free_list(lst);
    
    t_list *single = ft_lstnew("single");
    if (ft_lstsize(single) == 1)
        result_ok("ft_lstsize: single node with NULL next");
    else
        result_ko("ft_lstsize: single node with NULL next");
    free(single);
}

/* ========== ft_lstlast Tests ========== */

static void test_lstlast(void)
{
    printf("\n%s=== ft_lstlast ===%s\n", CLR_YELLOW, CLR_RESET);
    
    t_list *lst = NULL;
    if (ft_lstlast(lst) == NULL)
        result_ok("ft_lstlast: empty list");
    else
        result_ko("ft_lstlast: empty list");
    
    t_list *node1 = ft_lstnew("first");
    lst = node1;
    if (ft_lstlast(lst) == node1)
        result_ok("ft_lstlast: single node");
    else
        result_ko("ft_lstlast: single node");
    
    t_list *node2 = ft_lstnew("second");
    t_list *node3 = ft_lstnew("third");
    node1->next = node2;
    node2->next = node3;
    
    if (ft_lstlast(lst) == node3 && strcmp(ft_lstlast(lst)->content, "third") == 0)
        result_ok("ft_lstlast: multiple nodes");
    else
        result_ko("ft_lstlast: multiple nodes");
    
    free_list(lst);
}

/* ========== ft_lstadd_back Tests ========== */

static void test_lstadd_back(void)
{
    printf("\n%s=== ft_lstadd_back ===%s\n", CLR_YELLOW, CLR_RESET);
    
    t_list *lst = NULL;
    t_list *node1 = ft_lstnew("first");
    ft_lstadd_back(&lst, node1);
    if (lst == node1 && strcmp(lst->content, "first") == 0)
        result_ok("ft_lstadd_back: add to empty list");
    else
        result_ko("ft_lstadd_back: add to empty list");
    
    t_list *node2 = ft_lstnew("second");
    ft_lstadd_back(&lst, node2);
    if (lst == node1 && lst->next == node2 && 
        strcmp(lst->next->content, "second") == 0)
        result_ok("ft_lstadd_back: add to existing list");
    else
        result_ko("ft_lstadd_back: add to existing list");
    
    t_list *node3 = ft_lstnew("third");
    ft_lstadd_back(&lst, node3);
    if (ft_lstlast(lst) == node3 && list_length(lst) == 3)
        result_ok("ft_lstadd_back: multiple additions");
    else
        result_ko("ft_lstadd_back: multiple additions");
    
    free_list(lst);
    
    lst = NULL;
    ft_lstadd_back(&lst, NULL);
    if (lst == NULL)
        result_ok("ft_lstadd_back: NULL node");
    else
        result_ko("ft_lstadd_back: NULL node");
}

/* ========== ft_lstdelone Tests ========== */

static void test_lstdelone(void)
{
    printf("\n%s=== ft_lstdelone ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *content = strdup("test");
    t_list *node = ft_lstnew(content);
    ft_lstdelone(node, del_content);
    result_ok("ft_lstdelone: delete with content free");
    
    node = ft_lstnew("static string");
    ft_lstdelone(node, NULL);
    result_ok("ft_lstdelone: delete without content free");
    
    ft_lstdelone(NULL, del_content);
    result_ok("ft_lstdelone: NULL node");
}

/* ========== ft_lstclear Tests ========== */

static void test_lstclear(void)
{
    printf("\n%s=== ft_lstclear ===%s\n", CLR_YELLOW, CLR_RESET);
    
    t_list *lst = ft_lstnew(strdup("1"));
    ft_lstadd_back(&lst, ft_lstnew(strdup("2")));
    ft_lstadd_back(&lst, ft_lstnew(strdup("3")));
    
    ft_lstclear(&lst, del_content);
    if (lst == NULL)
        result_ok("ft_lstclear: clear entire list");
    else
        result_ko("ft_lstclear: clear entire list");
    
    lst = ft_lstnew(strdup("single"));
    ft_lstclear(&lst, del_content);
    if (lst == NULL)
        result_ok("ft_lstclear: clear single node");
    else
        result_ko("ft_lstclear: clear single node");
    
    lst = NULL;
    ft_lstclear(&lst, del_content);
    if (lst == NULL)
        result_ok("ft_lstclear: clear empty list");
    else
        result_ko("ft_lstclear: clear empty list");
    
    lst = ft_lstnew("test1");
    ft_lstadd_back(&lst, ft_lstnew("test2"));
    ft_lstclear(&lst, NULL);
    if (lst == NULL)
        result_ok("ft_lstclear: clear without del function");
    else
        result_ko("ft_lstclear: clear without del function");
}

/* ========== ft_lstiter Tests ========== */

static void test_lstiter(void)
{
    printf("\n%s=== ft_lstiter ===%s\n", CLR_YELLOW, CLR_RESET);
    
    char *str1 = strdup("hello");
    char *str2 = strdup("world");
    char *str3 = strdup("test");
    
    t_list *lst = ft_lstnew(str1);
    ft_lstadd_back(&lst, ft_lstnew(str2));
    ft_lstadd_back(&lst, ft_lstnew(str3));
    
    ft_lstiter(lst, iter_upper);
    
    if (strcmp(lst->content, "HELLO") == 0 && 
        strcmp(lst->next->content, "WORLD") == 0 && 
        strcmp(lst->next->next->content, "TEST") == 0)
        result_ok("ft_lstiter: apply function to all");
    else
        result_ko("ft_lstiter: apply function to all");
    
    free_list_with_content(lst);
    
    lst = NULL;
    ft_lstiter(lst, iter_upper);
    result_ok("ft_lstiter: empty list");
    
    lst = ft_lstnew(strdup("single"));
    ft_lstiter(lst, iter_upper);
    if (strcmp(lst->content, "SINGLE") == 0)
        result_ok("ft_lstiter: single node");
    else
        result_ko("ft_lstiter: single node");
    free_list_with_content(lst);
}

/* ========== ft_lstmap Tests ========== */

static void test_lstmap(void)
{
    printf("\n%s=== ft_lstmap ===%s\n", CLR_YELLOW, CLR_RESET);
    
    t_list *lst = ft_lstnew("first");
    ft_lstadd_back(&lst, ft_lstnew("second"));
    ft_lstadd_back(&lst, ft_lstnew("third"));
    
    t_list *mapped = ft_lstmap(lst, map_dup, del_content);
    
    if (mapped && 
        strcmp(mapped->content, "first") == 0 && 
        strcmp(mapped->next->content, "second") == 0 && 
        strcmp(mapped->next->next->content, "third") == 0 && 
        mapped->content != lst->content)
        result_ok("ft_lstmap: map with duplication");
    else
        result_ko("ft_lstmap: map with duplication");
    
    free_list(lst);
    ft_lstclear(&mapped, del_content);
    
    lst = NULL;
    mapped = ft_lstmap(lst, map_dup, del_content);
    if (mapped == NULL)
        result_ok("ft_lstmap: empty list");
    else
        result_ko("ft_lstmap: empty list");
    
    lst = ft_lstnew("single");
    mapped = ft_lstmap(lst, map_dup, del_content);
    if (mapped && strcmp(mapped->content, "single") == 0 && 
        mapped->next == NULL)
        result_ok("ft_lstmap: single node");
    else
        result_ko("ft_lstmap: single node");
    free_list(lst);
    ft_lstclear(&mapped, del_content);
}

/* ========== Edge Cases and Stress Tests ========== */

static void test_edge_cases(void)
{
    printf("\n%s=== Edge Cases ===%s\n", CLR_YELLOW, CLR_RESET);
    
    // Test large list
    t_list *large_list = NULL;
    for (int i = 0; i < 1000; i++)
    {
        char *content = strdup("node");
        ft_lstadd_back(&large_list, ft_lstnew(content));
    }
    
    if (ft_lstsize(large_list) == 1000)
        result_ok("Edge case: 1000 node list size");
    else
        result_ko("Edge case: 1000 node list size");
    
    if (strcmp(ft_lstlast(large_list)->content, "node") == 0)
        result_ok("Edge case: access last of 1000 nodes");
    else
        result_ko("Edge case: access last of 1000 nodes");
    
    ft_lstclear(&large_list, del_content);
    if (large_list == NULL)
        result_ok("Edge case: clear 1000 nodes");
    else
        result_ko("Edge case: clear 1000 nodes");
    
    // Test alternating operations
    t_list *alt = ft_lstnew("1");
    ft_lstadd_front(&alt, ft_lstnew("0"));
    ft_lstadd_back(&alt, ft_lstnew("2"));
    ft_lstadd_front(&alt, ft_lstnew("-1"));
    ft_lstadd_back(&alt, ft_lstnew("3"));
    
    if (ft_lstsize(alt) == 5 && 
        strcmp(alt->content, "-1") == 0 && 
        strcmp(ft_lstlast(alt)->content, "3") == 0)
        result_ok("Edge case: alternating front/back additions");
    else
        result_ko("Edge case: alternating front/back additions");
    
    free_list(alt);
    
    // Test list with NULL contents
    t_list *null_list = ft_lstnew(NULL);
    ft_lstadd_back(&null_list, ft_lstnew(NULL));
    ft_lstadd_back(&null_list, ft_lstnew(NULL));
    
    if (ft_lstsize(null_list) == 3)
        result_ok("Edge case: list with NULL contents");
    else
        result_ko("Edge case: list with NULL contents");
    
    free_list(null_list);
}

/* ========== Memory Leak Tests ========== */

static void test_memory_safety(void)
{
    printf("\n%s=== Memory Safety Tests ===%s\n", CLR_YELLOW, CLR_RESET);
    
    // Multiple allocations and frees
    for (int i = 0; i < 100; i++)
    {
        t_list *lst = ft_lstnew(strdup("test"));
        ft_lstadd_back(&lst, ft_lstnew(strdup("test2")));
        ft_lstclear(&lst, del_content);
    }
    result_ok("Memory safety: 100 allocation/free cycles");
    
    // Map and clear
    for (int i = 0; i < 50; i++)
    {
        t_list *orig = ft_lstnew("a");
        ft_lstadd_back(&orig, ft_lstnew("b"));
        t_list *mapped = ft_lstmap(orig, map_dup, del_content);
        free_list(orig);
        ft_lstclear(&mapped, del_content);
    }
    result_ok("Memory safety: 50 map/clear cycles");
    
    // Iter on allocated content
    for (int i = 0; i < 50; i++)
    {
        t_list *lst = ft_lstnew(strdup("test"));
        ft_lstadd_back(&lst, ft_lstnew(strdup("test2")));
        ft_lstiter(lst, iter_upper);
        ft_lstclear(&lst, del_content);
    }
    result_ok("Memory safety: 50 iter cycles");
}

/* ========== Main Test Runner ========== */

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    banner();
    
    printf("\n%s%s╔════════════════════════════════════════════════╗%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    printf("%s%s║    PART 3: Bonus Linked List Functions        ║%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    printf("%s%s╚════════════════════════════════════════════════╝%s\n", 
           CLR_BOLD, CLR_CYAN, CLR_RESET);
    
    test_lstnew();
    test_lstadd_front();
    test_lstsize();
    test_lstlast();
    test_lstadd_back();
    test_lstdelone();
    test_lstclear();
    test_lstiter();
    test_lstmap();
    test_edge_cases();
    test_memory_safety();
    
    summary();
    
    if (tests_run == tests_passed)
    {
        printf("\n%s%s🎉 ALL BONUS TESTS PASSED! 🎉%s\n\n", 
               CLR_BOLD, CLR_GREEN, CLR_RESET);
        printf("%s💡 Run 'make valgrind_b' to check for memory leaks%s\n\n",
               CLR_CYAN, CLR_RESET);
        return (0);
    }
    else
    {
        printf("\n%s%s❌ SOME TESTS FAILED ❌%s\n\n", 
               CLR_BOLD, CLR_RED, CLR_RESET);
        return (1);
    }
}
