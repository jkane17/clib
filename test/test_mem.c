
/*
    File Type   : C Test Source
    Description : Memory Operations
*/

#include "mem.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_mem_alloc(void) {
    void *mem = mem_alloc(0);
    TEST_ASSERT_NOT_NULL(mem);
    mem = mem_alloc(10);
    TEST_ASSERT_NOT_NULL(mem);
    free(mem);
}

void test_mem_realloc(void) {
    void *mem = NULL;

    mem = mem_realloc(mem, 0);
    TEST_ASSERT_NOT_NULL(mem);
    mem = mem_realloc(mem, 10);
    TEST_ASSERT_NOT_NULL(mem);
    free(mem);

    mem = mem_alloc(10);
    TEST_ASSERT_NOT_NULL(mem);
    mem = mem_realloc(mem, 20);
    TEST_ASSERT_NOT_NULL(mem);
    mem = mem_realloc(mem, 0);
    TEST_ASSERT_NULL(mem);

    free(mem);
}

void test_mem_shift(void) {
    int arr[] = { 1, 2, 3, 4, 5 };
    int exp[] = { 1, 2, 3, 3, 4 };

    mem_shift((void *) arr, 5, sizeof(int), 2, 1);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp, arr, 5);

    exp[2] = 1;
    exp[3] = 2;
    exp[4] = 3;
    mem_shift((void *) arr, 5, sizeof(int), 0, 2);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp, arr, 5);

    exp[1] = 3;
    mem_shift((void *) arr, 5, sizeof(int), 4, -3);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp, arr, 5);

    mem_shift((void *) arr, 5, sizeof(int), 10, 2);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp, arr, 5);

    mem_shift((void *) arr, 5, sizeof(int), 2, 5);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp, arr, 5);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_mem_alloc);
    RUN_TEST(test_mem_realloc);
    RUN_TEST(test_mem_shift);

    return UNITY_END();
}
