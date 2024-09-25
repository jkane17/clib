
/*
    File Type   : C Test Source
    Description : File Operations
*/

#include <stdio.h>
#include <string.h>

#include "file.h"
#include "unity.h"

#ifndef PATH_ROOT
    #define PATH_ROOT "."
#endif

#define PATH_DATA PATH_ROOT "/test/data/"

void createDataFile(const char *path, size_t lineLen, size_t lineCount) {
    file_delete(path);
    char content[lineLen * lineCount + 1];
    memset(content, 'A', lineLen * lineCount);
    for (size_t i = 1; i < lineCount; i++) content[(i * lineLen) - 1] = '\n';
    content[lineLen * lineCount] = '\0';
    file_create(path, content);
}

void checkRead(size_t lineLen, size_t lineCount) {
    char *path = PATH_DATA "file_read.txt";
    createDataFile(path, lineLen, lineCount);
    char *text = file_read(path);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_EQUAL_INT(lineLen * lineCount, strlen(text));
    free(text);
    file_delete(path);
}

void setUp(void) {}
void tearDown(void) {}

void test_file_create(void) {
    char *path = PATH_DATA "test_file_create.txt";
    char *content = "This is a test file for creation.";

    bool fileCreated = file_create(path, content);
    TEST_ASSERT_TRUE(fileCreated);

    // Contains the correct content
    FILE *file = fopen(path, "r");
    TEST_ASSERT_NOT_NULL(file);

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    TEST_ASSERT_EQUAL_STRING(content, buffer);

    fclose(file);

    // Try creating the file again - should return false because it already exists
    bool fileRecreated = file_create(path, "New content");
    TEST_ASSERT_FALSE(fileRecreated);

    // Clean up: delete the test file
    bool fileDeleted = file_delete(path);
    TEST_ASSERT_TRUE(fileDeleted);
}

void test_file_delete(void) {
    char *path = "test_file.txt";
    char *content = "This is a test file.";

    bool fileCreated = file_create(path, content);
    TEST_ASSERT_TRUE(fileCreated);

    bool fileDeleted = file_delete(path);
    TEST_ASSERT_TRUE(fileDeleted);

    // Check that the file no longer exists by attempting to delete again
    fileDeleted = file_delete(path);
    TEST_ASSERT_TRUE(fileDeleted);

    // Try to delete a non-existent file - should still return true
    fileDeleted = file_delete("non_existent_file.txt");
    TEST_ASSERT_TRUE(fileDeleted);
}

void test_file_open(void) {
    char *path = PATH_DATA "file_open.txt";
    char *content = "file_open\nI am some text for testing";

    file_delete(path);
    file_create(path, content);

    // Test opening an existing file
    FILE *f = file_open(path, "r");
    TEST_ASSERT_NOT_NULL(f);
    fclose(f);
    file_delete(path);

    // Test opening a non-existent file (expected to fail)
    f = file_open(PATH_DATA "non_existent_file.txt", "r");
    TEST_ASSERT_NULL(f);

    // Test opening a file with invalid mode
    f = file_open(path, "invalid_mode");
    TEST_ASSERT_NULL(f);
}

void test_file_read(void) {
    // Basic Functionality
    char *expText = "Hello World\nI am some text";
    char *path = PATH_DATA "hello.txt";
    file_delete(path);
    file_create(path, expText);

    char *text = file_read(path);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_EQUAL_STRING(expText, text);
    free(text);
    file_delete(path);

    // Long File
    checkRead(80, 100);

    // Long Line File
    checkRead(1000, 1);

    // Non-Existent File
    char *nonExistentText = file_read(PATH_DATA "non_existent_file.txt");
    TEST_ASSERT_NULL(nonExistentText);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_file_create);
    RUN_TEST(test_file_delete);
    RUN_TEST(test_file_open);
    RUN_TEST(test_file_read);

    return UNITY_END();
}
