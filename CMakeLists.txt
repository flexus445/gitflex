cmake_minimum_required(VERSION 3.31)
project(gitflex C)

set(CMAKE_C_STANDARD 11)

find_package(OpenSSL REQUIRED)

add_executable(gitflex main.c)
target_link_libraries(gitflex OpenSSL::SSL OpenSSL::Crypto z)