/**
 * @file test_int_sort.c
 * @brief Unit tests for integer sorting
 */

#include "test_framework.h"
#include "int_sort.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

TEST(int64_empty_array) {
    int64_t data[1] = {0};
    fc_status_t err = fc_sort_int64(data, 0, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);
}

TEST(int64_single_element) {
    int64_t data[1] = {42};
    fc_status_t err = fc_sort_int64(data, 1, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);
    ASSERT_EQ(data[0], 42);
}

TEST(int64_two_elements_ascending) {
    int64_t data[2] = {2, 1};
    fc_status_t err = fc_sort_int64(data, 2, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);
    ASSERT_EQ(data[0], 1);
    ASSERT_EQ(data[1], 2);
}

TEST(int64_two_elements_descending) {
    int64_t data[2] = {1, 2};
    fc_status_t err = fc_sort_int64(data, 2, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);
    ASSERT_EQ(data[0], 2);
    ASSERT_EQ(data[1], 1);
}

TEST(int64_small_array_ascending) {
    int64_t data[10] = {9, 3, 7, 1, 5, 8, 2, 6, 4, 0};
    fc_status_t err = fc_sort_int64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (int64_t)i);
    }
}

TEST(int64_small_array_descending) {
    int64_t data[10] = {0, 4, 6, 2, 8, 5, 1, 7, 3, 9};
    fc_status_t err = fc_sort_int64(data, 10, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (int64_t)(9 - i));
    }
}

TEST(int64_negative_numbers) {
    int64_t data[10] = {-5, 3, -2, 8, -9, 1, -7, 4, -1, 0};
    fc_status_t err = fc_sort_int64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    int64_t expected[10] = {-9, -7, -5, -2, -1, 0, 1, 3, 4, 8};
    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], expected[i]);
    }
}

TEST(int64_all_negative) {
    int64_t data[5] = {-1, -5, -3, -2, -4};
    fc_status_t err = fc_sort_int64(data, 5, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    int64_t expected[5] = {-5, -4, -3, -2, -1};
    for (size_t i = 0; i < 5; i++) {
        ASSERT_EQ(data[i], expected[i]);
    }
}

TEST(int64_duplicates) {
    int64_t data[10] = {5, 2, 8, 2, 9, 5, 1, 8, 5, 2};
    fc_status_t err = fc_sort_int64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    int64_t expected[10] = {1, 2, 2, 2, 5, 5, 5, 8, 8, 9};
    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], expected[i]);
    }
}

TEST(int64_medium_array) {
    const size_t n = 1000;
    int64_t* data = (int64_t*)malloc(n * sizeof(int64_t));
    ASSERT_NOT_NULL(data);

    for (size_t i = 0; i < n; i++) {
        data[i] = (int64_t)(n - i - 1);
    }

    fc_status_t err = fc_sort_int64(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(data[i], (int64_t)i);
    }

    free(data);
}

TEST(int64_large_array) {
    const size_t n = 100000;
    int64_t* data = (int64_t*)malloc(n * sizeof(int64_t));
    ASSERT_NOT_NULL(data);

    srand(12345);
    for (size_t i = 0; i < n; i++) {
        data[i] = (int64_t)(rand() % 10000) - 5000;
    }

    fc_status_t err = fc_sort_int64(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_EQ(fc_sort_is_sorted_int64(data, n, FC_SORT_ASCENDING), 1);

    free(data);
}

TEST(int64_extreme_values) {
    int64_t data[5] = {INT64_MAX, INT64_MIN, 0, INT64_MAX / 2, INT64_MIN / 2};
    fc_status_t err = fc_sort_int64(data, 5, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_EQ(data[0], INT64_MIN);
    ASSERT_EQ(data[1], INT64_MIN / 2);
    ASSERT_EQ(data[2], 0);
    ASSERT_EQ(data[3], INT64_MAX / 2);
    ASSERT_EQ(data[4], INT64_MAX);
}

TEST(uint64_small_array_ascending) {
    uint64_t data[10] = {9, 3, 7, 1, 5, 8, 2, 6, 4, 0};
    fc_status_t err = fc_sort_uint64(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (uint64_t)i);
    }
}

TEST(uint64_small_array_descending) {
    uint64_t data[10] = {0, 4, 6, 2, 8, 5, 1, 7, 3, 9};
    fc_status_t err = fc_sort_uint64(data, 10, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (uint64_t)(9 - i));
    }
}

