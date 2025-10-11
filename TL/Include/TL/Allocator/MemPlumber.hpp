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
        Block allocateImpl(size_t size, size_t alignment) override;
        Block reallocateImpl(Block block, size_t newSize, size_t alignment) override;
        void  freeImpl(Block block, size_t alignment) override;
    };
} // namespace TL
