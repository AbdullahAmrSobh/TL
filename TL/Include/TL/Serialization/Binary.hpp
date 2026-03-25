#pragma once

#include "TL/Block.hpp"
#include "TL/Flags.hpp"
#include "TL/Bits.hpp"
#include "TL/Serialization/SerializeTraits.hpp"

#include "TL/Stream.hpp"

#include <vector>
#include <unordered_map>
#include <iostream>
#include <optional>
#include <filesystem>
#include <fstream>

namespace TL
{
    // TODO: Breakdown to BinaryReader, BinaryWriter
    class BinaryArchive
    {
    public:
        BinaryArchive(TL::IStream& stream)
            : m_stream(&stream)
            , m_cursor(0)
        {
        }

        template<Serializable<BinaryArchive> T>
        void Encode(const T& value);

        template<Deserializable<BinaryArchive> T>
        void Decode(T& value);

        void StreamWrite(Block block)
        {
            TL_MAYBE_UNUSED auto [cursor, result] = m_stream->write(block, m_cursor);
            TL_ASSERT(IsSuccess(result));
            m_cursor += cursor;
        }

        void StreamRead(Block block)
        {
            TL_MAYBE_UNUSED auto [cursor, result] = m_stream->read(block, m_cursor);
            TL_ASSERT(IsSuccess(result));
            m_cursor += cursor;
        }

    private:
        TL::IStream* m_stream = nullptr;
        size_t       m_cursor = 0;
    };

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

    inline static void Encode(BinaryArchive& archive, Block block) { archive.StreamWrite(block); }

    inline static void Decode(BinaryArchive& archive, Block block) { archive.StreamRead(block); }

    inline static void Encode(BinaryArchive& archive, bool value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, bool& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, char value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, char& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, uint8_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, uint8_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, uint16_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, uint16_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, uint32_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, uint32_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, uint64_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, uint64_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, int8_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, int8_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, int16_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, int16_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, int32_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, int32_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, int64_t value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, int64_t& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, float value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, float& value) { archive.StreamRead(Block::create(value)); }

    inline static void Encode(BinaryArchive& archive, double value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, double& value) { archive.StreamRead(Block::create(value)); }

    // for 32 builds (wasm)
    inline static void Encode(BinaryArchive& archive, unsigned long value) { archive.StreamWrite(Block::create(value)); }

    inline static void Decode(BinaryArchive& archive, unsigned long& value) { archive.StreamRead(Block::create(value)); }

    template<typename T>
        requires std::is_enum_v<T>
    inline static void Encode(BinaryArchive& archive, T value)
    {
        archive.StreamWrite(Block::create(static_cast<std::underlying_type_t<T>>(value)));
    }

    template<typename T>
        requires std::is_enum_v<T>
    inline static void Decode(BinaryArchive& archive, T& value)
    {
        std::underlying_type_t<T> temp;
        archive.StreamRead(Block::create(temp));
        value = static_cast<T>(temp);
    }

    template<class CharType, class CharTraits, class AllocatorType>
    inline void Encode(BinaryArchive& archive, const std::basic_string<CharType, CharTraits, AllocatorType>& value)
    {
        Encode(archive, value.size());
        if (value.size() != 0)
        {
            archive.StreamWrite(Block{.ptr = (void*)value.data(), .size = value.size() * sizeof(CharType)});
        }
        else
        {
            // nothing to write
        }
    }

    template<class CharType, class CharTraits, class AllocatorType>
    inline void Decode(BinaryArchive& archive, std::basic_string<CharType, CharTraits, AllocatorType>& value)
    {
        size_t stringSize = 0;
        Decode(archive, stringSize);
        if (stringSize > 0)
        {
            value.resize(stringSize);
            archive.StreamRead(Block{.ptr = (void*)value.data(), .size = value.size() * sizeof(CharType)});
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
        size_t size = 0;
        Decode(archive, size);
        values.resize(size);
        for (size_t i = 0; i < size; i++)
        {
            Decode(archive, values[i]);
        }
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
        values.reserve(size);

        for (size_t i = 0; i < size; i++)
        {
            Key  key{};
            Type value{};
            Decode(archive, key);
            Decode(archive, value);
            values[std::move(key)] = std::move(value);
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

    template<typename T>
    inline static void Encode(BinaryArchive& archive, TL::Flags<T> flags)
    {
        Encode(archive, (TL::Flags<T>::MaskType)(flags));
    }

    template<typename T>
    inline static void Decode(BinaryArchive& archive, TL::Flags<T>& flags)
    {
        using UnderlyingType = typename TL::Flags<T>::MaskType;
        UnderlyingType value = flags;
        Decode(archive, value);
        flags = value;
    }
} // namespace TL

#define TL_ENCODE(archive, variable)                                \
    if constexpr (std::is_same_v(delctpye(archive), BinaryArchive)) \
        ::TL::Encode(archive, variable);                            \
    else                                                            \
        ::TL::Encode(archive, #variable, variable);

#define TL_DECODE(archive, variable)                                \
    if constexpr (std::is_same_v(delctpye(archive), BinaryArchive)) \
        ::TL::Decode(archive, variable);                            \
    else                                                            \
        ::TL::Decode(archive, #variable, variable);
