#include "TL/Literals.hpp"
#include "TL/Allocator/Arena.hpp"
#include "TL/Context.hpp"

#include <climits> // for SIZE_MAX
#include <cstddef> // for size_t
#include <cstdint>

namespace TL
{
    struct ArenaNode
    {
        ArenaNode*  next;
        size_t      size;
        size_t      cursor;  // absolute offset from node base ('this' pointer)

        inline void init(size_t totalBlockSize)
        {
            next   = nullptr;
            size   = totalBlockSize;
            cursor = sizeof(ArenaNode);
        }

        inline void* allocate(size_t reqSize, size_t alignment)
        {
            if (alignment == 0)
                alignment = alignof(max_align_t);
            TL_ASSERT((alignment & (alignment - 1)) == 0, "alignment must be power of two");

            uintptr_t raw     = (uintptr_t)(this) + cursor;
            uintptr_t aligned = (raw + (alignment - 1)) & ~(uintptr_t)(alignment - 1);
            size_t    padding = aligned - raw;
            size_t    total   = padding + reqSize;

            // Ensure we do not overflow and that we stay in the block
            if (cursor + total <= size) // size is whole block size now
            {
                void* ret = (void*)aligned;
                cursor += (uint32_t)total;
                return ret;
            }
            return nullptr;
        }

        inline void reset()
        {
            cursor = sizeof(ArenaNode);
        }
    };

    struct Arena::Impl
    {
        static constexpr size_t kMaxAllocationSize = 2_gb;
        static constexpr size_t kPageSize          = 64_mb;
        static constexpr float  kShrinkFactor      = 2.0f;

        TL::IAllocator*         m_allocator = nullptr;
        ArenaNode*              m_head      = nullptr;

        // statistics
        size_t                  m_totalPages    = 0;
        size_t                  m_totalBytes    = 0;
        size_t                  m_usedBytes     = 0;
        size_t                  m_peakUsedBytes = 0;

        Impl(TL::IAllocator* allocator)
            : m_allocator(allocator)
        {
        }

        ~Impl()
        {
            clear();
        }

        void* allocate(size_t size, size_t alignment = alignof(max_align_t))
        {
            if (size == 0)
            {
                TL_UNREACHABLE_MSG("Requested allocation size is zero");
                return nullptr;
            }
            else if (size > kMaxAllocationSize)
            {
                TL_UNREACHABLE_MSG("Requested allocation exceeds 2 GB limit");
                return nullptr;
            }

            // Try existing pages first
            for (ArenaNode* current = m_head; current != nullptr; current = current->next)
            {
                if (void* mem = current->allocate(size, alignment))
                {
                    updateUsage();
                    return mem;
                }
            }

            // request a page from the underlying allocator (returns Block)
            size_t nodeSize = std::max(kPageSize, size + sizeof(ArenaNode));
            Block  page     = m_allocator->allocate(nodeSize, alignof(ArenaNode));
            TL_ASSERT(page.ptr != nullptr, "Failed to allocate memory for new arena");

            // place node at the beginning of the block
            ArenaNode* node = (ArenaNode*)page.ptr;
            node->init(page.size);
            node->next = m_head;

            m_head = node;
            m_totalPages++;
            m_totalBytes += page.size;

            // allocate from this new node
            void* mem = m_head->allocate(size, alignment);
            TL_ASSERT(mem != nullptr, "Unexpected! new allocation failed");
            updateUsage();
            return mem;
        }

        void reset()
        {
            for (ArenaNode* current = m_head; current; current = current->next)
            {
                current->reset();
            }

            m_usedBytes = 0;
            shrinkIfNeeded();
        }

        void clear()
        {
            while (m_head)
            {
                ArenaNode* next = m_head->next;
                Block      blk  = {(void*)m_head, ((size_t)m_head->size + sizeof(ArenaNode))};
                m_allocator->free(blk, alignof(ArenaNode));
                m_head = next;
            }

            m_head          = 0;
            m_totalPages    = 0;
            m_totalBytes    = 0;
            m_usedBytes     = 0;
            m_peakUsedBytes = 0;
        }

        void updateUsage()
        {
            size_t currentUsed = 0;
            for (ArenaNode* n = m_head; n != 0; n = n->next)
                currentUsed += n->cursor;

            m_usedBytes = currentUsed;
            if (currentUsed > m_peakUsedBytes)
                m_peakUsedBytes = currentUsed;
        }

        void shrinkIfNeeded()
        {
            // target_capacity = max(kPageSize, peak * factor)
            size_t target_capacity = (size_t)((float)m_peakUsedBytes * kShrinkFactor);
            if (target_capacity < kPageSize)
            {
                target_capacity = kPageSize;
            }

            if (m_totalBytes <= target_capacity)
            {
                return;
            }

            // Free from the tail (oldest nodes) until under threshold
            // Note: O(n^2) in worst case because we find tail repeatedly.
            while (m_head && m_totalBytes > target_capacity)
            {
                ArenaNode* prev = 0;
                ArenaNode* node = m_head;

                // find last node
                while (node && node->next)
                {
                    prev = node;
                    node = node->next;
                }

                if (node == nullptr)
                {
                    break;
                }

                Block block = {(void*)node, ((size_t)node->size + sizeof(ArenaNode))};
                m_allocator->free(block, alignof(ArenaNode));

                m_totalBytes -= block.size;
                m_totalPages--;

                if (prev)
                    prev->next = 0;
                else
                    m_head = 0;
            }
        }
    };

    // ---------------- Arena API ----------------

    Arena::Arena()
    {
        m_arena = new Arena::Impl(TL::Context::getDefaultAllocator());
    }

    Arena::~Arena()
    {
        delete m_arena;
    }

    Block Arena::allocateImpl(size_t size, size_t alignment)
    {
        void* ptr = m_arena->allocate(size, alignment);
        TL_ASSERT(ptr != nullptr);
        return {ptr, ptr ? size : 0};
    }

    Block Arena::reallocateImpl(Block block, size_t newSize, size_t alignment)
    {
        void* ptr = m_arena->allocate(newSize, alignment);
        if (!ptr)
            return {nullptr, 0};

        if (block.ptr != nullptr && block.size > 0)
        {
            size_t toCopy = (block.size < newSize) ? block.size : newSize;
            memmove(ptr, block.ptr, toCopy);
        }
        return {ptr, newSize};
    }

    void Arena::freeImpl(Block block, size_t alignment)
    {
        // no-op for arena (free happens only on reset or clear)
        (void)block;
        (void)alignment;
    }

    void Arena::reset()
    {
        m_arena->reset();
    }
} // namespace TL