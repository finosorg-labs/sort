# FindOpenMP.cmake - Detect OpenMP support
#
# Sets:
#   OPENMP_FOUND - TRUE if OpenMP is available
#   OPENMP_C_FLAGS - Compiler flags for OpenMP
#   OPENMP_C_LIB_NAMES - OpenMP library names

include(FindPackageHandleStandardArgs)

# Check if OpenMP is available
find_package(OpenMP QUIET)

if(OpenMP_C_FOUND)
    set(OPENMP_FOUND TRUE)
    set(OPENMP_C_FLAGS ${OpenMP_C_FLAGS})
    set(OPENMP_C_LIB_NAMES ${OpenMP_C_LIB_NAMES})

    message(STATUS "OpenMP support: enabled")
    message(STATUS "  Flags: ${OpenMP_C_FLAGS}")
else()
    set(OPENMP_FOUND FALSE)
    message(STATUS "OpenMP support: disabled (not found)")
endif()

find_package_handle_standard_args(OpenMP DEFAULT_MSG OPENMP_FOUND)
