#
# benchmarks/CMakeLists.txt - Benchmark configuration for sort module
#

# Benchmark framework library
add_library(bench_framework OBJECT
    bench_framework.c
)

target_include_directories(bench_framework
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/modules/platform/include
)

# Benchmark executable
add_executable(sort_benchmarks
    bench_main.c
    bench_sort.c
    bench_int_sort.c
    bench_float_sort.c
    bench_multikey.c
    bench_argsort.c
    bench_topk.c
)

# Link with benchmark framework and merged sort library
target_link_libraries(sort_benchmarks
    PRIVATE
        bench_framework
        finkit_sort_static_interface
        m
)

target_compile_definitions(sort_benchmarks PRIVATE
    FC_STATIC_BUILD
)

# Include directories
target_include_directories(sort_benchmarks
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/modules/platform/include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler flags for benchmarks
target_compile_options(sort_benchmarks PRIVATE -Wall -Wextra -Wpedantic -ffp-contract=off)

# Installation
install(TARGETS sort_benchmarks
    RUNTIME DESTINATION bin
)
