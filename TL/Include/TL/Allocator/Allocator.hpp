#pragma once

#include "TL/Export.hpp"
#include "TL/Block.hpp"

#include <cstddef>

/// this is based on the awasome blog post (https://www.foonathan.net/2022/08/malloc-interface/)

namespace TL
{
    class TL_EXPORT Allocator
    {
    public:
        static Block Allocate(size_t size, size_t alignment);

        static void Release(Block block, size_t alignment);

        template<typename T>
        inline static T* Allocate(size_t count = 1)
        {
            return (T*)Allocate(sizeof(T), alignof(T)).ptr;
        }

        template<typename T>
        inline static void Release(T* object, size_t count)
        {
            Release(Block{ object, sizeof(T) * count }, alignof(T));
        }
    };

    class TL_EXPORT IAllocator
    {
    public:
        virtual ~IAllocator() = default;

        inline Block Allocate(size_t size, size_t alignment)
        {
            return AllocateImpl(size, alignment);
        }

        inline void Release(Block block, size_t alignment)
        {
            ReleaseImpl(block, alignment);
        }

        template<typename T>
        inline T* Allocate(size_t count = 1)
        {
            return (T*)this->AllocateImpl(sizeof(T) * count, alignof(T)).ptr;
        }

        template<typename T>
        inline void Release(T* object, size_t count)
        {
            this->ReleaseImpl({ object, sizeof(T) * count });
        }

    protected:
        virtual Block AllocateImpl(size_t size, size_t alignment) = 0;
        virtual void ReleaseImpl(Block block, size_t alignment) = 0;
    };
} // namespace TL