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
        Eof,
    };

    using IOResult = Result<size_t, IOResultCode>;

    class IStream
    {
    public:
        virtual size_t   size() const = 0;

        virtual size_t   position() const = 0;

        virtual IOResult read(Block block, uint64_t offset = 0) = 0;

        virtual IOResult read(String& string, uint64_t offset = 0) = 0;

        virtual IOResult write(Block block, uint64_t offset = 0) = 0;

        virtual IOResult write(String string, uint64_t offset = 0) = 0;
    };
} // namespace TL