#include "TL/Allocator/Arena.hpp"

#include <mimalloc.h>

namespace TL
{
    Arena::Arena()
    {
        m_arena = mi_heap_new();
    }

    Arena::~Arena()
    {
        mi_heap_destroy((mi_heap_t*)m_arena);
    }

    Block Arena::allocateImpl(size_t size, size_t alignment)
    {
        void* ptr = mi_heap_malloc_aligned((mi_heap_t*)m_arena, size, alignment);
        return Block{ptr, size};
    }

    Block Arena::reallocateImpl(Block block, size_t newSize, size_t alignment)
    {
        void* ptr = mi_heap_realloc_aligned((mi_heap_t*)m_arena, block.ptr, newSize, alignment);
        return Block{ptr, newSize};
    }

    void Arena::freeImpl(Block block, size_t alignment)
    {
        // no op
    }

    void Arena::Collect()
    {
        mi_heap_collect((mi_heap_t*)m_arena, true);
    }

    bool Arena::CheckOwned(const void* ptr)
    {
        return mi_heap_check_owned((mi_heap_t*)m_arena, ptr);
    }
} // namespace TL