#include "Tl/Allocator/Allocator.hpp"

namespace TL
{
    /// @note: modified version of (https://github.com/seladb/MemPlumber)
    class MemPlumber final : public IAllocator
    {
    private:
    public:
        MemPlumber() = default;

        // disable copy c'tor
        MemPlumber(const MemPlumber& other) = delete;

        /**
         * Start collecting information about memory allocations. Note that before calling this method no information is collected.
         * @param[in] verbose A flag indicating whether to dump information on each memory allocation and deallocation. The default value is false
         * @param[in] fileDumperName If the "verbose" flag is set to true, it is possible to dump the verbose information to a file.
         * If this parameter is set to an empty string (which is also the default value), the verbose information will be dumped to stdout
         * @param[in] append If the "verbose" flag is set to true and "fileDumperName" is a non-empty string and if this file already exists on disk,
         * this parameter indicates whether to append the verbose information to the existing file or start writing from scratch
         */
        static void start();

        /**
         * Stop collecting information about memory allocations.
         */
        static void stop();

        /**
         * Present information about memory allocations that were not yet freed.
         * @param[out] memLeakCount The number of memory allocations that were not yet freed
         * @param[out] memLeakSize The total amount of memory that was allocated but not yet freed
         * @param[in] verbose A flag indicating whether to dump information on all memory allocations that were not yet freed. The default value is false
         * @param[in] fileDumperName If the "verbose" flag is set to true, it is possible to dump the verbose information to a file.
         * If this parameter is set to an empty string (which is also the default value), the verbose information will be dumped to stdout
         * @param[in] append If the "verbose" flag is set to true and "fileDumperName" is a non-empty string and if this file already exists on disk,
         * this parameter indicates whether to append the verbose information to the existing file or start writing from scratch
         */
        static void memLeakCheck(size_t& memLeakCount, uint64_t& memLeakSize);

        Block AllocateImpl(size_t size, size_t alignment) override;
        void ReleaseImpl(Block block, size_t alignment) override;
    };
} // namespace TL
