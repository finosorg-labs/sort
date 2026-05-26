/**
 * @file bench_float_sort.c
 * @brief Performance benchmarks for floating-point sorting
 */

#include "bench_framework.h"
#include "float_sort.h"
#include <mem_aligned.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    double* data;
    double* backup;
    size_t n;
    fc_sort_order_t order;
} bench_sort_ctx_t;

static void bench_sort_f64_setup(bench_sort_ctx_t* ctx, size_t n) {
    ctx->n = n;
    ctx->data = (double*)fc_aligned_alloc(n * sizeof(double), 64);
    ctx->backup = (double*)fc_aligned_alloc(n * sizeof(double), 64);
    ctx->order = FC_SORT_ASCENDING;

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        ctx->data[i] = (double)rand() / RAND_MAX * 1000.0;
    }
    memcpy(ctx->backup, ctx->data, n * sizeof(double));
}

static void bench_sort_f64_teardown(bench_sort_ctx_t* ctx) {
    fc_aligned_free(ctx->data);
    fc_aligned_free(ctx->backup);
}

static void bench_sort_f64_reset(bench_sort_ctx_t* ctx) {
    memcpy(ctx->data, ctx->backup, ctx->n * sizeof(double));
}

static void bench_float64_small(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    bench_sort_f64_reset(ctx);
    fc_sort_float64(ctx->data, ctx->n, ctx->order);
}

static void bench_float64_medium(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    bench_sort_f64_reset(ctx);
    fc_sort_float64(ctx->data, ctx->n, ctx->order);
}

static void bench_float64_large(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    bench_sort_f64_reset(ctx);
    fc_sort_float64(ctx->data, ctx->n, ctx->order);
}

static void bench_float64_sorted(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    fc_sort_float64(ctx->data, ctx->n, ctx->order);
}

static void bench_float64_reverse(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    for (size_t i = 0; i < ctx->n / 2; i++) {
        double temp = ctx->data[i];
        ctx->data[i] = ctx->data[ctx->n - 1 - i];
        ctx->data[ctx->n - 1 - i] = temp;
    }
    fc_sort_float64(ctx->data, ctx->n, ctx->order);
}

static void run_bench_suite(const char* name, size_t n) {
    bench_sort_ctx_t ctx;
    bench_sort_f64_setup(&ctx, n);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.name = name;
    config.data_size = n * sizeof(double);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    printf("\n=== %s (n=%zu) ===\n", name, n);

    if (n <= 100) {
        config.name = "Random data";
        fc_bench_run(&config, bench_float64_small, &ctx, &result);
        fc_bench_result_print(&result);
    } else if (n <= 10000) {
        config.name = "Random data";
        fc_bench_run(&config, bench_float64_medium, &ctx, &result);
        fc_bench_result_print(&result);
    } else {
        config.name = "Random data";
        fc_bench_run(&config, bench_float64_large, &ctx, &result);
        fc_bench_result_print(&result);
    }

    bench_sort_f64_reset(&ctx);
    fc_sort_float64(ctx.data, ctx.n, ctx.order);
    config.name = "Already sorted";
    fc_bench_run(&config, bench_float64_sorted, &ctx, &result);
    fc_bench_result_print(&result);

    bench_sort_f64_reset(&ctx);
    config.name = "Reverse sorted";
    fc_bench_run(&config, bench_float64_reverse, &ctx, &result);
    fc_bench_result_print(&result);

    bench_sort_f64_teardown(&ctx);
}

static void bench_float64_vs_go(void) {
    const size_t sizes[] = {100, 1000, 10000, 100000, 1000000};
    const size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\n");
    printf("========================================\n");
    printf("Float64 Sorting Benchmarks\n");
    printf("========================================\n");

    for (size_t i = 0; i < num_sizes; i++) {
        run_bench_suite("float64_sort", sizes[i]);
    }
}

typedef struct {
    float* data;
    float* backup;
    size_t n;
    fc_sort_order_t order;
} bench_sort_f32_ctx_t;

static void bench_sort_f32_setup(bench_sort_f32_ctx_t* ctx, size_t n) {
    ctx->n = n;
    ctx->data = (float*)fc_aligned_alloc(n * sizeof(float), 64);
    ctx->backup = (float*)fc_aligned_alloc(n * sizeof(float), 64);
    ctx->order = FC_SORT_ASCENDING;

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        ctx->data[i] = (float)rand() / RAND_MAX * 1000.0f;
    }
    memcpy(ctx->backup, ctx->data, n * sizeof(float));
}

static void bench_sort_f32_teardown(bench_sort_f32_ctx_t* ctx) {
    fc_aligned_free(ctx->data);
    fc_aligned_free(ctx->backup);
}

static void bench_float32_random(void* user_data) {
    bench_sort_f32_ctx_t* ctx = (bench_sort_f32_ctx_t*)user_data;
    memcpy(ctx->data, ctx->backup, ctx->n * sizeof(float));
    fc_sort_float32(ctx->data, ctx->n, ctx->order);
}

static void bench_float32_suite(void) {
    const size_t sizes[] = {100, 1000, 10000, 100000};
    const size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("\n");
    printf("========================================\n");
    printf("Float32 Sorting Benchmarks\n");
    printf("========================================\n");

    for (size_t i = 0; i < num_sizes; i++) {
        bench_sort_f32_ctx_t ctx;
        bench_sort_f32_setup(&ctx, sizes[i]);

        fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
        config.name = "float32_sort";
        config.data_size = sizes[i] * sizeof(float);
        config.min_time_ms = 200.0;

        fc_bench_result_t result;

        printf("\n=== float32_sort (n=%zu) ===\n", sizes[i]);
        fc_bench_run(&config, bench_float32_random, &ctx, &result);
        fc_bench_result_print(&result);

        bench_sort_f32_teardown(&ctx);
    }
}

static void bench_algorithm_selection(void) {
    printf("\n");
    printf("========================================\n");
    printf("Algorithm Selection Benchmarks\n");
    printf("========================================\n");

    const size_t small_sizes[] = {8, 16, 32};
    const size_t medium_sizes[] = {100, 1000, 5000};
    const size_t large_sizes[] = {10000, 50000, 100000};

    printf("\nSmall arrays (sorting network + insertion sort):\n");
    for (size_t i = 0; i < 3; i++) {
        run_bench_suite("small", small_sizes[i]);
    }

    printf("\nMedium arrays (introsort):\n");
    for (size_t i = 0; i < 3; i++) {
        run_bench_suite("medium", medium_sizes[i]);
    }

    printf("\nLarge arrays (radix sort):\n");
    for (size_t i = 0; i < 3; i++) {
        run_bench_suite("large", large_sizes[i]);
    }
}

void bench_float_sort_run(void) {
    printf("\n");
    printf("========================================\n");
    printf("FLOAT SORT BENCHMARKS\n");
    printf("========================================\n");

    bench_float64_vs_go();
    bench_float32_suite();
    bench_algorithm_selection();

    printf("\n");
    printf("========================================\n");
    printf("Benchmarks Complete\n");
    printf("========================================\n");
    printf("\n");
}
