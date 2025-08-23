#pragma once

#include "TL/Export.hpp"
#include "TL/Block.hpp"

namespace TL
{
    /// Reading: (https://www.foonathan.net/2022/08/malloc-interface/)

    template<typename T>
    concept CAllocator = requires(T a, size_t size, size_t alignment, Block block, size_t newSize)
    {
        { a.Allocate(size, alignment) } -> std::same_as<Block>;
        { a.Reallocate(block, newSize, alignment) } -> std::same_as<Block>;
        { a.Release(block, alignment) } -> std::same_as<void>;
        { a == a } -> std::same_as<bool>;
        { a != a } -> std::same_as<bool>;
    };

    // Polymorphic allocator interface used by TL
    class TL_EXPORT IAllocator
    {
    public:
        virtual ~IAllocator() = default;

        /// Allocates a block of memory with the given size and alignment.
        Block Allocate(size_t size, size_t alignment)
        {
            return AllocateImpl(size, alignment);
        }

        /// Reallocates a block of memory, return the block with the same ptr if it succeeded (with modified size)
        Block Reallocate(Block block, size_t newSize, size_t alignment)
        {
            return ReallocateImpl(block, newSize, alignment);
        }

        /// Releases a previously allocated sblock of memory.
        void Release(Block block, size_t alignment)
        {
            return ReleaseImpl(block, alignment);
        }

        /// @fixme: make this pure function
        virtual bool operator==(TL_MAYBE_UNUSED const IAllocator& other) const noexcept { return false; }

        virtual bool operator!=(TL_MAYBE_UNUSED const IAllocator& other) const noexcept { return false; }

    protected:
        virtual Block AllocateImpl(size_t size, size_t alignment)                   = 0;
        virtual Block ReallocateImpl(Block block, size_t newSize, size_t alignment) = 0;
        virtual void  ReleaseImpl(Block block, size_t alignment)                    = 0;
    };
} // namespace TL
