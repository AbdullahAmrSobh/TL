#pragma once

#include <array>

namespace TL
{
    template<typename T, uint32_t COUNT>
    using Array = std::array<T, COUNT>;
} // namespace TL
