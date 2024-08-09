#include "Tl/Allocator.hpp"
#include "Tl/Allocator/BaseAllocator.hpp"

namespace TL
{
    Block Allocator::Allocate(size_t size, size_t alignment)
    {
        return BaseAllocator().Allocate(size, alignment);
    }

    Block Allocator::Reallocate(Block block, size_t size, size_t alignment)
    {
        return BaseAllocator().Reallocate(block, size, alignment);
    }

    void Allocator::Release(Block block)
    {
        return BaseAllocator().Release(block);
    }

} // namespace TL