#pragma once

#include "TL/Export.hpp"
#include "TL/Block.hpp"

#include <cstddef>

namespace TL
{
    class TL_EXPORT IAllocator
    {
    public:
        virtual ~IAllocator() = default;
        virtual Block Allocate(size_t size, size_t alignment = 1) = 0;
        virtual Block Reallocate(Block block, size_t size, size_t alignment = 1) = 0;
        virtual void Release(Block block) = 0;
    };

    class TL_EXPORT Allocator
    {
    public:
        static Block Allocate(size_t size, size_t alignment = 1);
        static Block Reallocate(Block block, size_t size, size_t alignment = 1);
        static void Release(Block block);
    };

    // Default allocator, implemented on top of mimalloc
    class TL_EXPORT DefaultAllocator
    {
    public:
        static Block Allocate(size_t size, size_t alignment = 1);
        static Block Reallocate(Block block, size_t size, size_t alignment = 1);
        static void Release(Block block);
    };
} // namespace TL