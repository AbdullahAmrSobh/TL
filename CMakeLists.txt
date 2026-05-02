cmake_minimum_required(VERSION 3.14)

# ---- In-source build guard ----
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(
        FATAL_ERROR
        "In-source builds are not supported. "
        "Please read the BUILDING document before trying to build this project. "
        "You may need to delete 'CMakeCache.txt' and 'CMakeFiles/' first."
    )
endif()

# Project declaration
project(
    TL
    VERSION 0.1.0
    DESCRIPTION "Template Library"
    HOMEPAGE_URL "https://github.com/AbdullahAmrSobh/TL"
    LANGUAGES CXX
)

# Set C++ standard and extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Enable developer mode
set(TL_DEVELOPER_MODE ${PROJECT_IS_TOP_LEVEL})


# ==== Options ====
option(TL_ENABLE_TRACY                "Enable Tracy profiler"                                                     ON)
option(TL_TRACY_MEMORY_PROFILING      "Enable Tracy memory profiling"                                             ON)
option(TL_TRACK_ALLOCATION_SIZES      "Enable collecting global allocation sizes, and report on exit"             ON)
option(TL_TRACK_ALLOCATION_CALLSTACKS "Enable collecting global allocation callstacks and report on exit (SLOW!)" OFF)
option(TL_BUILD_TESTS                 "Builds TL unittests"                                                       ${PROJECT_IS_TOP_LEVEL})

# ==== Messages ===
message(STATUS "TL config options")
message(STATUS "TL_ENABLE_TRACY                : ${TL_ENABLE_TRACY}")
message(STATUS "TL_TRACY_MEMORY_PROFILING      : ${TL_TRACY_MEMORY_PROFILING}")
message(STATUS "TL_TRACK_ALLOCATION_SIZES      : ${TL_TRACK_ALLOCATION_SIZES}")
message(STATUS "TL_TRACK_ALLOCATION_CALLSTACKS : ${TL_TRACK_ALLOCATION_CALLSTACKS}")
message(STATUS "TL_BUILD_TESTS                 : ${TL_BUILD_TESTS}")

# ==== Tracy / Allocation option validation ====
if(TL_TRACY_MEMORY_PROFILING AND NOT TL_ENABLE_TRACY)
    message(WARNING "TL_TRACY_MEMORY_PROFILING requires TL_ENABLE_TRACY to be ON.")
endif()

if(TL_TRACK_ALLOCATION_SIZES AND TL_TRACK_ALLOCATION_CALLSTACKS)
    message(WARNING "TL_TRACK_ALLOCATION_CALLSTACKS overrides TL_TRACK_ALLOCATION_SIZES. Only callstack tracking will be enabled.")
endif()

# ---- Add dependencies ----
include(package-lock.cmake)

# ---- Declare library ----
add_subdirectory(TL)

# ---- Tests ----
if(TL_BUILD_TESTS)
    add_subdirectory(TL-Playground)

    enable_testing()
    add_subdirectory(TL-Tests)
endif()
