#pragma once

#include <format>

#include "TL/Export.hpp"
#include "TL/String.hpp"
#include "TL/Containers.hpp"

namespace std
{
    template<>
    struct formatter<TL::StringView, char>
    {
        std::formatter<std::string_view, char> underlying;

        constexpr auto                         parse(format_parse_context& ctx)
        {
            return underlying.parse(ctx);
        }

        template<typename FormatContext>
        auto format(const TL::StringView& sv, FormatContext& ctx) const
        {
            return underlying.format(std::string_view(sv.data(), sv.size()), ctx);
        }
    };

    template<>
    struct formatter<TL::String, char>
    {
        std::formatter<std::string_view, char> underlying;

        constexpr auto                         parse(format_parse_context& ctx)
        {
            return underlying.parse(ctx);
        }

        template<typename FormatContext>
        auto format(const TL::String& s, FormatContext& ctx) const
        {
            return underlying.format(std::string_view(s.data(), s.size()), ctx);
        }
    };
} // namespace std
