#include "TL/Log.hpp"
#include "TL/Assert.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace TL
{
    void Log(LogLevel level, const char* message)
    {
        switch (level)
        {
        case LogLevel::None:  TL_UNREACHABLE(); break;
        case LogLevel::Debug: spdlog::debug(message); break;
        case LogLevel::Info:  spdlog::info(message); break;
        case LogLevel::Warn:  spdlog::warn(message); break;
        case LogLevel::Error: spdlog::error(message); break;
        }
    }

    void LogDebug(const char* message) { Log(LogLevel::Debug, message); }
    void LogInfo(const char* message)  { Log(LogLevel::Info, message); }
    void LogWarn(const char* message)  { Log(LogLevel::Warn, message); }
    void LogError(const char* message) { Log(LogLevel::Error, message); }
} // namespace TL
