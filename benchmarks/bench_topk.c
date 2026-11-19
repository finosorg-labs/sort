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

typedef struct {
    double** batch_data;
    double** batch_backup;
    size_t* batch_sizes;
    size_t* batch_k;
    size_t num_batches;
    size_t array_size;
    size_t k;
} bench_topk_batch_ctx_t;

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

/* Batch processing setup and teardown */

static void bench_topk_batch_setup(bench_topk_batch_ctx_t* ctx, size_t num_batches, size_t array_size, size_t k) {
    ctx->num_batches = num_batches;
    ctx->array_size = array_size;
    ctx->k = k;

    ctx->batch_data = (double**)malloc(num_batches * sizeof(double*));
    ctx->batch_backup = (double**)malloc(num_batches * sizeof(double*));
    ctx->batch_sizes = (size_t*)malloc(num_batches * sizeof(size_t));
    ctx->batch_k = (size_t*)malloc(num_batches * sizeof(size_t));

    srand(12345);
    for (size_t i = 0; i < num_batches; i++) {
        ctx->batch_data[i] = (double*)fc_aligned_alloc(array_size * sizeof(double), 64);
        ctx->batch_backup[i] = (double*)fc_aligned_alloc(array_size * sizeof(double), 64);
        ctx->batch_sizes[i] = array_size;
        ctx->batch_k[i] = k;

        for (size_t j = 0; j < array_size; j++) {
            ctx->batch_data[i][j] = (double)rand() / RAND_MAX * 1000.0;
        }
        memcpy(ctx->batch_backup[i], ctx->batch_data[i], array_size * sizeof(double));
    }
}

static void bench_topk_batch_teardown(bench_topk_batch_ctx_t* ctx) {
    for (size_t i = 0; i < ctx->num_batches; i++) {
        fc_aligned_free(ctx->batch_data[i]);
        fc_aligned_free(ctx->batch_backup[i]);
    }
    free(ctx->batch_data);
    free(ctx->batch_backup);
    free(ctx->batch_sizes);
    free(ctx->batch_k);
}

static void bench_topk_batch_reset(bench_topk_batch_ctx_t* ctx) {
    for (size_t i = 0; i < ctx->num_batches; i++) {
        memcpy(ctx->batch_data[i], ctx->batch_backup[i], ctx->array_size * sizeof(double));
    }
}

/* Batch processing benchmarks */

static void bench_topk_largest_f64_batch_call(void* user_data) {
    bench_topk_batch_ctx_t* ctx = (bench_topk_batch_ctx_t*)user_data;
    bench_topk_batch_reset(ctx);
    fc_topk_largest_f64_batch(ctx->batch_data, ctx->batch_sizes, ctx->batch_k, ctx->num_batches);
}

static void bench_topk_largest_f64_loop_call(void* user_data) {
    bench_topk_batch_ctx_t* ctx = (bench_topk_batch_ctx_t*)user_data;
    bench_topk_batch_reset(ctx);
    for (size_t i = 0; i < ctx->num_batches; i++) {
        fc_topk_largest_f64(ctx->batch_data[i], ctx->batch_sizes[i], ctx->batch_k[i]);
    }
}

static void bench_topk_smallest_f64_batch_call(void* user_data) {
    bench_topk_batch_ctx_t* ctx = (bench_topk_batch_ctx_t*)user_data;
    bench_topk_batch_reset(ctx);
    fc_topk_smallest_f64_batch(ctx->batch_data, ctx->batch_sizes, ctx->batch_k, ctx->num_batches);
}

static void bench_topk_smallest_f64_loop_call(void* user_data) {
    bench_topk_batch_ctx_t* ctx = (bench_topk_batch_ctx_t*)user_data;
    bench_topk_batch_reset(ctx);
    for (size_t i = 0; i < ctx->num_batches; i++) {
        fc_topk_smallest_f64(ctx->batch_data[i], ctx->batch_sizes[i], ctx->batch_k[i]);
    }
}

/* Indexed batch processing context and functions */

typedef struct {
    const double** batch_data;
    double** batch_data_storage;
    size_t* batch_sizes;
    size_t* batch_k;
    size_t num_batches;
    size_t array_size;
    size_t k;
    double** out_values;
    size_t** out_indices;
} bench_topk_indexed_batch_ctx_t;

