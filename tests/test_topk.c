/**
 * @file test_topk.c
 * @brief Unit tests for Top-K selection
 */

#include "test_framework.h"
#include "topk.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

TEST(topk_largest_f64_empty) {
    double data[1] = {0.0};
    fc_status_t err = fc_topk_largest_f64(data, 0, 0);
    ASSERT_EQ(err, FC_OK);
}

TEST(topk_largest_f64_null_pointer) {
    fc_status_t err = fc_topk_largest_f64(NULL, 10, 5);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(topk_largest_f64_k_greater_than_n) {
    double data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    fc_status_t err = fc_topk_largest_f64(data, 5, 10);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(topk_largest_f64_k_equals_n) {
    double data[5] = {3.0, 1.0, 4.0, 5.0, 2.0};
    fc_status_t err = fc_topk_largest_f64(data, 5, 5);
    ASSERT_EQ(err, FC_OK);

    /* Should be sorted in descending order */
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 5.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 4.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 3.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[3], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[4], 1.0, 1e-10);
}

TEST(topk_largest_f64_small_k) {
    double data[10] = {3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0, 5.0, 3.0};
    fc_status_t err = fc_topk_largest_f64(data, 10, 3);
    ASSERT_EQ(err, FC_OK);

    /* Top 3 should be 9.0, 6.0, 5.0 in descending order */
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 9.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 6.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 5.0, 1e-10);
}

TEST(topk_largest_f64_medium_k) {
    const size_t n = 1000;
    const size_t k = 150;
    double* data = (double*)malloc(n * sizeof(double));
    ASSERT_NOT_NULL(data);

    /* Fill with values 0 to 999 */
    for (size_t i = 0; i < n; i++) {
        data[i] = (double)i;
    }

    fc_status_t err = fc_topk_largest_f64(data, n, k);
    ASSERT_EQ(err, FC_OK);

    /* Top k should be 999, 998, ..., 850 in descending order */
    for (size_t i = 0; i < k; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (double)(n - 1 - i), 1e-10);
    }

    free(data);
}

TEST(topk_largest_f64_large_k) {
    const size_t n = 1000;
    const size_t k = 900;
    double* data = (double*)malloc(n * sizeof(double));
    ASSERT_NOT_NULL(data);

    /* Fill with random values */
    for (size_t i = 0; i < n; i++) {
        data[i] = (double)((i * 7919) % n);
    }

    fc_status_t err = fc_topk_largest_f64(data, n, k);
    ASSERT_EQ(err, FC_OK);

    /* Verify top k are in descending order */
    for (size_t i = 0; i < k - 1; i++) {
        ASSERT_TRUE(data[i] >= data[i + 1]);
    }

    free(data);
}

TEST(topk_smallest_f64_small_k) {
    double data[10] = {3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0, 5.0, 3.0};
    fc_status_t err = fc_topk_smallest_f64(data, 10, 3);
    ASSERT_EQ(err, FC_OK);

    /* Top 3 smallest should be 1.0, 1.0, 2.0 in ascending order */
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 2.0, 1e-10);
}

TEST(topk_smallest_f64_medium_k) {
    const size_t n = 1000;
    const size_t k = 150;
    double* data = (double*)malloc(n * sizeof(double));
    ASSERT_NOT_NULL(data);

    /* Fill with values 999 to 0 */
    for (size_t i = 0; i < n; i++) {
        data[i] = (double)(n - 1 - i);
    }

    fc_status_t err = fc_topk_smallest_f64(data, n, k);
    ASSERT_EQ(err, FC_OK);

    /* Top k smallest should be 0, 1, ..., 149 in ascending order */
    for (size_t i = 0; i < k; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (double)i, 1e-10);
    }

    free(data);
}

TEST(topk_largest_f32_small_k) {
    float data[10] = {3.0f, 1.0f, 4.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 5.0f, 3.0f};
    fc_status_t err = fc_topk_largest_f32(data, 10, 3);
    ASSERT_EQ(err, FC_OK);

    /* Top 3 should be 9.0, 6.0, 5.0 in descending order */
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 9.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 6.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 5.0f, 1e-6);
}

