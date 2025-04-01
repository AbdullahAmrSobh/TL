#pragma once

#include "TL/Allocator/Allocator.hpp"
#include "TL/Allocator/Arena.hpp"

namespace TL
{
    // Pushes allocator to the thread local allocators stack
    void        PushAllocator(IAllocator* allocator);

    // Pops allocator from the thread local allocators stack
    IAllocator* PopAllocator();

    // Get the current allocator at the top of the thread local allocators stack
    IAllocator& GetCurrentAllocator();

    // Utility class to manage allocators within a scope (i.e. function)
    // It is invalid to use this as a member variable for a class or struct or etc
    class ScopedAllocator
    {
    public:
        ScopedAllocator(IAllocator* allocator)
        {
            PushAllocator(allocator);
        }

        ScopedAllocator(const ScopedAllocator&)            = delete;
        ScopedAllocator(ScopedAllocator&&)                 = delete;
        ScopedAllocator& operator=(const ScopedAllocator&) = delete;
        ScopedAllocator& operator=(ScopedAllocator&&)      = delete;

        ~ScopedAllocator()
        {
            PopAllocator();
        }
    };

} // namespace TL