#pragma once

#include "TL/Export.hpp"

#include <format>

#define TL_LOG_DEBUG(...) ::TL::Logger::Debug(__VA_ARGS__)
#define TL_LOG_INFO(...) ::TL::Logger::Info(__VA_ARGS__)
#define TL_LOG_WARNNING(...) ::TL::Logger::Warnning(__VA_ARGS__)
#define TL_LOG_ERROR(...) ::TL::Logger::Error(__VA_ARGS__)

namespace TL
{
    namespace Logger
    {
        // @todo: revist this

        enum class Level
        {
            None,
            Debug,
            Info,
            Warnning,
            Error,
        };

        class Stream;

        TL_EXPORT Stream& GetStdCoutStream();

        TL_EXPORT void Log(Stream& stream, Level level, const char* message);

        inline static void Debug(Stream& stream, const char* message)
        {
            Log(stream, Level::Debug, message);
        }

        inline static void Debug(const char* message)
        {
            Debug(GetStdCoutStream(), message);
        }

        template<typename... Args>
        inline static void Debug(const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Info(message.c_str());
        }

        template<typename... Args>
        inline static void Debug(Stream& stream, const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Info(stream, message.c_str());
        }

        inline static void Info(Stream& stream, const char* message)
        {
            Log(stream, Level::Info, message);
        }

        inline static void Info(const char* message)
        {
            Info(GetStdCoutStream(), message);
        }

        template<typename... Args>
        inline static void Info(const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Info(message.c_str());
        }

        template<typename... Args>
        inline static void Info(Stream& stream, const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Info(stream, message.c_str());
        }

        inline static void Warnning(Stream& stream, const char* message)
        {
            Log(stream, Level::Warnning, message);
        }

        inline static void Warnning(const char* message)
        {
            Warnning(GetStdCoutStream(), message);
        }

        template<typename... Args>
        inline static void Warnning(const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Warnning(message.c_str());
        }

        template<typename... Args>
        inline static void Warnning(Stream& stream, const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Warnning(stream, message.c_str());
        }

        inline static void Error(Stream& stream, const char* message)
        {
            Log(stream, Level::Error, message);
        }

        inline static void Error(const char* message)
        {
            Error(GetStdCoutStream(), message);
        }

        template<typename... Args>
        inline static void Error(const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Error(message.c_str());
        }

        template<typename... Args>
        inline static void Error(Stream& stream, const char* formatString, Args&&... args)
        {
            std::string message = std::vformat(formatString, std::make_format_args(args...));
            Error(stream, message.c_str());
        }

    } // namespace Logger
} // namespace TL
