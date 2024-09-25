
/*
    File        : test_mem.c
    Description : Dynamic memory management operations.
*/

#include "mem.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_mem_alloc(void) {
    // Test zero-size allocation
    void *mem = mem_alloc(0);
    TEST_ASSERT_NULL(mem);

    // Test normal allocation
    mem = mem_alloc(10);
    TEST_ASSERT_NOT_NULL(mem);
    free(mem);
}

void test_mem_calloc(void) {
    // Test zero-size allocation
    void *mem = mem_calloc(0, 0);
    TEST_ASSERT_NULL(mem);

    mem = mem_calloc(10, 0);
    TEST_ASSERT_NULL(mem);

    mem = mem_calloc(0, 10);
    TEST_ASSERT_NULL(mem);

    // Test normal allocation and zero initialisation
    mem = mem_calloc(5, sizeof(int));
    TEST_ASSERT_NOT_NULL(mem);
    for (int i = 0; i < 5; i++) TEST_ASSERT_EQUAL_INT(0, ((int*)mem)[i]); 
    free(mem);
}

void test_mem_realloc(void) {
    void *mem = NULL;

    // Test realloc with NULL and size 0
    mem = mem_realloc(mem, 0);
    TEST_ASSERT_NULL(mem);

    // Test realloc with NULL and valid size (acts like malloc)
    mem = mem_realloc(NULL, 10);
    TEST_ASSERT_NOT_NULL(mem);
    free(mem);

    // Test realloc with a valid block
    mem = mem_alloc(10);
    TEST_ASSERT_NOT_NULL(mem);
    mem = mem_realloc(mem, 20);
    TEST_ASSERT_NOT_NULL(mem);
    free(mem);

    // Test realloc with size 0 on a valid block
    mem = mem_alloc(10);
    TEST_ASSERT_NOT_NULL(mem);
    mem = mem_realloc(mem, 0);
    TEST_ASSERT_NULL(mem);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_mem_alloc);
    RUN_TEST(test_mem_calloc);
    RUN_TEST(test_mem_realloc);

    return UNITY_END();
}
