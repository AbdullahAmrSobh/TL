#include "Tl/Allocator/Allocator.hpp"
#include "Tl/Allocator/Mimalloc.hpp"
#include "Tl/Allocator/MemPlumber.hpp"

#include <tracy/Tracy.hpp>

#if defined(TRACY_ENABLED) && TL_TRACY_MEMORY_TRACKER == 1
    #include <Tracy/Tracy.hpp>
    #define TL_ENABLE_TRACY_MEMORY_TRACKING
#endif

namespace TL
{

    static MemPlumber g_allocator;
    // static Mimalloc g_allocator;

    Block Allocator::Allocate(size_t size, size_t alignment)
    {
        auto block = g_allocator.Allocate(size, alignment);

#ifdef TL_ENABLE_TRACY_MEMORY_TRACKING
        TracyAllocS(block.ptr, 1, 20);
#endif

        return block;
    }

    void Allocator::Release(Block block, size_t alignment)
    {
#ifdef TL_ENABLE_TRACY_MEMORY_TRACKING
        TracyFreeS(block.ptr, 20);
#endif

        g_allocator.Release(block, alignment);
    }
} // namespace TL