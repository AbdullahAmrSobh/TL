#pragma once
#include "TL/Result.hpp"

namespace TL
{
    enum class IOResultCode
    {
        Success,
        Unknown,
        NotFound,
        Denied,
        Timeout,
        Full,
        InvalidParameter,
        Eof,
    };

    using IOResult = Result<size_t, IOResultCode>;

    enum class SeekOrigin
    {
        Begin,
        Current,
        End,
    };

    class IStream
    {
    public:
        /*
        /// Get the total size of the stream.
        virtual IOResult size() const = 0;

        /// Get the current cursor position in the stream.
        virtual IOResult cursor() const = 0;

        /// Seek to a specific position in the stream based on the origin.
        virtual IOResult seek(int64_t position, SeekOrigin origin = SeekOrigin::Begin) = 0;

        /// Move the cursor by a relative offset. Positive values move forward, negative values move backward.
        virtual IOResult move(int64_t offset) = 0;

        /// Seek to eof of the stream.
        virtual IOResult seekEOF() = 0;

        /// Read data from the stream into the provided block. If offset is provided, it seeks to that position before reading.
        virtual IOResult read(Block block) = 0;

        /// Read data from the stream into the provided string. If offset is provided, it seeks to that position before reading.
        virtual IOResult read(String& string) = 0;

        /// Write data from the provided block into the stream. If offset is provided, it seeks to that position before writing.
        virtual IOResult write(Block block) = 0;

        /// Write data from the provided string into the stream. If offset is provided, it seeks to that position before writing.
        virtual IOResult write(String string) = 0;
        */

        virtual size_t   size() const = 0;

        virtual size_t   position() const = 0;

        virtual IOResult read(Block block, uint64_t offset = 0) = 0;

        virtual IOResult read(String& string, uint64_t offset = 0) = 0;

        virtual IOResult write(Block block, uint64_t offset = 0) = 0;

        virtual IOResult write(String string, uint64_t offset = 0) = 0;
    };
} // namespace TL