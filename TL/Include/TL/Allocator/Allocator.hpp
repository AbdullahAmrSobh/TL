#pragma once

#include "TL/Export.hpp"
#include "TL/Block.hpp"

#include <cstddef>

/// This is based on the awesome blog post (https://www.foonathan.net/2022/08/malloc-interface/)

namespace TL
{
    class TL_EXPORT Allocator
    {
    public:
        /// Allocates a block of memory with the given size and alignment.
        static Block Allocate(size_t size, size_t alignment);

        /// Releases a previously allocated block of memory.
        static void  Release(Block block, size_t alignment);

        /// Allocates memory for an array of objects of type T.
        template<typename T>
        static T* Allocate(size_t count = 1);

        /// Constructs an object of type T in allocated memory.
        template<typename T, typename... Args>
        static T* Construct(Args&&... args);

        /// Releases memory for an array of objects of type T.
        template<typename T>
        static void Release(T* object, size_t count = 1);

        /// Destructs an object of type T and releases its memory.
        template<typename T>
        static void Destruct(T* object);
    };

    class TL_EXPORT IAllocator
    {
    public:
        virtual ~IAllocator() = default;

        /// Allocates a block of memory with the given size and alignment.
        Block Allocate(size_t size, size_t alignment);

        /// Releases a previously allocated block of memory.
        void  Release(Block block, size_t alignment);

        /// Allocates memory for an array of objects of type T.
        template<typename T>
        T* Allocate(size_t count = 1);

        /// Constructs an object of type T in allocated memory.
        template<typename T, typename... Args>
        T* Construct(Args&&... args);

        /// Releases memory for an array of objects of type T.
        template<typename T>
        void Release(T* object, size_t count = 1);

        /// Destructs an object of type T and releases its memory.
        template<typename T>
        void Destruct(T* object);

        /// @fixme: make this pure function
        virtual bool operator==( [[maybe_unused]] const IAllocator& other) const noexcept { return false; }
        virtual bool operator!=( [[maybe_unused]] const IAllocator& other) const noexcept { return false; }

    protected:
        /// Internal implementation of allocation.
        virtual Block AllocateImpl(size_t size, size_t alignment) = 0;

        /// Internal implementation of release.
        virtual void  ReleaseImpl(Block block, size_t alignment) = 0;

        // Will try to expand the memory inplace, before it attempts to reallocate. Usefully for e.g. containers which can omit calling the move constructor
        // virtual Result<bool, Block> GrowImpl(size_t newSize, size_t alignment) = 0;
    };
} // namespace TL

#include "Allocator.inl"
