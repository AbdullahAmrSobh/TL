#include "Tl/Allocator.hpp"

// #include "LeakTracker.inl"

#include <mimalloc.h>

#if defined(TRACY_ENABLED) && TL_TRACY_MEMORY_TRACKER == 1
    #include <Tracy/Tracy.hpp>
    #define TL_TRACY_MEMORY_TRACKING
#endif

#define TL_ALLOCATOR DefaultAllocator
// #define TL_ALLOCATOR LeakTracker

namespace TL
{
    Block Allocator::Allocate(size_t size, size_t alignment)
    {
        auto block = TL_ALLOCATOR::Allocate(size, alignment);
#ifdef TL_TRACY_MEMORY_TRACKING
        TracyAllocS(block.ptr, 1, 20);
#endif
        return block;
    }

    Block Allocator::Reallocate(Block block, size_t size, size_t alignment)
    {
        return TL_ALLOCATOR::Reallocate(block, size, alignment);
    }

    void Allocator::Release(Block block)
    {
        TL_ALLOCATOR::Release(block);

#ifdef TL_TRACY_MEMORY_TRACKING
        TracyFreeS(block.ptr, 20);
#endif
    }

    // Default allocator, implemented on top of mimalloc

    Block DefaultAllocator::Allocate(size_t size, size_t alignment)
    {
        return { mi_malloc_aligned(size, alignment), size };
    }

    Block DefaultAllocator::Reallocate(Block block, size_t size, size_t alignment)
    {
        return { mi_realloc_aligned(block.ptr, size, alignment), size };
    }

    void DefaultAllocator::Release(Block block)
    {
        mi_free(block.ptr);
    }
} // namespace TL