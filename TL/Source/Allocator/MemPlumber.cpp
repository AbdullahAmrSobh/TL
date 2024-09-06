#include <cstdlib>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "TL/Log.hpp"
#include "TL/Log.hpp"

#include <TL/Stacktrace.hpp>
#include "Tl/Allocator/Allocator.hpp"
#include "Tl/Allocator/MemPlumber.hpp"

#include <mimalloc.h>

#ifndef MEMPLUMBER_HASHTABLE_SIZE
    #define MEMPLUMBER_HASHTABLE_SIZE 16384
#endif

#ifndef MEMPLUMBER_HASH
    #define MEMPLUMBER_HASH(p) (((size_t)(p) >> 8) % MEMPLUMBER_HASHTABLE_SIZE)
#endif

namespace TL
{
    class MemPlumberImpl final : public IAllocator
    {
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
            m_Started = false;

            // zero the hashtables
            for (int i = 0; i < MEMPLUMBER_HASHTABLE_SIZE; i++)
            {
                m_PointerListHashtable[i] = NULL;
            }
        }

    public:
        static MemPlumberImpl& getInstance()
        {
            static MemPlumberImpl instance;
            return instance;
        }

        Block AllocateImpl(size_t size, [[maybe_unused]] size_t alignment) override
        {
            // if not started, allocate memory and exit
            if (!m_Started)
            {
                return {mi_malloc_aligned(size, alignment), size};
            }

            // total memory to allocated is the requested size + metadata size
            size_t totalSizeToAllocate = size + sizeof(new_ptr_list_t);

            // allocated memory
            new_ptr_list_t* pointerMetaDataRecord = (new_ptr_list_t*)mi_malloc(totalSizeToAllocate);
            memset(pointerMetaDataRecord, 0, sizeof(new_ptr_list_t));

            // if cannot allocate, return NULL
            if (pointerMetaDataRecord == NULL)
                return {};

            // calculate the actual pointer to provide to the user
            void*  actualPointer = (char*)pointerMetaDataRecord + sizeof(new_ptr_list_t);

            // find the hash index for this pointer
            size_t hashIndex = MEMPLUMBER_HASH(actualPointer);

            // chain this metadata to the linked list of the specific bucket
            pointerMetaDataRecord->next = m_PointerListHashtable[hashIndex];

            // fill in the metadata
            pointerMetaDataRecord->stacktrace = CaptureStacktrace(5);
            pointerMetaDataRecord->size       = size;

            // put this metadata in the head of the list
            m_PointerListHashtable[hashIndex] = pointerMetaDataRecord;

            return {actualPointer, size};
        }

        void ReleaseImpl(Block block, [[maybe_unused]] size_t alignment) override
        {
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
                    mi_free(metaDataBucketLinkedListElement);

                    return;
                }
            }

            // if got to here it means memory was allocated before monitoring started. Simply free the memory and return

            mi_free(block.ptr);
        }

        void start()
        {
            m_Started = true;
        }

        void stop()
        {
            m_Started = false;
        }

        void checkLeaks(size_t& memLeakCount, uint64_t& memLeakSize)
        {
            memLeakCount = 0;
            memLeakSize  = 0;

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
                        TL_LOG_INFO(
                            "Leaked object at 0x{:0x} (size {}[bytes]):\n {}\n",
                            ptr,
                            metaDataBucketLinkedListElement->size,
                            ReportStacktrace(metaDataBucketLinkedListElement->stacktrace));
                    }

                    // go to the next item on the list
                    metaDataBucketLinkedListElement = metaDataBucketLinkedListElement->next;
                }
            }
        }
    };

    void MemPlumber::start()
    {
        MemPlumberImpl::getInstance().start();
    }

    void MemPlumber::stop()
    {
        MemPlumberImpl::getInstance().stop();
    }

    void MemPlumber::memLeakCheck(size_t& memLeakCount, uint64_t& memLeakSize)
    {
        MemPlumberImpl::getInstance().checkLeaks(memLeakCount, memLeakSize);
    }

    Block MemPlumber::AllocateImpl(size_t size, size_t alignment)
    {
        return MemPlumberImpl::getInstance().AllocateImpl(size, alignment);
    }

    void MemPlumber::ReleaseImpl(Block block, size_t alignment)
    {
        return MemPlumberImpl::getInstance().ReleaseImpl(block, alignment);
    }
} // namespace TL