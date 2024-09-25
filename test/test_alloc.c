
/*
    File        : test_alloc.c
    Description : Dynamic Memory Allocator Library for managing and resizing 
                  memory blocks, with support for different allocation strategies.
*/

#include "alloc.h"
#include "unity.h"

#define SC sizeof(char)
#define SI sizeof(int)

#define checkSizes(b, used, size) { \
    TEST_ASSERT_NOT_NULL(b); \
    TEST_ASSERT_EQUAL_INT(used, alloc_getUsed(b)); \
    TEST_ASSERT_EQUAL_INT((size) - (used), alloc_getAvail(b)); \
    TEST_ASSERT_EQUAL_INT(size, alloc_getSize(b)); \
    TEST_ASSERT_EQUAL_INT(alloc_getUsed(b) == 0, alloc_isEmpty(b)); \
}

void setUp(void) {}
void tearDown(void) {}

void test_alloc_append_stratBuddy(void) {
    int i;
    AllocBlock *block;
    
    block = alloc_new(0, ALLOC_STRAT_BUDDY);

    // Append to an empty block
    i = 10;
    TEST_ASSERT_TRUE(alloc_append(block, &i, SI));
    checkSizes(block, SI, SI);
    TEST_ASSERT_FALSE(alloc_isEmpty(block));
    TEST_ASSERT_EQUAL_INT(10, *(int*)alloc_index(block, 0));

    // Append to a block with existing data
    i = 20;
    TEST_ASSERT_TRUE(alloc_append(block, &i, SI));
    checkSizes(block, 2 * SI, 2 * SI);
    int exp1[] = { 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 2);

    // Append multiple items
    int arr[] = { 30, 40 };
    TEST_ASSERT_TRUE(alloc_append(block, arr, 2 * SI));
    checkSizes(block, 4 * SI, 4 * SI);
    int exp2[] = { 10, 20, 30, 40 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 4);

    // Append to invoke buddy resize
    i = 50;
    TEST_ASSERT_TRUE(alloc_append(block, &i, SI));
    checkSizes(block, 5 * SI, 8 * SI);
    int exp3[] = { 10, 20, 30, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp3, (int*)alloc_getBlock(block), 5);

    alloc_free(block);
    block = alloc_new(0, ALLOC_STRAT_BUDDY);

    // Append string
    char str1[] = "Hello";
    size_t str1Size = sizeof(str1) - 1; // excluding null terminator
    TEST_ASSERT_TRUE(alloc_append(block, str1, str1Size));
    checkSizes(block, str1Size, 8);
    TEST_ASSERT_FALSE(alloc_isEmpty(block));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello", (char*)alloc_index(block, 0), str1Size);

    // Append another string
    char str2[] = " World";
    size_t str2Size = sizeof(str2) - 1;
    TEST_ASSERT_TRUE(alloc_append(block, str2, str2Size));
    checkSizes(block, str1Size + str2Size, 16 * SC);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World", (char*)alloc_index(block, 0), str1Size + str2Size);

    // Cap the string with the null terminator
    char strEnd[] = "\0";
    TEST_ASSERT_TRUE(alloc_append(block, strEnd, SC));
    checkSizes(block, str1Size + str2Size + SC, 16 * SC);
    TEST_ASSERT_EQUAL_STRING("Hello World", (char*)alloc_index(block, 0));

    alloc_free(block);
    block = NULL;
}

