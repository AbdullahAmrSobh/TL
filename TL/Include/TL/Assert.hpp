#pragma once

#if TL_DEBUG
    #include <TL/Log.hpp>

    #if defined(_WIN32)
        #define TL_DEBUG_BREAK() __debugbreak()
    #else
        #error "TL_DEBUG_BREAK not implementeed for the current platform"
    #endif

    #define TL_EXPAND(x) x

    #define TL_INTERNAL_ASSERT_WITH_MSG(type, check, ...)                                   \
        {                                                                                   \
            if (!(check))                                                                   \
            {                                                                               \
                TL_LOG_ERROR("expression: {} failed at {}:{}", #check, __FILE__, __LINE__); \
                TL_LOG_ERROR(__VA_ARGS__);                                                  \
                while (true)                                                                \
                {                                                                           \
                    TL_DEBUG_BREAK();                                                       \
                }                                                                           \
            }                                                                               \
        }

    #define TL_INTERNAL_ASSERT_NO_MSG(type, check)                                          \
        {                                                                                   \
            if (!(check))                                                                   \
            {                                                                               \
                TL_LOG_ERROR("expression: {} failed at {}:{}", #check, __FILE__, __LINE__); \
                while (true)                                                                \
                {                                                                           \
                    TL_DEBUG_BREAK();                                                       \
                }                                                                           \
            }                                                                               \
        }

    #define TL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define TL_INTERNAL_ASSERT_GET_MACRO(...) TL_EXPAND(TL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, TL_INTERNAL_ASSERT_WITH_MSG, TL_INTERNAL_ASSERT_NO_MSG))

    #define TL_ASSERT(...) TL_EXPAND(TL_EXPAND(TL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__))(_, __VA_ARGS__))
    #define TL_UNREACHABLE() TL_ASSERT(false, "Invalid code path")
#else
    #define TL_ASSERT(...)
    #define TL_UNREACHABLE()
#endif

// @todo: This macro definition is out of place, move from here
#define TL_NODISCARD [[nodiscard]]