static void bench_topk_indexed_batch_setup(
    bench_topk_indexed_batch_ctx_t* ctx,
    size_t num_batches,
    size_t array_size,
    size_t k
) {
    ctx->num_batches = num_batches;
    ctx->array_size = array_size;
    ctx->k = k;

    ctx->batch_data = (const double**)malloc(num_batches * sizeof(const double*));
    ctx->batch_data_storage = (double**)malloc(num_batches * sizeof(double*));
    ctx->batch_sizes = (size_t*)malloc(num_batches * sizeof(size_t));
    ctx->batch_k = (size_t*)malloc(num_batches * sizeof(size_t));
    ctx->out_values = (double**)malloc(num_batches * sizeof(double*));
    ctx->out_indices = (size_t**)malloc(num_batches * sizeof(size_t*));

    srand(12345);
    for (size_t i = 0; i < num_batches; i++) {
        ctx->batch_data_storage[i] = (double*)fc_aligned_alloc(array_size * sizeof(double), 64);
        ctx->batch_data[i] = ctx->batch_data_storage[i];
        ctx->batch_sizes[i] = array_size;
        ctx->batch_k[i] = k;

        for (size_t j = 0; j < array_size; j++) {
            ctx->batch_data_storage[i][j] = (double)rand() / RAND_MAX * 1000.0;
        }

        ctx->out_values[i] = (double*)fc_aligned_alloc(k * sizeof(double), 64);
        ctx->out_indices[i] = (size_t*)fc_aligned_alloc(k * sizeof(size_t), 64);
    }
}

static void bench_topk_indexed_batch_teardown(bench_topk_indexed_batch_ctx_t* ctx) {
    for (size_t i = 0; i < ctx->num_batches; i++) {
        fc_aligned_free(ctx->batch_data_storage[i]);
        fc_aligned_free(ctx->out_values[i]);
        fc_aligned_free(ctx->out_indices[i]);
    }
    free(ctx->batch_data);
    free(ctx->batch_data_storage);
    free(ctx->batch_sizes);
    free(ctx->batch_k);
    free(ctx->out_values);
    free(ctx->out_indices);
}

static void bench_topk_largest_f64_indexed_batch_call(void* user_data) {
    bench_topk_indexed_batch_ctx_t* ctx = (bench_topk_indexed_batch_ctx_t*)user_data;
    fc_topk_largest_f64_indexed_batch(
        ctx->batch_data,
        ctx->batch_sizes,
        ctx->batch_k,
        ctx->num_batches,
        ctx->out_values,
        ctx->out_indices
    );
}

static void bench_topk_largest_f64_indexed_loop_call(void* user_data) {
    bench_topk_indexed_batch_ctx_t* ctx = (bench_topk_indexed_batch_ctx_t*)user_data;
    for (size_t i = 0; i < ctx->num_batches; i++) {
        fc_topk_largest_f64_indexed(
            ctx->batch_data[i],
            ctx->batch_sizes[i],
            ctx->batch_k[i],
            ctx->out_values[i],
            ctx->out_indices[i]
        );
    }
}

static void bench_topk_smallest_f64_indexed_batch_call(void* user_data) {
    bench_topk_indexed_batch_ctx_t* ctx = (bench_topk_indexed_batch_ctx_t*)user_data;
    fc_topk_smallest_f64_indexed_batch(
        ctx->batch_data,
        ctx->batch_sizes,
        ctx->batch_k,
        ctx->num_batches,
        ctx->out_values,
        ctx->out_indices
    );
}

static void bench_topk_smallest_f64_indexed_loop_call(void* user_data) {
    bench_topk_indexed_batch_ctx_t* ctx = (bench_topk_indexed_batch_ctx_t*)user_data;
    for (size_t i = 0; i < ctx->num_batches; i++) {
        fc_topk_smallest_f64_indexed(
            ctx->batch_data[i],
            ctx->batch_sizes[i],
            ctx->batch_k[i],
            ctx->out_values[i],
            ctx->out_indices[i]
        );
    }
}