void test_alloc_append_stratDynamic(void) {
    int i;
    AllocBlock *block;
    
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    // Append to an empty block
    i = 10;
    TEST_ASSERT_TRUE(alloc_append(block, &i, SI));
    checkSizes(block, SI, SI);
    TEST_ASSERT_FALSE(alloc_isEmpty(block));
    TEST_ASSERT_EQUAL_INT(10, *(int*)alloc_index(block, 0));

    // Append to a block with existing data
    i = 20;
    TEST_ASSERT_TRUE(alloc_append(block, &i, SI));
    checkSizes(block, 2 * SI, 2 * SI);
    int exp1[] = { 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 2);

    // Append multiple items
    int arr[] = { 30, 40 };
    TEST_ASSERT_TRUE(alloc_append(block, &arr, 2 * SI));
    checkSizes(block, 4 * SI, 4 * SI);
    int exp2[] = { 10, 20, 30, 40 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 4);

    alloc_free(block);
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    // Append string
    char str1[] = "Hello";
    size_t str1Size = sizeof(str1) - 1; // excluding null terminator
    TEST_ASSERT_TRUE(alloc_append(block, str1, str1Size));
    checkSizes(block, str1Size, str1Size);
    TEST_ASSERT_FALSE(alloc_isEmpty(block));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello", (char*)alloc_index(block, 0), str1Size);

    // Append another string
    char str2[] = " World";
    size_t str2Size = sizeof(str2) - 1;
    TEST_ASSERT_TRUE(alloc_append(block, str2, str2Size));
    checkSizes(block, str1Size + str2Size, str1Size + str2Size);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World", (char*)alloc_index(block, 0), str1Size + str2Size);

    // Cap the string with the null terminator
    char strEnd[] = "\0";
    TEST_ASSERT_TRUE(alloc_append(block, strEnd, SC));
    checkSizes(block, str1Size + str2Size + SC, str1Size + str2Size + SC);
    TEST_ASSERT_EQUAL_STRING("Hello World", (char*)alloc_index(block, 0));

    alloc_free(block);
    block = NULL;
}

void test_alloc_clear_stratBuddy(void) {
    int arr1[] = { 1, 2, 3 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_BUDDY);

    // Insert some data into the block
    TEST_ASSERT_TRUE(alloc_append(block, arr1, 3 * SI));
    checkSizes(block, 3 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr1, (int*)alloc_getBlock(block), 3);

    alloc_clear(block);
    checkSizes(block, 0, 4 * SI);
    TEST_ASSERT_TRUE(alloc_isEmpty(block));

    // Check that inserting new data works correctly after clearing
    int arr2[] = { 10, 20 };
    TEST_ASSERT_TRUE(alloc_append(block, arr2, 2 * SI));
    checkSizes(block, 2 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr2, (int*)alloc_getBlock(block), 2);

    alloc_free(block);
}

