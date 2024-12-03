#pragma once

#include <new>
#include "TL/Assert.hpp"

#include <type_traits>
#include <utility>

namespace TL
{
    // Allocator definitions
    template <typename T>
    T* Allocator::Allocate(size_t count)
    {
        return static_cast<T*>(Allocate(sizeof(T) * count, alignof(T)).ptr);
    }

    template <typename T, typename... Args>
    T* Allocator::Construct(Args&&... args)
    {
        static_assert(std::is_constructible_v<T, Args...>, "Type T must be constructible with the provided arguments.");
        void* memory = Allocate<T>();
        if (!memory) return nullptr;
        return new (memory) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void Allocator::Release(T* object, size_t count)
    {
        if (object)
            Release(Block{object, sizeof(T) * count}, alignof(T));
    }

    template <typename T>
    void Allocator::Destruct(T* object)
    {
        if (!object) return;
        object->~T();
        Release(object);
    }

    // IAllocator definitions
    inline Block IAllocator::Allocate(size_t size, size_t alignment)
    {
        return AllocateImpl(size, alignment);
    }

    inline void IAllocator::Release(Block block, size_t alignment)
    {
        ReleaseImpl(block, alignment);
    }

    template <typename T>
    T* IAllocator::Allocate(size_t count)
    {
        return static_cast<T*>(AllocateImpl(sizeof(T) * count, alignof(T)).ptr);
    }

    template <typename T, typename... Args>
    T* IAllocator::Construct(Args&&... args)
    {
        static_assert(std::is_constructible_v<T, Args...>, "Type T must be constructible with the provided arguments.");
        void* memory = Allocate<T>();
        if (!memory) return nullptr;
        return new (memory) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void IAllocator::Release(T* object, size_t count)
    {
        if (object)
            ReleaseImpl({object, sizeof(T) * count}, alignof(T));
    }

    template <typename T>
    void IAllocator::Destruct(T* object)
    {
        if (!object) return;
        object->~T();
        Release(object);
    }
} // namespace TL
