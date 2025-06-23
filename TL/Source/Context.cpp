#include "TL/Context.hpp"

#include "TL/Allocator/Allocator.hpp"

#define TL_TRACK_LEAK_CALLSTACKS 1

#if TL_TRACK_LEAK_CALLSTACKS
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

#if TL_TRACK_LEAK_CALLSTACKS
        MemPlumber s_DefaultAllocator = MemPlumber();
#else
        Mimalloc s_DefaultAllocator = Mimalloc();
#endif
        uint64_t t_ContextStackCount        = 1;
        Context  t_ContextStack[k_MaxCount] = {{&s_DefaultAllocator}};

        ContextStack()
        {
#if TL_TRACK_LEAK_CALLSTACKS
            s_DefaultAllocator.start();
#endif
        }

        ~ContextStack()
        {
#if TL_TRACK_LEAK_CALLSTACKS
            s_DefaultAllocator.stop();
            uint64_t allocationsCount, totalLeakSize;
            s_DefaultAllocator.memLeakCheck(allocationsCount, totalLeakSize);
            if (allocationsCount > 0)
            {
                TL_LOG_WARNNING("Detected `{}` leaks (total size {}[bytes])", allocationsCount, totalLeakSize);
            }
#endif
        }
    };

    ContextStack* GetStatic()
    {
        static thread_local ContextStack t_ContextStack;
        return &t_ContextStack;
    }

    // Context impl
    Context* Context::Get()
    {
        auto stack = GetStatic();
        return &stack->t_ContextStack[stack->t_ContextStackCount - 1];
    }

    Context* Context::Push(Context* ctx)
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

    Context* Context::Pop()
    {
        auto stack = GetStatic();
        if (stack->t_ContextStackCount > 1)
        {
            --stack->t_ContextStackCount;
            return &stack->t_ContextStack[stack->t_ContextStackCount - 1];
        }
        return nullptr;
    }

    Block Allocate(size_t size, size_t alignment)
    {
        auto block = Context::Get()->GetAllocator()->Allocate(size, alignment);
        _OnAllocate(block);
        return block;
    }

    Block Reallocate(Block oldBlock, size_t alignment)
    {
        // return Context::Get()->GetAllocator()->Allocate(oldBlock, alignment);

        TL_LOG_WARNNING("TODO: Implement Reallocate");

        Block newBlock = Context::Get()->GetAllocator()->Allocate(oldBlock.size, alignment);
        if (newBlock.ptr && oldBlock.ptr && oldBlock.size > 0)
        {
            std::memcpy(newBlock.ptr, oldBlock.ptr, oldBlock.size);
            Context::Get()->GetAllocator()->Release(oldBlock, alignment);
        }
        return newBlock;
    }

    void Release(Block block, size_t alignment)
    {
        _OnRelease(block);
        Context::Get()->GetAllocator()->Release(block, alignment);
    }

    void _OnAllocate(TL::Block block)
    {
#if TL_ENABLE_TRACY
        TracyAllocS(block.ptr, block.size, 20);
#endif
    }

    void _OnRelease(TL::Block block)
    {
#if TL_ENABLE_TRACY
        TracyFreeS(block.ptr, 20);
#endif
    }

} // namespace TL
