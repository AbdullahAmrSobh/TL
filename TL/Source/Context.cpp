#include "TL/Context.hpp"

#include "TL/Allocator/Allocator.hpp"

#if TL_TRACK_ALLOCATION_CALLSTACKS
    #include "TL/Allocator/MemPlumber.hpp"
#else
    #include "TL/Allocator/Mimalloc.hpp"
#endif

namespace TL
{
//     IAllocator* getDefaultThreadLocalAllocator()
//     {
// #if TL_TRACK_ALLOCATION_CALLSTACKS
//         thread_local MemPlumber allocator = MemPlumber();
// #else
//         thread_local Mimalloc allocator = Mimalloc();
// #endif
//         return &allocator;
//     }

    IAllocator* getDefaultGlobalAllocator()
    {
#if TL_TRACK_ALLOCATION_CALLSTACKS
        static MemPlumber allocator = MemPlumber();
#else
        static Mimalloc allocator = Mimalloc();
#endif
        return &allocator;
    }

    struct ContextStack
    {
        inline static constexpr uint64_t k_MaxCount = 128;

        uint64_t                         t_ContextStackCount;
        Context                          t_ContextStack[k_MaxCount];

        ContextStack(IAllocator* defaultAllocator)
        {
            t_ContextStackCount = 1;
            t_ContextStack[0]   = defaultAllocator;
        }

        ~ContextStack()
        {
        }
    };

    ContextStack* getContextStack()
    {
        static thread_local ContextStack t_ContextStack = ContextStack(getDefaultGlobalAllocator());
        return &t_ContextStack;
    }

    // Context impl
    Context* Context::get()
    {
        auto stack = getContextStack();
        return &stack->t_ContextStack[stack->t_ContextStackCount - 1];
    }

    Context* Context::push(Context* ctx)
    {
        auto stack = getContextStack();
        TL_ASSERT(stack->t_ContextStackCount < ContextStack::k_MaxCount, "Context stack overflow: count exceeds ContextStack::k_MaxCount");
        if (stack->t_ContextStackCount < ContextStack::k_MaxCount)
        {
            stack->t_ContextStack[stack->t_ContextStackCount] = *ctx;
            ++stack->t_ContextStackCount;
            return &stack->t_ContextStack[stack->t_ContextStackCount - 2];
        }
        return nullptr;
    }

    Context* Context::pop()
    {
        auto stack = getContextStack();
        if (stack->t_ContextStackCount > 1)
        {
            --stack->t_ContextStackCount;
            return &stack->t_ContextStack[stack->t_ContextStackCount - 1];
        }
        return nullptr;
    }

    IAllocator* Context::getDefaultAllocator()
    {
        return getDefaultGlobalAllocator();
    }
} // namespace TL
