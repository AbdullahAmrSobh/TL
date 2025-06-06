#pragma once

#include "TL/Export.hpp"
#include "TL/Block.hpp"

#include <cstddef>

/// This is based on the awesome blog post (https://www.foonathan.net/2022/08/malloc-interface/)

namespace TL
{
    class TL_EXPORT IAllocator
    {
    public:
        virtual ~IAllocator() = default;

        /// Allocates memory for an array of objects of type T.
        template<typename T>
        T*    Allocate(size_t count = 1);

        /// Allocates a block of memory with the given size and alignment.
        Block Allocate(size_t size, size_t alignment)
        {
            return AllocateImpl(size, alignment);
        }

        /// Reallocates a block of memory ...
        // Block Reallocate(Block block, size_t alignment) { return {} }

        /// Releases a previously allocated block of memory.
        void Release(Block block, size_t alignment)
        {
            return ReleaseImpl(block, alignment);
        }

        /// Releases memory for an array of objects of type T.
        template<typename T>
        void Release(T* object, size_t count = 1);

        /// Constructs an object of type T in allocated memory.
        template<typename T, typename... Args>
        T* Construct(Args&&... args);

        /// Destructs an object of type T and releases its memory.
        template<typename T>
        void         Destruct(T* object);

        /// @fixme: make this pure function
        virtual bool operator==(TL_MAYBE_UNUSED const IAllocator& other) const noexcept { return false; }

        virtual bool operator!=(TL_MAYBE_UNUSED const IAllocator& other) const noexcept { return false; }

    protected:
        virtual Block AllocateImpl(size_t size, size_t alignment) = 0;
        virtual void  ReleaseImpl(Block block, size_t alignment)  = 0;

        // Will try to expand the memory inplace, before it attempts to reallocate. Usefully for e.g. containers which can omit calling the move constructor
        // virtual Result<bool, Block> GrowImpl(size_t newSize, size_t alignment) = 0;
    };

    template<typename T>
    inline T* IAllocator::Allocate(size_t count)
    {
        size_t size      = sizeof(T) * count;
        size_t alignment = alignof(T);
        Block  block     = Allocate(size, alignment);
        return static_cast<T*>(block.ptr);
    }

    template<typename T>
    inline void IAllocator::Release(T* object, size_t count)
    {
        if (object)
        {
            size_t size      = sizeof(T) * count;
            size_t alignment = alignof(T);
            Release(Block{object, size}, alignment);
        }
    }

    template<typename T, typename... Args>
    inline T* IAllocator::Construct(Args&&... args)
    {
        T* ptr = Allocate<T>(1);
        if (ptr)
            new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    template<typename T>
    inline void IAllocator::Destruct(T* object)
    {
        if (object)
        {
            object->~T();
            Release<T>(object, 1);
        }
    }

    TL_EXPORT void _OnAllocate(TL::Block block);
    TL_EXPORT void _OnRelease(TL::Block block);

} // namespace TL
