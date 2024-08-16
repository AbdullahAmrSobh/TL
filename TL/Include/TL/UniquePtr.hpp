#pragma once

#include <memory>

#include <TL/Containers.hpp>

namespace TL
{
    // @todo: should rename to UniquePtr?

    // @todo: expand this (should not be just an alias)

    template<typename T>
    using Ptr = std::unique_ptr<T>;

    template<typename T>
    using SharedRef = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<class T, class... Args>
    inline constexpr Ptr<T> CreatePtr(Args... args)
    {
        return std::make_unique<T, Args...>(std::forward<Args>(args)...);
    }

    template<typename T, typename U, typename... Args>
    inline constexpr T* EmplacePtr(TL::Vector<Ptr<U>>& container, Args... args)
    {
        return (T*)container.emplace_back(CreatePtr<T>(args...)).get();
    }
} // namespace TL