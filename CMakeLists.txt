cmake_minimum_required(VERSION 3.10)

project(lab_03 
    VERSION 1.0.0
    DESCRIPTION "LAB5"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

include_directories(${INCLUDE_DIR})

add_executable(lab_05_exe ${SRC_DIR}/main.cpp)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.0
    TLS_VERIFY     false
)
FetchContent_MakeAvailable(googletest)

add_executable(lab_05_tests ${TEST_DIR}/tests.cpp)

target_link_libraries(lab_05_tests PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(lab_05_tests PRIVATE ${INCLUDE_DIR})

include(GoogleTest)
gtest_discover_tests(lab_05_tests)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(lab_05_exe PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(lab_05_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()