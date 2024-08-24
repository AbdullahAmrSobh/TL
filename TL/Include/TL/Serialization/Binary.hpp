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

        template<Serializable<BinaryArchive> T>
        void Encode(const T& value);

        template<Deserializable<BinaryArchive> T>
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

namespace TL
{
    template<Serializable<BinaryArchive> T>
    inline void BinaryArchive::Encode(const T& value)
    {
        // Encode(*this, std::endian::native);
        value.Serialize(*this);
    }

    template<Deserializable<BinaryArchive> T>
    inline void BinaryArchive::Decode(T& value)
    {
        // Decode(*this, (int&)m_endieanness);
        value.Deserialize(*this);
    }

    template<typename T>
    inline T BinaryArchive::HandleEndianness(T value)
    {
        if (m_endieanness == std::endian::native)
        {
            return value;
        }
        else
        {
            return ByteSwap(value);
        }
    }

    // clang-format off

    inline static void Encode(BinaryArchive& archive, Block block)     { archive.StreamWrite( block );  }
    inline static void Decode(BinaryArchive& archive, Block block)     { archive.StreamRead( block );   }
    inline static void Encode(BinaryArchive& archive, bool value)      { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, bool& value)     { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, char value)      { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, char& value)     { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, uint8_t value)   { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, uint8_t& value)  { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, uint16_t value)  { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, uint16_t& value) { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, uint32_t value)  { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, uint32_t& value) { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, uint64_t value)  { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, uint64_t& value) { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, int8_t value)    { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, int8_t& value)   { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, int16_t value)   { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, int16_t& value)  { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, int32_t value)   { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, int32_t& value)  { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, int64_t value)   { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, int64_t& value)  { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, float value)     { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, float& value)    { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }
    inline static void Encode(BinaryArchive& archive, double value)    { auto newval = archive.HandleEndianness(value); archive.StreamWrite(Block::Create(newval)); }
    inline static void Decode(BinaryArchive& archive, double& value)   { archive.StreamRead(Block::Create(value)); value = archive.HandleEndianness(value);         }

    // clang-format on
    template<class CharType, class CharTraits, class AllocatorType>
    inline void Encode(BinaryArchive& archive, const std::basic_string<CharType, CharTraits, AllocatorType>& value)
    {
        Encode(archive, value.size());
        Encode(archive, Block{ (void*)value.c_str(), value.size() });
    }

    template<class CharType, class CharTraits, class AllocatorType>
    inline void Decode(BinaryArchive& archive, std::basic_string<CharType, CharTraits, AllocatorType>& value)
    {
        size_t stringSize = 0;
        Decode(archive, stringSize);
        value.resize(stringSize);
        if (stringSize > 0) {
            Decode(archive, Block{(void*)value.data(), stringSize * sizeof(CharType)});
        }
    }

    template<class Type, class Allocator>
    inline void Encode(BinaryArchive& archive, const std::vector<Type, Allocator>& values)
    {
        ::TL::Encode(archive, values.size());
        for (const auto& value : values)
        {
            ::TL::Encode(archive, value);
        }
    }

    template<class Type, class Allocator>
    inline void Decode(BinaryArchive& archive, std::vector<Type, Allocator>& values)
    {
        size_t size;
        ::TL::Decode(archive, size);
        values.resize(size);
        for (auto& value : values)
        {
            ::TL::Decode(archive, value);
        }
    }

    template<typename Key, typename Value>
    inline void Encode(BinaryArchive& archive, const std::pair<Key, Value>& value)
    {
        ::TL::Encode(archive, value.first);
        ::TL::Encode(archive, value.second);
    }

    template<typename Key, typename Value>
    inline void Decode(BinaryArchive& archive, std::pair<Key, Value>& value)
    {
        Key k;
        Value v;

        ::TL::Decode(archive, k);
        ::TL::Decode(archive, v);

        value = std::make_pair(k, v);
    }

    template<class Key, class Type, class Hasher = std::hash<Key>, class KeyEq, class Allocator>
    inline void Encode(BinaryArchive& archive, const std::unordered_map<Key, Type, Hasher, KeyEq, Allocator>& values)
    {
        ::TL::Encode(archive, values.size());
        for (const auto& value : values)
        {
            ::TL::Encode(archive, value);
        }
    }

    template<class Key, class Type, class Hasher = std::hash<Key>, class KeyEq, class Allocator>
    inline void Decode(BinaryArchive& archive, std::unordered_map<Key, Type, Hasher, KeyEq, Allocator>& values)
    {
        size_t size;
        Decode(archive, size);
        values.reserve(size);
        for (size_t i = 0; i < size; i++)
        {
            std::pair<Key, Type> pair;
            Decode(archive, pair);
            values.emplace(std::move(pair));
        }
    }

    template<Serializable<BinaryArchive> T>
    inline void Encode(BinaryArchive& archive, const T& value)
    {
        value.Serialize(archive);
    }

    template<Deserializable<BinaryArchive> T>
    inline void Decode(class BinaryArchive& archive, T& value)
    {
        value.Deserialize(archive);
    }
} // namespace TL