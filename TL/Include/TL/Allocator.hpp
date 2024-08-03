#pragma once

#include <TL/Export.hpp>
#include <TL/Assert.hpp> // used for TL_NODISCARD

#include <cstddef>

namespace TL
{
    struct TL_NODISCARD Block
    {
        void* ptr;
        size_t size;
    };

    class TL_EXPORT Allocator
    {
    public:
        virtual ~Allocator() = default;
        virtual Block Allocate(size_t size, size_t alignment = 1) = 0;
        virtual Block Reallocate(Block block, size_t size, size_t alignment = 1) = 0;
        virtual void Release(Block block) = 0;
    };
} // namespace TL