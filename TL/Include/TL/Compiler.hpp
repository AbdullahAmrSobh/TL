#pragma once

// Macro to apply lifetimebound attribute if supported by the compiler
#ifndef __has_cpp_attribute
    #define TL_LIFETIME_BOUND
#elif __has_cpp_attribute(msvc::lifetimebound)
    #define TL_LIFETIME_BOUND [[msvc::lifetimebound]]
#elif __has_cpp_attribute(clang::lifetimebound)
    #define TL_LIFETIME_BOUND [[clang::lifetimebound]]
#elif __has_cpp_attribute(lifetimebound)
    #define TL_LIFETIME_BOUND [[lifetimebound]]
#else
    #define TL_LIFETIME_BOUND
#endif

#ifndef __has_cpp_attribute
    #define TL_NO_UNIQUE_ADDRESS
#elif __has_cpp_attribute(msvc::no_unique_address)
    #define TL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif __has_cpp_attribute(clang::no_unique_address)
    #define TL_NO_UNIQUE_ADDRESS [[clang::no_unique_address]]
#elif __has_cpp_attribute(no_unique_address)
    #define TL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#else
    #define TL_NO_UNIQUE_ADDRESS
#endif


#ifndef __has_cpp_attribute
    #define TL_NODISCARD
#elif __has_cpp_attribute(nodiscard)
    #define TL_NODISCARD [[nodiscard]]
#elif __has_cpp_attribute(nodiscard)
    #define TL_NODISCARD [[nodiscard]]
#elif __has_cpp_attribute(nodiscard)
    #define TL_NODISCARD [[nodiscard]]
#else
    #define TL_NODISCARD
#endif

#ifndef __has_cpp_attribute
    #define TL_MAYBE_UNUSED
#elif __has_cpp_attribute(maybe_unused)
    #define TL_MAYBE_UNUSED [[maybe_unused]]
#elif __has_cpp_attribute(maybe_unused)
    #define TL_MAYBE_UNUSED [[maybe_unused]]
#elif __has_cpp_attribute(maybe_unused)
    #define TL_MAYBE_UNUSED [[maybe_unused]]
#else
    #define TL_MAYBE_UNUSED
#endif