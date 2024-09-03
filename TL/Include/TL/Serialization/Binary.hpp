#pragma once

#include "TL/Block.hpp"
#include "TL/Serialization/SerializeTraits.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <optional>
#include <filesystem>

namespace TL
{
    // clang-format off
    TL_EXPORT uint16_t ByteSwap(uint16_t value);
    TL_EXPORT uint32_t ByteSwap(uint32_t value);
    TL_EXPORT uint64_t ByteSwap(uint64_t value);

    inline static int16_t ByteSwap(int16_t value)      { return std::bit_cast<int16_t>(ByteSwap(std::bit_cast<uint16_t>(value))); }
    inline static int32_t ByteSwap(int32_t value)      { return std::bit_cast<int32_t>(ByteSwap(std::bit_cast<uint32_t>(value))); }
    inline static int64_t ByteSwap(int64_t value)      { return std::bit_cast<int64_t>(ByteSwap(std::bit_cast<uint64_t>(value))); }
    inline static float ByteSwap(float value)          { return std::bit_cast<float>(ByteSwap(std::bit_cast<uint32_t>(value))); }
    inline static double ByteSwap(double value)        { return std::bit_cast<double>(ByteSwap(std::bit_cast<uint64_t>(value))); }

    // clang-format on

    class BinaryArchive
    {
    public:
        BinaryArchive(std::iostream& stream)
            : m_endieanness(std::endian::native)
            , m_stream(&stream)
        {
        }

        template<Serializable<BinaryArchive> T>
        void Encode(const T& value);

        template<Deserializable<BinaryArchive> T>
        void Decode(T& value);

        void StreamWrite(Block block);
        void StreamRead(Block block);

        template<typename T>
        inline T ConvertByteOrder(T value)
        {
            return (m_endieanness == std::endian::native) ? value : ByteSwap(value);
        }

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

    // clang-format off
    inline static void Encode(BinaryArchive& archive, Block block)      { archive.StreamWrite(block); }
    inline static void Decode(BinaryArchive& archive, Block block)      { archive.StreamRead(block); }
    inline static void Encode(BinaryArchive& archive, bool value)       { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, bool& value)      { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, char value)       { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, char& value)      { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, uint8_t value)    { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, uint8_t& value)   { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, uint16_t value)   { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, uint16_t& value)  { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, uint32_t value)   { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, uint32_t& value)  { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, uint64_t value)   { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, uint64_t& value)  { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, int8_t value)     { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, int8_t& value)    { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, int16_t value)    { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, int16_t& value)   { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, int32_t value)    { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, int32_t& value)   { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, int64_t value)    { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, int64_t& value)   { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, float value)      { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, float& value)     { archive.StreamRead(Block::Create(value)); }
    inline static void Encode(BinaryArchive& archive, double value)     { archive.StreamWrite(Block::Create(value)); }
    inline static void Decode(BinaryArchive& archive, double& value)    { archive.StreamRead(Block::Create(value)); }

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
        if (stringSize > 0)
        {
            Decode(archive, Block{ (void*)value.data(), stringSize * sizeof(CharType) });
        }
    }

    template<class Type, class Allocator>
    inline void Encode(BinaryArchive& archive, const std::vector<Type, Allocator>& values)
    {
        Encode(archive, values.size());
        for (const auto& value : values)
        {
            Encode(archive, value);
        }
    }

    template<class Type, class Allocator>
    inline void Decode(BinaryArchive& archive, std::vector<Type, Allocator>& values)
    {
        size_t size;
        Decode(archive, size);
        values.resize(size);
        for (auto& value : values)
        {
            Decode(archive, value);
        }
    }

    template<typename Key, typename Value>
    inline void Encode(BinaryArchive& archive, const std::pair<Key, Value>& value)
    {
        Encode(archive, value.first);
        Encode(archive, value.second);
    }

    template<typename Key, typename Value>
        requires std::is_move_constructible_v<Key> && std::is_move_constructible_v<Value>
    inline void Decode(BinaryArchive& archive, std::pair<Key, Value>& value)
    {
        Key first = Decode<Key>(archive);                            // Decode into temporary Key
        Value second = Decode<Value>(archive);                       // Decode into temporary Value
        value = std::make_pair(std::move(first), std::move(second)); // Move into the pair
    }

    template<typename Key, typename Value>
    inline void Decode(BinaryArchive& archive, std::pair<Key, Value>& value)
    {
        Key k{};
        Value v{};

        Decode(archive, k);
        Decode(archive, v);

        value = std::make_pair(std::move(k), std::move(v));
    }

    template<class Key, class Type, class Hasher, class KeyEq, class Allocator>
    inline void Encode(BinaryArchive& archive, const std::unordered_map<Key, Type, Hasher, KeyEq, Allocator>& values)
    {
        Encode(archive, values.size());
        for (const auto& [key, value] : values)
        {
            Encode(archive, key);
            Encode(archive, value);
        }
    }

    template<class Key, class Type, class Hasher, class KeyEq, class Allocator>
    inline void Decode(BinaryArchive& archive, std::unordered_map<Key, Type, Hasher, KeyEq, Allocator>& values)
    {
        size_t size{};
        Decode(archive, size);
        values.reserve(size); // Reserve space in the unordered_map

        for (size_t i = 0; i < size; i++)
        {
            Key key = {};
            Decode(archive, key);

            values[key] = {};
            Decode(archive, values[key]);
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

    // Encode function for enums
    template<typename T>
        requires std::is_enum_v<T>
    inline void Encode(BinaryArchive& archive, const T& value)
    {
        using UnderlyingType = std::underlying_type_t<T>;
        Encode(archive, static_cast<UnderlyingType>(value));
    }

    // Decode function for enums
    template<typename T>
        requires std::is_enum_v<T>
    inline void Decode(BinaryArchive& archive, T& value)
    {
        using UnderlyingType = std::underlying_type_t<T>;
        UnderlyingType underlyingValue;
        Decode(archive, underlyingValue);
        value = static_cast<T>(underlyingValue);
    }

    template<typename T>
    inline static void Encode(BinaryArchive& archive, const std::optional<T>& opt)
    {
        bool has_value = opt.has_value();
        Encode(archive, has_value); // Encode whether the optional has a value
        if (has_value)
        {
            Encode(archive, *opt); // Encode the actual value
        }
    }

    template<typename T>
    inline static void Decode(BinaryArchive& archive, std::optional<T>& opt)
    {
        bool has_value;
        Decode(archive, has_value); // Decode whether the optional has a value
        if (has_value)
        {
            T value;
            Decode(archive, value); // Decode the actual value
            opt = std::move(value); // Assign the value to the optional
        }
        else
        {
            opt = std::nullopt; // Ensure the optional is empty
        }
    }

    inline static void Encode(BinaryArchive& archive, const std::filesystem::path& path)
    {
        std::string pathStr = path.string();
        Encode(archive, pathStr);
    }

    inline static void Decode(BinaryArchive& archive, std::filesystem::path& path)
    {
        std::string pathStr;
        Decode(archive, pathStr);
        path = std::filesystem::path(pathStr);
    }
} // namespace TL