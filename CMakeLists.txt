cmake_minimum_required(VERSION 3.14)

include(cmake/variables.cmake)

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
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable developer mode
set(TL_DEVELOPER_MODE ${PROJECT_IS_TOP_LEVEL})

option(TL_ENABLE_TRACY "Enable Tracy profiler" ${PROJECT_IS_TOP_LEVEL})

if(TL_ENABLE_TRACY)
    add_compile_definitions(TL_ENABLE_TRACY)

    option(TL_ENABLE_TRACY_MEMORY_TRACKING "Enable Tracy memory tracking (slows down application)" ${PROJECT_IS_TOP_LEVEL})
    if(TL_ENABLE_TRACY_MEMORY_TRACKING)
        add_compile_definitions(TL_ENABLE_TRACY_MEMORY_TRACKING)
    endif()
endif()

# ---- Add dependencies ----
include(package-lock.cmake)

# ---- Declare library ----
add_subdirectory(TL)

# ---- Tests ----
# if(TL_BUILD_TESTS)
#     add_subdirectory(Tests)
# endif()

# ---- Developer mode ----
if(NOT TL_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(AUTHOR_WARNING "Developer mode is intended for developers of RHI")
endif()

include(cmake/dev-mode.cmake)
