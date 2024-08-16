#pragma once

#include <concepts>

namespace TL
{
    template<typename T, typename Archive>
    concept Serializable = requires(T t, Archive& archive) {
        {
            t.Serialize(archive)
        } -> std::same_as<void>;
    };

    template<typename T, typename Archive>
    concept Deserializable = requires(T t, Archive& archive) {
        {
            t.Deserialize(archive)
        } -> std::same_as<void>;
    };
} // namespace TL