void test_alloc_clear_stratDynamic(void) {
    int arr1[] = { 1, 2, 3, 4 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    // Insert some data into the block
    TEST_ASSERT_TRUE(alloc_append(block, arr1, 4 * SI));
    checkSizes(block, 4 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr1, (int*)alloc_getBlock(block), 4);

    alloc_clear(block);
    checkSizes(block, 0, 4 * SI);

    // Check that inserting new data works correctly after clearing
    int arr2[] = { 10, 20, 30 };
    TEST_ASSERT_TRUE(alloc_append(block, arr2, 3 * SI));
    checkSizes(block, 3 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr2, (int*)alloc_getBlock(block), 3);

    alloc_free(block);
}

void test_alloc_copy_stratBuddy(void) {
    int arr[] = { 1, 2, 3, 4, 5 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_BUDDY);

    // Append some items to the block
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    checkSizes(block, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    AllocBlock *copy = alloc_copy(block);

    // Validate that the copied block has the same properties
    TEST_ASSERT_NOT_NULL(copy);
    checkSizes(copy, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(copy), 5);

    // Modify the copied block to ensure it's independent from the original
    int new_value = 10;
    alloc_setAt(copy, &new_value, 0, SI);
    TEST_ASSERT_EQUAL_INT(10, *(int*)alloc_index(copy, 0)); // Copy should have new value
    TEST_ASSERT_EQUAL_INT(1, *(int*)alloc_index(block, 0)); // Original should remain unchanged

    alloc_free(block);
    alloc_free(copy);
}

void test_alloc_copy_stratDynamic(void) {
    int arr[] = { 1, 2, 3, 4, 5 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    // Append some items to the block
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    checkSizes(block, 5 * SI, 5 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    AllocBlock *copy = alloc_copy(block);

    // Validate that the copied block has the same properties
    TEST_ASSERT_NOT_NULL(copy);
    checkSizes(copy, 5 * SI, 5 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(copy), 5);

    // Modify the copied block to ensure it's independent from the original
    int new_value = 10;
    TEST_ASSERT_TRUE(alloc_setAt(copy, &new_value, 0, SI));
    TEST_ASSERT_EQUAL_INT(10, *(int*)alloc_index(copy, 0)); // Copy should have new value
    TEST_ASSERT_EQUAL_INT(1, *(int*)alloc_index(block, 0)); // Original should remain unchanged

    alloc_free(block);
    alloc_free(copy);
}

void test_alloc_index(void) {
    AllocBlock *block;
    char data[] = "123456789";
    
    // Test with a valid index
    block = alloc_new(10, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_TRUE(alloc_insert(block, data, 0, 5 * SC)); // Insert "12345"
    char *p = (char*)alloc_index(block, 2);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_CHAR('3', *p);

    // Test with an index out of bounds (greater than used)
    p = (char*)alloc_index(block, 6);
    TEST_ASSERT_NULL(p);

    // Test with an index at the boundary of used
    p = (char*)alloc_index(block, 4);
    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_CHAR('5', *p);
    
    alloc_free(block);

    // Test with NULL block
    p = (char*)alloc_index(NULL, 2);
    TEST_ASSERT_NULL(p);
}

void test_alloc_insert_stratBuddy(void) {
    int i;
    AllocBlock *block;
    
    block = alloc_new(0, ALLOC_STRAT_BUDDY);

    // Insert at beginning (empty block)
    i = 10;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, 0, SI));
    checkSizes(block, SI, SI);
    TEST_ASSERT_EQUAL_INT(10, *(int*)alloc_index(block, 0));

    // Insert at end (appending)
    i = 20;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, SI, SI));
    checkSizes(block, 2 * SI, 2 * SI);
    int exp1[] = { 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 2);
    
    // Insert in the middle (shift elements)
    i = 30;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, SI, SI));
    checkSizes(block, 3 * SI, 4 * SI);
    int exp2[] = { 10, 30, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 3);

    // Insert at beginning (non-empty block)
    i = 40;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, 0, SI));
    checkSizes(block, 4 * SI, 4 * SI);
    int exp3[] = { 40, 10, 30, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp3, (int*)alloc_getBlock(block), 4);

    // Insert at an out-of-bounds index (should do nothing)
    i = 50;
    TEST_ASSERT_FALSE(alloc_insert(block, &i, 5 * SI, SI));
    checkSizes(block, 4 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp3, (int*)alloc_getBlock(block), 4);

    alloc_free(block);
    block = alloc_new(0, ALLOC_STRAT_BUDDY);
    size_t newSize;
 
    // Insert string at beginning (empty block)
    char str1[] = "Hello World";
    size_t str1Size = sizeof(str1) - 1; // excluding null terminator
    newSize = str1Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str1, 0, str1Size)); 
    checkSizes(block, newSize, 16 * SC);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(str1, (char*)alloc_index(block, 0), newSize);

    // Insert string at end (appending)
    char str2[] = "Another String";
    size_t str2Size = sizeof(str2) - 1;
    newSize += str2Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str2, alloc_getUsed(block), str2Size));
    checkSizes(block, newSize, 32 * SC);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "Hello WorldAnother String",
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Insert string in the middle (shift elements)
    char str3[] = "TEST";
    size_t str3Size = sizeof(str3) - 1;
    newSize += str3Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str3, 5 * SC, str3Size));
    checkSizes(block, newSize, 32 * SC);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "HelloTEST WorldAnother String", 
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Insert at beginning (non-empty block)
    char str4[] = "BEGIN ";
    size_t str4Size = sizeof(str4) - 1;
    newSize += str4Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str4, 0, str4Size));
    checkSizes(block, newSize, 64 * SC);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "BEGIN HelloTEST WorldAnother String", 
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Insert string at an out-of-bounds index (should do nothing)
    char str5[] = "OutOfBounds";
    size_t str5Size = sizeof(str5);
    TEST_ASSERT_FALSE(alloc_insert(block, str5, 100 * SC, str5Size));
    checkSizes(block, newSize, 64 * SC);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "BEGIN HelloTEST WorldAnother String", 
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Cap the string with the null terminator
    char strEnd[] = "\0";
    TEST_ASSERT_TRUE(alloc_insert(block, strEnd, alloc_getUsed(block), SC));
    checkSizes(block, newSize + SC, 64 * SC);
    TEST_ASSERT_EQUAL_STRING("BEGIN HelloTEST WorldAnother String", (char*)alloc_index(block, 0));

    alloc_free(block);
    block = NULL;
}

