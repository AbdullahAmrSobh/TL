# enable_testing()

set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Vector.cpp
)

add_executable(TL_Tests ${TEST_FILES})

target_link_libraries(TL_Tests TL GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TL_Tests)