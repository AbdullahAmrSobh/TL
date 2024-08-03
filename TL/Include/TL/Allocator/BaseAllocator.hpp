#pragma once

#include "TL/Allocator.hpp"

namespace TL
{
    class BaseAllocator final : public Allocator
    {
    public:
        ~BaseAllocator() = default;

        Block Allocate(size_t size, size_t alignment = 1) override;
        Block Reallocate(Block block, const size_t size, size_t alignment = 1) override;
        void Release(Block block) override;
    };
} // namespace TL