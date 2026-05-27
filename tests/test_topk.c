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
}
