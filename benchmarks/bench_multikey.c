#include "bench_framework.h"
#include <multikey.h>
#include <float_sort.h>
#include <mem_aligned.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    double price;
    int64_t time;
    uint32_t order_id;
} Order;

typedef struct {
    Order* orders;
    Order* backup;
    size_t count;
    fc_sort_key_spec_t keys[3];
    size_t num_keys;
} bench_multikey_ctx_t;

static void bench_multikey_setup(bench_multikey_ctx_t* ctx, size_t count, size_t num_keys) {
    ctx->count = count;
    ctx->num_keys = num_keys;
    ctx->orders = (Order*)fc_aligned_alloc(count * sizeof(Order), 64);
    ctx->backup = (Order*)fc_aligned_alloc(count * sizeof(Order), 64);

    srand(42);
    for (size_t i = 0; i < count; i++) {
        ctx->orders[i].price = 100.0 + (rand() % 100) * 0.01;
        ctx->orders[i].time = 1000000 + rand() % 100000;
        ctx->orders[i].order_id = (uint32_t)(rand() % 10000);
    }
    memcpy(ctx->backup, ctx->orders, count * sizeof(Order));

    ctx->keys[0].offset = offsetof(Order, price);
    ctx->keys[0].type = FC_SORT_KEY_DOUBLE;
    ctx->keys[0].order = FC_SORT_DESCENDING;

    ctx->keys[1].offset = offsetof(Order, time);
    ctx->keys[1].type = FC_SORT_KEY_INT64;
    ctx->keys[1].order = FC_SORT_ASCENDING;

    ctx->keys[2].offset = offsetof(Order, order_id);
    ctx->keys[2].type = FC_SORT_KEY_UINT32;
    ctx->keys[2].order = FC_SORT_ASCENDING;
}

static void bench_multikey_teardown(bench_multikey_ctx_t* ctx) {
    fc_aligned_free(ctx->orders);
    fc_aligned_free(ctx->backup);
}

static void bench_multikey_reset(bench_multikey_ctx_t* ctx) {
    memcpy(ctx->orders, ctx->backup, ctx->count * sizeof(Order));
}

static void bench_multikey_sort(void* user_data) {
    bench_multikey_ctx_t* ctx = (bench_multikey_ctx_t*)user_data;
    bench_multikey_reset(ctx);
    fc_sort_multikey(ctx->orders, ctx->count, sizeof(Order), ctx->keys, ctx->num_keys);
}

static void bench_multikey_indexed(void* user_data) {
    bench_multikey_ctx_t* ctx = (bench_multikey_ctx_t*)user_data;
    size_t* indices = (size_t*)fc_aligned_alloc(ctx->count * sizeof(size_t), 64);
    fc_sort_multikey_indexed(ctx->orders, ctx->count, sizeof(Order), ctx->keys, ctx->num_keys, indices);
    fc_aligned_free(indices);
}

static void run_bench_suite(const char* name, size_t count, size_t num_keys) {
    bench_multikey_ctx_t ctx;
    bench_multikey_setup(&ctx, count, num_keys);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.data_size = count * sizeof(Order);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    printf("\n=== %s (n=%zu, keys=%zu) ===\n", name, count, num_keys);

    config.name = "In-place sort";
    fc_bench_run(&config, bench_multikey_sort, &ctx, &result);
    fc_bench_result_print(&result);

    if (count <= 10000) {
        config.name = "Indexed sort";
        fc_bench_run(&config, bench_multikey_indexed, &ctx, &result);
        fc_bench_result_print(&result);
    }

    bench_multikey_teardown(&ctx);
}

void bench_multikey_run(void) {
    printf("\n=== Multi-key Sorting Benchmarks ===\n");

    run_bench_suite("Small arrays (2 keys)", 100, 2);
    run_bench_suite("Small arrays (3 keys)", 100, 3);
    run_bench_suite("Medium arrays (2 keys)", 1000, 2);
    run_bench_suite("Medium arrays (3 keys)", 1000, 3);
    run_bench_suite("Large arrays (2 keys)", 10000, 2);
    run_bench_suite("Large arrays (3 keys)", 10000, 3);

    printf("\n");
}
