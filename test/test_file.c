
/*
    File Type   : C Test Source
    Description : File Operations
*/

#include "file.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_file_open(void) {
    FILE *f = file_open("test/data/hello.txt", "r");
    TEST_ASSERT_NOT_NULL(f);
}

void test_file_read(void) {
    char *text = file_read("test/data/hello.txt");
    char *expText = "Hello World\nI am some text";
    TEST_ASSERT_EQUAL_CHAR_ARRAY(expText, text, strlen(expText));
    free(text);

    text = file_read("test/data/long.txt");
    TEST_ASSERT_EQUAL_INT(81 * 13 - 1, strlen(text));
    free(text);

    text = file_read("test/data/longLine.txt");
    TEST_ASSERT_EQUAL_INT(80 * 14 + 1, strlen(text));
    free(text);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_file_open);
    RUN_TEST(test_file_read);

    return UNITY_END();
}
