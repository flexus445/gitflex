cmake_minimum_required(VERSION 3.10)
project(gitflex)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find required packages
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# Find CUnit
find_library(CUNIT_LIBRARY NAMES cunit)
find_path(CUNIT_INCLUDE_DIR NAMES CUnit/Basic.h)

if(NOT CUNIT_LIBRARY OR NOT CUNIT_INCLUDE_DIR)
    message(FATAL_ERROR "CUnit not found. Please install it using: brew install cunit")
endif()

# Add main executable
add_executable(gitflex 
    main.c
    src/gitflex_impl.c
)
target_link_libraries(gitflex OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB)

# Add test executable
add_executable(gitflex_tests 
    tests/test_runner.c
    src/gitflex_impl.c
)
target_link_libraries(gitflex_tests 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    ZLIB::ZLIB 
    ${CUNIT_LIBRARY}
)
target_include_directories(gitflex_tests PRIVATE ${CUNIT_INCLUDE_DIR})

# Enable testing
enable_testing()
add_test(NAME gitflex_tests COMMAND gitflex_tests)