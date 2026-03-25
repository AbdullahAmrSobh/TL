#pragma once

#include <memory>
#include <utility>

#include <TL/Containers/Vector.hpp>

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
                destruct(ptr);
            }
        }
    };

    template<typename T>
    using Ptr = std::unique_ptr<T, AllocatorDeleter<T>>;

    template<typename T>
    using SharedRef = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<typename T>
    using SharedRef = std::shared_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<class T, class... Args>
    inline constexpr Ptr<T> CreatePtr(Args&&... args)
    {
        return Ptr<T>(construct<T>(std::forward<Args>(args)...));
    }

    template<class T, class... Args>
    inline constexpr Ref<T> CreateRef(Args... args)
    {
        return std::make_shared<T, Args...>(std::forward<Args>(args)...);
    }

    template<typename T, typename U, typename... Args>
    inline constexpr T* EmplacePtr(TL::Vector<Ptr<U>>& container, Args... args)
    {
        return (T*)container.emplace_back(CreatePtr<T>(args...)).get();
    }

    template<typename T, typename U, typename... Args>
    inline constexpr T* EmplacePtr(TL::Vector<Ptr<U>>& container, Args&&... args)
    {
        return (T*)container.emplace_back(CreatePtr<T>(std::forward<Args>(args)...)).get();
    }

} // namespace TL
