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

#include "TL/Context.hpp"
#include "TL/Containers/String.hpp"


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

    template<typename T, typename AllocatorType = IAllocator>
    class StlAllocatorAdapter
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

        constexpr StlAllocatorAdapter(AllocatorType& allocator) noexcept
            : m_allocator(&allocator)
        {
        }

        template<typename U>
        constexpr StlAllocatorAdapter(const StlAllocatorAdapter<U, AllocatorType>& other) noexcept
            : m_allocator(other.m_allocator)
        {
        }

        TL_NODISCARD T* allocate(std::size_t n)
        {
            if constexpr (std::is_same_v<AllocatorType, IAllocator>)
            {
                return Allocate<T>(m_allocator, n);
            }
            else
            {
                static_assert(sizeof(value_type) > 0, "value_type must be complete before calling allocate.");
                return AllocatorType::template Allocate<T>(n);
            }
        }

        void deallocate(T* ptr, std::size_t n)
        {
            if constexpr (std::is_same_v<AllocatorType, IAllocator>)
            {
                Release<T>(m_allocator, ptr, n);
            }
            else
            {
                AllocatorType::Release(ptr, n);
            }
        }

        template<typename U>
        struct rebind
        {
            using other = StlAllocatorAdapter<U, AllocatorType>;
        };

        template<typename OtherAllocType>
        bool operator==(const OtherAllocType&) const noexcept
        {
            return false;
        }

        template<typename OtherAllocType>
        bool operator!=(const OtherAllocType&) const noexcept
        {
            return true;
        }

        AllocatorType* m_allocator = TL::Context::Get()->GetAllocator();
    };

    // STL Container Adapters
    template<typename T>
    using Vector = std::vector<T, StlAllocatorAdapter<T>>;

    template<typename T, std::size_t Capacity = 16, bool AllowExceed = false>
    using SmallVector = std::vector<T, StlAllocatorAdapter<T>>; // Custom implementation may be needed for small storage.

    template<typename Key, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using Set = std::unordered_set<Key, Hasher, KeyEq, StlAllocatorAdapter<Key>>;

    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    using Map = std::unordered_map<Key, Value, Hasher, KeyEq, StlAllocatorAdapter<std::pair<const Key, Value>>>;

    template<typename T>
    using Deque = std::deque<T, StlAllocatorAdapter<T>>;

    /// @fixme: revert this change back (needed because StlAllocatorAdapter does not support comparesion which emscripten needs)

    // Function wrapper
    template<typename T>
    using Function = std::function<T>;
} // namespace TL
