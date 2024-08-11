#pragma once

#if TL_DEBUG
    #include <TL/Log.hpp>

    #if defined(_WIN32)
        #define TL_DEBUG_BREAK() __debugbreak()
    #else
        #error "TL_DEBUG_BREAK not implemented for the current platform"
    #endif

    #define TL_EXPAND(x) x

    #define TL_INTERNAL_ASSERT_WITH_MSG(expression, ...)                                         \
        {                                                                                        \
            if (!(expression))                                                                   \
            {                                                                                    \
                TL_LOG_ERROR("expression: {} failed at {}:{}", #expression, __FILE__, __LINE__); \
                TL_LOG_ERROR(__VA_ARGS__);                                                       \
                while (true)                                                                     \
                {                                                                                \
                    TL_DEBUG_BREAK();                                                            \
                }                                                                                \
            }                                                                                    \
        }

    #define TL_INTERNAL_ASSERT_NO_MSG(_, expression)                                             \
        {                                                                                        \
            if (!(expression))                                                                   \
            {                                                                                    \
                TL_LOG_ERROR("expression: {} failed at {}:{}", #expression, __FILE__, __LINE__); \
                while (true)                                                                     \
                {                                                                                \
                    TL_DEBUG_BREAK();                                                            \
                }                                                                                \
            }                                                                                    \
        }

    #define TL_ASSERT(...) TL_ASSERT_CHOOSE(__VA_ARGS__, TL_ASSERT_3, TL_ASSERT_2, TL_ASSERT_1)(__VA_ARGS__)

    #define TL_ASSERT_CHOOSE(_1, _2, _3, NAME, ...) NAME

    #define TL_ASSERT_1(expression) TL_INTERNAL_ASSERT_NO_MSG(_, expression)

    #define TL_ASSERT_2(expression, message) TL_INTERNAL_ASSERT_WITH_MSG(expression, message)

    #define TL_ASSERT_3(expression, message, ...) TL_INTERNAL_ASSERT_WITH_MSG(expression, message, __VA_ARGS__)

    // TODO: support varags
    #define TL_UNREACHABLE() TL_ASSERT(false)

#else
    #define TL_ASSERT(...)
    #define TL_UNREACHABLE()
#endif

// @todo: This macro definition is out of place, move from here
#define TL_NODISCARD [[nodiscard]]