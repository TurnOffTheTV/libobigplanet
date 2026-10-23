cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests libobigplanet_test.cpp)
target_compile_definitions(unit_tests PRIVATE FILES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/files")
target_link_libraries(unit_tests PRIVATE libobigplanet GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)