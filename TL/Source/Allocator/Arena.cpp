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

    Block Arena::AllocateImpl(size_t size, size_t alignment)
    {
        auto ptr = mi_heap_malloc_aligned((mi_heap_t*)m_arena, size, alignment);
        return Block{ ptr, size };
    }

    void Arena::ReleaseImpl(Block block, size_t alignment)
    {
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