TEST(topk_smallest_f32_small_k) {
    float data[10] = {3.0f, 1.0f, 4.0f, 1.0f, 5.0f, 9.0f, 2.0f, 6.0f, 5.0f, 3.0f};
    fc_status_t err = fc_topk_smallest_f32(data, 10, 3);
    ASSERT_EQ(err, FC_OK);

    /* Top 3 smallest should be 1.0, 1.0, 2.0 in ascending order */
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 1.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 1.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 2.0f, 1e-6);
}

TEST(topk_largest_f64_indexed_basic) {
    double data[5] = {3.0, 1.0, 4.0, 1.0, 5.0};
    double values[2];
    size_t indices[2];

    fc_status_t err = fc_topk_largest_f64_indexed(data, 5, 2, values, indices);
    ASSERT_EQ(err, FC_OK);

    /* Top 2 largest: 5.0 (index 4), 4.0 (index 2) */
    FC_TEST_ASSERT_DOUBLE_EQ(values[0], 5.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(values[1], 4.0, 1e-10);
    ASSERT_EQ(indices[0], 4);
    ASSERT_EQ(indices[1], 2);
}

TEST(topk_smallest_f64_indexed_basic) {
    double data[5] = {3.0, 1.0, 4.0, 1.0, 5.0};
    double values[2];
    size_t indices[2];

    fc_status_t err = fc_topk_smallest_f64_indexed(data, 5, 2, values, indices);
    ASSERT_EQ(err, FC_OK);

    /* Top 2 smallest: 1.0 (index 1), 1.0 (index 3) */
    FC_TEST_ASSERT_DOUBLE_EQ(values[0], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(values[1], 1.0, 1e-10);
    ASSERT_TRUE(indices[0] == 1 || indices[0] == 3);
    ASSERT_TRUE(indices[1] == 1 || indices[1] == 3);
    ASSERT_TRUE(indices[0] != indices[1]);
}

TEST(topk_largest_f64_indexed_null_pointer) {
    double data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double values[2];
    size_t indices[2];

    fc_status_t err = fc_topk_largest_f64_indexed(NULL, 5, 2, values, indices);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);

    err = fc_topk_largest_f64_indexed(data, 5, 2, NULL, indices);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);

    err = fc_topk_largest_f64_indexed(data, 5, 2, values, NULL);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(topk_largest_f32_indexed_basic) {
    float data[5] = {3.0f, 1.0f, 4.0f, 1.0f, 5.0f};
    float values[2];
    size_t indices[2];

    fc_status_t err = fc_topk_largest_f32_indexed(data, 5, 2, values, indices);
    ASSERT_EQ(err, FC_OK);

    /* Top 2 largest: 5.0 (index 4), 4.0 (index 2) */
    FC_TEST_ASSERT_DOUBLE_EQ(values[0], 5.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(values[1], 4.0f, 1e-6);
    ASSERT_EQ(indices[0], 4);
    ASSERT_EQ(indices[1], 2);
}

TEST(topk_smallest_f32_indexed_basic) {
    float data[5] = {3.0f, 1.0f, 4.0f, 1.0f, 5.0f};
    float values[2];
    size_t indices[2];

    fc_status_t err = fc_topk_smallest_f32_indexed(data, 5, 2, values, indices);
    ASSERT_EQ(err, FC_OK);

    /* Top 2 smallest: 1.0 (index 1), 1.0 (index 3) */
    FC_TEST_ASSERT_DOUBLE_EQ(values[0], 1.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(values[1], 1.0f, 1e-6);
    ASSERT_TRUE(indices[0] == 1 || indices[0] == 3);
    ASSERT_TRUE(indices[1] == 1 || indices[1] == 3);
    ASSERT_TRUE(indices[0] != indices[1]);
}

TEST(topk_largest_f64_with_duplicates) {
    double data[10] = {5.0, 5.0, 3.0, 3.0, 1.0, 1.0, 4.0, 4.0, 2.0, 2.0};
    fc_status_t err = fc_topk_largest_f64(data, 10, 4);
    ASSERT_EQ(err, FC_OK);

    /* Top 4 should be 5.0, 5.0, 4.0, 4.0 */
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 5.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 5.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 4.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[3], 4.0, 1e-10);
}

TEST(topk_smallest_f64_with_duplicates) {
    double data[10] = {5.0, 5.0, 3.0, 3.0, 1.0, 1.0, 4.0, 4.0, 2.0, 2.0};
    fc_status_t err = fc_topk_smallest_f64(data, 10, 4);
    ASSERT_EQ(err, FC_OK);

    /* Top 4 smallest should be 1.0, 1.0, 2.0, 2.0 */
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[3], 2.0, 1e-10);
}

TEST(topk_largest_f64_single_element) {
    double data[1] = {42.0};
    fc_status_t err = fc_topk_largest_f64(data, 1, 1);
    ASSERT_EQ(err, FC_OK);
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 42.0, 1e-10);
}

