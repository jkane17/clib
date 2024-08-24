
/*
    File Type   : C Test Source
    Description : Memory Allocator.
*/

#include "darr.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_darr_insert(void) {
    DArr *d = darr_new(10, sizeof(int), ALLOC_STRAT_DYNAMIC);

    // int i0 = 0;
    // parray_insert(a, &i0, 0);
    // TEST_ASSERT_FALSE(parray_isEmpty(a));
    // TEST_ASSERT_EQUAL_INT(0, *(int *) parray_index(a, 0));
    // TEST_ASSERT_EQUAL_INT(0, *(int *) parray_first(a));
    // TEST_ASSERT_EQUAL_INT(0, *(int *) parray_last(a));
    // TEST_ASSERT_EQUAL_INT(1, parray_len(a));
    // TEST_ASSERT_EQUAL_INT(3, a->size);

    // int i1 = 1;
    // parray_insert(a, &i1, 1);
    // TEST_ASSERT_EQUAL_INT(1, *(int *) parray_index(a, 1));
    // TEST_ASSERT_EQUAL_INT(0, *(int *) parray_first(a));
    // TEST_ASSERT_EQUAL_INT(1, *(int *) parray_last(a));
    // TEST_ASSERT_EQUAL_INT(2, parray_len(a));
    // TEST_ASSERT_EQUAL_INT(3, a->size);
    
    // int i2 = 2;
    // parray_insert(a, &i2, 1);
    // TEST_ASSERT_EQUAL_INT(2, *(int *) parray_index(a, 1));
    // TEST_ASSERT_EQUAL_INT(0, *(int *) parray_first(a));
    // TEST_ASSERT_EQUAL_INT(1, *(int *) parray_last(a));
    // TEST_ASSERT_EQUAL_INT(3, parray_len(a));
    // TEST_ASSERT_EQUAL_INT(3, a->size);

    // // PArray should resize to accomodate new item
    // int i3 = 3;
    // parray_insert(a, &i3, 3);
    // TEST_ASSERT_EQUAL_INT(3, *(int *) parray_index(a, 3));
    // TEST_ASSERT_EQUAL_INT(0, *(int *) parray_first(a));
    // TEST_ASSERT_EQUAL_INT(3, *(int *) parray_last(a));
    // TEST_ASSERT_EQUAL_INT(4, parray_len(a));
    // TEST_ASSERT_EQUAL_INT(4, a->size);

    // // Insert is past end of parray so should not do anything
    // int i5 = 5;
    // parray_insert(a, &i5, 5);
    // TEST_ASSERT_EQUAL_INT(2, *(int *) parray_index(a, 1));
    // TEST_ASSERT_EQUAL_INT(0, *(int *) parray_first(a));
    // TEST_ASSERT_EQUAL_INT(3, *(int *) parray_last(a));
    // TEST_ASSERT_EQUAL_INT(4, parray_len(a));
    // TEST_ASSERT_EQUAL_INT(4, a->size);

    darr_free(d);
    d = NULL;
}

void test_darr_new(void) {
    DArr *d = darr_new(10, sizeof(int), ALLOC_STRAT_DYNAMIC);
    
    TEST_ASSERT_NOT_NULL(d);
    TEST_ASSERT_NOT_NULL(d->block);
    TEST_ASSERT_EQUAL_INT(sizeof(int), d->itemSize);
    TEST_ASSERT_TRUE(darr_isEmpty(d));
    TEST_ASSERT_EQUAL_INT(0, darr_len(d));
    
    darr_free(d);
    d = NULL;
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_darr_insert);
    RUN_TEST(test_darr_new);

    return UNITY_END();
}
