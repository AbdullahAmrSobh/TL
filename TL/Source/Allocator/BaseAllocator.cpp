#include "TL/Allocator/BaseAllocator.hpp"

namespace TL
{
    Block BaseAllocator::Allocate(size_t size, size_t alignment)
    {
        return Allocator::Allocate(size, alignment);
    }

    Block BaseAllocator::Reallocate(Block block, size_t size, size_t alignment)
    {
        return Allocator::Reallocate(block, size, alignment);
    }

    void BaseAllocator::Release(Block block)
    {
        Allocator::Release(block);
    }
} // namespace TL