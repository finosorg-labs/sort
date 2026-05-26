/**
 * @file test_sort.c
 * @brief sort module test entry point
 *
 * This file serves as the main test registration point for the sort module.
 * Individual test modules are in separate files:
 */

#include "test_framework.h"

/* External test registration functions from sub-modules */
extern void register_xxx_tests(void);

/* Entry point for sort tests */
void register_sort_tests(void) {
    /* Register all sub-module tests */
    register_xxx_tests();
}
