#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "gitflex.h"

// Test content
const char* TEST_CONTENT = "Hello, World!\n";
const size_t TEST_CONTENT_SIZE = 14; // Length of "Hello, World!\n" including null terminator

// Helper function to create a test file with given content
void create_test_file(const char* filename, const char* content, size_t size) {
    FILE* file = fopen(filename, "wb");
    CU_ASSERT_PTR_NOT_NULL(file);
    if (file) {
        fwrite(content, 1, size, file);
        fclose(file);
    }
}

// Test suite initialization and cleanup
int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    // Clean up any test files
    system("rm -rf .gitflex");
    remove("test.txt");
    return 0;
}

// Test cases
void test_init(void) {
    // Test that init creates the correct directory structure
    runInit();
    
    struct stat st;
    CU_ASSERT(stat(".gitflex", &st) == 0);
    CU_ASSERT(stat(".gitflex/objects", &st) == 0);
    CU_ASSERT(stat(".gitflex/refs", &st) == 0);
    CU_ASSERT(stat(".gitflex/refs/heads", &st) == 0);
    CU_ASSERT(stat(".gitflex/HEAD", &st) == 0);
    
    // Check HEAD content
    FILE *head = fopen(".gitflex/HEAD", "r");
    CU_ASSERT_PTR_NOT_NULL(head);
    if (head) {
        char content[256];
        fgets(content, sizeof(content), head);
        CU_ASSERT_STRING_EQUAL(content, "ref: refs/heads/main\n");
        fclose(head);
    }
}

void test_hash_object(void) {
    // Create test file with predefined content
    create_test_file("test.txt", TEST_CONTENT, TEST_CONTENT_SIZE);
    
    // Hash the test file
    runHashObject("test.txt");
    
    // Verify the object was created
    struct stat st;
    CU_ASSERT(stat(".gitflex/objects", &st) == 0);
}

void test_cat_file(void) {
    // Create test file with predefined content
    create_test_file("test.txt", TEST_CONTENT, TEST_CONTENT_SIZE);
    
    // Hash the file
    runHashObject("test.txt");
    
    // TODO: We need to capture the hash from runHashObject to use in runCatFile
    // For now, this is a placeholder
    // runCatFile("hash_here");
} 