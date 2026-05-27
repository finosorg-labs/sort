/**
 * @file bench_int_sort.c
 * @brief Performance benchmarks for integer sorting
 */

#include "bench_framework.h"
#include "int_sort.h"
#include <mem_aligned.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int64_t* data_i64;
    int64_t* backup_i64;
    uint64_t* data_u64;
    uint64_t* backup_u64;
    int32_t* data_i32;
    int32_t* backup_i32;
    uint32_t* data_u32;
    uint32_t* backup_u32;
    size_t n;
    fc_sort_order_t order;
} bench_sort_ctx_t;

static void bench_sort_i64_setup(bench_sort_ctx_t* ctx, size_t n) {
    ctx->n = n;
    ctx->data_i64 = (int64_t*)fc_aligned_alloc(n * sizeof(int64_t), 64);
    ctx->backup_i64 = (int64_t*)fc_aligned_alloc(n * sizeof(int64_t), 64);
    ctx->order = FC_SORT_ASCENDING;

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        ctx->data_i64[i] = (int64_t)(rand() % 1000000) - 500000;
    }
    memcpy(ctx->backup_i64, ctx->data_i64, n * sizeof(int64_t));
}

static void bench_sort_u64_setup(bench_sort_ctx_t* ctx, size_t n) {
    ctx->n = n;
    ctx->data_u64 = (uint64_t*)fc_aligned_alloc(n * sizeof(uint64_t), 64);
    ctx->backup_u64 = (uint64_t*)fc_aligned_alloc(n * sizeof(uint64_t), 64);
    ctx->order = FC_SORT_ASCENDING;

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        ctx->data_u64[i] = (uint64_t)rand() * rand();
    }
    memcpy(ctx->backup_u64, ctx->data_u64, n * sizeof(uint64_t));
}

static void bench_sort_i32_setup(bench_sort_ctx_t* ctx, size_t n) {
    ctx->n = n;
    ctx->data_i32 = (int32_t*)fc_aligned_alloc(n * sizeof(int32_t), 64);
    ctx->backup_i32 = (int32_t*)fc_aligned_alloc(n * sizeof(int32_t), 64);
    ctx->order = FC_SORT_ASCENDING;

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        ctx->data_i32[i] = (int32_t)(rand() % 1000000) - 500000;
    }
    memcpy(ctx->backup_i32, ctx->data_i32, n * sizeof(int32_t));
}

static void bench_sort_u32_setup(bench_sort_ctx_t* ctx, size_t n) {
    ctx->n = n;
    ctx->data_u32 = (uint32_t*)fc_aligned_alloc(n * sizeof(uint32_t), 64);
    ctx->backup_u32 = (uint32_t*)fc_aligned_alloc(n * sizeof(uint32_t), 64);
    ctx->order = FC_SORT_ASCENDING;

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        ctx->data_u32[i] = (uint32_t)rand();
    }
    memcpy(ctx->backup_u32, ctx->data_u32, n * sizeof(uint32_t));
}

static void bench_sort_i64_teardown(bench_sort_ctx_t* ctx) {
    fc_aligned_free(ctx->data_i64);
    fc_aligned_free(ctx->backup_i64);
}

static void bench_sort_u64_teardown(bench_sort_ctx_t* ctx) {
    fc_aligned_free(ctx->data_u64);
    fc_aligned_free(ctx->backup_u64);
}

static void bench_sort_i32_teardown(bench_sort_ctx_t* ctx) {
    fc_aligned_free(ctx->data_i32);
    fc_aligned_free(ctx->backup_i32);
}

static void bench_sort_u32_teardown(bench_sort_ctx_t* ctx) {
    fc_aligned_free(ctx->data_u32);
    fc_aligned_free(ctx->backup_u32);
}

static void bench_sort_i64_reset(bench_sort_ctx_t* ctx) {
    memcpy(ctx->data_i64, ctx->backup_i64, ctx->n * sizeof(int64_t));
}

static void bench_sort_u64_reset(bench_sort_ctx_t* ctx) {
    memcpy(ctx->data_u64, ctx->backup_u64, ctx->n * sizeof(uint64_t));
}

static void bench_sort_i32_reset(bench_sort_ctx_t* ctx) {
    memcpy(ctx->data_i32, ctx->backup_i32, ctx->n * sizeof(int32_t));
}

static void bench_sort_u32_reset(bench_sort_ctx_t* ctx) {
    memcpy(ctx->data_u32, ctx->backup_u32, ctx->n * sizeof(uint32_t));
}

static void bench_int64_random(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    bench_sort_i64_reset(ctx);
    fc_sort_int64(ctx->data_i64, ctx->n, ctx->order);
}