static void run_indexed_batch_bench_suite(
    const char* name,
    size_t num_batches,
    size_t array_size,
    size_t k
) {
    bench_topk_indexed_batch_ctx_t ctx;
    bench_topk_indexed_batch_setup(&ctx, num_batches, array_size, k);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.data_size = num_batches * array_size * sizeof(double);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    printf("\n=== %s (batches=%zu, array_size=%zu, k=%zu) ===\n", name, num_batches, array_size, k);

    config.name = "Indexed Batch call - Largest";
    fc_bench_run(&config, bench_topk_largest_f64_indexed_batch_call, &ctx, &result);
    fc_bench_result_print(&result);
    double batch_time_largest = result.mean_ns;

    config.name = "Indexed Loop call - Largest";
    fc_bench_run(&config, bench_topk_largest_f64_indexed_loop_call, &ctx, &result);
    fc_bench_result_print(&result);
    double loop_time_largest = result.mean_ns;

    config.name = "Indexed Batch call - Smallest";
    fc_bench_run(&config, bench_topk_smallest_f64_indexed_batch_call, &ctx, &result);
    fc_bench_result_print(&result);
    double batch_time_smallest = result.mean_ns;

    config.name = "Indexed Loop call - Smallest";
    fc_bench_run(&config, bench_topk_smallest_f64_indexed_loop_call, &ctx, &result);
    fc_bench_result_print(&result);
    double loop_time_smallest = result.mean_ns;

    printf("\nIndexed Batch vs Loop comparison:\n");
    printf("  Largest:  %.2fx %s (batch: %.1f µs, loop: %.1f µs)\n",
           loop_time_largest / batch_time_largest,
           batch_time_largest < loop_time_largest ? "faster" : "slower",
           batch_time_largest / 1000.0,
           loop_time_largest / 1000.0);
    printf("  Smallest: %.2fx %s (batch: %.1f µs, loop: %.1f µs)\n",
           loop_time_smallest / batch_time_smallest,
           batch_time_smallest < loop_time_smallest ? "faster" : "slower",
           batch_time_smallest / 1000.0,
           loop_time_smallest / 1000.0);
    printf("  Avg time per array (batch): %.1f µs\n", batch_time_largest / num_batches / 1000.0);
    printf("  Throughput: %.0f arrays/sec\n", 1e9 / (batch_time_largest / num_batches));

    bench_topk_indexed_batch_teardown(&ctx);
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

static void run_batch_bench_suite(const char* name, size_t num_batches, size_t array_size, size_t k) {
    bench_topk_batch_ctx_t ctx;
    bench_topk_batch_setup(&ctx, num_batches, array_size, k);

    fc_bench_config_t config = FC_BENCH_CONFIG_DEFAULT;
    config.data_size = num_batches * array_size * sizeof(double);
    config.min_time_ms = 200.0;
    config.warmup_ms = 50.0;

    fc_bench_result_t result;

    printf("\n=== %s (batches=%zu, array_size=%zu, k=%zu) ===\n",
           name, num_batches, array_size, k);

    config.name = "Batch call - Largest";
    fc_bench_run(&config, bench_topk_largest_f64_batch_call, &ctx, &result);
    fc_bench_result_print(&result);
    double batch_time_largest = result.mean_ns;

    config.name = "Loop call - Largest";
    fc_bench_run(&config, bench_topk_largest_f64_loop_call, &ctx, &result);
    fc_bench_result_print(&result);
    double loop_time_largest = result.mean_ns;

    config.name = "Batch call - Smallest";
    fc_bench_run(&config, bench_topk_smallest_f64_batch_call, &ctx, &result);
    fc_bench_result_print(&result);
    double batch_time_smallest = result.mean_ns;

    config.name = "Loop call - Smallest";
    fc_bench_run(&config, bench_topk_smallest_f64_loop_call, &ctx, &result);
    fc_bench_result_print(&result);
    double loop_time_smallest = result.mean_ns;

    printf("\nBatch vs Loop comparison:\n");
    printf("  Largest:  %.2fx %s (batch: %.1f µs, loop: %.1f µs)\n",
           loop_time_largest / batch_time_largest,
           batch_time_largest < loop_time_largest ? "faster" : "slower",
           batch_time_largest / 1000.0, loop_time_largest / 1000.0);
    printf("  Smallest: %.2fx %s (batch: %.1f µs, loop: %.1f µs)\n",
           loop_time_smallest / batch_time_smallest,
           batch_time_smallest < loop_time_smallest ? "faster" : "slower",
           batch_time_smallest / 1000.0, loop_time_smallest / 1000.0);
    printf("  Avg time per array (batch): %.1f µs\n", batch_time_largest / num_batches / 1000.0);
    printf("  Throughput: %.0f arrays/sec\n", 1e9 / (batch_time_largest / num_batches));

    bench_topk_batch_teardown(&ctx);
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

    /* Batch processing benchmarks */
    printf("\n");
    printf("  Batch Processing Benchmarks\n");

    /* Small batch (10 arrays) */
    run_batch_bench_suite("Small batch", 10, 1000, 100);

    /* Medium batch (100 arrays) */
    run_batch_bench_suite("Medium batch", 100, 1000, 100);

    /* Large batch (1000 arrays) - simulates market-wide aggregation */
    run_batch_bench_suite("Large batch (market-wide)", 1000, 1000, 100);

    /* Scaling with different K values (1000 arrays) */
    printf("\n=== Batch scaling with K (1000 arrays, 1000 elements each) ===\n");
    run_batch_bench_suite("k=10", 1000, 1000, 10);
    run_batch_bench_suite("k=50", 1000, 1000, 50);
    run_batch_bench_suite("k=100", 1000, 1000, 100);
    run_batch_bench_suite("k=500", 1000, 1000, 500);

    /* Indexed batch processing benchmarks */
    printf("\n");
    printf("  Indexed Batch Processing Benchmarks\n");

    /* Small batch (10 arrays) */
    run_indexed_batch_bench_suite("Small indexed batch", 10, 1000, 100);

    /* Medium batch (100 arrays) */
    run_indexed_batch_bench_suite("Medium indexed batch", 100, 1000, 100);

    /* Large batch (1000 arrays) - simulates market-wide aggregation */
    run_indexed_batch_bench_suite("Large indexed batch (market-wide)", 1000, 1000, 100);

    /* Scaling with different K values (1000 arrays) */
    printf("\n=== Indexed Batch scaling with K (1000 arrays, 1000 elements each) ===\n");
    run_indexed_batch_bench_suite("k=10", 1000, 1000, 10);
    run_indexed_batch_bench_suite("k=50", 1000, 1000, 50);
    run_indexed_batch_bench_suite("k=100", 1000, 1000, 100);
}
