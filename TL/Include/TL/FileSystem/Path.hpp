#pragma once

#include <TL/String.hpp>

namespace TL::Path
{
    static TL::String sanitize(TL::StringView path, TL::IAllocator* allocator);
    static TL::String parent(TL::StringView path, TL::IAllocator* allocator);
    static TL::String absolute(TL::StringView path, TL::IAllocator* allocator);

    static bool       isFile(TL::StringView path, TL::IAllocator* allocator);
    static bool       isDirectory(TL::StringView path, TL::IAllocator* allocator);
    static bool       isValid(TL::StringView path, TL::IAllocator* allocator);

    static bool       copy();
    static bool       move();
} // namespace TL::Path