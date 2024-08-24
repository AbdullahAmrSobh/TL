cmake_minimum_required(VERSION 3.16)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Assert.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator/Allocator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator/Arena.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Memory.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Flags.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Log.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Span.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/UniquePtr.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Serialization/Binary.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Stacktrace.hpp
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Allocator/Allocator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Allocator/Arena.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Allocator/mimalloc-override.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Serialization/Serialization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Serialization/Binary.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Stacktrace.cpp
    ${tracy_SOURCE_DIR}/public/TracyClient.cpp
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
target_include_directories(TL SYSTEM PUBLIC ${tracy_SOURCE_DIR}/public)

target_link_libraries(TL PRIVATE mimalloc-static)
target_link_libraries(TL PRIVATE spdlog)
target_link_libraries(TL PUBLIC lz4)
target_link_libraries(TL PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(TL PRIVATE Dbghelp.lib)
# target_link_libraries(TL PUBLIC )
# target_link_libraries(TL PRIVATE fmt)
# target_link_libraries(TL PRIVATE backward)
