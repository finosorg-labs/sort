#
# tests/CMakeLists.txt - Test configuration for sort module
#

# Test executable
add_executable(sort_tests
    test_framework.c
    test_main.c
    test_sort.c
    test_int_sort.c
    test_multikey.c
    test_float_sort.c
    test_argsort.c
    test_topk.c
)

# Link with sort library
target_link_libraries(sort_tests
    PRIVATE
        finkit_sort_static
        m
)

target_compile_definitions(sort_tests PRIVATE
    FC_STATIC_BUILD
)

# Include directories
target_include_directories(sort_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/modules/platform/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler flags for tests
target_compile_options(sort_tests PRIVATE -Wall -Wextra -Wpedantic)

# Run tests with ctest
add_test(
    NAME "sort_tests"
    COMMAND sort_tests
)

# Installation
install(TARGETS sort_tests
    RUNTIME DESTINATION bin
)