TEST(uint64_large_values) {
    uint64_t data[5] = {UINT64_MAX, 0, UINT64_MAX / 2, 1, UINT64_MAX - 1};
    fc_status_t err = fc_sort_uint64(data, 5, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_EQ(data[0], 0);
    ASSERT_EQ(data[1], 1);
    ASSERT_EQ(data[2], UINT64_MAX / 2);
    ASSERT_EQ(data[3], UINT64_MAX - 1);
    ASSERT_EQ(data[4], UINT64_MAX);
}

TEST(uint64_medium_array) {
    const size_t n = 1000;
    uint64_t* data = (uint64_t*)malloc(n * sizeof(uint64_t));
    ASSERT_NOT_NULL(data);

    for (size_t i = 0; i < n; i++) {
        data[i] = (uint64_t)(n - i - 1);
    }

    fc_status_t err = fc_sort_uint64(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(data[i], (uint64_t)i);
    }

    free(data);
}

TEST(int32_small_array_ascending) {
    int32_t data[10] = {9, 3, 7, 1, 5, 8, 2, 6, 4, 0};
    fc_status_t err = fc_sort_int32(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (int32_t)i);
    }
}

TEST(int32_small_array_descending) {
    int32_t data[10] = {0, 4, 6, 2, 8, 5, 1, 7, 3, 9};
    fc_status_t err = fc_sort_int32(data, 10, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (int32_t)(9 - i));
    }
}

TEST(int32_negative_numbers) {
    int32_t data[10] = {-5, 3, -2, 8, -9, 1, -7, 4, -1, 0};
    fc_status_t err = fc_sort_int32(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    int32_t expected[10] = {-9, -7, -5, -2, -1, 0, 1, 3, 4, 8};
    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], expected[i]);
    }
}

TEST(int32_extreme_values) {
    int32_t data[5] = {INT32_MAX, INT32_MIN, 0, INT32_MAX / 2, INT32_MIN / 2};
    fc_status_t err = fc_sort_int32(data, 5, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_EQ(data[0], INT32_MIN);
    ASSERT_EQ(data[1], INT32_MIN / 2);
    ASSERT_EQ(data[2], 0);
    ASSERT_EQ(data[3], INT32_MAX / 2);
    ASSERT_EQ(data[4], INT32_MAX);
}

TEST(int32_medium_array) {
    const size_t n = 1000;
    int32_t* data = (int32_t*)malloc(n * sizeof(int32_t));
    ASSERT_NOT_NULL(data);

    for (size_t i = 0; i < n; i++) {
        data[i] = (int32_t)(n - i - 1);
    }

    fc_status_t err = fc_sort_int32(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(data[i], (int32_t)i);
    }

    free(data);
}

TEST(uint32_small_array_ascending) {
    uint32_t data[10] = {9, 3, 7, 1, 5, 8, 2, 6, 4, 0};
    fc_status_t err = fc_sort_uint32(data, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (uint32_t)i);
    }
}

TEST(uint32_small_array_descending) {
    uint32_t data[10] = {0, 4, 6, 2, 8, 5, 1, 7, 3, 9};
    fc_status_t err = fc_sort_uint32(data, 10, FC_SORT_DESCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < 10; i++) {
        ASSERT_EQ(data[i], (uint32_t)(9 - i));
    }
}

TEST(uint32_large_values) {
    uint32_t data[5] = {UINT32_MAX, 0, UINT32_MAX / 2, 1, UINT32_MAX - 1};
    fc_status_t err = fc_sort_uint32(data, 5, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    ASSERT_EQ(data[0], 0);
    ASSERT_EQ(data[1], 1);
    ASSERT_EQ(data[2], UINT32_MAX / 2);
    ASSERT_EQ(data[3], UINT32_MAX - 1);
    ASSERT_EQ(data[4], UINT32_MAX);
}

TEST(uint32_medium_array) {
    const size_t n = 1000;
    uint32_t* data = (uint32_t*)malloc(n * sizeof(uint32_t));
    ASSERT_NOT_NULL(data);

    for (size_t i = 0; i < n; i++) {
        data[i] = (uint32_t)(n - i - 1);
    }

    fc_status_t err = fc_sort_uint32(data, n, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_OK);

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(data[i], (uint32_t)i);
    }

    free(data);
}

