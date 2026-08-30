# enable_testing()

set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bits-test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fmt-test.cpp
)

add_executable(TL_Tests ${TEST_FILES})

target_link_libraries(TL_Tests TL GTest::gtest_main)

include(GoogleTest)
if(EMSCRIPTEN)
    gtest_add_tests(TARGET TL_Tests)
else()
    gtest_discover_tests(TL_Tests)
endif()