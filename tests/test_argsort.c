/**
 * @file test_argsort.c
 * @brief Unit tests for argsort functions
 */

#include "argsort.h"
#include "test_framework.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TEST(argsort_f64_basic_ascending) {
    double data[]       = {3.0, 1.0, 2.0, 5.0, 4.0};
    size_t n            = 5;
    size_t indices[5]   = {0};
    size_t expected[5]  = {1, 2, 0, 4, 3};
    fc_status_t status  = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_f64_basic_descending) {
    double data[]       = {3.0, 1.0, 2.0, 5.0, 4.0};
    size_t n            = 5;
    size_t indices[5]   = {0};
    size_t expected[5]  = {3, 4, 0, 2, 1};
    fc_status_t status  = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_DESCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_f64_empty) {
    double data[1] = {0.0};
    size_t indices[1] = {0};
    fc_status_t status = fc_sort_argsort_f64(data, 0, indices, FC_ARGSORT_ASCENDING);
    ASSERT_EQ(status, FC_OK);
}

TEST(argsort_f64_single) {
    double data[]      = {42.0};
    size_t indices[1]  = {0};
    fc_status_t status = fc_sort_argsort_f64(data, 1, indices, FC_ARGSORT_ASCENDING);
    ASSERT_EQ(status, FC_OK);
    ASSERT_EQ(indices[0], 0);
}

TEST(argsort_f64_null_pointer) {
    double data[]      = {1.0, 2.0, 3.0};
    size_t indices[3];
    fc_status_t status = fc_sort_argsort_f64(NULL, 3, indices, FC_ARGSORT_ASCENDING);
    ASSERT_EQ(status, FC_ERR_INVALID_ARG);

    status = fc_sort_argsort_f64(data, 3, NULL, FC_ARGSORT_ASCENDING);
    ASSERT_EQ(status, FC_ERR_INVALID_ARG);
}

TEST(argsort_f64_with_nan) {
    double data[]      = {3.0, NAN, 1.0, 2.0, NAN};
    size_t n           = 5;
    size_t indices[5]  = {0};
    fc_status_t status = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_EQ(indices[0], 2);
    ASSERT_EQ(indices[1], 3);
    ASSERT_EQ(indices[2], 0);
    ASSERT_TRUE(isnan(data[indices[3]]) || isnan(data[indices[4]]));
}

TEST(argsort_f64_with_inf) {
    double data[]      = {3.0, INFINITY, 1.0, -INFINITY, 2.0};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {3, 2, 4, 0, 1};
    fc_status_t status = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_f64_duplicates) {
    double data[]      = {3.0, 1.0, 2.0, 1.0, 3.0};
    size_t n           = 5;
    size_t indices[5]  = {0};
    fc_status_t status = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    FC_TEST_ASSERT_DOUBLE_EQ(data[indices[0]], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[indices[1]], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[indices[2]], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[indices[3]], 3.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[indices[4]], 3.0, 1e-10);
}

TEST(argsort_f64_stable_preserves_order) {
    double data[]      = {3.0, 1.0, 2.0, 1.0, 3.0};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {1, 3, 2, 0, 4};
    fc_status_t status = fc_sort_argsort_f64_stable(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_f64_large_array) {
    size_t n = 10000;
    double* data = (double*)malloc(n * sizeof(double));
    size_t* indices = (size_t*)malloc(n * sizeof(size_t));

    for (size_t i = 0; i < n; i++) {
        data[i] = (double)(n - i);
    }

    fc_status_t status = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);
    ASSERT_EQ(status, FC_OK);

    for (size_t i = 0; i < n - 1; i++) {
        ASSERT_TRUE(data[indices[i]] <= data[indices[i + 1]]);
    }

    free(data);
    free(indices);
}

TEST(argsort_f32_basic) {
    float data[]       = {3.0f, 1.0f, 2.0f, 5.0f, 4.0f};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {1, 2, 0, 4, 3};
    fc_status_t status = fc_sort_argsort_f32(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_i64_basic) {
    int64_t data[]     = {3, 1, 2, 5, 4};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {1, 2, 0, 4, 3};
    fc_status_t status = fc_sort_argsort_i64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_i64_descending) {
    int64_t data[]     = {3, 1, 2, 5, 4};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {3, 4, 0, 2, 1};
    fc_status_t status = fc_sort_argsort_i64(data, n, indices, FC_ARGSORT_DESCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_i64_negative) {
    int64_t data[]     = {-3, 1, -2, 5, -4};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {4, 0, 2, 1, 3};
    fc_status_t status = fc_sort_argsort_i64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_i64_stable) {
    int64_t data[]     = {3, 1, 2, 1, 3};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {1, 3, 2, 0, 4};
    fc_status_t status = fc_sort_argsort_i64_stable(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_i32_basic) {
    int32_t data[]     = {3, 1, 2, 5, 4};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {1, 2, 0, 4, 3};
    fc_status_t status = fc_sort_argsort_i32(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_i32_stable) {
    int32_t data[]     = {3, 1, 2, 1, 3};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {1, 3, 2, 0, 4};
    fc_status_t status = fc_sort_argsort_i32_stable(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_f64_already_sorted) {
    double data[]      = {1.0, 2.0, 3.0, 4.0, 5.0};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {0, 1, 2, 3, 4};
    fc_status_t status = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_f64_reverse_sorted) {
    double data[]      = {5.0, 4.0, 3.0, 2.0, 1.0};
    size_t n           = 5;
    size_t indices[5]  = {0};
    size_t expected[5] = {4, 3, 2, 1, 0};
    fc_status_t status = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    ASSERT_TRUE(memcmp(indices, expected, n * sizeof(size_t)) == 0);
}

TEST(argsort_f64_all_equal) {
    double data[]      = {2.0, 2.0, 2.0, 2.0, 2.0};
    size_t n           = 5;
    size_t indices[5]  = {0};
    fc_status_t status = fc_sort_argsort_f64(data, n, indices, FC_ARGSORT_ASCENDING);

    ASSERT_EQ(status, FC_OK);
    for (size_t i = 0; i < n; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[indices[i]], 2.0, 1e-10);
    }
}


void register_argsort_tests(void) {
    RUN_TEST(argsort_f64_basic_ascending);
    RUN_TEST(argsort_f64_basic_descending);
    RUN_TEST(argsort_f64_empty);
    RUN_TEST(argsort_f64_single);
    RUN_TEST(argsort_f64_null_pointer);
    RUN_TEST(argsort_f64_with_nan);
    RUN_TEST(argsort_f64_with_inf);
    RUN_TEST(argsort_f64_duplicates);
    RUN_TEST(argsort_f64_stable_preserves_order);
    RUN_TEST(argsort_f64_large_array);
    RUN_TEST(argsort_f64_already_sorted);
    RUN_TEST(argsort_f64_reverse_sorted);
    RUN_TEST(argsort_f64_all_equal);
    RUN_TEST(argsort_f32_basic);
    RUN_TEST(argsort_i64_basic);
    RUN_TEST(argsort_i64_descending);
    RUN_TEST(argsort_i64_negative);
    RUN_TEST(argsort_i64_stable);
    RUN_TEST(argsort_i32_basic);
    RUN_TEST(argsort_i32_stable);
}
