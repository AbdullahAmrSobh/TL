#pragma once

#include "TL/Allocator.hpp"
#include "TL/Block.hpp"
#include "TL/Containers.hpp"

#include <sstream>
#include <cstring>
#include <stdexcept>
#include <type_traits>

namespace TL
{
    // TODO: rename decoding serialize to deserialize
    // TODO: wrap process in macro, and abstract interface to add json support
    // TODO: rename ArchiveEncoder/ArchiveDecoder to BinaryEncoder/BinaryDecoder
    // TODO: change the stringstream to improve performance
    // TODO: See if error reporting could be improved through concepts

    /// @brief Checks if the system is big endian
    inline static bool IsBigEndian()
    {
        union
        {
            uint32_t i;
            char c[4];
        } bint = { 0x01020304 };

        return bint.c[0] == 1;
    }

    /// @brief A simple class for serializing and deserializing data.
    /// @todo move Encode to ArchiveEncoder, and Decode to ArchiveDecoder.
    class Archive
    {
        template<typename T>
        friend void Process(struct ArchiveEncoder& archive, const T& t);

        template<typename T>
        friend void Process(struct ArchiveDecoder& archive, T& t);

        friend struct ArchiveEncoder;
        friend struct ArchiveDecoder;

    protected:
        Archive();
        Archive(Block block);

    public:
        /// @brief Serializes an object into a Block.
        template<typename T>
        static Block Encode(const T& t);

        /// @brief Deserializes a Block into an object.
        template<typename T>
        static T Decode(Block block);

    private:
        Block GetBlock() const;

        template<typename T>
        void Write(const T& value);

        template<typename T>
        void Read(T& value);

        template<typename T>
        T SwapEndian(T value) const;

        void WriteHeader();
        void ReadHeader();

    private:
        std::stringstream m_stream;
        bool m_isBigEndianStream = false;
    };

    struct ArchiveEncoder : public Archive
    {
    };

    struct ArchiveDecoder : public Archive
    {
        ArchiveDecoder(Block block)
            : Archive(block)
        {
            ReadHeader();
        };
    };

    template<typename T>
    Block Archive::Encode(const T& t)
    {
        ArchiveEncoder ar;
        ar.WriteHeader();
        Process(ar, t);
        return ar.GetBlock();
    }

    template<typename T>
    T Archive::Decode(Block block)
    {
        ArchiveDecoder ar(block);
        T t;
        Process(ar, t);
        return t;
    }

    template<typename T>
    void Archive::Write(const T& value)
    {
        T temp = value;
        if (IsBigEndian() != m_isBigEndianStream)
        {
            temp = SwapEndian(value);
        }
        m_stream.write(reinterpret_cast<const char*>(&temp), sizeof(T));
    }

    template<typename T>
    void Archive::Read(T& value)
    {
        m_stream.read(reinterpret_cast<char*>(&value), sizeof(T));
        if (m_stream.gcount() != sizeof(T))
        {
            throw std::runtime_error("Failed to read data from stream.");
        }

        if (IsBigEndian() != m_isBigEndianStream)
        {
            value = SwapEndian(value);
        }
    }

    template<typename T>
    T Archive::SwapEndian(T value) const
    {
        union
        {
            T value;
            uint8_t bytes[sizeof(T)];
        } source, dest;

        source.value = value;
        for (size_t i = 0; i < sizeof(T); i++)
        {
            dest.bytes[i] = source.bytes[sizeof(T) - i - 1];
        }
        return dest.value;
    }

    /// @brief Writes the endianess information to the stream.
    inline void Archive::WriteHeader()
    {
        bool isBigEndian = IsBigEndian();
        m_stream.write(reinterpret_cast<char*>(&isBigEndian), sizeof(isBigEndian));
    }

    /// @brief Reads the endianess information from the stream.
    inline void Archive::ReadHeader()
    {
        bool streamEndianess;
        m_stream.read(reinterpret_cast<char*>(&streamEndianess), sizeof(streamEndianess));
        if (m_stream.gcount() != sizeof(streamEndianess))
        {
            throw std::runtime_error("Failed to read stream header.");
        }
        m_isBigEndianStream = streamEndianess;
    }

    template<typename T>
    inline static void Process(ArchiveEncoder& archive, const T& t)
    {
        t.Serialize(archive);
    }

