#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "test_gitflex.c"

int main() {
    CU_pSuite pSuite = NULL;
    
    // Initialize CUnit
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    // Add a suite to the registry
    pSuite = CU_add_suite("GitFlex Test Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // Add the tests to the suite
    if ((NULL == CU_add_test(pSuite, "test of init", test_init)) ||
        (NULL == CU_add_test(pSuite, "test of hash-object", test_hash_object)) ||
        (NULL == CU_add_test(pSuite, "test of cat-file", test_cat_file))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
} 