#include "TL/Allocator/Allocator.hpp"

namespace TL
{
    class Mimalloc final : public IAllocator
    {
    private:
        Block AllocateImpl(size_t size, size_t alignment) override;

        void ReleaseImpl(Block block, size_t alignment) override;
    };
} // namespace TL