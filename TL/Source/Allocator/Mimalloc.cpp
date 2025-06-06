#include "TL/Allocator/Mimalloc.hpp"

#include <mimalloc.h>

#include <stdlib.h>

#include <tracy/Tracy.hpp>

namespace TL
{
    Mimalloc::~Mimalloc()
    {
        mi_stats_print(nullptr);
    }

    Block Mimalloc::AllocateImpl(size_t size, size_t alignment)
    {
        // return {mi_malloc_aligned(size, alignment), size};
        return {mi_malloc(size), size};
    }

    void Mimalloc::ReleaseImpl(Block block, size_t alignment)
    {
        // mi_free_size_aligned(block.ptr, block.size, alignment);
        mi_free(block.ptr);
    }
} // namespace TL
