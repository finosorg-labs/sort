#include "test_framework.h"
#include <multikey.h>
#include <float_sort.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_ASSERT(cond, msg) FC_TEST_ASSERT_MSG(cond, msg)

typedef struct {
    double price;
    int64_t time;
    uint32_t order_id;
} Order;

typedef struct {
    int32_t sector;
    double market_cap;
    char ticker[8];
} Stock;

typedef struct {
    float x;
    float y;
    int32_t id;
} Point;

TEST(test_multikey_basic_two_keys) {
    Order orders[] = {
        {100.5, 1000, 1}, {100.5, 900, 2},  {100.0, 1100, 3},
        {100.5, 1000, 4}, {101.0, 800, 5},  {100.0, 1000, 6},
        {100.5, 1100, 7}, {100.0, 1000, 8},
    };
    size_t count = sizeof(orders) / sizeof(orders[0]);

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_DESCENDING},
        {offsetof(Order, time), FC_SORT_KEY_INT64, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(orders, count, sizeof(Order), keys, 2);
    TEST_ASSERT(status == FC_OK, "Sort should succeed");

    TEST_ASSERT(orders[0].price == 101.0 && orders[0].order_id == 5,
                "First should be highest price");
    TEST_ASSERT(orders[1].price == 100.5 && orders[1].time == 900,
                "Second should be 100.5 with earliest time");
    TEST_ASSERT(orders[2].price == 100.5 && orders[2].time == 1000,
                "Third should be 100.5 with time 1000");
    TEST_ASSERT(orders[7].price == 100.0 && orders[7].time == 1100,
                "Last should be lowest price with latest time");

    int is_sorted = fc_sort_is_sorted_multikey(orders, count, sizeof(Order), keys, 2);
    TEST_ASSERT(is_sorted == 1, "Array should be sorted");
}

TEST(test_multikey_three_keys) {
    Order orders[] = {
        {100.5, 1000, 3}, {100.5, 1000, 1}, {100.5, 1000, 2},
        {100.5, 900, 5},  {100.5, 900, 4},  {101.0, 800, 6},
    };
    size_t count = sizeof(orders) / sizeof(orders[0]);

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_DESCENDING},
        {offsetof(Order, time), FC_SORT_KEY_INT64, FC_SORT_ASCENDING},
        {offsetof(Order, order_id), FC_SORT_KEY_UINT32, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(orders, count, sizeof(Order), keys, 3);
    TEST_ASSERT(status == FC_OK, "Sort should succeed");

    TEST_ASSERT(orders[0].price == 101.0, "First should be highest price");
    TEST_ASSERT(orders[1].price == 100.5 && orders[1].time == 900 && orders[1].order_id == 4,
                "Second should be 100.5, time 900, order_id 4");
    TEST_ASSERT(orders[2].price == 100.5 && orders[2].time == 900 && orders[2].order_id == 5,
                "Third should be 100.5, time 900, order_id 5");
    TEST_ASSERT(orders[3].order_id == 1, "Fourth should have order_id 1");
    TEST_ASSERT(orders[4].order_id == 2, "Fifth should have order_id 2");
    TEST_ASSERT(orders[5].order_id == 3, "Sixth should have order_id 3");
}

TEST(test_multikey_ascending_order) {
    Point points[] = {
        {3.0f, 2.0f, 5}, {1.0f, 4.0f, 2}, {3.0f, 1.0f, 4},
        {1.0f, 3.0f, 1}, {2.0f, 2.0f, 3},
    };
    size_t count = sizeof(points) / sizeof(points[0]);

    fc_sort_key_spec_t keys[] = {
        {offsetof(Point, x), FC_SORT_KEY_FLOAT, FC_SORT_ASCENDING},
        {offsetof(Point, y), FC_SORT_KEY_FLOAT, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(points, count, sizeof(Point), keys, 2);
    TEST_ASSERT(status == FC_OK, "Sort should succeed");

    TEST_ASSERT(points[0].x == 1.0f && points[0].y == 3.0f, "First point should be (1,3)");
    TEST_ASSERT(points[1].x == 1.0f && points[1].y == 4.0f, "Second point should be (1,4)");
    TEST_ASSERT(points[2].x == 2.0f && points[2].y == 2.0f, "Third point should be (2,2)");
    TEST_ASSERT(points[3].x == 3.0f && points[3].y == 1.0f, "Fourth point should be (3,1)");
    TEST_ASSERT(points[4].x == 3.0f && points[4].y == 2.0f, "Fifth point should be (3,2)");
}

TEST(test_multikey_indexed) {
    FC_TEST_SKIP("Indexed sorting not yet implemented - will be in argsort.c");

    Order orders[] = {
        {100.5, 1000, 1}, {100.5, 900, 2}, {100.0, 1100, 3}, {101.0, 800, 4},
    };
    size_t count = sizeof(orders) / sizeof(orders[0]);
    size_t indices[4];

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_DESCENDING},
        {offsetof(Order, time), FC_SORT_KEY_INT64, FC_SORT_ASCENDING},
    };

    fc_status_t status =
        fc_sort_multikey_indexed(orders, count, sizeof(Order), keys, 2, indices);
    TEST_ASSERT(status == FC_OK, "Indexed sort should succeed");

    TEST_ASSERT(indices[0] == 3, "First index should be 3 (101.0)");
    TEST_ASSERT(indices[1] == 1, "Second index should be 1 (100.5, 900)");
    TEST_ASSERT(indices[2] == 0, "Third index should be 0 (100.5, 1000)");
    TEST_ASSERT(indices[3] == 2, "Fourth index should be 2 (100.0, 1100)");

    TEST_ASSERT(orders[0].order_id == 1, "Original data should not be modified");
}

