
/*
    File Type   : C Test Source
    Description : Mathematical Operations
*/

#include "math.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_math_abs(void) {
    TEST_ASSERT_EQUAL_INT(1, math_abs(1));
    TEST_ASSERT_EQUAL_INT(0, math_abs(0));
    TEST_ASSERT_EQUAL_INT(1, math_abs(-1));
    
    TEST_ASSERT_EQUAL_FLOAT(1.0f, math_abs(1.0f));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, math_abs(0.0f));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, math_abs(-1.0f));
}

void test_math_min(void) {
    TEST_ASSERT_EQUAL_INT(1, math_min(1, 2));
    TEST_ASSERT_EQUAL_INT(1, math_min(2, 1));
    TEST_ASSERT_EQUAL_INT(-10, math_min(10, -10));
    TEST_ASSERT_EQUAL_INT(0, math_min(10, 0));
    TEST_ASSERT_EQUAL_INT(5, math_min(5, 5));
    
    TEST_ASSERT_EQUAL_FLOAT(1.0f, math_min(1.0f, 2.0f));
    TEST_ASSERT_EQUAL_FLOAT(1.0f, math_min(2.0f, 1.0f));
    TEST_ASSERT_EQUAL_FLOAT(-10.0f, math_min(10.0f, -10.0f));
    TEST_ASSERT_EQUAL_FLOAT(0.0f, math_min(10.0f, 0.0f));
    TEST_ASSERT_EQUAL_FLOAT(5.0f, math_min(5.0f, 5.0f));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_math_abs);
    RUN_TEST(test_math_min);

    return UNITY_END();
}
