/**
 * @file test_float_sort.c
 * @brief Unit tests for floating-point sorting
 */

#include "test_framework.h"
#include "float_sort.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

TEST(float64_empty_array) {
    double data[1] = {0.0};
    fc_status_t err = fc_sort_float64(data, 0, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);
}

TEST(float64_single_element) {
    double data[1] = {42.0};
    fc_status_t err = fc_sort_float64(data, 1, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 42.0, 1e-10);
}

TEST(float64_two_elements_ascending) {
    double data[2] = {2.0, 1.0};
    fc_status_t err = fc_sort_float64(data, 2, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 2.0, 1e-10);
}

TEST(float64_two_elements_descending) {
    double data[2] = {1.0, 2.0};
    fc_status_t err = fc_sort_float64(data, 2, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);
    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 1.0, 1e-10);
}

TEST(float64_small_array_ascending) {
    double data[10] = {9.0, 3.0, 7.0, 1.0, 5.0, 8.0, 2.0, 6.0, 4.0, 0.0};
    fc_status_t err = fc_sort_float64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (double)i, 1e-10);
    }
}

TEST(float64_small_array_descending) {
    double data[10] = {0.0, 4.0, 6.0, 2.0, 8.0, 5.0, 1.0, 7.0, 3.0, 9.0};
    fc_status_t err = fc_sort_float64(data, 10, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (double)(9 - i), 1e-10);
    }
}

TEST(float64_medium_array) {
    const size_t n = 1000;
    double* data = (double*)malloc(n * sizeof(double));
    ASSERT_NOT_NULL(data);

    for (size_t i = 0; i < n; i++) {
        data[i] = (double)(n - i - 1);
    }

    fc_status_t err = fc_sort_float64(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < n; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (double)i, 1e-10);
    }

    free(data);
}

TEST(float64_large_array) {
    const size_t n = 100000;
    double* data = (double*)malloc(n * sizeof(double));
    ASSERT_NOT_NULL(data);

    for (size_t i = 0; i < n; i++) {
        data[i] = (double)((i * 7919) % n);
    }

    fc_status_t err = fc_sort_float64(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_TRUE(fc_sort_is_sorted_float64(data, n, FC_SORT_ASCENDING));

    free(data);
}

TEST(float64_with_duplicates) {
    double data[10] = {5.0, 2.0, 8.0, 2.0, 9.0, 1.0, 5.0, 3.0, 8.0, 1.0};
    fc_status_t err = fc_sort_float64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    double expected[10] = {1.0, 1.0, 2.0, 2.0, 3.0, 5.0, 5.0, 8.0, 8.0, 9.0};
    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], expected[i], 1e-10);
    }
}

TEST(float64_already_sorted) {
    double data[10] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    fc_status_t err = fc_sort_float64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (double)i, 1e-10);
    }
}

TEST(float64_reverse_sorted) {
    double data[10] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
    fc_status_t err = fc_sort_float64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (double)i, 1e-10);
    }
}

TEST(float64_with_negatives) {
    double data[10] = {-5.0, 3.0, -2.0, 8.0, -9.0, 1.0, -1.0, 6.0, 0.0, -7.0};
    fc_status_t err = fc_sort_float64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    double expected[10] = {-9.0, -7.0, -5.0, -2.0, -1.0, 0.0, 1.0, 3.0, 6.0, 8.0};
    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], expected[i], 1e-10);
    }
}

TEST(float64_with_zeros) {
    double data[6] = {-0.0, 1.0, 0.0, -1.0, 0.0, -0.0};
    fc_status_t err = fc_sort_float64(data, 6, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    FC_TEST_ASSERT_DOUBLE_EQ(data[0], -1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[5], 1.0, 1e-10);
}

TEST(float64_with_nan_last) {
    double data[5] = {3.0, NAN, 1.0, 2.0, NAN};
    fc_status_t err = fc_sort_float64_ex(data, 5, FC_SORT_ASCENDING, FC_SORT_NAN_LAST);
    ASSERT_EQ(err, FC_OK);

    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 3.0, 1e-10);
    ASSERT_TRUE(isnan(data[3]));
    ASSERT_TRUE(isnan(data[4]));
}

TEST(float64_with_nan_first) {
    double data[5] = {3.0, NAN, 1.0, 2.0, NAN};
    fc_status_t err = fc_sort_float64_ex(data, 5, FC_SORT_ASCENDING, FC_SORT_NAN_FIRST);
    ASSERT_EQ(err, FC_OK);

    ASSERT_TRUE(isnan(data[0]));
    ASSERT_TRUE(isnan(data[1]));
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 1.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[3], 2.0, 1e-10);
    FC_TEST_ASSERT_DOUBLE_EQ(data[4], 3.0, 1e-10);
}