TEST(topk_smallest_f64_single_element) {
    double data[1] = {42.0};
    fc_status_t err = fc_topk_smallest_f64(data, 1, 1);
    ASSERT_EQ(err, FC_OK);
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 42.0, 1e-10);
}

TEST(topk_largest_f64_k_zero) {
    double data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    fc_status_t err = fc_topk_largest_f64(data, 5, 0);
    ASSERT_EQ(err, FC_OK);
}

TEST(topk_smallest_f64_k_zero) {
    double data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    fc_status_t err = fc_topk_smallest_f64(data, 5, 0);
    ASSERT_EQ(err, FC_OK);
}

/* Batch processing tests */

TEST(topk_largest_f64_batch_basic) {
    const size_t num_batches = 3;
    const size_t array_size = 10;
    const size_t k = 3;

    /* Prepare batch data */
    double* arrays[3];
    size_t sizes[3];
    size_t ks[3];

    for (size_t i = 0; i < num_batches; i++) {
        arrays[i] = (double*)malloc(array_size * sizeof(double));
        ASSERT_NOT_NULL(arrays[i]);
        sizes[i] = array_size;
        ks[i] = k;
    }

    /* Array 0: {9, 8, 7, 6, 5, 4, 3, 2, 1, 0} */
    for (size_t i = 0; i < array_size; i++) {
        arrays[0][i] = (double)(array_size - 1 - i);
    }

    /* Array 1: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} */
    for (size_t i = 0; i < array_size; i++) {
        arrays[1][i] = (double)i;
    }

    /* Array 2: {5, 3, 8, 1, 9, 2, 7, 4, 6, 0} */
    double arr2[] = {5.0, 3.0, 8.0, 1.0, 9.0, 2.0, 7.0, 4.0, 6.0, 0.0};
    memcpy(arrays[2], arr2, array_size * sizeof(double));

    /* Call batch function */
    fc_status_t err = fc_topk_largest_f64_batch(arrays, sizes, ks, num_batches);
    ASSERT_EQ(err, FC_OK);

    /* Verify results */
    /* Array 0: top 3 should be {9, 8, 7} */
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[0][0], 9.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[0][1], 8.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[0][2], 7.0, 1e-10);

    /* Array 1: top 3 should be {9, 8, 7} */
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[1][0], 9.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[1][1], 8.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[1][2], 7.0, 1e-10);

    /* Array 2: top 3 should be {9, 8, 7} */
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[2][0], 9.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[2][1], 8.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[2][2], 7.0, 1e-10);

    /* Cleanup */
    for (size_t i = 0; i < num_batches; i++) {
        free(arrays[i]);
    }
}

