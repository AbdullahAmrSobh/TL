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

    TL_EXPORT Block Allocate(size_t size, size_t alignment);

    TL_EXPORT Block Reallocate(Block oldBlock, size_t alignment);

    TL_EXPORT void  Release(Block block, size_t alignment);

    // template<typename T>
    // static T* Allocate(size_t count = 1);

    // template<typename T>
    // static void Release(T* object, size_t count = 1);

    // template<typename T, typename... Args>
    // static T* Construct(Args&&... args);

    // template<typename T>
    // static void Destruct(T* object);

    // TODO: Move this section to inl file
    template<typename T>
    inline static T* Allocate(size_t count = 1)
    {
        size_t alignment = alignof(T);
        size_t size      = sizeof(T) * count;
        Block  block     = Allocate(size, alignment);
        return reinterpret_cast<T*>(block.ptr);
    }

    template<typename T>
    inline static void Release(T* object, size_t count = 1)
    {
        if (!object)
            return;
        size_t alignment = alignof(T);
        Block  block{reinterpret_cast<void*>(object), sizeof(T) * count};
        Release(block, alignment);
    }

    template<typename T, typename... Args>
    inline static T* Construct(Args&&... args)
    {
        T* ptr = Allocate<T>(1);
        if (ptr)
            new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    template<typename T>
    inline static void Destruct(T* object)
    {
        if (object)
        {
            object->~T();
            Release<T>(object, 1);
        }
    }

} // namespace TL

// Goals
// - No memory leaks
// - Report any leaks on exit
// - Allocator hooks for profiling
// - Log hooks
// -