static void bench_uint64_random(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    bench_sort_u64_reset(ctx);
    fc_sort_uint64(ctx->data_u64, ctx->n, ctx->order);
}

static void bench_int32_random(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    bench_sort_i32_reset(ctx);
    fc_sort_int32(ctx->data_i32, ctx->n, ctx->order);
}

static void bench_uint32_random(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    bench_sort_u32_reset(ctx);
    fc_sort_uint32(ctx->data_u32, ctx->n, ctx->order);
}

static void bench_int64_sorted(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    fc_sort_int64(ctx->data_i64, ctx->n, ctx->order);
}

static void bench_int64_reverse(void* user_data) {
    bench_sort_ctx_t* ctx = (bench_sort_ctx_t*)user_data;
    for (size_t i = 0; i < ctx->n / 2; i++) {
        int64_t temp = ctx->data_i64[i];
        ctx->data_i64[i] = ctx->data_i64[ctx->n - 1 - i];
        ctx->data_i64[ctx->n - 1 - i] = temp;
    }
    fc_sort_int64(ctx->data_i64, ctx->n, ctx->order);
}

static void run_bench_suite_i64(const char* name, size_t n) {
    bench_sort_ctx_t ctx;
    bench_sort_i64_setup(&ctx, n);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.name = name;
    config.data_size = n * sizeof(int64_t);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    fc_bench_run(&config, bench_int64_random, &ctx, &result);

    bench_sort_i64_teardown(&ctx);
}

static void run_bench_suite_u64(const char* name, size_t n) {
    bench_sort_ctx_t ctx;
    bench_sort_u64_setup(&ctx, n);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.name = name;
    config.data_size = n * sizeof(uint64_t);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    fc_bench_run(&config, bench_uint64_random, &ctx, &result);

    bench_sort_u64_teardown(&ctx);
}

static void run_bench_suite_i32(const char* name, size_t n) {
    bench_sort_ctx_t ctx;
    bench_sort_i32_setup(&ctx, n);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.name = name;
    config.data_size = n * sizeof(int32_t);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    fc_bench_run(&config, bench_int32_random, &ctx, &result);

    bench_sort_i32_teardown(&ctx);
}

static void run_bench_suite_u32(const char* name, size_t n) {
    bench_sort_ctx_t ctx;
    bench_sort_u32_setup(&ctx, n);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.name = name;
    config.data_size = n * sizeof(uint32_t);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    fc_bench_run(&config, bench_uint32_random, &ctx, &result);

    bench_sort_u32_teardown(&ctx);
}

static void run_bench_special_cases(void) {
    bench_sort_ctx_t ctx;
    bench_sort_i64_setup(&ctx, 10000);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.data_size = 10000 * sizeof(int64_t);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    config.name = "int64_sorted_10K";
    fc_sort_int64(ctx.data_i64, ctx.n, ctx.order);
    fc_bench_run(&config, bench_int64_sorted, &ctx, &result);

    bench_sort_i64_reset(&ctx);
    config.name = "int64_reverse_10K";
    fc_bench_run(&config, bench_int64_reverse, &ctx, &result);

    bench_sort_i64_teardown(&ctx);
}

void bench_int_sort_run(void) {
    printf("\nInteger Sort Benchmarks\n");
    printf("=======================\n\n");

    printf("int64 benchmarks:\n");
    run_bench_suite_i64("int64_100", 100);
    run_bench_suite_i64("int64_1K", 1000);
    run_bench_suite_i64("int64_10K", 10000);
    run_bench_suite_i64("int64_100K", 100000);
    run_bench_suite_i64("int64_1M", 1000000);

    printf("\nuint64 benchmarks:\n");
    run_bench_suite_u64("uint64_100", 100);
    run_bench_suite_u64("uint64_1K", 1000);
    run_bench_suite_u64("uint64_10K", 10000);
    run_bench_suite_u64("uint64_100K", 100000);
    run_bench_suite_u64("uint64_1M", 1000000);

    printf("\nint32 benchmarks:\n");
    run_bench_suite_i32("int32_100", 100);
    run_bench_suite_i32("int32_1K", 1000);
    run_bench_suite_i32("int32_10K", 10000);
    run_bench_suite_i32("int32_100K", 100000);
    run_bench_suite_i32("int32_1M", 1000000);

    printf("\nuint32 benchmarks:\n");
    run_bench_suite_u32("uint32_100", 100);
    run_bench_suite_u32("uint32_1K", 1000);
    run_bench_suite_u32("uint32_10K", 10000);
    run_bench_suite_u32("uint32_100K", 100000);
    run_bench_suite_u32("uint32_1M", 1000000);

    printf("\nSpecial cases:\n");
    run_bench_special_cases();
}
