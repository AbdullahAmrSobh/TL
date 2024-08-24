#pragma once

#include "TL/Export.hpp"
#include "TL/Allocator/Allocator.hpp"

namespace TL
{
    class TL_EXPORT Arena : public IAllocator
    {
    public:
        Arena();
        ~Arena();

        void Collect();

        bool CheckOwned(const void* ptr);

    private:
        Block AllocateImpl(size_t size, size_t alignment) override;

        void ReleaseImpl(Block block, size_t alignment) override;

    private:
        void* m_arena;
    };
} // namespace TL