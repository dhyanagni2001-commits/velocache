# CMakeLists.txt for velocache's GoogleTest suite.
#
# This file exists ONLY to build and run the unit tests (tests/test_cache.cpp).
# The production binaries (v_server, v_bench) still build with the existing
# Makefile — nothing about that workflow changes.
#
# GoogleTest is fetched automatically via FetchContent, the CMake equivalent
# of a conda/pip install: the dependency is downloaded once into an isolated
# local folder (cmake-build/_deps) and built from source, so nothing gets
# installed system-wide and the project stays true to its "zero external
# runtime dependencies" claim (this is a dev/test-only dependency).

cmake_minimum_required(VERSION 3.14)
project(velocache_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# Keep GoogleTest's own runtime settings consistent with ours on Windows/MSVC.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_cache
    tests/test_cache.cpp
    src/cache.cpp
    src/storage.cpp
    utils/input_validation.cpp
)

target_include_directories(test_cache PRIVATE include)
# Linking against `gtest` (not `gtest_main`) because test_cache.cpp defines
# its own main() to back up/restore assets/cache_data.txt around the run.
target_link_libraries(test_cache PRIVATE gtest)

# Run from the project root so the cache's hardcoded "assets/cache_data.txt"
# path resolves the same way it does for v_server.
add_test(NAME velocache_unit_tests COMMAND test_cache WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
