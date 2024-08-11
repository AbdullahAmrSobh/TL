#pragma once

#include "TL/Allocator.hpp"

namespace TL
{
    // Base allocator for pmr containers
    class BaseAllocator final : public IAllocator
    {
    public:
        ~BaseAllocator() = default;

        Block Allocate(size_t size, size_t alignment = 1) override;
        Block Reallocate(Block block, size_t size, size_t alignment = 1) override;
        void Release(Block block) override;
    };
} // namespace TL