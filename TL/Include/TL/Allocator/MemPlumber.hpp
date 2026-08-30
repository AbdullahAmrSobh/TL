#pragma once

#include "TL/Allocator/Allocator.hpp"

namespace TL
{
    /// @note: modified version of (https://github.com/seladb/MemPlumber)
    class MemPlumber final : public IAllocator
    {
    public:
        MemPlumber();
        ~MemPlumber();

        Block allocate(size_t size, size_t alignment) override;
        Block reallocate(Block block, size_t newSize, size_t alignment) override;
        void free(Block block, size_t alignment) override;
    };
} // namespace TL
