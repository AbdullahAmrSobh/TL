cmake_minimum_required(VERSION 3.16)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Assert.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator/BaseAllocator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Flags.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Log.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Span.hpp
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Allocator/BaseAllocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/mimalloc-override.cpp
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../CMake/add-target.cmake)
tl_add_target(
    NAME TL
    NAMESPACE TL
    STATIC
    HEADERS ${HEADER_FILES}
    SOURCES ${SOURCE_FILES}
)

target_compile_definitions(TL PUBLIC $<$<CONFIG:Debug>:TL_DEBUG>)

target_include_directories(TL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(TL PRIVATE mimalloc-static)
target_link_libraries(TL PRIVATE spdlog)
# target_link_libraries(TL PRIVATE fmt)
# target_link_libraries(TL PRIVATE backward)