TEST(float64_with_nan_reject) {
    double data[5] = {3.0, NAN, 1.0, 2.0, 4.0};
    fc_status_t err = fc_sort_float64_ex(data, 5, FC_SORT_ASCENDING, FC_SORT_NAN_REJECT);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(float64_all_nan) {
    double data[3] = {NAN, NAN, NAN};
    fc_status_t err = fc_sort_float64(data, 3, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_TRUE(isnan(data[0]));
    ASSERT_TRUE(isnan(data[1]));
    ASSERT_TRUE(isnan(data[2]));
}

TEST(float64_null_pointer) {
    fc_status_t err = fc_sort_float64(NULL, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(float32_basic) {
    float data[10] = {9.0f, 3.0f, 7.0f, 1.0f, 5.0f, 8.0f, 2.0f, 6.0f, 4.0f, 0.0f};
    fc_status_t err = fc_sort_float32(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (float)i, 1e-6);
    }
}

TEST(float32_descending) {
    float data[10] = {0.0f, 4.0f, 6.0f, 2.0f, 8.0f, 5.0f, 1.0f, 7.0f, 3.0f, 9.0f};
    fc_status_t err = fc_sort_float32(data, 10, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        FC_TEST_ASSERT_DOUBLE_EQ(data[i], (float)(9 - i), 1e-6);
    }
}

TEST(float32_large_array) {
    const size_t n = 50000;
    float* data = (float*)malloc(n * sizeof(float));
    ASSERT_NOT_NULL(data);

    for (size_t i = 0; i < n; i++) {
        data[i] = (float)((i * 7919) % n);
    }

    fc_status_t err = fc_sort_float32(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_TRUE(fc_sort_is_sorted_float32(data, n, FC_SORT_ASCENDING));

    free(data);
}

TEST(float32_with_nan) {
    float data[5] = {3.0f, NAN, 1.0f, 2.0f, NAN};
    fc_status_t err = fc_sort_float32(data, 5, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    FC_TEST_ASSERT_DOUBLE_EQ(data[0], 1.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(data[1], 2.0f, 1e-6);
    FC_TEST_ASSERT_DOUBLE_EQ(data[2], 3.0f, 1e-6);
    ASSERT_TRUE(isnan(data[3]));
    ASSERT_TRUE(isnan(data[4]));
}

TEST(is_sorted_true) {
    double data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    ASSERT_TRUE(fc_sort_is_sorted_float64(data, 5, FC_SORT_ASCENDING));
}

TEST(is_sorted_false) {
    double data[5] = {1.0, 3.0, 2.0, 4.0, 5.0};
    ASSERT_FALSE(fc_sort_is_sorted_float64(data, 5, FC_SORT_ASCENDING));
}

TEST(is_sorted_descending) {
    double data[5] = {5.0, 4.0, 3.0, 2.0, 1.0};
    ASSERT_TRUE(fc_sort_is_sorted_float64(data, 5, FC_SORT_DESCENDING));
}

void register_float_sort_tests(void) {
    RUN_TEST(float64_empty_array);
    RUN_TEST(float64_single_element);
    RUN_TEST(float64_two_elements_ascending);
    RUN_TEST(float64_two_elements_descending);
    RUN_TEST(float64_small_array_ascending);
    RUN_TEST(float64_small_array_descending);
    RUN_TEST(float64_medium_array);
    RUN_TEST(float64_large_array);
    RUN_TEST(float64_with_duplicates);
    RUN_TEST(float64_already_sorted);
    RUN_TEST(float64_reverse_sorted);
    RUN_TEST(float64_with_negatives);
    RUN_TEST(float64_with_zeros);
    RUN_TEST(float64_with_nan_last);
    RUN_TEST(float64_with_nan_first);
    RUN_TEST(float64_with_nan_reject);
    RUN_TEST(float64_all_nan);
    RUN_TEST(float64_null_pointer);
    RUN_TEST(float32_basic);
    RUN_TEST(float32_descending);
    RUN_TEST(float32_large_array);
    RUN_TEST(float32_with_nan);
    RUN_TEST(is_sorted_true);
    RUN_TEST(is_sorted_false);
    RUN_TEST(is_sorted_descending);
}
