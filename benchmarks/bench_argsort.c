/**
 * @file bench_argsort.c
 * @brief Performance benchmarks for argsort functions
 */

#include "bench_framework.h"
#include "argsort.h"
#include <mem_aligned.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    double* data;
    size_t* indices;
    size_t n;
    fc_argsort_order_t order;
} bench_argsort_ctx_t;

static void bench_argsort_f64_setup(bench_argsort_ctx_t* ctx, size_t n) {
    ctx->n = n;
    ctx->data = (double*)fc_aligned_alloc(n * sizeof(double), 64);
    ctx->indices = (size_t*)fc_aligned_alloc(n * sizeof(size_t), 64);
    ctx->order = FC_ARGSORT_ASCENDING;

    srand(42);
    for (size_t i = 0; i < n; i++) {
        ctx->data[i] = (double)rand() / RAND_MAX;
    }
}

static void bench_argsort_f64_teardown(bench_argsort_ctx_t* ctx) {
    fc_aligned_free(ctx->data);
    fc_aligned_free(ctx->indices);
}

static void bench_argsort_f64_random(void* user_data) {
    bench_argsort_ctx_t* ctx = (bench_argsort_ctx_t*)user_data;
    fc_sort_argsort_f64(ctx->data, ctx->n, ctx->indices, ctx->order);
}

static void bench_argsort_f64_stable_random(void* user_data) {
    bench_argsort_ctx_t* ctx = (bench_argsort_ctx_t*)user_data;
    fc_sort_argsort_f64_stable(ctx->data, ctx->n, ctx->indices, ctx->order);
}

static void run_bench_suite(const char* name, size_t n) {
    bench_argsort_ctx_t ctx;
    bench_argsort_f64_setup(&ctx, n);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.name = name;
    config.data_size = n * sizeof(double);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    printf("\n=== %s (n=%zu) ===\n", name, n);

    config.name = "argsort_f64 (unstable)";
    fc_bench_run(&config, bench_argsort_f64_random, &ctx, &result);
    fc_bench_result_print(&result);

    config.name = "argsort_f64_stable";
    fc_bench_run(&config, bench_argsort_f64_stable_random, &ctx, &result);
    fc_bench_result_print(&result);

    bench_argsort_f64_teardown(&ctx);
}

void bench_argsort_run(void) {
    const size_t sizes[] = {1000, 10000, 100000};
    const size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\n");
    printf("========================================\n");
    printf("Argsort Benchmarks\n");
    printf("========================================\n");

    for (size_t i = 0; i < num_sizes; i++) {
        run_bench_suite("argsort", sizes[i]);
    }
}
