#pragma once

namespace TL
{
    template<typename T>
    inline void BinaryArchive::Encode(const T& value)
    {
        // Encode(std::endian::native);
        value.Serialize(*this);
    }

    template<typename T>
    inline void BinaryArchive::Decode(T& value)
    {
        // Decode(m_endieanness);
        value.Deserialize(*this);
    }

    template<typename T>
    inline T BinaryArchive::HandleEndianness(T value)
    {
        return value;

        // if (m_endieanness == std::endian::native)
        // {
        //     return value;
        // }
        // else
        // {
        //     return ByteSwap(value);
        // }
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