TEST(int64_null_pointer) {
    fc_status_t err = fc_sort_int64(NULL, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(uint64_null_pointer) {
    fc_status_t err = fc_sort_uint64(NULL, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(int32_null_pointer) {
    fc_status_t err = fc_sort_int32(NULL, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(uint32_null_pointer) {
    fc_status_t err = fc_sort_uint32(NULL, 10, FC_SORT_ASCENDING);
    ASSERT_EQ(err, FC_ERR_INVALID_ARG);
}

TEST(is_sorted_int64_ascending) {
    int64_t data[5] = {1, 2, 3, 4, 5};
    ASSERT_EQ(fc_sort_is_sorted_int64(data, 5, FC_SORT_ASCENDING), 1);
}

TEST(is_sorted_int64_descending) {
    int64_t data[5] = {5, 4, 3, 2, 1};
    ASSERT_EQ(fc_sort_is_sorted_int64(data, 5, FC_SORT_DESCENDING), 1);
}

TEST(is_sorted_int64_not_sorted) {
    int64_t data[5] = {1, 3, 2, 4, 5};
    ASSERT_EQ(fc_sort_is_sorted_int64(data, 5, FC_SORT_ASCENDING), 0);
}

TEST(is_sorted_uint64_ascending) {
    uint64_t data[5] = {1, 2, 3, 4, 5};
    ASSERT_EQ(fc_sort_is_sorted_uint64(data, 5, FC_SORT_ASCENDING), 1);
}

TEST(is_sorted_uint64_not_sorted) {
    uint64_t data[5] = {1, 3, 2, 4, 5};
    ASSERT_EQ(fc_sort_is_sorted_uint64(data, 5, FC_SORT_ASCENDING), 0);
}

TEST(is_sorted_int32_ascending) {
    int32_t data[5] = {1, 2, 3, 4, 5};
    ASSERT_EQ(fc_sort_is_sorted_int32(data, 5, FC_SORT_ASCENDING), 1);
}

TEST(is_sorted_uint32_ascending) {
    uint32_t data[5] = {1, 2, 3, 4, 5};
    ASSERT_EQ(fc_sort_is_sorted_uint32(data, 5, FC_SORT_ASCENDING), 1);
}

void register_int_sort_tests(void) {
    RUN_TEST(int64_empty_array);
    RUN_TEST(int64_single_element);
    RUN_TEST(int64_two_elements_ascending);
    RUN_TEST(int64_two_elements_descending);
    RUN_TEST(int64_small_array_ascending);
    RUN_TEST(int64_small_array_descending);
    RUN_TEST(int64_negative_numbers);
    RUN_TEST(int64_all_negative);
    RUN_TEST(int64_duplicates);
    RUN_TEST(int64_medium_array);
    RUN_TEST(int64_large_array);
    RUN_TEST(int64_extreme_values);

    RUN_TEST(uint64_small_array_ascending);
    RUN_TEST(uint64_small_array_descending);
    RUN_TEST(uint64_large_values);
    RUN_TEST(uint64_medium_array);

    RUN_TEST(int32_small_array_ascending);
    RUN_TEST(int32_small_array_descending);
    RUN_TEST(int32_negative_numbers);
    RUN_TEST(int32_extreme_values);
    RUN_TEST(int32_medium_array);

    RUN_TEST(uint32_small_array_ascending);
    RUN_TEST(uint32_small_array_descending);
    RUN_TEST(uint32_large_values);
    RUN_TEST(uint32_medium_array);

    RUN_TEST(int64_null_pointer);
    RUN_TEST(uint64_null_pointer);
    RUN_TEST(int32_null_pointer);
    RUN_TEST(uint32_null_pointer);

    RUN_TEST(is_sorted_int64_ascending);
    RUN_TEST(is_sorted_int64_descending);
    RUN_TEST(is_sorted_int64_not_sorted);
    RUN_TEST(is_sorted_uint64_ascending);
    RUN_TEST(is_sorted_uint64_not_sorted);
    RUN_TEST(is_sorted_int32_ascending);
    RUN_TEST(is_sorted_uint32_ascending);
}
