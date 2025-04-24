#pragma once

namespace TL
{
    class String;
    class Allocator;

    template<typename... FMT_ARGS>
    inline static TL::String StringFmt(const char* fmt, FMT_ARGS... args);

    template<typename... FMT_ARGS>
    inline static TL::String StringFmt(Allocator* allocator, const char* fmt, FMT_ARGS... args);

} // namespace TL