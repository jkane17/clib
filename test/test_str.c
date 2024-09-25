
/*
    File        : test_str.c
    Description : String operations.
*/

#include "str.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_str_join(void) {
    char *s;
    const char *s1 = "Hello";
    const char *s2 = " World";
    const char *s3 = "";

    // Test joining two non-empty strings
    s = str_join(s1, s2);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_STRING("Hello World", s);
    TEST_ASSERT_EQUAL_INT(11, strlen(s));
    free(s);
    
    // Test joining a non-empty string with an empty string
    s = str_join(s1, s3);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_STRING("Hello", s);
    TEST_ASSERT_EQUAL_INT(5, strlen(s));
    free(s);

    // Test joining an empty string with a non-empty string
    s = str_join(s3, s2);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_STRING(" World", s);
    TEST_ASSERT_EQUAL_INT(6, strlen(s));
    free(s);

    // Test joining two empty strings
    s = str_join(s3, s3);
    TEST_ASSERT_NOT_NULL(s);
    TEST_ASSERT_EQUAL_STRING("", s);
    TEST_ASSERT_EQUAL_INT(0, strlen(s));
    free(s);

    // Test joining with NULL input (edge case)
    s = str_join(NULL, s2);
    TEST_ASSERT_NULL(s);

    s = str_join(s1, NULL);
    TEST_ASSERT_NULL(s);

    s = str_join(NULL, NULL);
    TEST_ASSERT_NULL(s);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_str_join);

    return UNITY_END();
}
