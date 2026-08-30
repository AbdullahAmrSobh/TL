#pragma once

#include "TL/Export.hpp"
#include "TL/Block.hpp"

namespace TL
{
    /// Reading: (https://www.foonathan.net/2022/08/malloc-interface/)

    // Polymorphic allocator interface used by TL
    class TL_EXPORT IAllocator
    {
    public:
        virtual ~IAllocator() = default;

        /// Allocates a block of memory with the given size and alignment.
        virtual Block allocate(size_t size, size_t alignment) = 0;

        /// Reallocates a block of memory, return the block with the same ptr if it succeeded (with modified size)
        virtual Block reallocate(Block block, size_t newSize, size_t alignment) = 0;

        /// Releases a previously allocated sblock of memory.
        virtual void free(Block block, size_t alignment) = 0;
    };
} // namespace TL
