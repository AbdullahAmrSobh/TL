#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <deque>

#include "TL/Allocator/BaseAllocator.hpp"

namespace TL
{
    template<typename T>
    class ContainerDefaultAllocator
    {
    public:
        static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed.");
        static_assert(!std::is_function_v<T>, "The C++ Standard forbids allocators for function elements because of [allocator.requirements].");
        static_assert(!std::is_reference_v<T>, "The C++ Standard forbids allocators for reference elements because of [allocator.requirements].");

        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        constexpr ContainerDefaultAllocator() noexcept {}

        constexpr ContainerDefaultAllocator(const ContainerDefaultAllocator&) noexcept = default;

        template<class Other>
        constexpr ContainerDefaultAllocator(const ContainerDefaultAllocator<Other>&) noexcept;

        constexpr ~ContainerDefaultAllocator() = default;

        constexpr ContainerDefaultAllocator& operator=(const ContainerDefaultAllocator&) = default;

        TL_NODISCARD TL_EXPORT constexpr T* allocate(const size_t count);

        constexpr void deallocate(T* const ptr, const size_t count);
    };

    template<typename T>
    using Vector = std::vector<T, ContainerDefaultAllocator<T>>;

    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using UnorderedMap = std::unordered_map<Key, Value, Hasher, KeyEq, ContainerDefaultAllocator<std::pair<const Key, Value>>>;

    template<typename Key, typename Value>
    using Map = std::map<Key, Value, ContainerDefaultAllocator<std::pair<Key, Value>>>;

    template<typename Key, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using UnorderedSet = std::unordered_set<Key, Hasher, KeyEq, ContainerDefaultAllocator<Key>>;

    template<typename T>
    using Set = std::set<T, ContainerDefaultAllocator<T>>;

    template<typename T>
    using Deque = std::deque<T, ContainerDefaultAllocator<T>>;

    using String = std::basic_string<char, std::char_traits<char>, ContainerDefaultAllocator<char>>;
    using WString = std::basic_string<wchar_t, std::char_traits<wchar_t>, ContainerDefaultAllocator<wchar_t>>;
    using U8string = std::basic_string<char8_t, std::char_traits<char8_t>, ContainerDefaultAllocator<char8_t>>;
    using U16string = std::basic_string<char16_t, std::char_traits<char16_t>, ContainerDefaultAllocator<char16_t>>;
    using U32string = std::basic_string<char32_t, std::char_traits<char32_t>, ContainerDefaultAllocator<char32_t>>;

    template<typename T>
    template<class Other>
    constexpr ContainerDefaultAllocator<T>::ContainerDefaultAllocator(const ContainerDefaultAllocator<Other>&) noexcept
    {
    }

    template<typename T>
    constexpr T* ContainerDefaultAllocator<T>::allocate(const size_t count)
    {
        static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
        return (T*)BaseAllocator().Allocate(count * sizeof(T), alignof(T)).ptr;
    }

    template<typename T>
    constexpr void ContainerDefaultAllocator<T>::deallocate(T* const ptr, const size_t count)
    {
        TL_ASSERT(ptr != nullptr || count == 0);
        return BaseAllocator().Release({ ptr, sizeof(T) * count });
    }

} // namespace TL