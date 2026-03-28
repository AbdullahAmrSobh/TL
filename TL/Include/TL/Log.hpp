#pragma once

#include "TL/Export.hpp"

#include <fmt/format.h>

namespace TL
{
    enum class LogLevel
    {
        None,
        Debug,
        Info,
        Warn,
        Error,
    };

    TL_EXPORT void Log(LogLevel level, const char* message);

    TL_EXPORT void LogDebug(const char* message);
    TL_EXPORT void LogInfo(const char* message);
    TL_EXPORT void LogWarn(const char* message);
    TL_EXPORT void LogError(const char* message);

    template<typename... Args>
        requires (sizeof...(Args) > 0)
    void LogDebug(fmt::format_string<Args...> formatString, Args&&... args)
    {
        LogDebug(fmt::format(formatString, std::forward<Args>(args)...).c_str());
    }

    template<typename... Args>
        requires (sizeof...(Args) > 0)
    void LogInfo(fmt::format_string<Args...> formatString, Args&&... args)
    {
        LogInfo(fmt::format(formatString, std::forward<Args>(args)...).c_str());
    }

    template<typename... Args>
        requires (sizeof...(Args) > 0)
    void LogWarn(fmt::format_string<Args...> formatString, Args&&... args)
    {
        LogWarn(fmt::format(formatString, std::forward<Args>(args)...).c_str());
    }

    template<typename... Args>
        requires (sizeof...(Args) > 0)
    void LogError(fmt::format_string<Args...> formatString, Args&&... args)
    {
        LogError(fmt::format(formatString, std::forward<Args>(args)...).c_str());
    }

} // namespace TL
