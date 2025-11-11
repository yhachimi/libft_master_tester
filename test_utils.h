/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhachimi <yhachimi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11                                    */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_UTILS_H
# define TEST_UTILS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdint.h>
# include <limits.h>
# include <ctype.h>
# include "libft.h"

/* 🎨 ANSI Color Codes */
# define CLR_RESET  "\x1b[0m"
# define CLR_RED    "\x1b[31m"
# define CLR_GREEN  "\x1b[32m"
# define CLR_YELLOW "\x1b[33m"
# define CLR_BLUE   "\x1b[34m"
# define CLR_MAG    "\x1b[35m"
# define CLR_CYAN   "\x1b[36m"
# define CLR_BOLD   "\x1b[1m"

/* 🧮 Global Counters (declared as extern, defined in test files) */
extern int tests_run;
extern int tests_passed;

/* 💀 Animated banner */
static inline void banner(void)
{
    const char *lines[] = {
        " __  __                 _                 _____         _   ",
        "|  \\/  | ___  _ __  ___| |_ ___ _ __ ___|_   _|__  ___| |_ ",
        "| |\\/| |/ _ \\| '_ \\/ __| __/ _ \\ '__/ __|| |/ _ \\/ __| __|",
        "| |  | | (_) | | | \\__ \\ ||  __/ |  \\__ \\| |  __/\\__ \\ |_ ",
        "|_|  |_|\\___/|_| |_|___/\\__\\___|_|  |___/|_|\\___||___/\\__|",
        "                                                            ",
        "         🧪 Comprehensive libft Test Suite 🧪              ",
        "            Created by Younes Hachimi (yhachimi)           ",
        NULL
    };

    printf("\n%s%s", CLR_MAG, CLR_BOLD);
    for (int i = 0; lines[i]; ++i)
    {
        printf("%s\n", lines[i]);
        fflush(stdout);
        usleep(60000);
    }
    printf("%s\n", CLR_RESET);
}

/* ✅ Test helpers */
static inline void result_ok(const char *msg)
{
    printf("%s  ✓ %s%s\n", CLR_GREEN, msg, CLR_RESET);
    tests_passed++;
    tests_run++;
}

static inline void result_ko(const char *msg)
{
    printf("%s  ✗ %s%s\n", CLR_RED, msg, CLR_RESET);
    tests_run++;
}

static inline void summary(void)
{
    printf("\n%s%s", CLR_BOLD, CLR_CYAN);
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║              TEST SUMMARY                      ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("%s", CLR_RESET);
    
    float percentage = tests_run > 0 ? ((float)tests_passed / tests_run) * 100 : 0;
    
    printf("%sTotal tests:  %s%d%s\n", CLR_BOLD, CLR_BLUE, tests_run, CLR_RESET);
    printf("%sPassed:       %s%d%s\n", CLR_BOLD, CLR_GREEN, tests_passed, CLR_RESET);
    printf("%sFailed:       %s%d%s\n", CLR_BOLD, CLR_RED, 
           tests_run - tests_passed, CLR_RESET);
    printf("%sSuccess rate: %s%.1f%%%s\n\n", CLR_BOLD, 
           percentage == 100 ? CLR_GREEN : CLR_YELLOW, percentage, CLR_RESET);
}

#endif
