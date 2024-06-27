
/*
    File Type   : C Test Source
    Description : Memory Allocator
*/

#include "alloc.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_alloc_append_stratBuddy(void) {
    int item;
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_BUDDY);

    TEST_ASSERT_NULL(alloc_index(block, 0, sizeof(int)));
    
    item = 10;
    alloc_append(block, &item, sizeof(int));
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(4, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(4, alloc_getUsed(block));
    TEST_ASSERT_EQUAL_INT(10, *(int *)alloc_index(block, 0, sizeof(int)));

    item = 20;
    alloc_append(block, &item, sizeof(int));
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(8, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(8, alloc_getUsed(block));
    TEST_ASSERT_EQUAL_INT(10, *(int *)alloc_index(block, 0, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(20, *(int *)alloc_index(block, 1, sizeof(int)));

    item = 30;
    alloc_append(block, &item, sizeof(int));
    TEST_ASSERT_EQUAL_INT(4, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(16, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(12, alloc_getUsed(block));
    TEST_ASSERT_EQUAL_INT(10, *(int *)alloc_index(block, 0, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(20, *(int *)alloc_index(block, 1, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(30, *(int *)alloc_index(block, 2, sizeof(int)));

    alloc_free(block);
}

void test_alloc_append_stratDynamic(void) {
    int item;
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    TEST_ASSERT_NULL(alloc_index(block, 0, sizeof(int)));
    
    item = 10;
    alloc_append(block, &item, sizeof(int));
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(4, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(4, alloc_getUsed(block));
    TEST_ASSERT_EQUAL_INT(10, *(int *)alloc_index(block, 0, sizeof(int)));

    item = 20;
    alloc_append(block, &item, sizeof(int));
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(8, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(8, alloc_getUsed(block));
    TEST_ASSERT_EQUAL_INT(10, *(int *)alloc_index(block, 0, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(20, *(int *)alloc_index(block, 1, sizeof(int)));

    item = 30;
    alloc_append(block, &item, sizeof(int));
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(12, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(12, alloc_getUsed(block));
    TEST_ASSERT_EQUAL_INT(10, *(int *)alloc_index(block, 0, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(20, *(int *)alloc_index(block, 1, sizeof(int)));
    TEST_ASSERT_EQUAL_INT(30, *(int *)alloc_index(block, 2, sizeof(int)));

    alloc_free(block);
}

void test_alloc_new_stratBuddy(void) {
    AllocBlock *block;

    block = alloc_new(0, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_BUDDY, alloc_getStrat(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));
    alloc_free(block);

    block = alloc_new(10, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    TEST_ASSERT_EQUAL_INT(16, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(16, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_BUDDY, alloc_getStrat(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));
    alloc_free(block);
}

void test_alloc_new_stratDynamic(void) {
    AllocBlock *block;

    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));
    alloc_free(block);

    block = alloc_new(10, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    TEST_ASSERT_EQUAL_INT(10, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(10, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));
    alloc_free(block);
}

void test_alloc_resize_stratBuddy(void) {
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_BUDDY);

    alloc_resize(block, 10);
    TEST_ASSERT_EQUAL_INT(16, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(16, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 20);
    TEST_ASSERT_EQUAL_INT(32, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(32, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 5);
    TEST_ASSERT_EQUAL_INT(8, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(8, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 0);
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));
    TEST_ASSERT_NULL(alloc_getBlock(block));

    alloc_free(block);

    // TODO - Test resize below value of used
}

void test_alloc_resize_stratDynamic(void) {
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    alloc_resize(block, 10);
    TEST_ASSERT_EQUAL_INT(10, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(10, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 20);
    TEST_ASSERT_EQUAL_INT(20, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(20, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 5);
    TEST_ASSERT_EQUAL_INT(5, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(5, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 0);
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));
    TEST_ASSERT_NULL(alloc_getBlock(block));

    alloc_free(block);

    // TODO - Test resize below value of used
}

void test_alloc_setStrat(void) {
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    alloc_setStrat(block, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_EQUAL_INT(0, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_BUDDY, alloc_getStrat(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 10);
    TEST_ASSERT_EQUAL_INT(16, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(16, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_setStrat(block, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_EQUAL_INT(16, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(16, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_resize(block, 20);
    TEST_ASSERT_EQUAL_INT(20, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(20, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_setStrat(block, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_EQUAL_INT(32, alloc_getAvail(block));
    TEST_ASSERT_EQUAL_INT(32, alloc_getSize(block));
    TEST_ASSERT_EQUAL_INT(0, alloc_getUsed(block));

    alloc_free(block);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_alloc_append_stratBuddy);
    RUN_TEST(test_alloc_append_stratDynamic);
    RUN_TEST(test_alloc_new_stratBuddy);
    RUN_TEST(test_alloc_new_stratDynamic);
    RUN_TEST(test_alloc_resize_stratBuddy);
    RUN_TEST(test_alloc_resize_stratDynamic);

    return UNITY_END();
}
