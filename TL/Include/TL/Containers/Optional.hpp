#pragma once

#include <optional>

namespace TL
{
    template<typename T>
    using Optional = std::optional<T>;
    using Nullopt_t = std::nullopt_t;
    inline static constexpr Nullopt_t Nullopt{std::nullopt};
} // namespace TL
