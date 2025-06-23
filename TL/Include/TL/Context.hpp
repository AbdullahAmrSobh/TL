#pragma once

#include "TL/Export.hpp"
#include "TL/Block.hpp"
#include "TL/Allocator/Allocator.hpp"

namespace TL
{
    class TL_EXPORT Context
    {
    public:
        Context() = default;

        Context(IAllocator* allocator)
            : m_allocator(allocator)
        {
        }

        // Get current active context
        static Context*   Get();

        // Push new context on top of the stack and return current
        static Context*   Push(Context* ctx);

        // Pop and return current context
        static Context*   Pop();

        // Get current context active allocator
        class IAllocator* GetAllocator() const { return m_allocator; }

    private:
        IAllocator* m_allocator = nullptr;
    };

    // Allocate with explicit allocator
    inline static Block Allocate(IAllocator* allocator, size_t size, size_t alignment)
    {
        return allocator->Allocate(size, alignment);
    }

    // Allocate with current context
    inline static Block Allocate(size_t size, size_t alignment)
    {
        return Allocate(Context::Get()->GetAllocator(), size, alignment);
    }

    // Reallocate with explicit allocator
    inline static Block Reallocate(IAllocator* allocator, Block block, size_t newSize, size_t alignment)
    {
        return allocator->Reallocate(block, newSize, alignment);
    }

    // Reallocate with current context
    inline static Block Reallocate(Block block, size_t newSize, size_t alignment)
    {
        return Reallocate(Context::Get()->GetAllocator(), block, newSize, alignment);
    }

    // Release with explicit allocator
    inline static void Release(IAllocator* allocator, Block block, size_t alignment)
    {
        allocator->Release(block, alignment);
    }

    // Release with current context
    inline static void Release(Block block, size_t alignment)
    {
        Release(Context::Get()->GetAllocator(), block, alignment);
    }

    // Allocate typed with explicit allocator
    template<typename T>
    inline static T* Allocate(IAllocator* allocator, size_t count = 1)
    {
        size_t alignment = alignof(T);
        size_t size      = sizeof(T) * count;
        Block  block     = Allocate(allocator, size, alignment);
        return reinterpret_cast<T*>(block.ptr);
    }

    // Allocate typed with current context
    template<typename T>
    inline static T* Allocate(size_t count = 1)
    {
        return Allocate<T>(Context::Get()->GetAllocator(), count);
    }

    // Release typed with explicit allocator
    template<typename T>
    inline static void Release(IAllocator* allocator, T* object, size_t count = 1)
    {
        if (!object)
            return;
        size_t alignment = alignof(T);
        Block  block{reinterpret_cast<void*>(object), sizeof(T) * count};
        Release(allocator, block, alignment);
    }

    // Release typed with current context
    template<typename T>
    inline static void Release(T* object, size_t count = 1)
    {
        Release<T>(Context::Get()->GetAllocator(), object, count);
    }

    // Construct with explicit allocator
    template<typename T, typename... Args>
    inline static T* ConstructFrom(IAllocator* allocator, Args&&... args)
    {
        T* ptr = Allocate<T>(allocator, 1);
        if (ptr)
            new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    // Construct with current context
    template<typename T, typename... Args>
    inline static T* Construct(Args&&... args)
    {
        return ConstructFrom<T>(Context::Get()->GetAllocator(), std::forward<Args>(args)...);
    }

    // Destruct with explicit allocator
    template<typename T>
    inline static void Destruct(IAllocator* allocator, T* object)
    {
        if (object)
        {
            object->~T();
            Release<T>(allocator, object, 1);
        }
    }

    // Destruct with current context
    template<typename T>
    inline static void Destruct(T* object)
    {
        Destruct<T>(Context::Get()->GetAllocator(), object);
    }

} // namespace TL
