#pragma once

#include <TL/Assert.hpp>

#include <TL/Containers/String.hpp>

#define TL_DEFINE_POD_HASH(Type)                        \
    template<>                                          \
    struct hash<Type>                                   \
    {                                                   \
        std::size_t operator()(const Type& value) const \
        {                                               \
            return ::TL::HashAny<Type>(value);          \
        }                                               \
    };

namespace TL
{
    template<typename T>
    inline static bool IsPow2(T x)
    {
        return (x & (x - 1)) == 0;
    }

    template<typename T>
    inline constexpr static T AlignUp(T val, T alignment)
    {
        TL_ASSERT(IsPow2(alignment));
        return (val + alignment - 1) & ~(alignment - 1);
    }

    template<typename T>
    inline constexpr static uint64_t HashAny(const T& data)
    {
        auto stream = TL::String(reinterpret_cast<const char*>(&data), sizeof(data));
        auto hasher = std::hash<TL::String>{};
        return hasher(stream);
    }

    inline constexpr static uint64_t HashCombine(uint64_t v1, uint64_t v2)
    {
        v1 ^= v2 + 0x9e3779b9 + (v1 << 6) + (v1 >> 2);
        return v1;
    }

    class NonCopyable
    {
    public:
        NonCopyable()                              = default;
        NonCopyable(const NonCopyable&)            = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
        ~NonCopyable()                             = default;
    };
} // namespace TL