void test_alloc_insert_stratDynamic(void) {
    int i;
    AllocBlock *block;
    
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    // Insert at beginning (empty block)
    i = 10;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, 0, SI));
    checkSizes(block, SI, SI);
    TEST_ASSERT_EQUAL_INT(10, *(int*)alloc_index(block, 0));

    // Insert at end (appending)
    i = 20;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, SI, SI));
    checkSizes(block, 2 * SI, 2 * SI);
    int exp1[] = { 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 2);
    
    // Insert in the middle (shift elements)
    i = 30;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, SI, SI));
    checkSizes(block, 3 * SI, 3 * SI);
    int exp2[] = { 10, 30, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 3);

    // Insert at beginning (non-empty block)
    i = 40;
    TEST_ASSERT_TRUE(alloc_insert(block, &i, 0, SI));
    checkSizes(block, 4 * SI, 4 * SI);
    int exp3[] = { 40, 10, 30, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp3, (int*)alloc_getBlock(block), 4);

    // Insert at an out-of-bounds index (should do nothing)
    i = 50;
    TEST_ASSERT_FALSE(alloc_insert(block, &i, 5 * SI, SI));
    checkSizes(block, 4 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp3, (int*)alloc_getBlock(block), 4);

    alloc_free(block);
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);
    size_t newSize;
 
    // Insert string at beginning (empty block)
    char str1[] = "Hello World";
    size_t str1Size = sizeof(str1) - 1; // excluding null terminator
    newSize = str1Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str1, 0, str1Size)); 
    checkSizes(block, newSize, newSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(str1, (char*)alloc_index(block, 0), newSize);

    // Insert string at end (appending)
    char str2[] = "Another String";
    size_t str2Size = sizeof(str2) - 1;
    newSize += str2Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str2, alloc_getSize(block), str2Size));
    checkSizes(block, newSize, newSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "Hello WorldAnother String", 
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Insert string in the middle (shift elements)
    char str3[] = "TEST";
    size_t str3Size = sizeof(str3) - 1;
    newSize += str3Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str3, 5 * SC, str3Size));
    checkSizes(block, newSize, newSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "HelloTEST WorldAnother String", 
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Insert at beginning (non-empty block)
    char str4[] = "BEGIN ";
    size_t str4Size = sizeof(str4) - 1;
    newSize += str4Size;
    TEST_ASSERT_TRUE(alloc_insert(block, str4, 0, str4Size));
    checkSizes(block, newSize, newSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "BEGIN HelloTEST WorldAnother String", 
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Insert string at an out-of-bounds index (should do nothing)
    char str5[] = "OutOfBounds";
    size_t str5Size = sizeof(str5);
    TEST_ASSERT_FALSE(alloc_insert(block, str5, 100 * SC, str5Size));
    checkSizes(block, newSize, newSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "BEGIN HelloTEST WorldAnother String", 
        (char*)alloc_index(block, 0), 
        newSize
    );

    // Cap the string with the null terminator
    char strEnd[] = "\0";
    TEST_ASSERT_TRUE(alloc_insert(block, strEnd, alloc_getSize(block), SC));
    checkSizes(block, newSize + SC, newSize + SC);
    TEST_ASSERT_EQUAL_STRING("BEGIN HelloTEST WorldAnother String", (char*)alloc_index(block, 0));

    alloc_free(block);
    block = NULL;
}

void test_alloc_new_stratBuddy(void) {
    AllocBlock *block;

    // Test with zero size
    block = alloc_new(0, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 0);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_BUDDY, alloc_getStrat(block));
    alloc_free(block);

    // Test with non-zero size
    block = alloc_new(10, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 16 * SC);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_BUDDY, alloc_getStrat(block));
    alloc_free(block);

    // Test with minimum non-zero size (e.g., 1 byte)
    block = alloc_new(1, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 1 * SC);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));
    alloc_free(block);

    // Test with a very large size (edge case)
    block = alloc_new(SIZE_MAX, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NULL(block); // Allocation should fail
}

