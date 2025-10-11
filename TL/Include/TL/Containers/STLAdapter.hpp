#pragma once

#include "TL/Context.hpp"
#include "TL/Allocator/Allocator.hpp"

#include <cstddef>
#include <type_traits>

namespace TL
{
    // Adapter to make TL allocators usable with STL containers
    template<typename T, typename AllocatorType = IAllocator>
    class StlAllocatorAdapter
    {
    public:
        static_assert(!std::is_const_v<T>, "STL forbids allocators for const types (allocator<const T> is ill-formed).");
        static_assert(!std::is_function_v<T>, "STL forbids allocators for function types.");
        static_assert(!std::is_reference_v<T>, "STL forbids allocators for reference types.");

        using value_type      = T;
        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer         = T*;
        using const_pointer   = const T*;

        constexpr StlAllocatorAdapter() noexcept
            : m_allocator(TL::Context::get() ? TL::Context::get()->getAllocator() : nullptr)
        {
        }

        constexpr StlAllocatorAdapter(AllocatorType& allocator) noexcept
            : m_allocator(&allocator)
        {
        }

        template<typename U>
        constexpr StlAllocatorAdapter(const StlAllocatorAdapter<U, AllocatorType>& other) noexcept
            : m_allocator(other.m_allocator)
        {
        }

        // Allocate memory for n objects of type T
        [[nodiscard]] T* allocate(std::size_t n)
        {
            static_assert(sizeof(T) > 0, "value_type must be complete before allocation.");
            if constexpr (std::is_same_v<AllocatorType, IAllocator>)
            {
                return TL::allocate<T>(m_allocator, n);
            }
            else
            {
                return AllocatorType::template allocate<T>(n);
            }
        }

        // Deallocate memory for n objects of type T
        void deallocate(T* ptr, std::size_t n) noexcept
        {
            if (!ptr)
                return;

            if constexpr (std::is_same_v<AllocatorType, IAllocator>)
            {
                TL::free<T>(m_allocator, ptr, n);
            }
            else
            {
                AllocatorType::free(ptr, n);
            }
        }

        // Rebind allocator to another type
        template<typename U>
        struct rebind
        {
            using other = StlAllocatorAdapter<U, AllocatorType>;
        };

        // Comparison operators (STL requires these)
        template<typename OtherAllocType>
        bool operator==(const StlAllocatorAdapter<T, OtherAllocType>& other) const noexcept
        {
            return m_allocator == other.m_allocator;
        }

        template<typename OtherAllocType>
        bool operator!=(const StlAllocatorAdapter<T, OtherAllocType>& other) const noexcept
        {
            return !(*this == other);
        }

    // private:
    //     template<typename T, typename T2>
    //     friend class StlAllocatorAdapter<T, T2>;
        AllocatorType* m_allocator = TL::Context::get()->getAllocator();
    };

} // namespace TL
