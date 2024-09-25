
/*
    File        : test_math.c
    Description : Mathematical utility functions and macros.
*/

#include "math.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_math_max(void) {
    // Integer tests
    TEST_ASSERT_EQUAL_INT(2, math_max(1, 2));     // Testing for larger value
    TEST_ASSERT_EQUAL_INT(2, math_max(2, 1));     // Testing for smaller value
    TEST_ASSERT_EQUAL_INT(10, math_max(10, -10)); // Testing with negative value
    TEST_ASSERT_EQUAL_INT(10, math_max(10, 0));   // Testing with zero
    TEST_ASSERT_EQUAL_INT(5, math_max(5, 5));     // Testing with equal values

    // Floating-point tests
    TEST_ASSERT_EQUAL_FLOAT(2.0f, math_max(1.0f, 2.0f));     // Testing for larger value
    TEST_ASSERT_EQUAL_FLOAT(2.0f, math_max(2.0f, 1.0f));     // Testing for smaller value
    TEST_ASSERT_EQUAL_FLOAT(10.0f, math_max(10.0f, -10.0f)); // Testing with negative value
    TEST_ASSERT_EQUAL_FLOAT(10.0f, math_max(10.0f, 0.0f));   // Testing with zero
    TEST_ASSERT_EQUAL_FLOAT(5.0f, math_max(5.0f, 5.0f));     // Testing with equal values
}

void test_math_min(void) {
    // Integer tests
    TEST_ASSERT_EQUAL_INT(1, math_min(1, 2));      // Testing for smaller value
    TEST_ASSERT_EQUAL_INT(1, math_min(2, 1));      // Testing for larger value
    TEST_ASSERT_EQUAL_INT(-10, math_min(10, -10)); // Testing with negative value
    TEST_ASSERT_EQUAL_INT(0, math_min(10, 0));     // Testing with zero
    TEST_ASSERT_EQUAL_INT(5, math_min(5, 5));      // Testing with equal values

    // Floating-point tests
    TEST_ASSERT_EQUAL_FLOAT(1.0f, math_min(1.0f, 2.0f));      // Testing for smaller value
    TEST_ASSERT_EQUAL_FLOAT(1.0f, math_min(2.0f, 1.0f));      // Testing for larger value
    TEST_ASSERT_EQUAL_FLOAT(-10.0f, math_min(10.0f, -10.0f)); // Testing with negative value
    TEST_ASSERT_EQUAL_FLOAT(0.0f, math_min(10.0f, 0.0f));     // Testing with zero
    TEST_ASSERT_EQUAL_FLOAT(5.0f, math_min(5.0f, 5.0f));      // Testing with equal values
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_math_max);
    RUN_TEST(test_math_min);

    return UNITY_END();
}
