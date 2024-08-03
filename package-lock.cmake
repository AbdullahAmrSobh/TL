include(${CMAKE_SOURCE_DIR}/CMake/CPM.cmake)

CPMAddPackage(
    NAME           mimalloc
	GIT_REPOSITORY git@github.com:microsoft/mimalloc.git
	GIT_TAG        v2.1.7
	OPTIONS
		"MI_SECURE OFF"               # "Use full security mitigations (like guard pages, allocation randomization, double-free mitigation, and free-list corruption detection)"
		"MI_DEBUG_FULL OFF"           # "Use full internal heap invariant checking in DEBUG mode (expensive)"
		"MI_PADDING OFF"              # "Enable padding to detect heap block overflow (always on in DEBUG or SECURE mode, or with Valgrind/ASAN)"
		"MI_OVERRIDE ON"              # "Override the standard malloc interface (e.g. define entry points for malloc() etc)"
		"MI_XMALLOC OFF"              # "Enable abort() call on memory allocation failure by default"
		"MI_SHOW_ERRORS OFF"          # "Show error and warning messages by default (only enabled by default in DEBUG mode)"
		"MI_TRACK_VALGRIND OFF"       # "Compile with Valgrind support (adds a small overhead)"
		"MI_TRACK_ASAN OFF"           # "Compile with address sanitizer support (adds a small overhead)"
		"MI_TRACK_ETW OFF"            # "Compile with Windows event tracing (ETW) support (adds a small overhead)"
		"MI_USE_CXX OFF"              # "Use the C++ compiler to compile the library (instead of the C compiler)"
		"MI_SEE_ASM OFF"              # "Generate assembly files"
		"MI_OSX_INTERPOSE ON"         # "Use interpose to override standard malloc on macOS"
		"MI_OSX_ZONE ON"              # "Use malloc zone to override standard malloc on macOS"
		"MI_WIN_REDIRECT ON"          # "Use redirection module ('mimalloc-redirect') on Windows if compiling mimalloc as a DLL"
		"MI_LOCAL_DYNAMIC_TLS OFF"    # "Use slightly slower, dlopen-compatible TLS mechanism (Unix)"
		"MI_LIBC_MUSL OFF"            # "Set this when linking with musl libc"
		"MI_BUILD_SHARED OFF"         # "Build shared library"
		"MI_BUILD_STATIC ON"          # "Build static library"
		"MI_BUILD_OBJECT OFF"         # "Build object library"
		"MI_BUILD_TESTS OFF"          # "Build test executables"
		"MI_DEBUG_TSAN OFF"           # "Build with thread sanitizer (needs clang)"
		"MI_DEBUG_UBSAN OFF"          # "Build with undefined-behavior sanitizer (needs clang++)"
		"MI_SKIP_COLLECT_ON_EXIT OFF" # "Skip collecting memory on program exit"
		"MI_NO_PADDING OFF"           # "Force no use of padding even in DEBUG mode etc."
		"MI_INSTALL_TOPLEVEL OFF"     # "Install directly into $CMAKE_INSTALL_PREFIX instead of PREFIX/lib/mimalloc-version"
		"MI_NO_THP OFF"               # "Disable transparent huge pages support on Linux/Android for the mimalloc process only"
)

CPMAddPackage(
    NAME           spdlog
	GIT_REPOSITORY git@github.com:gabime/spdlog.git
	GIT_TAG        v1.14.1
	OPTIONS
		# Default options are good
)

CPMAddPackage(
    NAME           backward-cpp
	GIT_REPOSITORY git@github.com:bombela/backward-cpp.git
	GIT_TAG        v1.6
	OPTIONS
)

CPMAddPackage(
    NAME           fmt
	GIT_REPOSITORY git@github.com:fmtlib/fmt.git
	GIT_TAG        11.0.2
	OPTIONS
)
