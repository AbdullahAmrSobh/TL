#pragma once

#include "TL/Block.hpp"
#include "TL/Serialization/SerializeTraits.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>

namespace TL
{
    TL_EXPORT uint16_t ByteSwap(uint16_t value);
    TL_EXPORT uint32_t ByteSwap(uint32_t value);
    TL_EXPORT uint64_t ByteSwap(uint64_t value);

    inline static int16_t ByteSwap(int16_t value)
    {
        return std::bit_cast<int16_t>(ByteSwap(std::bit_cast<uint16_t>(value)));
    }

    inline static int32_t ByteSwap(int32_t value)
    {
        return std::bit_cast<int32_t>(ByteSwap(std::bit_cast<uint32_t>(value)));
    }

    inline static int64_t ByteSwap(int64_t value)
    {
        return std::bit_cast<int64_t>(ByteSwap(std::bit_cast<uint64_t>(value)));
    }

    inline static float ByteSwap(float value)
    {
        return std::bit_cast<float>(ByteSwap(std::bit_cast<uint32_t>(value)));
    }

    inline static double ByteSwap(double value)
    {
        return std::bit_cast<double>(ByteSwap(std::bit_cast<uint64_t>(value)));
    }

    class BinaryArchive
    {
        template<Serializable<BinaryArchive> T>
        friend void Encode(BinaryArchive& encoder, const T& value);

        template<Deserializable<BinaryArchive> T>
        friend void Decode(class BinaryArchive& decoder, T& value);

        friend void Encode(BinaryArchive& encoder, bool value);
        friend void Decode(BinaryArchive& decoder, bool& value);
        friend void Encode(BinaryArchive& encoder, char value);
        friend void Decode(BinaryArchive& decoder, char& value);
        friend void Encode(BinaryArchive& encoder, uint8_t value);
        friend void Decode(BinaryArchive& decoder, uint8_t& value);
        friend void Encode(BinaryArchive& encoder, uint16_t value);
        friend void Decode(BinaryArchive& decoder, uint16_t& value);
        friend void Encode(BinaryArchive& encoder, uint32_t value);
        friend void Decode(BinaryArchive& decoder, uint32_t& value);
        friend void Encode(BinaryArchive& encoder, uint64_t value);
        friend void Decode(BinaryArchive& decoder, uint64_t& value);
        friend void Encode(BinaryArchive& encoder, int8_t value);
        friend void Decode(BinaryArchive& decoder, int8_t& value);
        friend void Encode(BinaryArchive& encoder, int16_t value);
        friend void Decode(BinaryArchive& decoder, int16_t& value);
        friend void Encode(BinaryArchive& encoder, int32_t value);
        friend void Decode(BinaryArchive& decoder, int32_t& value);
        friend void Encode(BinaryArchive& encoder, int64_t value);
        friend void Decode(BinaryArchive& decoder, int64_t& value);
        friend void Encode(BinaryArchive& encoder, size_t value);
        friend void Decode(BinaryArchive& decoder, size_t& value);
        friend void Encode(BinaryArchive& encoder, float value);
        friend void Decode(BinaryArchive& decoder, float& value);
        friend void Encode(BinaryArchive& encoder, double value);
        friend void Decode(BinaryArchive& decoder, double& value);
        friend void Encode(BinaryArchive& encoder, Block block);
        friend void Decode(BinaryArchive& decoder, Block block);

    public:
        BinaryArchive(std::iostream& stream)
            : m_stream(&stream)
        {
        }

        template<typename T>
        void Encode(const T& value);

        template<typename T>
        void Decode(T& value);

    private:
        void StreamWrite(Block block);
        void StreamRead(Block block);

        template<typename T>
        T HandleEndianness(T value);

    private:
        std::endian m_endieanness;
        std::iostream* m_stream;
    };
} // namespace TL
