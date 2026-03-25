#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <TL/Stacktrace.hpp>
#include "Tl/Allocator/MemPlumber.hpp"
#include "Tl/Allocator/Mimalloc.hpp"

#ifndef MEMPLUMBER_HASHTABLE_SIZE
    #define MEMPLUMBER_HASHTABLE_SIZE 16384
#endif

#ifndef MEMPLUMBER_HASH
    #define MEMPLUMBER_HASH(p) (((size_t)(p) >> 8) % MEMPLUMBER_HASHTABLE_SIZE)
#endif

namespace TL
{
    class MemPlumberImpl
    {
        Mimalloc m_allocator;

    private:
        struct new_ptr_list_t
        {
            new_ptr_list_t* next;
            Stacktrace      stacktrace;
            size_t          size;
        };

        new_ptr_list_t* m_PointerListHashtable[MEMPLUMBER_HASHTABLE_SIZE];

        bool            m_Started;

        // private c'tor
        MemPlumberImpl()
        {
            m_Started = true;

            // zero the hashtables
            for (int i = 0; i < MEMPLUMBER_HASHTABLE_SIZE; i++)
            {
                m_PointerListHashtable[i] = NULL;
            }
        }

        ~MemPlumberImpl()
        {
            m_Started = false;
        }

    public:
        static MemPlumberImpl& getInstance()
        {
            static MemPlumberImpl instance;
            return instance;
        }

        Block allocate(size_t size, [[maybe_unused]] size_t alignment)
        {
            alignment = std::max(alignment, alignof(new_ptr_list_t));
            // if not started, allocate memory and exit
            if (!m_Started)
            {
                return m_allocator.allocate(size, alignment);
            }

            // total memory to allocated is the requested size + metadata size
            size_t          totalSizeToAllocate = size + sizeof(new_ptr_list_t);

            // allocated memory
            void*           rawMem                = m_allocator.allocate(totalSizeToAllocate, alignof(new_ptr_list_t)).ptr;
            new_ptr_list_t* pointerMetaDataRecord = static_cast<new_ptr_list_t*>(rawMem);
            memset(pointerMetaDataRecord, 0, sizeof(new_ptr_list_t));

            // if cannot allocate, return NULL
            if (pointerMetaDataRecord == NULL)
                return {};

            // calculate the actual pointer to provide to the user
            void*  actualPointer = static_cast<char*>(rawMem) + sizeof(new_ptr_list_t);

            // find the hash index for this pointer
            size_t hashIndex = MEMPLUMBER_HASH(actualPointer);

            // chain this metadata to the linked list of the specific bucket
            pointerMetaDataRecord->next = m_PointerListHashtable[hashIndex];

            // fill in the metadata
            // skip the following callstacks
            // MemPlumberImpl::allocate
            // MemPlumber::allocateImpl
            pointerMetaDataRecord->stacktrace = CaptureStacktrace(5);
            pointerMetaDataRecord->size       = size;

            // put this metadata in the head of the list
            m_PointerListHashtable[hashIndex] = pointerMetaDataRecord;

            return {actualPointer, size};
        }

        void release(Block block, [[maybe_unused]] size_t alignment)
        {
            alignment = std::max(alignment, alignof(new_ptr_list_t));

            if (block.ptr == NULL)
            {
                return;
            }

            // find the metadata record bucket in the hash table
            size_t          hashIndex                           = MEMPLUMBER_HASH(block.ptr);
            new_ptr_list_t* metaDataBucketLinkedListElement     = m_PointerListHashtable[hashIndex];
            new_ptr_list_t* metaDataBucketLinkedListPrevElement = NULL;

            // inside the bucket, go over the linked list until you find the specific pointer
            while (metaDataBucketLinkedListElement != NULL)
            {
                // get the actual pointer from the record
                void* actualPointerInRecord = (char*)metaDataBucketLinkedListElement + sizeof(new_ptr_list_t);

                // if this is not the pointer we're looking for - continue the search
                if (actualPointerInRecord != block.ptr)
                {
                    metaDataBucketLinkedListPrevElement = metaDataBucketLinkedListElement;
                    metaDataBucketLinkedListElement     = metaDataBucketLinkedListElement->next;
                    continue;
                }
                else
                { // this is the pointer we're looking for

                    // remove the current element from the linked list
                    if (metaDataBucketLinkedListPrevElement == NULL)
                    { // this is the first item in the list
                        m_PointerListHashtable[hashIndex] = metaDataBucketLinkedListElement->next;
                    }
                    else
                    { // this is not the first item in the list
                        metaDataBucketLinkedListPrevElement->next = metaDataBucketLinkedListElement->next;
                    }

                    // free the memory of the current item
                    m_allocator.free({metaDataBucketLinkedListElement, metaDataBucketLinkedListElement->size + sizeof(new_ptr_list_t)}, alignment);

                    return;
                }
            }

            // if got to here it means memory was allocated before monitoring started. Simply free the memory and return
            TL_DEBUG_BREAK();

            m_allocator.free(block, alignment);
        }

        void checkLeaks()
        {
            uint64_t memLeakCount = 0;
            uint64_t memLeakSize  = 0;

            // go over all buckets in the hashmap
            for (int index = 0; index < MEMPLUMBER_HASHTABLE_SIZE; ++index)
            {
                new_ptr_list_t* metaDataBucketLinkedListElement = m_PointerListHashtable[index];

                // if bucket is empty - continue
                if (metaDataBucketLinkedListElement == NULL)
                {
                    continue;
                }

                // go over all of the elements in the link list in this bucket
                while (metaDataBucketLinkedListElement != NULL)
                {
                    memLeakCount++;
                    memLeakSize += (uint64_t)metaDataBucketLinkedListElement->size;

                    {
                        auto ptr = size_t((char*)metaDataBucketLinkedListElement + sizeof(new_ptr_list_t));
                        printf(
                            "Leaked allocation at 0x%zx (size %zu [bytes]):\n%s\n",
                            ptr,
                            metaDataBucketLinkedListElement->size,
                            ReportStacktrace(metaDataBucketLinkedListElement->stacktrace).c_str());
                    }

                    // go to the next item on the list
                    metaDataBucketLinkedListElement = metaDataBucketLinkedListElement->next;
                }
            }
            printf("[TL::MemPlumber exit allocations count: %zu, Total allocation size: %zu]\n", memLeakCount, memLeakSize);
        }
    };

    MemPlumber::MemPlumber()
    {
    }

    MemPlumber::~MemPlumber()
    {
        MemPlumberImpl::getInstance().checkLeaks();
    }

    Block MemPlumber::allocateImpl(size_t size, size_t alignment)
    {
        return MemPlumberImpl::getInstance().allocate(size, alignment);
    }

    Block MemPlumber::reallocateImpl(Block block, size_t newSize, size_t alignment)
    {
        // Simple implementation: allocate new, copy, release old
        // TODO: Implement real real realloc
        if (block.ptr == nullptr)
            return allocateImpl(newSize, alignment);

        Block newBlock = allocateImpl(newSize, alignment);
        if (newBlock.ptr && block.ptr)
        {
            size_t copySize = (block.size < newSize) ? block.size : newSize;
            memcpy(newBlock.ptr, block.ptr, copySize);
            freeImpl(block, alignment);
        }
        return newBlock;
    }

    void MemPlumber::freeImpl(Block block, size_t alignment)
    {
        MemPlumberImpl::getInstance().release(block, alignment);
    }
} // namespace TL