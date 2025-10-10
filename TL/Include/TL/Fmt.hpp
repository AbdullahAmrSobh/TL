#pragma once

#include <format>

#include "TL/Containers/String.hpp"

namespace std
{
    // Formatters for string types

    // Formatter for TL::String
    template<>
    struct formatter<TL::String, char>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::String& s, FormatContext& ctx) const
        {
            return std::copy(s.data(), s.data() + s.size(), ctx.out());
        }
    };

    // Formatter for TL::WString
    template<>
    struct formatter<TL::WString, wchar_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::WString& s, FormatContext& ctx) const
        {
            return std::copy(s.data(), s.data() + s.size(), ctx.out());
        }
    };

    // Formatter for TL::U8string
    template<>
    struct formatter<TL::U8string, char8_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::U8string& s, FormatContext& ctx) const
        {
            return std::copy(s.data(), s.data() + s.size(), ctx.out());
        }
    };

    // Formatter for TL::U16string
    template<>
    struct formatter<TL::U16string, char16_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::U16string& s, FormatContext& ctx) const
        {
            return std::copy(s.data(), s.data() + s.size(), ctx.out());
        }
    };

    // Formatter for TL::U32string
    template<>
    struct formatter<TL::U32string, char32_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::U32string& s, FormatContext& ctx) const
        {
            return std::copy(s.data(), s.data() + s.size(), ctx.out());
        }
    };

    // Formatter for TL::StringView
    template<>
    struct formatter<TL::StringView, char>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::StringView& sv, FormatContext& ctx) const
        {
            return std::copy(sv.data(), sv.data() + sv.size(), ctx.out());
        }
    };

    // Formatter for TL::WStringView
    template<>
    struct formatter<TL::WStringView, wchar_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::WStringView& sv, FormatContext& ctx) const
        {
            return std::copy(sv.data(), sv.data() + sv.size(), ctx.out());
        }
    };

    // Formatter for TL::U8stringView
    template<>
    struct formatter<TL::U8stringView, char8_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::U8stringView& sv, FormatContext& ctx) const
        {
            return std::copy(sv.data(), sv.data() + sv.size(), ctx.out());
        }
    };

    // Formatter for TL::U16stringView
    template<>
    struct formatter<TL::U16stringView, char16_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::U16stringView& sv, FormatContext& ctx) const
        {
            return std::copy(sv.data(), sv.data() + sv.size(), ctx.out());
        }
    };

    // Formatter for TL::U32stringView
    template<>
    struct formatter<TL::U32stringView, char32_t>
    {
        template<typename ParseContext>
        constexpr auto parse(ParseContext& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const TL::U32stringView& sv, FormatContext& ctx) const
        {
            return std::copy(sv.data(), sv.data() + sv.size(), ctx.out());
        }
    };

    // Formatter for ranged types

    // TODO!

    // Vector

    // Span

    // Map

    // Set

} // namespace std


namespace TL::FMT
{

}