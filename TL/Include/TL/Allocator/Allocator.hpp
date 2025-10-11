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
        Block allocate(size_t size, size_t alignment)
        {
            return allocateImpl(size, alignment);
        }

        /// Reallocates a block of memory, return the block with the same ptr if it succeeded (with modified size)
        Block reallocate(Block block, size_t newSize, size_t alignment)
        {
            return reallocateImpl(block, newSize, alignment);
        }

        /// Releases a previously allocated sblock of memory.
        void free(Block block, size_t alignment)
        {
            return freeImpl(block, alignment);
        }

        /// @fixme: make this pure function
        virtual bool operator==(TL_MAYBE_UNUSED const IAllocator& other) const noexcept { return false; }

        virtual bool operator!=(TL_MAYBE_UNUSED const IAllocator& other) const noexcept { return false; }

    protected:
        virtual Block allocateImpl(size_t size, size_t alignment)                   = 0;
        virtual Block reallocateImpl(Block block, size_t newSize, size_t alignment) = 0;
        virtual void  freeImpl(Block block, size_t alignment)                       = 0;
    };
} // namespace TL
