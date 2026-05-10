include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)

# Prevent GTest from overriding compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    test_discrete_distribution_common.cpp
    test_discrete_distributor_improved.cpp
    test_discrete_distribution_add_elem_common.cpp
)

target_include_directories(tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tests
    PRIVATE
        GTest::gtest_main
)

target_compile_options(tests
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

include(GoogleTest)
gtest_discover_tests(tests)