void test_alloc_new_stratDynamic(void) {
    AllocBlock *block;

    // Test with zero size
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 0);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));
    alloc_free(block);

    // Test with non-zero size
    block = alloc_new(10, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 10 * SC);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));
    alloc_free(block);

    // Test with minimum non-zero size (e.g., 1 byte)
    block = alloc_new(1, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 1 * SC);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));
    alloc_free(block);

    // Test with a very large size (edge case)
    block = alloc_new(SIZE_MAX, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NULL(block); // Allocation should fail
}

void test_alloc_remove_stratBuddy(void) {
    int arr[] = { 10, 20, 30, 40, 50 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_BUDDY);

    // Append some items to the block
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    checkSizes(block, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    // Remove middle item (index 2, value 30)
    TEST_ASSERT_TRUE(alloc_remove(block, 2 * SI, SI));
    checkSizes(block, 4 * SI, 4 * SI);
    int exp1[] = { 10, 20, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 4);

    // Remove first item (index 0, value 10)
    TEST_ASSERT_TRUE(alloc_remove(block, 0, SI));
    checkSizes(block, 3 * SI, 4 * SI);
    int exp2[] = { 20, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 3);

    // Remove last two items (index 1, values 40 and 50)
    TEST_ASSERT_TRUE(alloc_remove(block, SI, 2 * SI));
    checkSizes(block, SI, SI);
    TEST_ASSERT_EQUAL_INT(20, *(int*)alloc_index(block, 0));

    // Remove the last remaining item (index 0, value 20)
    TEST_ASSERT_TRUE(alloc_remove(block, 0, SI));
    checkSizes(block, 0, 0);

    // Reset
    alloc_free(block);
    block = alloc_new(0, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));

    // Removing out of bounds should do nothing
    TEST_ASSERT_FALSE(alloc_remove(block, 10 * SI, SI));
    checkSizes(block, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    // Removing more items than are in block should do nothing
    TEST_ASSERT_FALSE(alloc_remove(block, 0, 6 * SI));
    checkSizes(block, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    // Full remove
    TEST_ASSERT_TRUE(alloc_remove(block, 0, 5 * SI));
    checkSizes(block, 0, 0);

    alloc_free(block);
}

void test_alloc_remove_stratDynamic(void) {
    int arr[] = { 10, 20, 30, 40, 50 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    // Append some items to the block
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    checkSizes(block, 5 * SI, 5 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    // Remove middle item (index 2, value 30)
    TEST_ASSERT_TRUE(alloc_remove(block, 2 * SI, SI));
    checkSizes(block, 4 * SI, 4 * SI);
    int exp1[] = { 10, 20, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 4);

    // Remove first item (index 0, value 10)
    TEST_ASSERT_TRUE(alloc_remove(block, 0, SI));
    checkSizes(block, 3 * SI, 3 * SI);
    int exp2[] = { 20, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 3);

    // Remove last two items (index 1, values 40 and 50)
    TEST_ASSERT_TRUE(alloc_remove(block, SI, 2 * SI));
    checkSizes(block, SI, SI);
    TEST_ASSERT_EQUAL_INT(20, *(int*)alloc_index(block, 0));

    // Remove the last remaining item (index 0, value 20)
    TEST_ASSERT_TRUE(alloc_remove(block, 0, SI));
    checkSizes(block, 0, 0);
    TEST_ASSERT_TRUE(alloc_isEmpty(block));

    // Reset
    alloc_free(block);
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));

    // Removing out of bounds should do nothing
    TEST_ASSERT_FALSE(alloc_remove(block, 10 * SI, SI));
    checkSizes(block, 5 * SI, 5 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    // Removing more items than are in block should do nothing
    TEST_ASSERT_FALSE(alloc_remove(block, 0, 6 * SI));
    checkSizes(block, 5 * SI, 5 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(block), 5);

    // Full remove
    TEST_ASSERT_TRUE(alloc_remove(block, 0, 5 * SI));
    checkSizes(block, 0, 0);

    alloc_free(block);
}

void test_alloc_resize_stratBuddy(void) {
    AllocBlock *block;

    // Test with initial zero size
    block = alloc_new(0, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 0);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_BUDDY, alloc_getStrat(block));

    // Resize to 10 bytes (should round up to next power of 2)
    TEST_ASSERT_TRUE(alloc_resize(block, 10));
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 16);

    // Resize to 20 bytes (should round up to next power of 2)
    TEST_ASSERT_TRUE(alloc_resize(block, 20));
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 32);

    // Resize to 5 bytes (should round up to next power of 2)
    TEST_ASSERT_TRUE(alloc_resize(block, 5));
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 8);

    // Resize to 0 bytes
    TEST_ASSERT_TRUE(alloc_resize(block, 0));
    TEST_ASSERT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 0);

    alloc_free(block);

    // TODO - Test resize below value of used
}

