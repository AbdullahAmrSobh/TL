#pragma once

#include "TL/Allocator/Allocator.hpp"

namespace TL
{
    class Mimalloc final : public IAllocator
    {
    public:
        Mimalloc();
        ~Mimalloc();

    private:
        Block AllocateImpl(size_t size, size_t alignment) override;
        Block ReallocateImpl(Block block, size_t newSize, size_t alignment) override;
        void  ReleaseImpl(Block block, size_t alignment) override;

    #if TL_TRACK_ALLOCATION_SIZES
        size_t m_totalAllocationSize, m_allocationsCount;
    #endif
    };
} // namespace TL