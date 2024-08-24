#include "Tl/Allocator/Allocator.hpp"

#include <mimalloc.h>

#include <iostream>

#if defined(TRACY_ENABLED) && TL_TRACY_MEMORY_TRACKER == 1
    #include <Tracy/Tracy.hpp>
    #define TL_TRACY_MEMORY_TRACKING
#endif

static std::atomic_size_t g_totalResources = 0;

struct LeakReporter
{
    LeakReporter()
    {
    }

    ~LeakReporter()
    {
        if (g_totalResources != 0)
        {
            std::cout << std::format("Leaked memory: {} allocations were never freed.", g_totalResources.load());
        }
    }
};

static LeakReporter g_leakReporter = LeakReporter();

namespace TL
{
    Block Allocator::Allocate(size_t size, size_t alignment)
    {
        Block block{ mi_malloc_aligned(size, alignment), size };

#ifdef TL_TRACY_MEMORY_TRACKING
        TracyAllocS(block.ptr, 1, 20);
#endif

        g_totalResources += 1;

        return block;
    }

    void Allocator::Release(Block block, size_t alignment)
    {
        g_totalResources -= 1;

#ifdef TL_TRACY_MEMORY_TRACKING
        TracyFreeS(block.ptr, 20);
#endif

        mi_free(block.ptr);
    }
} // namespace TL