TEST(test_multikey_single_key) {
    Order orders[] = {
        {100.5, 1000, 1}, {100.0, 900, 2}, {101.0, 1100, 3}, {99.5, 800, 4},
    };
    size_t count = sizeof(orders) / sizeof(orders[0]);

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(orders, count, sizeof(Order), keys, 1);
    TEST_ASSERT(status == FC_OK, "Single key sort should succeed");

    TEST_ASSERT(orders[0].price == 99.5, "First should be 99.5");
    TEST_ASSERT(orders[1].price == 100.0, "Second should be 100.0");
    TEST_ASSERT(orders[2].price == 100.5, "Third should be 100.5");
    TEST_ASSERT(orders[3].price == 101.0, "Fourth should be 101.0");
}

TEST(test_multikey_large_array) {
    const size_t count = 1000;
    Order* orders      = (Order*)malloc(count * sizeof(Order));
    TEST_ASSERT(orders != NULL, "Memory allocation should succeed");

    for (size_t i = 0; i < count; i++) {
        orders[i].price    = 100.0 + (double)(i % 10);
        orders[i].time     = 1000 + (int64_t)(i % 100);
        orders[i].order_id = (uint32_t)i;
    }

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_ASCENDING},
        {offsetof(Order, time), FC_SORT_KEY_INT64, FC_SORT_ASCENDING},
        {offsetof(Order, order_id), FC_SORT_KEY_UINT32, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(orders, count, sizeof(Order), keys, 3);
    TEST_ASSERT(status == FC_OK, "Large array sort should succeed");

    int is_sorted = fc_sort_is_sorted_multikey(orders, count, sizeof(Order), keys, 3);
    TEST_ASSERT(is_sorted == 1, "Large array should be sorted");

    for (size_t i = 1; i < count; i++) {
        TEST_ASSERT(orders[i].price >= orders[i - 1].price, "Prices should be ascending");
        if (orders[i].price == orders[i - 1].price) {
            TEST_ASSERT(orders[i].time >= orders[i - 1].time, "Times should be ascending");
            if (orders[i].time == orders[i - 1].time) {
                TEST_ASSERT(orders[i].order_id >= orders[i - 1].order_id,
                            "Order IDs should be ascending");
            }
        }
    }

    free(orders);
}

