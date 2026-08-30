#pragma once

#include "TL/Export.hpp"
#include "TL/Allocator/Allocator.hpp"

namespace TL
{
    class TL_EXPORT Arena final : public IAllocator
    {
    public:
        Arena();
        ~Arena();

        void reset();
        Block allocate(size_t size, size_t alignment) override;
        Block reallocate(Block block, size_t newSize, size_t alignment) override;
        void free(Block block, size_t alignment) override;

    private:
        struct Impl;
        Impl* m_arena;
    };
} // namespace TL