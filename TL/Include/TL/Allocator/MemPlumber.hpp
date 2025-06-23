#pragma once

#include "Tl/Allocator/Allocator.hpp"

namespace TL
{
    /// @note: modified version of (https://github.com/seladb/MemPlumber)
    class MemPlumber final : public IAllocator
    {
    public:
        MemPlumber();
        ~MemPlumber();

    private:
        Block AllocateImpl(size_t size, size_t alignment) override;
        Block ReallocateImpl(Block block, size_t newSize, size_t alignment) override;
        void  ReleaseImpl(Block block, size_t alignment) override;
    };
} // namespace TL
