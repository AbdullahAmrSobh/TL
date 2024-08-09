#pragma once

#include <TL/Allocator.hpp>

#include <sstream>
#include <cstring>

namespace TL
{
    inline static bool IsBigEndian()
    {
        union
        {
            uint32_t i;
            char c[4];
        } bint = { 0x01020304 };

        return bint.c[0] == 1;
    }

    class Archive
    {
        template<typename T>
        friend void Process(struct ArchiveEncoder& archive, T& t);

        template<typename T>
        friend void Process(struct ArchiveDecoder& archive, T& t);

    protected:
        Archive();
        Archive(Block block);

    public:
        // @todo: encode should take T as const
        template<typename T>
        static Block Encode(T& t);

        template<typename T>
        static T Decode(Block block);

    private:
        Block GetBlock() const;

        template<typename T>
        void Write(T& value)
        {
            if (IsBigEndian())
            {
                T temp = SwapEndian(value);
                m_stream.write(reinterpret_cast<char*>(&temp), sizeof(T));
            }
            else
            {
                m_stream.write(reinterpret_cast<char*>(&value), sizeof(T));
            }
        }

        template<typename T>
        void Read(T& value)
        {
            m_stream.read(reinterpret_cast<char*>(&value), sizeof(T));

            if (IsBigEndian())
            {
                value = SwapEndian(value);
            }
        }

        template<typename T>
        T SwapEndian(T value)
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

    private:
        std::stringstream m_stream;
    };

    struct ArchiveEncoder : public Archive
    {
    };

    struct ArchiveDecoder : public Archive
    {
        ArchiveDecoder(Block block)
            : Archive(block){};
    };

    template<typename T>
    Block Archive::Encode(T& t)
    {
        ArchiveEncoder ar;
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
    inline static void Process(ArchiveEncoder& archive, T& t)
    {
        t.Serialize(archive);
    }

    template<typename T>
    inline void Process(ArchiveEncoder& archive, T* values, size_t count)
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
    template<> inline void Process<bool>     (ArchiveEncoder& archive, bool& value)      { archive.Write(value); }
    template<> inline void Process<char>     (ArchiveEncoder& archive, char& value)      { archive.Write(value); }
    template<> inline void Process<uint8_t>  (ArchiveEncoder& archive, uint8_t& value)   { archive.Write(value); }
    template<> inline void Process<uint16_t> (ArchiveEncoder& archive, uint16_t& value)  { archive.Write(value); }
    template<> inline void Process<uint32_t> (ArchiveEncoder& archive, uint32_t& value)  { archive.Write(value); }
    template<> inline void Process<uint64_t> (ArchiveEncoder& archive, uint64_t& value)  { archive.Write(value); }
    template<> inline void Process<int8_t>   (ArchiveEncoder& archive, int8_t& value)    { archive.Write(value); }
    template<> inline void Process<int16_t>  (ArchiveEncoder& archive, int16_t& value)   { archive.Write(value); }
    template<> inline void Process<int32_t>  (ArchiveEncoder& archive, int32_t& value)   { archive.Write(value); }
    template<> inline void Process<int64_t>  (ArchiveEncoder& archive, int64_t& value)   { archive.Write(value); }
    template<> inline void Process<float>    (ArchiveEncoder& archive, float& value)     { archive.Write(value); }
    template<> inline void Process<double>   (ArchiveEncoder& archive, double& value)    { archive.Write(value); }

    template<> inline void Process<bool>     (ArchiveDecoder& archive, bool& value)      { archive.Read(value); }
    template<> inline void Process<char>     (ArchiveDecoder& archive, char& value)      { archive.Read(value); }
    template<> inline void Process<uint8_t>  (ArchiveDecoder& archive, uint8_t& value)   { archive.Read(value); }
    template<> inline void Process<uint16_t> (ArchiveDecoder& archive, uint16_t& value)  { archive.Read(value); }
    template<> inline void Process<uint32_t> (ArchiveDecoder& archive, uint32_t& value)  { archive.Read(value); }
    template<> inline void Process<uint64_t> (ArchiveDecoder& archive, uint64_t& value)  { archive.Read(value); }
    template<> inline void Process<int8_t>   (ArchiveDecoder& archive, int8_t& value)    { archive.Read(value); }
    template<> inline void Process<int16_t>  (ArchiveDecoder& archive, int16_t& value)   { archive.Read(value); }
    template<> inline void Process<int32_t>  (ArchiveDecoder& archive, int32_t& value)   { archive.Read(value); }
    template<> inline void Process<int64_t>  (ArchiveDecoder& archive, int64_t& value)   { archive.Read(value); }
    template<> inline void Process<float>    (ArchiveDecoder& archive, float& value)     { archive.Read(value); }
    template<> inline void Process<double>   (ArchiveDecoder& archive, double& value)    { archive.Read(value); }
    // clang-format off

    template<typename T, typename AllocatorType>
    inline void Process(ArchiveEncoder& archive, std::vector<T, AllocatorType>& value)
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

    // @todo: add more specialization for more container types e.g. maps, set, list ...etc
} // namespace TL

