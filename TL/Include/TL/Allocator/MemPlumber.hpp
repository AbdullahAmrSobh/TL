#pragma once

#include "Tl/Allocator/Allocator.hpp"

namespace TL
{
    /// @note: modified version of (https://github.com/seladb/MemPlumber)
    class MemPlumber final : public IAllocator
    {
    private:

    public:
        MemPlumber() = default;

        MemPlumber(const MemPlumber& other) = delete;

        static void start(); // TODO: rename to init

        static void stop(); // TODO: rename to shutdown

        static void memLeakCheck(size_t& memLeakCount, uint64_t& memLeakSize);

        Block       AllocateImpl(size_t size, size_t alignment) override;
        void        ReleaseImpl(Block block, size_t alignment) override;
    };
} // namespace TL
