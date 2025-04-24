#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <deque>
#include <optional>
#include <variant>
#include <functional>

#include "TL/Memory.hpp"

namespace TL
{
    // Optional and Nullopt
    template<typename T>
    using Optional = std::optional<T>;

    using Nullopt_t = std::nullopt_t;
    inline static constexpr Nullopt_t Nullopt{std::nullopt};

    // Variant
    template<typename... Types>
    using Variant = std::variant<Types...>;

    template<typename T, typename AllocatorType = Allocator>
    class StlAllocatorAdapter;

    // STL Allocator Adapter
    template<typename T>
    class StlAllocatorAdapter<T, IAllocator>
    {
    public:
        static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed.");
        static_assert(!std::is_function_v<T>, "The C++ Standard forbids allocators for function elements because of [allocator.requirements].");
        static_assert(!std::is_reference_v<T>, "The C++ Standard forbids allocators for reference elements because of [allocator.requirements].");

        using value_type      = T;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer         = T*;
        using const_pointer   = const T*;

        constexpr StlAllocatorAdapter(IAllocator& allocator) noexcept
            : m_allocator(&allocator)
        {
        }

        template<typename U>
        constexpr StlAllocatorAdapter(const StlAllocatorAdapter<U, IAllocator>& other) noexcept
            : m_allocator(other.m_allocator)
        {
        }

        TL_NODISCARD T* allocate(std::size_t n)
        {
            return m_allocator->template Allocate<T>(n);
        }

        void deallocate(T* ptr, std::size_t n)
        {
            m_allocator->Release(ptr, n);
        }

        template<typename U>
        struct rebind
        {
            using other = StlAllocatorAdapter<U, IAllocator>;
        };

        // In the IAllocator specialization
        template<typename OtherAllocType>
        bool operator==(const OtherAllocType& other) const noexcept { return false; }

        template<typename OtherAllocType>
        bool operator!=(const OtherAllocType& other) const noexcept { return true; }

        IAllocator* m_allocator;
    };

    template<typename T>
    class StlAllocatorAdapter<T, Allocator>
    {
    public:
        static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed.");
        static_assert(!std::is_function_v<T>, "The C++ Standard forbids allocators for function elements because of [allocator.requirements].");
        static_assert(!std::is_reference_v<T>, "The C++ Standard forbids allocators for reference elements because of [allocator.requirements].");

        using value_type      = T;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer         = T*;
        using const_pointer   = const T*;

        constexpr StlAllocatorAdapter() noexcept = default;

        template<typename U>
        constexpr StlAllocatorAdapter(const StlAllocatorAdapter<U, Allocator>&) noexcept
        {
        }

        TL_NODISCARD T* allocate(std::size_t n)
        {
            static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
            return Allocator::Allocate<T>(n);
        }

        void deallocate(T* ptr, std::size_t n)
        {
            Allocator::Release(ptr, n);
        }

        template<typename U>
        struct rebind
        {
            using other = StlAllocatorAdapter<U, Allocator>;
        };
    };

    // STL Container Adapters
    template<typename T, typename AllocatorType = Allocator>
    using Vector = std::vector<T, StlAllocatorAdapter<T, AllocatorType>>;

    template<typename T, std::size_t Capacity = 16, bool AllowExceed = false>
    using SmallVector = std::vector<T, StlAllocatorAdapter<T>>; // Custom implementation may be needed for small storage.

    template<typename Key, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>, typename AllocatorType = Allocator>
    using Set = std::unordered_set<Key, Hasher, KeyEq, StlAllocatorAdapter<Key, AllocatorType>>;

    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>, typename AllocatorType = Allocator>
    using Map = std::unordered_map<Key, Value, Hasher, KeyEq, StlAllocatorAdapter<std::pair<const Key, Value>, AllocatorType>>;

    template<typename T, typename AllocatorType = Allocator>
    using Deque = std::deque<T, StlAllocatorAdapter<T, AllocatorType>>;

    /// @fixme: revert this change back (needed because StlAllocatorAdapter does not support comparesion which emscripten needs)

    // template<typename AllocatorType = Allocator>
    using String = std::basic_string<char>; //, std::char_traits<char>, StlAllocatorAdapter<char, Allocator>>;

    // template<typename AllocatorType = Allocator>
    using WString = std::basic_string<wchar_t>; //, std::char_traits<wchar_t>, StlAllocatorAdapter<wchar_t, Allocator>>;

    // template<typename AllocatorType = Allocator>
    using U8string = std::basic_string<char8_t>; //, std::char_traits<char8_t>, StlAllocatorAdapter<char8_t, Allocator>>;

    // template<typename AllocatorType = Allocator>
    using U16string = std::basic_string<char16_t>; //, std::char_traits<char16_t>, StlAllocatorAdapter<char16_t, Allocator>>;

    // template<typename AllocatorType = Allocator>
    using U32string = std::basic_string<char32_t>; //, std::char_traits<char32_t>, StlAllocatorAdapter<char32_t, Allocator>>;

    // Function wrapper
    template<typename T>
    using Function = std::function<T>;
} // namespace TL