TEST(test_multikey_stability) {
    typedef struct {
        int32_t key;
        int32_t original_index;
    } Record;

    Record records[] = {
        {1, 0}, {2, 1}, {1, 2}, {3, 3}, {2, 4}, {1, 5},
    };
    size_t count = sizeof(records) / sizeof(records[0]);

    fc_sort_key_spec_t keys[] = {
        {offsetof(Record, key), FC_SORT_KEY_INT32, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(records, count, sizeof(Record), keys, 1);
    TEST_ASSERT(status == FC_OK, "Sort should succeed");

    TEST_ASSERT(records[0].key == 1 && records[0].original_index == 0,
                "First 1 should maintain order");
    TEST_ASSERT(records[1].key == 1 && records[1].original_index == 2,
                "Second 1 should maintain order");
    TEST_ASSERT(records[2].key == 1 && records[2].original_index == 5,
                "Third 1 should maintain order");
    TEST_ASSERT(records[3].key == 2 && records[3].original_index == 1,
                "First 2 should maintain order");
    TEST_ASSERT(records[4].key == 2 && records[4].original_index == 4,
                "Second 2 should maintain order");
}

TEST(test_multikey_empty_and_single) {
    Order orders[1] = {{100.0, 1000, 1}};

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(orders, 0, sizeof(Order), keys, 1);
    TEST_ASSERT(status == FC_OK, "Empty array should succeed");

    status = fc_sort_multikey(orders, 1, sizeof(Order), keys, 1);
    TEST_ASSERT(status == FC_OK, "Single element should succeed");
    TEST_ASSERT(orders[0].order_id == 1, "Single element should be unchanged");
}

TEST(test_multikey_invalid_args) {
    Order orders[4] = {{100.0, 1000, 1}, {101.0, 900, 2}, {99.0, 1100, 3}, {100.5, 800, 4}};

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(NULL, 4, sizeof(Order), keys, 1);
    TEST_ASSERT(status == FC_ERR_INVALID_ARG, "NULL data should fail");

    status = fc_sort_multikey(orders, 4, sizeof(Order), NULL, 1);
    TEST_ASSERT(status == FC_ERR_INVALID_ARG, "NULL keys should fail");

    status = fc_sort_multikey(orders, 4, sizeof(Order), keys, 0);
    TEST_ASSERT(status == FC_ERR_INVALID_ARG, "Zero keys should fail");

    status = fc_sort_multikey(orders, 4, sizeof(Order), keys, 9);
    TEST_ASSERT(status == FC_ERR_INVALID_ARG, "Too many keys should fail");

    status = fc_sort_multikey(orders, 4, 0, keys, 1);
    TEST_ASSERT(status == FC_ERR_INVALID_ARG, "Zero record size should fail");
}

TEST(test_multikey_all_equal) {
    Order orders[] = {
        {100.0, 1000, 1}, {100.0, 1000, 2}, {100.0, 1000, 3}, {100.0, 1000, 4},
    };
    size_t count = sizeof(orders) / sizeof(orders[0]);

    fc_sort_key_spec_t keys[] = {
        {offsetof(Order, price), FC_SORT_KEY_DOUBLE, FC_SORT_ASCENDING},
        {offsetof(Order, time), FC_SORT_KEY_INT64, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(orders, count, sizeof(Order), keys, 2);
    TEST_ASSERT(status == FC_OK, "All equal sort should succeed");

    TEST_ASSERT(orders[0].order_id == 1, "Order should be preserved (stable)");
    TEST_ASSERT(orders[1].order_id == 2, "Order should be preserved (stable)");
    TEST_ASSERT(orders[2].order_id == 3, "Order should be preserved (stable)");
    TEST_ASSERT(orders[3].order_id == 4, "Order should be preserved (stable)");
}

TEST(test_multikey_mixed_types) {
    typedef struct {
        uint64_t id;
        int32_t priority;
        float score;
        double value;
    } MixedRecord;

    MixedRecord records[] = {
        {100, 1, 95.5f, 1000.0}, {200, 1, 98.0f, 900.0},  {300, 2, 95.5f, 1100.0},
        {400, 1, 95.5f, 950.0},  {500, 2, 98.0f, 1050.0},
    };
    size_t count = sizeof(records) / sizeof(records[0]);

    fc_sort_key_spec_t keys[] = {
        {offsetof(MixedRecord, priority), FC_SORT_KEY_INT32, FC_SORT_ASCENDING},
        {offsetof(MixedRecord, score), FC_SORT_KEY_FLOAT, FC_SORT_DESCENDING},
        {offsetof(MixedRecord, value), FC_SORT_KEY_DOUBLE, FC_SORT_ASCENDING},
    };

    fc_status_t status = fc_sort_multikey(records, count, sizeof(MixedRecord), keys, 3);
    TEST_ASSERT(status == FC_OK, "Mixed types sort should succeed");

    TEST_ASSERT(records[0].priority == 1 && records[0].score == 98.0f, "First should be p1, s98");
    TEST_ASSERT(records[1].priority == 1 && records[1].score == 95.5f &&
                    records[1].value == 950.0,
                "Second should be p1, s95.5, v950");
    TEST_ASSERT(records[2].priority == 1 && records[2].score == 95.5f &&
                    records[2].value == 1000.0,
                "Third should be p1, s95.5, v1000");
}

void register_multikey_tests(void) {
    RUN_TEST(test_multikey_basic_two_keys);
    RUN_TEST(test_multikey_three_keys);
    RUN_TEST(test_multikey_ascending_order);
    RUN_TEST(test_multikey_indexed);
    RUN_TEST(test_multikey_single_key);
    RUN_TEST(test_multikey_large_array);
    RUN_TEST(test_multikey_stability);
    RUN_TEST(test_multikey_empty_and_single);
    RUN_TEST(test_multikey_invalid_args);
    RUN_TEST(test_multikey_all_equal);
    RUN_TEST(test_multikey_mixed_types);
}