void test_alloc_resize_stratDynamic(void) {
    AllocBlock *block;

    // Test with initial zero size
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(block);
    TEST_ASSERT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 0);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));

    // Resize to 10 bytes
    TEST_ASSERT_TRUE(alloc_resize(block, 10));
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 10);

    // Resize to 20 bytes
    TEST_ASSERT_TRUE(alloc_resize(block, 20));
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 20);

    // Resize to 5 bytes
    alloc_resize(block, 5);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 5);

    // Resize to 0 bytes
    TEST_ASSERT_TRUE(alloc_resize(block, 0));
    TEST_ASSERT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 0);

    alloc_free(block);

    // TODO - Test resize below value of used
}

void test_alloc_setAt_stratBuddy(void) {
    int arr1[] = { 1, 2, 3 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_BUDDY);

    // Set items in an empty block
    TEST_ASSERT_TRUE(alloc_setAt(block, arr1, 0, 3 * SI));
    checkSizes(block, 3 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr1, (int*)alloc_getBlock(block), 3);

    // Set items at an index within the current size
    int arr2[] = { 10, 20 };
    TEST_ASSERT_TRUE(alloc_setAt(block, arr2, SI, 2 * SI));
    checkSizes(block, 3 * SI, 4 * SI);
    int exp1[] = { 1, 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 3);

    // Set items at an index that requires expanding the block
    int arr3[] = { 30, 40, 50 };
    TEST_ASSERT_TRUE(alloc_setAt(block, arr3, 2 * SI, 3 * SI));
    checkSizes(block, 5 * SI, 8 * SI);
    int exp2[] = { 1, 10, 30, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 4);

    // Set past last index should do nothing
    int arr4[] = { 100, 200 };
    TEST_ASSERT_FALSE(alloc_setAt(block, arr4, 10 * SI, 2 * SI));
    checkSizes(block, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 4);

    alloc_free(block);
}

void test_alloc_setAt_stratDynamic(void) {
    int arr1[] = { 1, 2, 3, 4 };
    AllocBlock *block = alloc_new(0, ALLOC_STRAT_DYNAMIC);

    // Set items in an empty block
    TEST_ASSERT_TRUE(alloc_setAt(block, arr1, 0, 4 * SI));
    checkSizes(block, 4 * SI, 4 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr1, (int*)alloc_getBlock(block), 4);

    // Set items at an index within the current size
    int arr2[] = { 10, 20 };
    TEST_ASSERT_TRUE(alloc_setAt(block, arr2, 2 * SI, 2 * SI));
    checkSizes(block, 4 * SI, 4 * SI);
    int exp1[] = { 1, 2, 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)alloc_getBlock(block), 4);

    // Set items at an index that requires expanding the block
    int arr3[] = { 30, 40, 50 };
    TEST_ASSERT_TRUE(alloc_setAt(block, arr3, 3 * SI, 3 * SI));
    checkSizes(block, 6 * SI, 6 * SI);
    int exp2[] = { 1, 2, 10, 30, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 6);

    // Set past last index should do nothing
    int arr4[] = { 100, 200 };
    TEST_ASSERT_FALSE(alloc_setAt(block, arr4, 10 * SI, 2 * SI));
    checkSizes(block, 6 * SI, 6 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)alloc_getBlock(block), 6);

    alloc_free(block);
}

