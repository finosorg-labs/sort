/**
 * @file bench_sort.c
 * @brief sort module benchmark entry point
 *
 * This file serves as the main benchmark registration point for the sort module.
 * Individual benchmark modules are in separate files:
 */

#include "bench_framework.h"
#include <simd_detect.h>
#include <stdio.h>

/* External benchmark functions from sub-modules */
extern void bench_multikey_run(void);

/* Entry point for sort benchmarks */
void bench_sort_run(void) {
    printf("\n");
    printf("============================================================\n");
    printf("  sort Module Performance Benchmarks\n");
    printf("  SIMD level: %s\n", fc_simd_level_string(fc_detect_simd()));
    printf("============================================================\n");

    /* Run all sub-module benchmarks */
    bench_multikey_run();

    printf("\n");
    printf("============================================================\n");
    printf("  sort benchmarks complete\n");
    printf("============================================================\n");
}
