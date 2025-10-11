#pragma once

#include <variant>

namespace TL
{
    template<typename... Types>
    using Variant = std::variant<Types...>;
} // namespace TL
