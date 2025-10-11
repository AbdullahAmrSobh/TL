#include "TL/Context.hpp"

#include "TL/Allocator/Allocator.hpp"

#if TL_TRACK_ALLOCATION_CALLSTACKS
    #include "TL/Allocator/MemPlumber.hpp"
#else
    #include "TL/Allocator/Mimalloc.hpp"
#endif

#if TL_ENABLE_TRACY
    #include <tracy/Tracy.hpp>
#endif

namespace TL
{
    struct ContextStack
    {
        inline static constexpr uint64_t k_MaxCount = 128;

#if TL_TRACK_ALLOCATION_CALLSTACKS
        MemPlumber s_DefaultAllocator = MemPlumber();
#else
        Mimalloc s_DefaultAllocator = Mimalloc();
#endif
        uint64_t t_ContextStackCount        = 1;
        Context  t_ContextStack[k_MaxCount] = {{&s_DefaultAllocator}};

        ContextStack()
        {
        }

        ~ContextStack()
        {
        }
    };

    ContextStack* GetStatic()
    {
        static thread_local ContextStack t_ContextStack;
        return &t_ContextStack;
    }

    // Context impl
    Context* Context::get()
    {
        auto stack = GetStatic();
        return &stack->t_ContextStack[stack->t_ContextStackCount - 1];
    }

    Context* Context::push(Context* ctx)
    {
        auto stack = GetStatic();
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
        auto stack = GetStatic();
        if (stack->t_ContextStackCount > 1)
        {
            --stack->t_ContextStackCount;
            return &stack->t_ContextStack[stack->t_ContextStackCount - 1];
        }
        return nullptr;
    }
} // namespace TL