void test_alloc_setStrat(void) {
    AllocBlock *block;

    // Test with initial dynamic strategy and zero size
    block = alloc_new(0, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(block); 
    TEST_ASSERT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 0);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));

    // Change to buddy strategy with zero size
    alloc_setStrat(block, ALLOC_STRAT_BUDDY);
    checkSizes(block, 0, 0);
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_BUDDY, alloc_getStrat(block));

    // Resize the block with buddy strategy
    TEST_ASSERT_TRUE(alloc_resize(block, 10));
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 16); // Buddy strategy rounds up to 16

    // Switch back to dynamic strategy (should not change size)
    alloc_setStrat(block, ALLOC_STRAT_DYNAMIC);
    checkSizes(block, 0, 16);

    // Resize the block with dynamic strategy
    TEST_ASSERT_TRUE(alloc_resize(block, 20));
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 20); // No rounding needed for dynamic

    // Set an invalid strategy (should be ignored)
    alloc_setStrat(block, 9999); // Invalid strategy
    TEST_ASSERT_EQUAL_INT(ALLOC_STRAT_DYNAMIC, alloc_getStrat(block));
    checkSizes(block, 0, 20);

    // Change back to buddy strategy and resize again
    alloc_setStrat(block, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(alloc_getBlock(block));
    checkSizes(block, 0, 32); // Buddy strategy rounds up

    alloc_free(block);
}

