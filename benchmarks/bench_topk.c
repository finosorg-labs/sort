/**
 * @file bench_topk.c
 * @brief Performance benchmarks for Top-K selection
 */

#include "bench_framework.h"
#include "topk.h"
#include <mem_aligned.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    double* data;
    double* backup;
    size_t n;
    size_t k;
} bench_topk_ctx_t;

static void bench_topk_f64_setup(bench_topk_ctx_t* ctx, size_t n, size_t k) {
    ctx->n = n;
    ctx->k = k;
    ctx->data = (double*)fc_aligned_alloc(n * sizeof(double), 64);
    ctx->backup = (double*)fc_aligned_alloc(n * sizeof(double), 64);

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        ctx->data[i] = (double)rand() / RAND_MAX * 1000.0;
    }
    memcpy(ctx->backup, ctx->data, n * sizeof(double));
}

static void bench_topk_f64_teardown(bench_topk_ctx_t* ctx) {
    fc_aligned_free(ctx->data);
    fc_aligned_free(ctx->backup);
}

static void bench_topk_f64_reset(bench_topk_ctx_t* ctx) {
    memcpy(ctx->data, ctx->backup, ctx->n * sizeof(double));
}

static void bench_topk_largest_f64(void* user_data) {
    bench_topk_ctx_t* ctx = (bench_topk_ctx_t*)user_data;
    bench_topk_f64_reset(ctx);
    fc_topk_largest_f64(ctx->data, ctx->n, ctx->k);
}

static void bench_topk_smallest_f64(void* user_data) {
    bench_topk_ctx_t* ctx = (bench_topk_ctx_t*)user_data;
    bench_topk_f64_reset(ctx);
    fc_topk_smallest_f64(ctx->data, ctx->n, ctx->k);
}

static void bench_topk_largest_f64_indexed(void* user_data) {
    bench_topk_ctx_t* ctx = (bench_topk_ctx_t*)user_data;
    double* values = (double*)fc_aligned_alloc(ctx->k * sizeof(double), 64);
    size_t* indices = (size_t*)fc_aligned_alloc(ctx->k * sizeof(size_t), 64);

    fc_topk_largest_f64_indexed(ctx->backup, ctx->n, ctx->k, values, indices);

    fc_aligned_free(values);
    fc_aligned_free(indices);
}

static void run_bench_suite(const char* name, size_t n, size_t k) {
    bench_topk_ctx_t ctx;
    bench_topk_f64_setup(&ctx, n, k);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.data_size = n * sizeof(double);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    printf("\n=== %s (n=%zu, k=%zu, k/n=%.2f%%) ===\n", name, n, k, (double)k / n * 100.0);

    config.name = "Top-K Largest";
    fc_bench_run(&config, bench_topk_largest_f64, &ctx, &result);
    fc_bench_result_print(&result);

    config.name = "Top-K Smallest";
    fc_bench_run(&config, bench_topk_smallest_f64, &ctx, &result);
    fc_bench_result_print(&result);

    /* Only run indexed version for small K (< 100) to avoid O(k*n) performance issues */
    if (k < 100) {
        config.name = "Top-K Largest (indexed)";
        fc_bench_run(&config, bench_topk_largest_f64_indexed, &ctx, &result);
        fc_bench_result_print(&result);
    }

    bench_topk_f64_teardown(&ctx);
}

void bench_topk_run(void) {
    printf("\n");
    printf("============================================================\n");
    printf("  Top-K Selection Benchmarks\n");
    printf("============================================================\n");

    /* Small K (heap-based) */
    run_bench_suite("Small K (heap)", 100000, 10);
    run_bench_suite("Small K (heap)", 100000, 50);
    run_bench_suite("Small K (heap)", 100000, 99);

    /* Medium K (QuickSelect) */
    run_bench_suite("Medium K (QuickSelect)", 100000, 100);
    run_bench_suite("Medium K (QuickSelect)", 100000, 500);
    run_bench_suite("Medium K (QuickSelect)", 100000, 5000);

    /* Large K (full sort) */
    run_bench_suite("Large K (full sort)", 100000, 10000);
    run_bench_suite("Large K (full sort)", 100000, 50000);
    run_bench_suite("Large K (full sort)", 100000, 90000);

    /* Different array sizes with small K */
    printf("\n=== Scaling with array size (k=10) ===\n");
    run_bench_suite("n=1K", 1000, 10);
    run_bench_suite("n=10K", 10000, 10);
    run_bench_suite("n=100K", 100000, 10);
    run_bench_suite("n=1M", 1000000, 10);

    /* Different array sizes with medium K */
    printf("\n=== Scaling with array size (k=100) ===\n");
    run_bench_suite("n=1K", 1000, 100);
    run_bench_suite("n=10K", 10000, 100);
    run_bench_suite("n=100K", 100000, 100);
    run_bench_suite("n=1M", 1000000, 100);
}