TEST(topk_smallest_f64_batch_basic) {
    const size_t num_batches = 2;
    const size_t array_size = 8;
    const size_t k = 4;

    /* Prepare batch data */
    double* arrays[2];
    size_t sizes[2];
    size_t ks[2];

    for (size_t i = 0; i < num_batches; i++) {
        arrays[i] = (double*)malloc(array_size * sizeof(double));
        ASSERT_NOT_NULL(arrays[i]);
        sizes[i] = array_size;
        ks[i] = k;
    }

    /* Array 0: {7, 6, 5, 4, 3, 2, 1, 0} */
    for (size_t i = 0; i < array_size; i++) {
        arrays[0][i] = (double)(array_size - 1 - i);
    }

    /* Array 1: {3, 1, 4, 1, 5, 9, 2, 6} */
    double arr1[] = {3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0};
    memcpy(arrays[1], arr1, array_size * sizeof(double));

    /* Call batch function */
    fc_status_t err = fc_topk_smallest_f64_batch(arrays, sizes, ks, num_batches);
    ASSERT_EQ(err, FC_OK);

    /* Verify results */
    /* Array 0: top 4 smallest should be {0, 1, 2, 3} */
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[0][0], 0.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[0][1], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[0][2], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[0][3], 3.0, 1e-10);

    /* Array 1: top 4 smallest should be {1, 1, 2, 3} */
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[1][0], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[1][1], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[1][2], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(arrays[1][3], 3.0, 1e-10);

    /* Cleanup */
    for (size_t i = 0; i < num_batches; i++) {
        free(arrays[i]);
    }
}

TEST(topk_largest_f64_batch_empty) {
    fc_status_t err = fc_topk_largest_f64_batch(NULL, NULL, NULL, 0);
    ASSERT_EQ(err, FC_OK);
}

TEST(topk_largest_f64_batch_null_pointer) {
    fc_status_t err = fc_topk_largest_f64_batch(NULL, NULL, NULL, 10);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(topk_largest_f64_batch_null_array) {
    double* arrays[3] = {NULL, NULL, NULL};
    size_t sizes[3] = {10, 10, 10};
    size_t ks[3] = {5, 5, 5};

    fc_status_t err = fc_topk_largest_f64_batch(arrays, sizes, ks, 3);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(topk_largest_f32_batch_basic) {
    const size_t num_batches = 2;
    const size_t array_size = 6;
    const size_t k = 3;

    /* Prepare batch data */
    float* arrays[2];
    size_t sizes[2];
    size_t ks[2];

    for (size_t i = 0; i < num_batches; i++) {
        arrays[i] = (float*)malloc(array_size * sizeof(float));
        ASSERT_NOT_NULL(arrays[i]);
        sizes[i] = array_size;
        ks[i] = k;
    }

    /* Array 0: {5, 4, 3, 2, 1, 0} */
    for (size_t i = 0; i < array_size; i++) {
        arrays[0][i] = (float)(array_size - 1 - i);
    }

    /* Array 1: {1, 5, 2, 4, 3, 0} */
    float arr1[] = {1.0f, 5.0f, 2.0f, 4.0f, 3.0f, 0.0f};
    memcpy(arrays[1], arr1, array_size * sizeof(float));

    /* Call batch function */
    fc_status_t err = fc_topk_largest_f32_batch(arrays, sizes, ks, num_batches);
    ASSERT_EQ(err, FC_OK);

    /* Verify results */
    /* Array 0: top 3 should be {5, 4, 3} */
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[0][0], 5.0, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[0][1], 4.0, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[0][2], 3.0, 1e-6);

    /* Array 1: top 3 should be {5, 4, 3} */
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[1][0], 5.0, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[1][1], 4.0, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[1][2], 3.0, 1e-6);

    /* Cleanup */
    for (size_t i = 0; i < num_batches; i++) {
        free(arrays[i]);
    }
}

TEST(topk_smallest_f32_batch_basic) {
    const size_t num_batches = 2;
    const size_t array_size = 5;
    const size_t k = 2;

    /* Prepare batch data */
    float* arrays[2];
    size_t sizes[2];
    size_t ks[2];

    for (size_t i = 0; i < num_batches; i++) {
        arrays[i] = (float*)malloc(array_size * sizeof(float));
        ASSERT_NOT_NULL(arrays[i]);
        sizes[i] = array_size;
        ks[i] = k;
    }

    /* Array 0: {4, 3, 2, 1, 0} */
    for (size_t i = 0; i < array_size; i++) {
        arrays[0][i] = (float)(array_size - 1 - i);
    }

    /* Array 1: {9, 2, 7, 1, 5} */
    float arr1[] = {9.0f, 2.0f, 7.0f, 1.0f, 5.0f};
    memcpy(arrays[1], arr1, array_size * sizeof(float));

    /* Call batch function */
    fc_status_t err = fc_topk_smallest_f32_batch(arrays, sizes, ks, num_batches);
    ASSERT_EQ(err, FC_OK);

    /* Verify results */
    /* Array 0: top 2 smallest should be {0, 1} */
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[0][0], 0.0, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[0][1], 1.0, 1e-6);

    /* Array 1: top 2 smallest should be {1, 2} */
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[1][0], 1.0, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ((double)arrays[1][1], 2.0, 1e-6);

    /* Cleanup */
    for (size_t i = 0; i < num_batches; i++) {
        free(arrays[i]);
    }
}

