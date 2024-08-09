#pragma once

#include <TL/Export.hpp>
#include <TL/Assert.hpp> // used for TL_NODISCARD

#include <cstddef>

namespace TL
{
    struct TL_NODISCARD Block
    {
        void* ptr;
        size_t size;
    };

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
} // namespace TL