cmake_minimum_required(VERSION 3.16)

set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Assert.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator/Allocator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator/Arena.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator/Mimalloc.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Allocator/MemPlumber.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/OffsetAllocator/OffsetAllocator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Flags.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Log.hpp


    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/Vector.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/Map.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/Set.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/String.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/StringView.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/STLAdapter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/Optional.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/Function.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Containers/Varient.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Span.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Ptr.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Serialization/Binary.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Stacktrace.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Context.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Include/TL/Library.hpp

    $<$<CXX_COMPILER_ID:MSVC>:${CMAKE_CURRENT_SOURCE_DIR}/TL.natvis>
)

set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Allocator/Arena.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Allocator/Mimalloc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Allocator/MemPlumber.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Source/OffsetAllocator/OffsetAllocator.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Stacktrace.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Context.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Library/Library.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Source/File/File.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/FileSystem/FileSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/FileSystem/FileSystemWatcher.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Source/DynamicLibrary.cpp

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
target_include_directories(TL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source)
target_include_directories(TL SYSTEM PUBLIC ${tracy_SOURCE_DIR}/public)

target_link_libraries(TL PRIVATE spdlog)

# target_link_libraries(TL PUBLIC lz4)

target_link_libraries(TL PUBLIC nlohmann_json::nlohmann_json)

target_link_libraries(TL PRIVATE mimalloc-static)

if(NOT EMSCRIPTEN)
    target_link_libraries(TL PRIVATE Dbghelp.lib)
endif()