TEST(topk_largest_f64_batch_large_scale) {
    const size_t num_batches = 100;
    const size_t array_size = 1000;
    const size_t k = 100;

    /* Prepare batch data */
    double** arrays = (double**)malloc(num_batches * sizeof(double*));
    size_t* sizes = (size_t*)malloc(num_batches * sizeof(size_t));
    size_t* ks = (size_t*)malloc(num_batches * sizeof(size_t));
    ASSERT_NOT_NULL(arrays);
    ASSERT_NOT_NULL(sizes);
    ASSERT_NOT_NULL(ks);

    for (size_t i = 0; i < num_batches; i++) {
        arrays[i] = (double*)malloc(array_size * sizeof(double));
        ASSERT_NOT_NULL(arrays[i]);
        sizes[i] = array_size;
        ks[i] = k;

        /* Fill with values 0 to 999 */
        for (size_t j = 0; j < array_size; j++) {
            arrays[i][j] = (double)j;
        }
    }

    /* Call batch function */
    fc_status_t err = fc_topk_largest_f64_batch(arrays, sizes, ks, num_batches);
    ASSERT_EQ(err, FC_OK);

    /* Verify all arrays have correct top K in descending order */
    for (size_t i = 0; i < num_batches; i++) {
        for (size_t j = 0; j < k; j++) {
            FC_TEST_ASSERT_DOUBLE_EQ(arrays[i][j], (double)(array_size - 1 - j), 1e-10);
        }
    }

    /* Cleanup */
    for (size_t i = 0; i < num_batches; i++) {
        free(arrays[i]);
    }
    free(arrays);
    free(sizes);
    free(ks);
}


void register_topk_tests(void) {
    RUN_TEST(topk_largest_f64_empty);
    RUN_TEST(topk_largest_f64_null_pointer);
    RUN_TEST(topk_largest_f64_k_greater_than_n);
    RUN_TEST(topk_largest_f64_k_equals_n);
    RUN_TEST(topk_largest_f64_small_k);
    RUN_TEST(topk_largest_f64_medium_k);
    RUN_TEST(topk_largest_f64_large_k);
    RUN_TEST(topk_smallest_f64_small_k);
    RUN_TEST(topk_smallest_f64_medium_k);
    RUN_TEST(topk_largest_f32_small_k);
    RUN_TEST(topk_smallest_f32_small_k);
    RUN_TEST(topk_largest_f64_indexed_basic);
    RUN_TEST(topk_smallest_f64_indexed_basic);
    RUN_TEST(topk_largest_f64_indexed_null_pointer);
    RUN_TEST(topk_largest_f32_indexed_basic);
    RUN_TEST(topk_smallest_f32_indexed_basic);
    RUN_TEST(topk_largest_f64_with_duplicates);
    RUN_TEST(topk_smallest_f64_with_duplicates);
    RUN_TEST(topk_largest_f64_single_element);
    RUN_TEST(topk_smallest_f64_single_element);
    RUN_TEST(topk_largest_f64_k_zero);
    RUN_TEST(topk_smallest_f64_k_zero);

    /* Batch processing tests */
    RUN_TEST(topk_largest_f64_batch_basic);
    RUN_TEST(topk_smallest_f64_batch_basic);
    RUN_TEST(topk_largest_f64_batch_empty);
    RUN_TEST(topk_largest_f64_batch_null_pointer);
    RUN_TEST(topk_largest_f64_batch_null_array);
    RUN_TEST(topk_largest_f32_batch_basic);
    RUN_TEST(topk_smallest_f32_batch_basic);
    RUN_TEST(topk_largest_f64_batch_large_scale);
}
