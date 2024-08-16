#include <TL/Serialization/Binary.hpp>
#include <TL/Serialization/Binary.inl>

#include <TL/Compression/Compression.hpp>
#include <TL/Containers.hpp>
#include <TL/FileSystem/FileSystem.hpp>
#include <TL/Allocator.hpp>
#include <TL/Log.hpp>
#include <TL/Assert.hpp>
#include <TL/Stacktrace.hpp>

struct Foo
{
    int i = 13;
    int b = 14;
    template<typename Archive>
    void Serialize(Archive& archive) const
    {
        TL::Encode(archive, i);
        TL::Encode(archive, b);
    }

    template<typename Archive>
    void Deserialize(Archive& archive)
    {
        TL::Decode(archive, i);
        TL::Decode(archive, b);
    }
};

struct Bar
{
    float f;
    float b;
    TL::String n;
    TL::UnorderedMap<TL::String, TL::String> names;
    std::vector<Foo> foos;


    template<typename Archive>
    void Serialize(Archive& archive) const
    {
        TL::Encode(archive, f);
        TL::Encode(archive, b);
        TL::Encode(archive, n);
        TL::Encode(archive, names);
        TL::Encode(archive, foos);
    }

    template<typename Archive>
    void Deserialize(Archive& archive)
    {
        TL::Decode(archive, f);
        TL::Decode(archive, b);
        TL::Decode(archive, n);
        TL::Decode(archive, names);
        TL::Decode(archive, foos);
    }
};

int main()
{
    // bool condition = false;

    // TL_ASSERT(condition);
    // TL_ASSERT(condition, "hello");
    // TL_ASSERT(condition, "hello {}", condition);

    [[maybe_unused]] auto _unusedBlock = TL::Allocator::Allocate(12, 1);

    for (auto i : TL::Span<const int>{ 1, 2, 3, 4, 5, 6 })
    {
        TL_LOG_INFO("{}", i);
    }

    auto stacktrace = TL::CaptureStacktrace(0);

    TL_LOG_WARNNING("Stack report: {}", TL::ReportStacktrace(stacktrace));

    Bar b;
    b.f = 3.14f;
    b.b = 2.16f;
    b.n = "Hello-There";
    b.names = {
        { "Hello", "World" },
        { "one", "1" },
        { "two", "2" },
        { "three", "3" },
        { "four", "4" },
    };
    b.foos = { {1,2}, {2, 3}, {4, 5} };

    {
        std::fstream fileStream{ "Bar.bin", std::ios::binary | std::ios::out };
        auto encoder = TL::BinaryArchive(fileStream);
        encoder.Encode(b);
    }

    Bar decoded{};
    {
        std::fstream fileStream{ "Bar.bin", std::ios::binary | std::ios::in };
        auto decoder = TL::BinaryArchive(fileStream);
        decoder.Decode(decoded);
    }

    {
        // auto decoded = TL::BinaryArchive::Decode<Bar>(encoded);
    }

    TL_LOG_INFO(" f: {}, b: {}, n: {}", decoded.f, decoded.b, decoded.n);
    for (auto [key, value] : decoded.names)
    {
        TL_LOG_INFO("{} {}", key, value);
    }
    for (auto f : decoded.foos)
    {
        TL_LOG_INFO("{} {}", f.i, f.b);
    }
}