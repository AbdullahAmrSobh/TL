#pragma once

#include "TL/Stacktrace.hpp"
#include "TL/Block.hpp"
#include "TL/Allocator.hpp"

#include <cstddef>
#include <cstdlib>

namespace TL
{
    struct LeakTrackerHeader
    {
        size_t blockSize;
        LeakTrackerHeader* next;
        LeakTrackerHeader* prev;
        Stacktrace stacktrace;
    };

    class TL_EXPORT LeakTracker
    {
    public:
        LeakTracker() = default;

        ~LeakTracker()
        {
            while (head)
            {
                TL_LOG_ERROR("Leak: {}", "Cant report error because it will trigger allocation");
                head = head->next;
            }
        }

        static Block Allocate(size_t size, size_t alignment);
        static Block Reallocate(Block block, size_t size, size_t alignment);
        static void Release(Block block);

    private:
        Block _Allocate(size_t size, size_t alignment)
        {
            // Align the size and allocation.
            size_t totalSize = size + sizeof(LeakTrackerHeader);
            void* rawPtr = DefaultAllocator::Allocate(totalSize, alignment).ptr;

            // Initialize LeakTrackerHeader.
            auto* header = new (rawPtr) LeakTrackerHeader();
            header->blockSize = size;
            header->next = nullptr;
            header->prev = nullptr;
            header->stacktrace = CaptureStacktrace();

            // Link into the list.
            if (tail)
            {
                tail->next = header;
                header->prev = tail;
            }
            else
            {
                head = header;
            }
            tail = header;

            // Return the block, excluding the header.
            return { (char*)rawPtr + sizeof(LeakTrackerHeader), size };
        }

        Block _Reallocate(Block block, size_t size, size_t alignment)
        {
            // Find the header associated with this block.
            auto* header = (LeakTrackerHeader*)((char*)block.ptr - sizeof(LeakTrackerHeader));

            // Adjust the size and allocate new memory.
            Block newBlock = Allocate(size, alignment);

            // Copy the old data to the new block.
            size_t copySize = std::min(size, header->blockSize);
            std::memcpy(newBlock.ptr, block.ptr, copySize);

            // Release the old block.
            Release(block);

            return newBlock;
        }

        void _Release(Block block)
        {
            // Find the header associated with this block.
            auto* header = (LeakTrackerHeader*)((char*)block.ptr - sizeof(LeakTrackerHeader));

            // Remove from the list.
            if (header->prev)
            {
                header->prev->next = header->next;
            }
            else
            {
                head = header->next;
            }

            if (header->next)
            {
                header->next->prev = header->prev;
            }
            else
            {
                tail = header->prev;
            }

            DefaultAllocator::Release({ header, header->blockSize + sizeof(LeakTrackerHeader) });
        }

        LeakTrackerHeader* head = nullptr;
        LeakTrackerHeader* tail = nullptr;
    };

    static LeakTracker tracker;

    inline Block LeakTracker::Allocate(size_t size, size_t alignment)
    {
        return tracker._Allocate(size, alignment);
    }

    inline Block LeakTracker::Reallocate(Block block, size_t size, size_t alignment)
    {
        return tracker._Reallocate(block, size, alignment);
    }

    inline void LeakTracker::Release(Block block)
    {
        return tracker._Release(block);
    }

} // namespace TL