    template<typename T>
    inline static void Process(ArchiveEncoder& archive, const T* values, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            Process(archive, values[i]);
        }
    }

    template<typename T>
    inline static void Process(ArchiveDecoder& archive, T& t)
    {
        t.Serialize(archive);
    }

    template<typename T>
    inline static void Process(ArchiveDecoder& archive, T* values, size_t count)
    {
        for (size_t i = 0; i < count; i++)
        {
            Process(archive, values[i]);
        }
    }

    // clang-format off
    template<> inline void Process<bool>     (ArchiveEncoder& archive, const bool& value)      { archive.Write(value); }
    template<> inline void Process<bool>     (ArchiveDecoder& archive, bool& value)            { archive.Read(value); }

    template<> inline void Process<char>     (ArchiveEncoder& archive, const char& value)      { archive.Write(value); }
    template<> inline void Process<char>     (ArchiveDecoder& archive, char& value)            { archive.Read(value); }

    template<> inline void Process<uint8_t>  (ArchiveEncoder& archive, const uint8_t& value)   { archive.Write(value); }
    template<> inline void Process<uint8_t>  (ArchiveDecoder& archive, uint8_t& value)         { archive.Read(value); }

    template<> inline void Process<uint16_t> (ArchiveEncoder& archive, const uint16_t& value)  { archive.Write(value); }
    template<> inline void Process<uint16_t> (ArchiveDecoder& archive, uint16_t& value)        { archive.Read(value); }

    template<> inline void Process<uint32_t> (ArchiveEncoder& archive, const uint32_t& value)  { archive.Write(value); }
    template<> inline void Process<uint32_t> (ArchiveDecoder& archive, uint32_t& value)        { archive.Read(value); }

    template<> inline void Process<uint64_t> (ArchiveEncoder& archive, const uint64_t& value)  { archive.Write(value); }
    template<> inline void Process<uint64_t> (ArchiveDecoder& archive, uint64_t& value)        { archive.Read(value); }

    template<> inline void Process<int8_t>   (ArchiveEncoder& archive, const int8_t& value)    { archive.Write(value); }
    template<> inline void Process<int8_t>   (ArchiveDecoder& archive, int8_t& value)          { archive.Read(value); }

    template<> inline void Process<int16_t>  (ArchiveEncoder& archive, const int16_t& value)   { archive.Write(value); }
    template<> inline void Process<int16_t>  (ArchiveDecoder& archive, int16_t& value)         { archive.Read(value); }

    template<> inline void Process<int32_t>  (ArchiveEncoder& archive, const int32_t& value)   { archive.Write(value); }
    template<> inline void Process<int32_t>  (ArchiveDecoder& archive, int32_t& value)         { archive.Read(value); }

    template<> inline void Process<int64_t>  (ArchiveEncoder& archive, const int64_t& value)   { archive.Write(value); }
    template<> inline void Process<int64_t>  (ArchiveDecoder& archive, int64_t& value)         { archive.Read(value); }

    template<> inline void Process<float>    (ArchiveEncoder& archive, const float& value)     { archive.Write(value); }
    template<> inline void Process<float>    (ArchiveDecoder& archive, float& value)           { archive.Read(value); }

    template<> inline void Process<double>   (ArchiveEncoder& archive, const double& value)    { archive.Write(value); }
    template<> inline void Process<double>   (ArchiveDecoder& archive, double& value)          { archive.Read(value); }

    // clang-format off

    inline void Process(ArchiveEncoder& archive, Block& block)
    {
        Process(archive, block.size);
        Process(archive, static_cast<const uint8_t*>(block.ptr), block.size);
    }

    inline void Process(ArchiveDecoder& archive, Block& block)
    {
        Process(archive, block.size);
        block = Allocator::Allocate(block.size);
        Process(archive, static_cast<uint8_t*>(block.ptr), block.size);
    }

    template<typename T, typename AllocatorType>
    inline void Process(ArchiveEncoder& archive, const std::vector<T, AllocatorType>& value)
    {
        auto size = value.size();
        Process(archive, size);
        Process(archive, value.data(), value.size());
    }

    template<typename T, typename AllocatorType>
    inline void Process(ArchiveDecoder& archive, std::vector<T, AllocatorType>& value)
    {
        size_t vec_size = 0;
        Process(archive, vec_size);
        value.resize(vec_size);
        Process(archive, value.data(), value.size());
    }

    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    inline void Process(ArchiveEncoder& archive, const std::unordered_map<Key, Value, Hasher, KeyEq, StlAllocator<std::pair<const Key, Value>>>& value)
    {
        auto size = value.size();
        Process(archive, size);
        for (const auto& entry : value)
        {
            Process(archive, entry.first);
            Process(archive, entry.second);
        }
    }

    template<typename Key, typename Value, typename Hasher = std::hash<Key>, typename KeyEq = std::equal_to<Key>>
    inline void Process(ArchiveDecoder& archive, std::unordered_map<Key, Value, Hasher, KeyEq, StlAllocator<std::pair<const Key, Value>>>& value)
    {
        size_t size = 0;
        Process(archive, size);
        value.clear();

        for (size_t i = 0; i < size; ++i)
        {
            Key key;
            Value entry_value;
            Process(archive, key);
            Process(archive, entry_value);
            value.emplace(std::move(key), std::move(entry_value));
        }
    }

    template<typename CharT, typename Traits, typename Allocator>
    inline void Process(ArchiveEncoder& archive, const std::basic_string<CharT, Traits, Allocator>& value)
    {
        auto size = value.size();
        Process(archive, size);
        Process(archive, value.data(), value.size());
    }

    template<typename CharT, typename Traits, typename Allocator>
    inline void Process(ArchiveDecoder& archive, std::basic_string<CharT, Traits, Allocator>& value)
    {
        size_t str_size = 0;
        Process(archive, str_size);
        value.resize(str_size);
        Process(archive, value.data(), value.size());
    }
}
