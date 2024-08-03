#include "TL/Allocator/BaseAllocator.hpp"

#include <mimalloc.h>

#if defined(TRACY_ENABLED) && TL_TRACY_MEMORY_TRACKER == 1
    #include <Tracy/Tracy.hpp>
    #define TL_TRACY_MEMORY_TRACKING
#endif

namespace TL
{
    Block BaseAllocator::Allocate(size_t size, size_t alignment)
    {
        Block block;
        // if (alignment > 1)
            block = { mi_malloc_aligned(size, alignment), size };
        // else
        //     block = { mi_malloc_aligned(size, alignment), size };


#ifdef TL_TRACY_MEMORY_TRACKING
        TracyAllocS(block.ptr, 1, 20);
#endif

        return block;
    }

    Block BaseAllocator::Reallocate(Block block, const size_t size, size_t alignment)
    {
        return Block{ mi_realloc_aligned(block.ptr, size, alignment), size };
    }

    void BaseAllocator::Release(Block block)
    {
#ifdef TL_TRACY_MEMORY_TRACKING
        TracyFreeS(block.ptr, 20);
#endif
        mi_free(block.ptr);
    }

} // namespace TL