#pragma once

// #include "TL/Export.hpp"

#ifdef TL_DEBUG
    #if defined(_WIN32)
        #define TL_DEBUG_BREAK() __debugbreak()
    #else
        #error "TL_DEBUG_BREAK not implementeed for the current platform"
    #endif

    #define TL_EXPAND_MACRO(x) x
    #define TL_STRINGIFY_MACRO(x) #x

    #define TL_INTERNAL_ASSERT_WITH_MSG(type, check, ...)                 \
        {                                                                 \
            while (!(check))                                              \
            {                                                             \
                ::TL::ReportAssertionMessage(#check, __FILE__, __LINE__); \
                TL_DEBUG_BREAK();                                         \
            }                                                             \
        }

    #define TL_INTERNAL_ASSERT_NO_MSG(type, check, ...)                   \
        {                                                                 \
            while (!(check))                                              \
            {                                                             \
                ::TL::ReportAssertionMessage(#check, __FILE__, __LINE__); \
                ::TL::ReportAssertionMessage(__VA_ARGS__);                \
                TL_DEBUG_BREAK();                                         \
            }                                                             \
        }

    #define TL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define TL_INTERNAL_ASSERT_GET_MACRO(...) TL_EXPAND_MACRO(TL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, TL_INTERNAL_ASSERT_WITH_MSG, TL_INTERNAL_ASSERT_NO_MSG))

    #define TL_ASSERT(...) TL_EXPAND_MACRO(TL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
    #define TL_UNREACHABLE() TL_ASSERT(false, "Invalid code path")
#else
    #define TL_ASSERT(...)
    #define TL_UNREACHABLE()
#endif

namespace TL
{
    // @todo: Implement assertion message reporting after logging support is added

    inline static void ReportAssertionMessage(const char* expression, const char* file, int line)
    {
        (void)expression;
        (void)file;
        (void)line;
    }

    template<typename... Args>
    inline static void ReportAssertionMessage([[maybe_unused]] Args... args)
    {
    }
} // namespace TL

// @todo: This macro definition is out of place, move from here
#define TL_NODISCARD [[nodiscard]]