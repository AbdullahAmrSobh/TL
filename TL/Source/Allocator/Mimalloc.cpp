#include "TL/Allocator/Mimalloc.hpp"

#include <mimalloc.h>

#include <stdlib.h>

#if TL_ENABLE_TRACY
    #include <tracy/Tracy.hpp>
#endif

namespace TL
{
    Mimalloc::Mimalloc()
    {
    }

    Mimalloc::~Mimalloc()
    {
#if TL_TRACK_ALLOCATION_SIZES
        printf("[TL::Mimalloc exit allocations count: %zu, Total allocation size: %zu]\n", m_allocationsCount, m_totalAllocationSize);
#endif
        // mi_stats_print(nullptr);
    }

    Block Mimalloc::AllocateImpl(size_t size, size_t alignment)
    {
        TL_ASSERT(alignment != 0);
        TL_ASSERT(size != 0);

        void* ptr = mi_malloc_aligned(size, alignment);

#if TL_TRACK_ALLOCATION_SIZES
        m_totalAllocationSize += size;
        m_allocationsCount++;
#endif

#if defined(TL_TRACY_MEMORY_PROFILING) && TL_ENABLE_TRACY
        TracyAlloc(ptr, size);
#endif

        return Block{ptr, size};
    }

    Block Mimalloc::ReallocateImpl(Block block, size_t newSize, size_t alignment)
    {
        TL_ASSERT(alignment != 0);
        TL_ASSERT(block.size != 0);
        TL_ASSERT(newSize != block.size);

#if TL_TRACK_ALLOCATION_SIZES
        m_totalAllocationSize -= block.size;
        m_totalAllocationSize += newSize;
#endif

#if defined(TL_TRACY_MEMORY_PROFILING) && TL_ENABLE_TRACY
        TracyFree(block.ptr);
#endif

        void* ptr = mi_realloc_aligned(block.ptr, block.size, alignment);

#if TL_TRACK_ALLOCATION_SIZES
        // No change to m_totalAllocationSize since size is unchanged.
#endif

#if defined(TL_TRACY_MEMORY_PROFILING) && TL_ENABLE_TRACY
        TracyAlloc(ptr, block.size);
#endif

        return Block{ptr, block.size};
    }

    void Mimalloc::ReleaseImpl(Block block, size_t alignment)
    {
        TL_ASSERT(alignment != 0);
        TL_ASSERT(block.size != 0);
        TL_ASSERT(block.ptr != nullptr);

#if TL_TRACK_ALLOCATION_SIZES
        m_totalAllocationSize -= block.size;
        m_allocationsCount--;
#endif

#if defined(TL_TRACY_MEMORY_PROFILING) && TL_ENABLE_TRACY
        TracyFree(block.ptr);
#endif

        mi_free_aligned(block.ptr, alignment);
    }
} // namespace TL
