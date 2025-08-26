#pragma once

#include <cstdint>

#include "TL/Export.hpp"

#include <bit>

namespace TL
{
    enum class Endianness : uint8_t
    {
        Little = 0,
        Big    = 1
    };

    inline constexpr Endianness GetSystemEndianness()
    {
        if constexpr (std::endian::native == std::endian::little)
            return Endianness::Little;
        else if constexpr (std::endian::native == std::endian::big)
            return Endianness::Big;
        else
            return Endianness::Little; // Default to little endian
    }

    inline static constexpr uint16_t ByteSwap(uint16_t value) { return std::byteswap(value); }

    inline static constexpr uint32_t ByteSwap(uint32_t value) { return std::byteswap(value); }

    inline static constexpr uint64_t ByteSwap(uint64_t value) { return std::byteswap(value); }

    inline static constexpr int16_t  ByteSwap(int16_t value) { return std::bit_cast<int16_t>(ByteSwap(std::bit_cast<uint16_t>(value))); }

    inline static constexpr int32_t  ByteSwap(int32_t value) { return std::bit_cast<int32_t>(ByteSwap(std::bit_cast<uint32_t>(value))); }

    inline static constexpr int64_t  ByteSwap(int64_t value) { return std::bit_cast<int64_t>(ByteSwap(std::bit_cast<uint64_t>(value))); }

    inline static constexpr float    ByteSwap(float value) { return std::bit_cast<float>(ByteSwap(std::bit_cast<uint32_t>(value))); }

    inline static constexpr double   ByteSwap(double value) { return std::bit_cast<double>(ByteSwap(std::bit_cast<uint64_t>(value))); }
} // namespace TL