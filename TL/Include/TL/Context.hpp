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
        static Context* get();

        // Push new context on top of the stack and return the previous one
        static Context* push(Context* ctx);

        // Pop and return the current context
        static Context* pop();

        // Get current context's active allocator
        IAllocator* getAllocator() const { return m_allocator; }

    private:
        IAllocator* m_allocator = nullptr;
    };

    // -------------------------------------------------------------------------
    // Allocation Helpers (Consistent with IAllocator Interface)
    // -------------------------------------------------------------------------

    // Allocate with explicit allocator
    inline Block allocate(IAllocator* allocator, size_t size, size_t alignment)
    {
        return allocator->allocate(size, alignment);
    }

    // Allocate with current context
    inline Block allocate(size_t size, size_t alignment)
    {
        return allocate(Context::get()->getAllocator(), size, alignment);
    }

    // Free with explicit allocator
    inline void free(IAllocator* allocator, Block block, size_t alignment)
    {
        allocator->free(block, alignment);
    }

    // Free with current context
    inline void free(Block block, size_t alignment)
    {
        free(Context::get()->getAllocator(), block, alignment);
    }

    // -------------------------------------------------------------------------
    // Typed Allocation Helpers
    // -------------------------------------------------------------------------

    template<typename T>
    inline T* allocate(IAllocator* allocator, size_t count = 1)
    {
        size_t alignment = alignof(T);
        size_t size = sizeof(T) * count;
        Block block = allocate(allocator, size, alignment);
        return reinterpret_cast<T*>(block.ptr);
    }

    template<typename T>
    inline T* allocate(size_t count = 1)
    {
        return allocate<T>(Context::get()->getAllocator(), count);
    }

    template<typename T>
    inline void free(IAllocator* allocator, T* object, size_t count = 1)
    {
        if (!object)
            return;
        size_t alignment = alignof(T);
        Block block{ reinterpret_cast<void*>(object), sizeof(T) * count };
        free(allocator, block, alignment);
    }

    template<typename T>
    inline void free(T* object, size_t count = 1)
    {
        free<T>(Context::get()->getAllocator(), object, count);
    }

    // -------------------------------------------------------------------------
    // Construction Helpers
    // -------------------------------------------------------------------------

    template<typename T, typename... Args>
    inline T* constructFrom(IAllocator* allocator, Args&&... args)
    {
        T* ptr = allocate<T>(allocator, 1);
        if (ptr)
            new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    template<typename T, typename... Args>
    inline T* construct(Args&&... args)
    {
        return constructFrom<T>(Context::get()->getAllocator(), std::forward<Args>(args)...);
    }

    template<typename T>
    inline void destructFrom(IAllocator* allocator, T* object)
    {
        if (object)
        {
            object->~T();
            free<T>(allocator, object, 1);
        }
    }

    template<typename T>
    inline void destruct(T* object)
    {
        destructFrom(Context::get()->getAllocator(), object);
    }

} // namespace TL
