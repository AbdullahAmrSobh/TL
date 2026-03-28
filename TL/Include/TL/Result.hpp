#pragma once

#include <TL/Assert.hpp>
#include <TL/Containers/String.hpp>
#include <TL/Containers/StringView.hpp>

#include <type_traits>

namespace TL
{
    struct NoError_T
    {
    };

    /// @brief A class representing an error state, optionally storing an error message.
    class Error
    {
    public:
        /// @brief Constructs a success state (no error).
        Error()
            : m_success(true)
        {
        }

        Error(NoError_T)
            : m_success(true)
        {
        }

        /// @brief Constructs an error state with a message.
        explicit Error(std::string msg)
            : m_success(false)
            , m_message(std::move(msg))
        {
        }

        explicit Error(const char* fmt)
            : m_success(false)
            , m_message(fmt)
        {
        }

        explicit Error(TL::StringView fmt)
            : m_success(false)
            , m_message(fmt)
        {
        }

        /// @brief Checks if the result indicates success.
        /// @return true if successful, false otherwise.
        inline bool        IsSuccess() const { return m_success; }

        /// @brief Checks if the result indicates an error.
        /// @return true if an error occurred, false otherwise.
        inline bool        IsError() const { return !m_success; }

        /// @brief Retrieves the error message.
        /// @return The error message, or an empty string if there’s no error.
        inline const char* GetMessage() const { return m_message.c_str(); }

    private:
        bool       m_success; ///< Indicates whether the operation was successful.
        TL::String m_message; ///< Stores the error message if an error occurred.
    };

    static constexpr NoError_T NoError = {};

    /// @brief Helper functions for handling enum-based result codes.
    template<typename Enum>
    constexpr bool IsSuccess(Enum code)
    {
        return code == Enum::Success;
    }

    template<typename Enum>
    constexpr bool IsError(Enum code)
    {
        return !IsSuccess(code);
    }

    /// @brief A templated structure that holds a result value and a corresponding result code.
    /// @tparam Value The type of the value.
    /// @tparam ResultCode The type of the result code (either an enum class or an Error object).
    template<typename Value, typename ErrorType = Error>
    struct TL_NODISCARD Result
    {
        /// @brief Constructs a result with a result code and a default value.
        /// @param code The result code.
        Result(ErrorType code)
            : value()
            , result(code)
        {
        }

        /// @brief Constructs a result with a value and assumes success.
        /// @param t The value to store.
        Result(Value t)
            : value(t)
            , result(ErrorType())
        {
        }

        /// @brief Constructs a result with both a value and a result code.
        /// @param t The value to store.
        /// @param code The result code.
        Result(Value t, ErrorType code)
            : value(t)
            , result(code)
        {
        }

        Value       value;  ///< The result value.
        ErrorType   result; ///< The result code.

        /// @brief Checks if the result indicates success.
        /// @return true if the result is `ResultCode::Success`, false otherwise.
        inline bool IsSuccess() const
        {
            if constexpr (std::is_enum_v<ErrorType>)
            {
                return ::TL::IsSuccess(result);
            }
            else
            {
                return result.IsSuccess();
            }
        }

        /// @brief Checks if the result indicates an error.
        /// @return true if the result is not `ResultCode::Success`, false otherwise.
        inline bool IsError() const
        {
            return !IsSuccess();
        }

        /// @brief Retrieves the result value, asserting that the result was successful.
        /// @return The result value.
        inline Value GetValue()
        {
            TL_ASSERT(IsSuccess());
            return std::move(value);
        }
    };
} // namespace TL

template<typename Expr, typename Err>
inline static Err TL_ERR_FROM_EXPR(Expr);

template<typename Expr, typename Err>
inline static Err TL_ERR_FROM_EXPR(Expr);

#define TL_TRY(expr)                                                                                                      \
    {                                                                                                                     \
        if (auto err = TL_ERR_FROM_EXPR(expr))                                                                            \
        {                                                                                                                 \
            /* need to add a cleanup mechanisim; e.g. call .Shutdown or iterate over all objects and call shutdown ... */ \
            return err;                                                                                                   \
        }                                                                                                                 \
        else                                                                                                              \
        {                                                                                                                 \
            (void)expr;                                                                                                   \
        }                                                                                                                 \
    }

#define TL_TRY_MSG(expr, ...)                  \
    {                                          \
        if (auto err = TL_ERR_FROM_EXPR(expr)) \
        {                                      \
            ::TL::LogError(__VA_ARGS__);       \
            return err;                        \
        }                                      \
        else                                   \
        {                                      \
            (void)expr;                        \
        }                                      \
    }
