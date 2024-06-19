
/*
    File Type   : C Test Source
    Description : String Operations
*/

#include "str.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_str_join(void) {
    char *s, *s1 = "Hello", *s2 = " World", *s3 = "";
    
    s = str_join(s1, s2);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello World", s, 11);
    TEST_ASSERT_EQUAL_INT(11, strlen(s));
    free(s);

    s = str_join(s1, s3);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("Hello", s, 5);
    TEST_ASSERT_EQUAL_INT(5, strlen(s));
    free(s);

    s = str_join(s3, s2);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(" World", s, 6);
    TEST_ASSERT_EQUAL_INT(6, strlen(s));
    free(s);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_str_join);

    return UNITY_END();
}
