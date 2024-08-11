#include <TL/Serialization/Serialization.hpp>
#include <TL/Compression/Compression.hpp>
#include <TL/Containers.hpp>
#include <TL/FileSystem/FileSystem.hpp>
#include <TL/Allocator.hpp>
#include <TL/Log.hpp>
#include <TL/Assert.hpp>

struct Bar
{
    float f;
    float b;
    TL::String n;
    TL::UnorderedMap<TL::String, TL::String> name;

    void Serialize(TL::ArchiveEncoder& archive) const
    {
        ::TL::Process(archive, f);
        ::TL::Process(archive, b);
        ::TL::Process(archive, n);
        ::TL::Process(archive, name);
    }

    void Serialize(TL::ArchiveDecoder& archive)
    {
        ::TL::Process(archive, f);
        ::TL::Process(archive, b);
        ::TL::Process(archive, n);
        ::TL::Process(archive, name);
    }
};

int main()
{
    // bool condition = false;

    // TL_ASSERT(condition);
    // TL_ASSERT(condition, "hello");
    // TL_ASSERT(condition, "hello {}", condition);

    for (auto i : TL::Span<const int> { 1, 2, 3, 4, 5, 6})
    {
        TL_LOG_INFO("{}", i);
    }

    Bar b;
    b.f = 3.14;
    b.b = 2.16;
    b.n = "Hello-There";
    b.name = {
        { "Hello", "World" },
        { "one", "1" },
        { "two", "2" },
        { "three", "3" },
        { "four", "4" },
    };

    auto encoded = TL::Archive::Encode(b);
    auto compressed = TL::CompressBlock(encoded);
    // save to disk

    // read from disk
    auto decompressed = TL::DecompressBlock(compressed);
    auto decoded = TL::Archive::Decode<Bar>(decompressed);

    TL_LOG_INFO(" f: {}, b: {}, n: {}", decoded.f, decoded.b, decoded.n);
    for (auto [key, value] : decoded.name)
    {
        TL_LOG_INFO("{} {}", key, value);
    }
}