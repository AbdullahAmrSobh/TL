#pragma once

#include <fmt/format.h>

#include "TL/Containers/String.hpp"

namespace TL
{
    template<typename... Args>
    String fmt(::fmt::format_string<Args...> formatString, Args&&... args)
    {
        return ::fmt::format(formatString, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Print(::fmt::format_string<Args...> formatString, Args&&... args)
    {
        ::fmt::print(formatString, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Println(::fmt::format_string<Args...> formatString, Args&&... args)
    {
        ::fmt::println(formatString, std::forward<Args>(args)...);
    }
} // namespace TL
