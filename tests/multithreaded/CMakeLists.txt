find_package(Threads REQUIRED)

add_executable(multithreaded_tests test_mt_discrete_distribution.cpp)

target_include_directories(multithreaded_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(multithreaded_tests
    PRIVATE
        GTest::gtest_main
        Threads::Threads
)

target_compile_options(multithreaded_tests
    PRIVATE
        -Wall -Wextra -Wpedantic
        -fsanitize=thread
)

target_link_options(multithreaded_tests PRIVATE -fsanitize=thread)

include(GoogleTest)
gtest_discover_tests(multithreaded_tests DISCOVERY_MODE_PRE_TEST)
