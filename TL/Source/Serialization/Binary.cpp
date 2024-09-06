#include <TL/Serialization/Binary.hpp>

#include <stdlib.h>

#include <cstdint>

namespace TL
{
#if defined(__GNUC__) || defined(__clang__)
    #define TL_COMPILER_GCC_CLANG
#elif defined(_MSC_VER)
    #define TL_COMPILER_MSVC
#else
    #warning "Unknown compiler, falling back to manual byteswap implementation."
#endif

    uint16_t ByteSwap(uint16_t value)
    {
#if defined(TL_COMPILER_GCC_CLANG)
        return __builtin_bswap16(value);
#elif defined(TL_COMPILER_MSVC)
        return _byteswap_ushort(value);
#else
        return static_cast<uint16_t>(((value & 0x00FF) << 8) |
                                     ((value & 0xFF00) >> 8));
#endif
    }

    uint32_t ByteSwap(uint32_t value)
    {
#if defined(TL_COMPILER_GCC_CLANG)
        return __builtin_bswap32(value);
#elif defined(TL_COMPILER_MSVC)
        return _byteswap_ulong(value);
#else
        return ((value & 0x000000FF) << 24) |
               ((value & 0x0000FF00) << 8) |
               ((value & 0x00FF0000) >> 8) |
               ((value & 0xFF000000) >> 24);
#endif
    }

    uint64_t ByteSwap(uint64_t value)
    {
#if defined(TL_COMPILER_GCC_CLANG)
        return __builtin_bswap64(value);
#elif defined(TL_COMPILER_MSVC)
        return _byteswap_uint64(value);
#else
        return ((value & 0x00000000000000FFULL) << 56) |
               ((value & 0x000000000000FF00ULL) << 40) |
               ((value & 0x0000000000FF0000ULL) << 24) |
               ((value & 0x00000000FF000000ULL) << 8) |
               ((value & 0x000000FF00000000ULL) >> 8) |
               ((value & 0x0000FF0000000000ULL) >> 24) |
               ((value & 0x00FF000000000000ULL) >> 40) |
               ((value & 0xFF00000000000000ULL) >> 56);
#endif
    }

    void BinaryArchive::StreamWrite(Block block)
    {
        try
        {
            m_stream->write((const char*)(block.ptr), block.size);
        }
        catch (const std::exception& e)
        {
            TL_ASSERT(m_stream->good(), "Serialization error: {}", e.what());
        }
    }

    void BinaryArchive::StreamRead(Block block)
    {
        try
        {
            m_stream->read((char*)block.ptr, block.size);
        }
        catch (const std::exception& e)
        {
            TL_ASSERT(m_stream->good(), "Deserialization error: {}", e.what());
        }
    }
} // namespace TL