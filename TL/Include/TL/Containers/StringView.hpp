#pragma once

#include <string_view>

namespace TL
{
    template<typename T>
    using BasicStringView = std::basic_string_view<T>;
    using StringView      = BasicStringView<char>;
    using WStringView     = BasicStringView<wchar_t>;
    using U8stringView    = BasicStringView<char8_t>;
    using U16stringView   = BasicStringView<char16_t>;
    using U32stringView   = BasicStringView<char32_t>;
} // namespace TL