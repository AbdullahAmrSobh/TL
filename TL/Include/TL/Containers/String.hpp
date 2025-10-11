#pragma once

#include "TL/Containers/STLAdapter.hpp"

#include <string>

namespace TL
{
    template<typename char_type>
    using BasicString = std::basic_string<char_type>;
    using String      = BasicString<char>;     //, std::char_traits<char>, StlAllocatorAdapter<char, Allocator>>;
    using WString     = BasicString<wchar_t>;  //, std::char_traits<wchar_t>, StlAllocatorAdapter<wchar_t, Allocator>>;
    using U8string    = BasicString<char8_t>;  //, std::char_traits<char8_t>, StlAllocatorAdapter<char8_t, Allocator>>;
    using U16string   = BasicString<char16_t>; //, std::char_traits<char16_t>, StlAllocatorAdapter<char16_t, Allocator>>;
    using U32string   = BasicString<char32_t>; //, std::char_traits<char32_t>, StlAllocatorAdapter<char32_t, Allocator>>;
} // namespace TL