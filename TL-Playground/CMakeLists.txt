set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Playground.cpp
)

include(${CMAKE_SOURCE_DIR}/CMake/add-target.cmake)
tl_add_target(
    NAME TL_Playground
    EXECUTABLE
    SOURCES ${SOURCE_FILES}
)

target_link_libraries(TL_Playground PUBLIC TL)