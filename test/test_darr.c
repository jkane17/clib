
/*
    File        : test_darr.c
    Description : Dynamic Array Library for managing and resizing arrays of any single type.
*/

#include "darr.h"
#include "unity.h"

#define SC sizeof(char)
#define SI sizeof(int)

void setUp(void) {}
void tearDown(void) {}

#define checkValues(d, exp, n) { \
    TEST_ASSERT_EQUAL_INT(n, darr_len(d)); \
    if (n > 0) \
        TEST_ASSERT_FALSE(darr_isEmpty(darr)); \
    else \
        TEST_ASSERT_TRUE(darr_isEmpty(darr)); \
    TEST_ASSERT_EQUAL_INT(exp[0], *(int*)darr_first(d)); \
    TEST_ASSERT_EQUAL_INT(exp[n - 1], *(int*)darr_last(d)); \
    TEST_ASSERT_EQUAL_INT_ARRAY(exp, (int*)darr_index(darr, 0), n); \
}

void test_darr_append(void) {
    DArr *darr;

    // Initialize dynamic array
    darr = darr_new(0, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Append to an empty dynamic array
    int i = 10;
    TEST_ASSERT_TRUE(darr_append(darr, &i, 1));
    int exp1[] = { 10 };
    checkValues(darr, exp1, 1);

    // Append to a dynamic array with existing data
    i = 20;
    TEST_ASSERT_TRUE(darr_append(darr, &i, 1));
    int exp2[] = { 10, 20 };
    checkValues(darr, exp2, 2);

    // Append multiple items at once
    int arr[] = { 30, 40 };
    TEST_ASSERT_TRUE(darr_append(darr, arr, 2));
    int exp3[] = { 10, 20, 30, 40 };
    checkValues(darr, exp3, 4);

    darr_free(darr);

    // Test with strings
    darr = darr_new(0, SC, ALLOC_STRAT_DYNAMIC);

    // Append first string
    char str1[] = "Hello";
    size_t str1Size = sizeof(str1) - 1; // Exclude null terminator
    TEST_ASSERT_TRUE(darr_append(darr, str1, str1Size));
    TEST_ASSERT_EQUAL_INT(str1Size, darr_len(darr));
    TEST_ASSERT_FALSE(darr_isEmpty(darr));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello", (char*)darr_index(darr, 0), str1Size);

    // Append second string
    char str2[] = " World";
    size_t str2Size = sizeof(str2) - 1;
    TEST_ASSERT_TRUE(darr_append(darr, str2, str2Size));
    TEST_ASSERT_EQUAL_INT(str1Size + str2Size, darr_len(darr));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World", (char*)darr_index(darr, 0), str1Size + str2Size);

    // Append null terminator to cap the string
    char strEnd[] = "\0";
    TEST_ASSERT_TRUE(darr_append(darr, strEnd, SC));
    TEST_ASSERT_EQUAL_STRING("Hello World", (char*)darr_index(darr, 0));

    darr_free(darr);
}

void test_darr_clear(void) {
    DArr *darr = darr_new(5, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert items into the DArr
    int data1[] = { 10, 20, 30 };
    TEST_ASSERT_TRUE(darr_insert(darr, data1, 0, 3));
    checkValues(darr, data1, 3);

    // Clear the DArr
    darr_clear(darr);
    TEST_ASSERT_EQUAL_INT(0, darr_len(darr));
    TEST_ASSERT_TRUE(darr_isEmpty(darr));

    // Check that memory is still allocated (not freed)
    TEST_ASSERT_NOT_NULL(darr->block);
    TEST_ASSERT_EQUAL_INT(5 * SI, alloc_getSize(darr->block)); 

    // Add more data after clearing
    int data2[] = { 40, 50 };
    TEST_ASSERT_TRUE(darr_insert(darr, data2, 0, 2));
    int exp[] = { 40, 50 };
    checkValues(darr, exp, 2);

    darr_free(darr);
}

void test_darr_copy(void) {
    DArr *original = darr_new(5, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(original);

    // Insert some items
    int data[] = { 1, 2, 3, 4, 5 };
    TEST_ASSERT_TRUE(darr_insert(original, data, 0, 5));

    // Copy the DArr
    DArr *copy = darr_copy(original);
    TEST_ASSERT_NOT_NULL(copy);
    
    // Check that the copy has the same length and contents
    TEST_ASSERT_EQUAL_INT(darr_len(original), darr_len(copy));
    TEST_ASSERT_EQUAL_INT_ARRAY(data, (int*)darr_index(copy, 0), 5);
    
    // Modify the original and check that the copy remains unchanged
    int newData = 99;
    TEST_ASSERT_TRUE(darr_insert(original, &newData, 0, 1)); // Insert at the start

    // Verify that the copy has not changed
    TEST_ASSERT_EQUAL_INT(6, darr_len(original));
    TEST_ASSERT_EQUAL_INT(5, darr_len(copy)); // The length of the copy should remain 5
    TEST_ASSERT_EQUAL_INT(newData, *(int*)darr_index(original, 0)); // New item in original
    TEST_ASSERT_EQUAL_INT(1, *(int*)darr_index(copy, 0)); // First item in copy remains the same

    // Clean up
    darr_free(original);
    darr_free(copy);
}

void test_darr_expand(void) {
    DArr *darr = darr_new(5, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert 3 items
    int data[] = { 1, 2, 3 };
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 3));
    int exp1[] = { 1, 2, 3 };
    checkValues(darr, exp1, 3);

    // Expand to a larger size
    TEST_ASSERT_TRUE(darr_expand(darr, 10));
    TEST_ASSERT_EQUAL_INT(10, darr_size(darr));
    checkValues(darr, exp1, 3); // Ensure existing data is intact

    // Expand to a smaller size (should fail)
    TEST_ASSERT_FALSE(darr_expand(darr, 2)); // Cannot shrink below current length
    TEST_ASSERT_EQUAL_INT(10, darr_size(darr)); // Size remains unchanged
    checkValues(darr, exp1, 3);

    // Expand to the same size (should succeed)
    TEST_ASSERT_TRUE(darr_expand(darr, 10));
    TEST_ASSERT_EQUAL_INT(10, darr_size(darr));

    darr_free(darr);

    // Test with ALLOC_STRAT_BUDDY strategy
    darr = darr_new(5, SI, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert 3 items
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 3));
    checkValues(darr, exp1, 3);

    // Expand to 6 items
    TEST_ASSERT_TRUE(darr_expand(darr, 6));
    TEST_ASSERT_EQUAL_INT(8, darr_size(darr)); // Size rounded up to 8
    checkValues(darr, exp1, 3);

    // Expand to 9 items
    TEST_ASSERT_TRUE(darr_expand(darr, 9));
    TEST_ASSERT_EQUAL_INT(16, darr_size(darr)); // Size rounded up to 16
    checkValues(darr, exp1, 3);

    // Expand to a smaller size (should fail)
    TEST_ASSERT_FALSE(darr_expand(darr, 2)); // Cannot shrink below current length
    TEST_ASSERT_EQUAL_INT(16, darr_size(darr)); // Size remains unchanged

    darr_free(darr);
}

void test_darr_index(void) {
    DArr *darr = darr_new(5, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert a few test items
    int data[] = { 1, 2, 3, 4, 5 };
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 5));

    // Test valid index
    TEST_ASSERT_EQUAL_INT(3, *(int*)darr_index(darr, 2));

    // Test out-of-bounds index
    TEST_ASSERT_NULL((int*)darr_index(darr, 10));

    // Test empty array
    DArr *emptyDarr = darr_new(0, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(emptyDarr);
    TEST_ASSERT_NULL(darr_index(emptyDarr, 0));

    darr_free(darr);
    darr_free(emptyDarr);
}

void test_darr_insert(void) {
    DArr *darr = darr_new(5, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert 3 items at index 0
    int data1[] = { 1, 2, 3 };
    TEST_ASSERT_TRUE(darr_insert(darr, data1, 0, 3));
    checkValues(darr, data1, 3);

    // Insert 2 more items at index 3 (end)
    int data2[] = { 4, 5 };
    TEST_ASSERT_TRUE(darr_insert(darr, data2, 3, 2));
    int exp1[] = { 1, 2, 3, 4, 5 };
    checkValues(darr, exp1, 5);

    // Insert 1 item at index 2 (middle)
    int data3 = 99;
    TEST_ASSERT_TRUE(darr_insert(darr, &data3, 2, 1));
    int exp2[] = { 1, 2, 99, 3, 4, 5 };
    checkValues(darr, exp2, 6);

    // Test invalid cases (NULL items, count 0, out-of-bounds)
    TEST_ASSERT_FALSE(darr_insert(darr, NULL, 2, 1));
    TEST_ASSERT_FALSE(darr_insert(darr, data1, 2, 0));
    TEST_ASSERT_FALSE(darr_insert(darr, data1, 100, 1));
    checkValues(darr, exp2, 6);

    darr_free(darr);

    // String insertion test
    darr = darr_new(5, SC, ALLOC_STRAT_DYNAMIC);  
    TEST_ASSERT_NOT_NULL(darr);

    // Insert a string at index 0
    char str1[] = "Hello";
    size_t str1Size = sizeof(str1) - 1; // Exclude null terminator
    TEST_ASSERT_TRUE(darr_insert(darr, str1, 0, str1Size));
    TEST_ASSERT_EQUAL_INT(str1Size, darr_len(darr));
    TEST_ASSERT_FALSE(darr_isEmpty(darr));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello", (char*)darr_index(darr, 0), str1Size);

    // Insert another string at the end
    char str2[] = " World";
    size_t str2Size = sizeof(str2) - 1;
    TEST_ASSERT_TRUE(darr_insert(darr, str2, str1Size, str2Size));
    TEST_ASSERT_EQUAL_INT(str1Size + str2Size, darr_len(darr));
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World", (char*)darr_index(darr, 0), str1Size + str2Size);

    // Insert in the middle
    char str3[] = " amazing";
    size_t str3Size = sizeof(str3) - 1;
    TEST_ASSERT_TRUE(darr_insert(darr, str3, 5, str3Size)); // Insert after "Hello"
    TEST_ASSERT_EQUAL_INT(str1Size + str2Size + str3Size, darr_len(darr));
    TEST_ASSERT_EQUAL_CHAR_ARRAY(
        "Hello amazing World", 
        (char*)darr_index(darr, 0), 
        str1Size + str2Size + str3Size
    );

    // Insert a null terminator to cap the string
    char strEnd[] = "\0";
    TEST_ASSERT_TRUE(darr_insert(darr, strEnd, darr_len(darr), SC));
    TEST_ASSERT_EQUAL_STRING("Hello amazing World", (char*)darr_index(darr, 0));

    darr_free(darr);
}

void test_darr_new(void) {
    DArr *darr;

    // Test with zero item size
    darr = darr_new(10, 0, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NULL(darr);

    // Test with zero size and valid itemSize
    darr = darr_new(0, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);
    TEST_ASSERT_EQUAL_INT(SI, darr_itemSize(darr));
    TEST_ASSERT_EQUAL_INT(0, darr_len(darr));
    TEST_ASSERT_EQUAL_INT(0, darr_size(darr));
    TEST_ASSERT_TRUE(darr_isEmpty(darr));
    darr_free(darr);

    // Test with non-zero size and itemSize
    darr = darr_new(10, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);
    TEST_ASSERT_NOT_NULL(darr->block);
    TEST_ASSERT_EQUAL_INT(SI, darr_itemSize(darr));
    TEST_ASSERT_EQUAL_INT(0, darr_len(darr));
    TEST_ASSERT_EQUAL_INT(10, darr_size(darr));
    TEST_ASSERT_TRUE(darr_isEmpty(darr));
    darr_free(darr);

    // Test with a very large size (edge case)
    darr = darr_new(SIZE_MAX, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NULL(darr); // Allocation should fail due to size

    // Test with a different allocation strategy
    darr = darr_new(5, SI, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(darr);
    TEST_ASSERT_NOT_NULL(darr->block);
    TEST_ASSERT_EQUAL_INT(SI, darr_itemSize(darr));
    TEST_ASSERT_EQUAL_INT(0, darr_len(darr));
    TEST_ASSERT_EQUAL_INT(8, darr_size(darr));
    TEST_ASSERT_TRUE(darr_isEmpty(darr));
    darr_free(darr);
}

void test_darr_remove(void) {
    DArr *darr = darr_new(10, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert 6 items
    int data[] = { 1, 2, 3, 4, 5, 6 };
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 6));
    int exp1[] = { 1, 2, 3, 4, 5, 6 };
    checkValues(darr, exp1, 6);

    // Remove 2 items from the middle
    TEST_ASSERT_TRUE(darr_remove(darr, 2, 2)); // Remove 3 and 4
    int exp2[] = { 1, 2, 5, 6 };
    checkValues(darr, exp2, 4);

    // Remove 1 item from the beginning
    TEST_ASSERT_TRUE(darr_remove(darr, 0, 1)); // Remove 1
    int exp3[] = { 2, 5, 6 };
    checkValues(darr, exp3, 3);

    // Remove multiple items from the end
    TEST_ASSERT_TRUE(darr_remove(darr, 1, 2)); // Remove 5 and 6
    int exp4[] = { 2 };
    checkValues(darr, exp4, 1);

    // Test invalid cases (out-of-bounds, count 0)
    TEST_ASSERT_FALSE(darr_remove(darr, 1, 1)); // Out-of-bounds
    TEST_ASSERT_FALSE(darr_remove(darr, 0, 0)); // Count 0
    checkValues(darr, exp4, 1);

    darr_free(darr);
}

void test_darr_resize(void) {
    DArr *darr = darr_new(5, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert 3 items
    int data[] = { 1, 2, 3 };
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 3));
    int exp1[] = { 1, 2, 3 };
    checkValues(darr, exp1, 3);

    // Resize to a larger size
    TEST_ASSERT_TRUE(darr_resize(darr, 10));
    TEST_ASSERT_EQUAL_INT(10, darr_size(darr));
    checkValues(darr, exp1, 3); // Ensure existing data is intact

    // Resize to a smaller size 
    TEST_ASSERT_TRUE(darr_resize(darr, 2));
    TEST_ASSERT_EQUAL_INT(2, darr_size(darr));
    int exp2[] = { 1, 2 };
    checkValues(darr, exp2, 2);

    // Resize to the same size
    TEST_ASSERT_TRUE(darr_resize(darr, 2));
    TEST_ASSERT_EQUAL_INT(2, darr_size(darr));

    darr_free(darr);

    // Test with ALLOC_STRAT_BUDDY strategy
    darr = darr_new(5, SI, ALLOC_STRAT_BUDDY);
    TEST_ASSERT_NOT_NULL(darr);

    // Insert 3 items
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 3));
    checkValues(darr, exp1, 3);

    // Resize to 6 items
    TEST_ASSERT_TRUE(darr_resize(darr, 6));
    TEST_ASSERT_EQUAL_INT(8, darr_size(darr)); // Size rounded up to 8
    checkValues(darr, exp1, 3);

    // Resize to 9 items
    TEST_ASSERT_TRUE(darr_resize(darr, 9));
    TEST_ASSERT_EQUAL_INT(16, darr_size(darr)); // Size rounded up to 16
    checkValues(darr, exp1, 3);

    // Resize to a smaller size
    TEST_ASSERT_TRUE(darr_resize(darr, 3));
    TEST_ASSERT_EQUAL_INT(4, darr_size(darr));

    darr_free(darr);
}

void test_darr_setAt(void) {
    DArr *darr = darr_new(5, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    // Initialize with some values
    int initial[] = { 1, 2, 3, 4, 5 };
    TEST_ASSERT_TRUE(darr_insert(darr, initial, 0, 5));
    TEST_ASSERT_EQUAL_INT_ARRAY(initial, (int*)darr_index(darr, 0), 5);

    // Set new values at index 1
    int newValues[] = { 10, 20 };
    TEST_ASSERT_TRUE(darr_setAt(darr, newValues, 1, 2));
    int exp1[] = {1, 10, 20, 4, 5};
    TEST_ASSERT_EQUAL_INT_ARRAY(exp1, (int*)darr_index(darr, 0), 5);

    // Set a single value at the last index
    int newValue = 30;
    TEST_ASSERT_TRUE(darr_setAt(darr, &newValue, 4, 1));
    int exp2[] = { 1, 10, 20, 4, 30 };
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)darr_index(darr, 0), 5);

    // Test out-of-bounds insertion
    int invalidValues[] = {40, 50};
    TEST_ASSERT_FALSE(darr_setAt(darr, invalidValues, 5, 2)); // idx + count > len
    TEST_ASSERT_EQUAL_INT_ARRAY(exp2, (int*)darr_index(darr, 0), 5); // Should remain unchanged

    // Test invalid cases
    TEST_ASSERT_FALSE(darr_setAt(darr, NULL, 0, 2)); // Null items
    TEST_ASSERT_FALSE(darr_setAt(darr, newValues, 2, 0)); // Zero count
    darr_free(darr);
}

