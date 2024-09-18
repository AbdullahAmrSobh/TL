#pragma once

#include <memory>
#include <utility>
#include <TL/Containers.hpp>

namespace TL
{
    // Custom deleter using Allocator
    template<typename T>
    struct AllocatorDeleter
    {
        void operator()(T* ptr) const
        {
            if (ptr)
            {
                ptr->~T();                  // Explicitly call the destructor
                Allocator::Release(ptr, 1); // Use the allocator to release memory
            }
        }
    };

    template<typename T>
    class Ptr : public std::unique_ptr<T, AllocatorDeleter<T>>
    {
    public:
        using Base = std::unique_ptr<T, AllocatorDeleter<T>>;

        // Inherit constructors from std::unique_ptr
        using Base::Base;

        // Implicit conversion from Ptr<Derived> to Ptr<Base>
        template<typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
        Ptr(Ptr<U>&& other) noexcept
            : Base(other.release(), AllocatorDeleter<T>())
        {
        }

        Ptr(const Ptr&)            = delete;
        Ptr& operator=(const Ptr&) = delete;

        Ptr(Ptr&& other) noexcept            = default;
        Ptr& operator=(Ptr&& other) noexcept = default;
    };

    template<typename T>
    using SharedRef = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<class T, class... Args>
    inline constexpr Ptr<T> CreatePtr(Args&&... args)
    {
        // Allocate memory through Allocator
        T* memory = Allocator::Allocate<T>(1);
        // Use placement new to construct the object in allocated memory
        new (memory) T(std::forward<Args>(args)...);
        // Return Ptr with custom deleter
        return Ptr<T>(memory);
    }

    template<typename T, typename U, typename... Args>
    inline constexpr T* EmplacePtr(TL::Vector<Ptr<U>>& container, Args&&... args)
    {
        return (T*)container.emplace_back(CreatePtr<T>(std::forward<Args>(args)...)).get();
    }
} // namespace TL