void test_alloc_split_stratBuddy(void) {
    int arr[] = { 10, 20, 30, 40, 50 };
    AllocBlock *block = alloc_new(5 * SI, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));

    AllocBlock *leftBlock = NULL;
    AllocBlock *rightBlock = NULL;

    // Split at index 2 (should result in two blocks: left with [10, 20], right with [30, 40, 50])
    TEST_ASSERT_TRUE(alloc_split(block, &leftBlock, &rightBlock, 2 * SI));
    block = NULL;

    // Validate left block
    checkSizes(leftBlock, 2 * SI, 2 * SI);
    int expLeft[] = { 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(expLeft, (int*)alloc_getBlock(leftBlock), 2);

    // Validate right block
    checkSizes(rightBlock, 3 * SI, 4 * SI);
    int expRight[] = { 30, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(expRight, (int*)alloc_getBlock(rightBlock), 3);

    alloc_free(leftBlock);
    alloc_free(rightBlock);
    leftBlock = rightBlock = NULL;

    // Split at index 0 (left block should be empty, right should have all items)
    block = alloc_new(5 * SI, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    TEST_ASSERT_TRUE(alloc_split(block, &leftBlock, &rightBlock, 0));
    block = NULL;

    checkSizes(leftBlock, 0, 0); // Left block should be empty

    // Right block should contain all the elements
    checkSizes(rightBlock, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(rightBlock), 5);

    // Free the blocks
    alloc_free(leftBlock);
    alloc_free(rightBlock);
    leftBlock = rightBlock = NULL;

    // Split at index 5 (left should have all, right should be empty)
    block = alloc_new(5 * SI, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    TEST_ASSERT_TRUE(alloc_split(block, &leftBlock, &rightBlock, 5 * SI));
    block = NULL;

    // Left block should contain all the elements
    checkSizes(leftBlock, 5 * SI, 8 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(leftBlock), 5);

    checkSizes(rightBlock, 0, 0); // Right block should be empty

    // Free the blocks
    alloc_free(leftBlock);
    alloc_free(rightBlock);
    leftBlock = rightBlock = NULL;

    // Attempt an invalid split (out of bounds)
    block = alloc_new(5 * SI, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    TEST_ASSERT_FALSE(alloc_split(block, &leftBlock, &rightBlock, 10 * SI));

    // Ensure leftBlock and rightBlock remain NULL (nothing was split)
    TEST_ASSERT_NULL(leftBlock);
    TEST_ASSERT_NULL(rightBlock);

    alloc_free(block);
}

void test_alloc_split_stratDynamic(void) {
    int arr[] = { 10, 20, 30, 40, 50 };
    AllocBlock *block = alloc_new(5 * SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));

    AllocBlock *leftBlock = NULL;
    AllocBlock *rightBlock = NULL;

    // Split at index 2 (should result in two blocks: left with [10, 20], right with [30, 40, 50])
    TEST_ASSERT_TRUE(alloc_split(block, &leftBlock, &rightBlock, 2 * SI));
    block = NULL;

    // Validate left block
    checkSizes(leftBlock, 2 * SI, 2 * SI);
    int expLeft[] = { 10, 20 };
    TEST_ASSERT_EQUAL_INT_ARRAY(expLeft, (int*)alloc_getBlock(leftBlock), 2);

    // Validate right block
    checkSizes(rightBlock, 3 * SI, 3 * SI);
    int expRight[] = { 30, 40, 50 };
    TEST_ASSERT_EQUAL_INT_ARRAY(expRight, (int*)alloc_getBlock(rightBlock), 3);

    alloc_free(leftBlock);
    alloc_free(rightBlock);
    leftBlock = rightBlock = NULL;

    // Split at index 0 (left block should be empty, right should have all items)
    block = alloc_new(5 * SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    TEST_ASSERT_TRUE(alloc_split(block, &leftBlock, &rightBlock, 0));
    block = NULL;

    checkSizes(leftBlock, 0, 0); // Left block should be empty

    // Right block should contain all the elements
    checkSizes(rightBlock, 5 * SI, 5 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(rightBlock), 5);

    // Free the blocks
    alloc_free(leftBlock);
    alloc_free(rightBlock);
    leftBlock = rightBlock = NULL;

    // Split at index 5 (left should have all, right should be empty)
    block = alloc_new(5 * SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    TEST_ASSERT_TRUE(alloc_split(block, &leftBlock, &rightBlock, 5 * SI));
    block = NULL;

    // Left block should contain all the elements
    checkSizes(leftBlock, 5 * SI, 5 * SI);
    TEST_ASSERT_EQUAL_INT_ARRAY(arr, (int*)alloc_getBlock(leftBlock), 5);

    checkSizes(rightBlock, 0, 0); // Right block should be empty

    // Free the blocks
    alloc_free(leftBlock);
    alloc_free(rightBlock);
    leftBlock = rightBlock = NULL;

    // Attempt an invalid split (out of bounds)
    block = alloc_new(5, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_TRUE(alloc_append(block, arr, 5 * SI));
    TEST_ASSERT_FALSE(alloc_split(block, &leftBlock, &rightBlock, 10 * SI));

    // Ensure leftBlock and rightBlock remain NULL (nothing was split)
    TEST_ASSERT_NULL(leftBlock);
    TEST_ASSERT_NULL(rightBlock);

    alloc_free(block);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_alloc_append_stratBuddy);
    RUN_TEST(test_alloc_append_stratDynamic);
    RUN_TEST(test_alloc_clear_stratBuddy);
    RUN_TEST(test_alloc_clear_stratDynamic);
    RUN_TEST(test_alloc_copy_stratBuddy);
    RUN_TEST(test_alloc_copy_stratDynamic);
    RUN_TEST(test_alloc_index);
    RUN_TEST(test_alloc_insert_stratBuddy);
    RUN_TEST(test_alloc_insert_stratDynamic);
    RUN_TEST(test_alloc_new_stratBuddy);
    RUN_TEST(test_alloc_new_stratDynamic);
    RUN_TEST(test_alloc_remove_stratBuddy);
    RUN_TEST(test_alloc_remove_stratDynamic);
    RUN_TEST(test_alloc_resize_stratBuddy);
    RUN_TEST(test_alloc_resize_stratDynamic);
    RUN_TEST(test_alloc_setAt_stratBuddy);
    RUN_TEST(test_alloc_setAt_stratDynamic);
    RUN_TEST(test_alloc_setStrat);
    RUN_TEST(test_alloc_split_stratBuddy);
    RUN_TEST(test_alloc_split_stratDynamic);

    return UNITY_END();
}
