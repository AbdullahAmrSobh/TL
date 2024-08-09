#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <deque>

#include "TL/Allocator.hpp"

namespace TL
{
    template<typename T>
    class StlAllocator
    {
    public:
        static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed.");
        static_assert(!std::is_function_v<T>, "The C++ Standard forbids allocators for function elements because of [allocator.requirements].");
        static_assert(!std::is_reference_v<T>, "The C++ Standard forbids allocators for reference elements because of [allocator.requirements].");

        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        constexpr StlAllocator() noexcept {}

        constexpr StlAllocator(const StlAllocator&) noexcept = default;

        template<class Other>
        constexpr StlAllocator(const StlAllocator<Other>&) noexcept;

        constexpr ~StlAllocator() = default;

        constexpr StlAllocator& operator=(const StlAllocator&) = default;

        TL_NODISCARD TL_EXPORT constexpr T* allocate(const size_t count);

        constexpr void deallocate(T* const ptr, const size_t count);
    };

    template<typename T>
    using Vector = std::vector<T, StlAllocator<T>>;

    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using UnorderedMap = std::unordered_map<Key, Value, Hasher, KeyEq, StlAllocator<std::pair<const Key, Value>>>;

    template<typename Key, typename Value>
    using Map = std::map<Key, Value, StlAllocator<std::pair<Key, Value>>>;

    template<typename Key, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using UnorderedSet = std::unordered_set<Key, Hasher, KeyEq, StlAllocator<Key>>;

    template<typename T>
    using Set = std::set<T, StlAllocator<T>>;

    template<typename T>
    using Deque = std::deque<T, StlAllocator<T>>;

    using String = std::basic_string<char, std::char_traits<char>, StlAllocator<char>>;
    using WString = std::basic_string<wchar_t, std::char_traits<wchar_t>, StlAllocator<wchar_t>>;
    using U8string = std::basic_string<char8_t, std::char_traits<char8_t>, StlAllocator<char8_t>>;
    using U16string = std::basic_string<char16_t, std::char_traits<char16_t>, StlAllocator<char16_t>>;
    using U32string = std::basic_string<char32_t, std::char_traits<char32_t>, StlAllocator<char32_t>>;

    template<typename T>
    template<class Other>
    constexpr StlAllocator<T>::StlAllocator(const StlAllocator<Other>&) noexcept
    {
    }

    template<typename T>
    constexpr T* StlAllocator<T>::allocate(const size_t count)
    {
        static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
        return (T*)Allocator::Allocate(count * sizeof(T), alignof(T)).ptr;
    }

    template<typename T>
    constexpr void StlAllocator<T>::deallocate(T* const ptr, const size_t count)
    {
        TL_ASSERT(ptr != nullptr || count == 0);
        return Allocator::Release({ ptr, sizeof(T) * count });
    }

} // namespace TL