void test_darr_split(void) {
    // Create a DArr and insert some data
    DArr *darr = darr_new(10, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 10));

    DArr *left = NULL;
    DArr *right = NULL;

    // Split at index 5
    TEST_ASSERT_TRUE(darr_split(darr, &left, &right, 5));

    // Check the left DArr
    int expLeft[] = { 1, 2, 3, 4, 5 };
    TEST_ASSERT_EQUAL_INT_ARRAY(expLeft, (int*)darr_index(left, 0), 5);
    TEST_ASSERT_EQUAL_INT(5, darr_len(left));

    // Check the right DArr
    int expRight[] = { 6, 7, 8, 9, 10 };
    TEST_ASSERT_EQUAL_INT_ARRAY(expRight, (int*)darr_index(right, 0), 5);
    TEST_ASSERT_EQUAL_INT(5, darr_len(right));

    darr_free(left);
    darr_free(right);
    left = NULL;
    right = NULL;

    // Test split at start
    darr = darr_new(10, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);

    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 10));

    // Split at index 0 (empty left)
    TEST_ASSERT_TRUE(darr_split(darr, &left, &right, 0));
    
    // Left DArr should be empty
    TEST_ASSERT_EQUAL_INT(0, darr_len(left));
    TEST_ASSERT_TRUE(darr_isEmpty(left));

    // Right DArr should contain all elements
    TEST_ASSERT_EQUAL_INT_ARRAY(data, (int*)darr_index(right, 0), 10);
    TEST_ASSERT_EQUAL_INT(10, darr_len(right));

    darr_free(left);
    darr_free(right);
    left = NULL;
    right = NULL;

    // Test split at end
    darr = darr_new(10, SI, ALLOC_STRAT_DYNAMIC);
    TEST_ASSERT_NOT_NULL(darr);
    TEST_ASSERT_TRUE(darr_insert(darr, data, 0, 10));

    // Split at the last index (empty right)
    TEST_ASSERT_TRUE(darr_split(darr, &left, &right, 10));

    // Left DArr should contain all elements
    TEST_ASSERT_EQUAL_INT_ARRAY(data, (int*)darr_index(left, 0), 10);
    TEST_ASSERT_EQUAL_INT(10, darr_len(left));

    // Right DArr should be empty
    TEST_ASSERT_EQUAL_INT(0, darr_len(right));
    TEST_ASSERT_TRUE(darr_isEmpty(right));

    darr_free(left);
    darr_free(right);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_darr_append);
    RUN_TEST(test_darr_clear);
    RUN_TEST(test_darr_copy);
    RUN_TEST(test_darr_expand);
    RUN_TEST(test_darr_index);
    RUN_TEST(test_darr_insert);
    RUN_TEST(test_darr_new);
    RUN_TEST(test_darr_remove);
    RUN_TEST(test_darr_resize);
    RUN_TEST(test_darr_setAt);
    RUN_TEST(test_darr_split);

    return UNITY_END();
}
