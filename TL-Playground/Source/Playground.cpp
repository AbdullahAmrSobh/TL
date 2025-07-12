#include <TL/Allocator/Arena.hpp>
#include <TL/Allocator/MemPlumber.hpp>
#include <TL/DynamicLibrary.hpp>
#include <TL/Serialization/Binary.hpp>

#include <TL/Serialization/Binary.hpp>
#include <TL/Containers.hpp>
#include <TL/FileSystem/File.hpp>
#include <TL/Log.hpp>
#include <TL/Assert.hpp>
#include <TL/Stacktrace.hpp>
#include <TL/FileSystem/FileWatcher.hpp>
#include <thread>

class FooLibrary final : public TL::LibraryWatcher
{
public:
    FooLibrary()
        : TL::LibraryWatcher("TL_Dll_Playground.dll")
    {
    }

    ~FooLibrary()
    {
        TL_LOG_INFO("FooLibrary destroyed");
    }

    void onUpdate(TL::LibraryLoaderAction action, void* state) override
    {
        switch (action)
        {
        case TL::LibraryLoaderAction::Load:
            TL_LOG_INFO("Library loaded");
            break;
        case TL::LibraryLoaderAction::Unload:
            TL_LOG_INFO("Library unloaded");
            break;
        case TL::LibraryLoaderAction::Reload:
            TL_LOG_INFO("Library reloaded");
            break;
        default:
            break;
        }
    }
};

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

struct Foo2
{
    Foo2()            = default;
    Foo2(const Foo2&) = delete;

    template<typename Archive>
    void Serialize(Archive& archive) const
    {
        TL::Encode(archive, val);
    }

    template<typename Archive>
    void Deserialize(Archive& archive)
    {
        TL::Decode(archive, val);
    }

    int val;
};

struct Bar
{
    float                           f;
    float                           b;
    TL::String                      n;
    TL::Map<TL::String, TL::String> names;
    TL::Map<TL::String, Foo2>       names2;
    std::vector<Foo>                foos;

    template<typename Archive>
    void Serialize(Archive& archive) const
    {
        TL::Encode(archive, f);
        TL::Encode(archive, b);
        TL::Encode(archive, n);
        TL::Encode(archive, names);
        TL::Encode(archive, names2);
        TL::Encode(archive, foos);
    }

    template<typename Archive>
    void Deserialize(Archive& archive)
    {
        TL::Decode(archive, f);
        TL::Decode(archive, b);
        TL::Decode(archive, n);
        TL::Decode(archive, names);
        TL::Decode(archive, names2);
        TL::Decode(archive, foos);
    }
};

int main()
{
    {
        bool condition = true;
        TL_ASSERT(condition);
        TL_ASSERT(condition, "hello");
        TL_ASSERT(condition, "hello {}", condition);

        TL_MAYBE_UNUSED auto _unusedBlock  = TL::Allocate(12, 1);
        TL_MAYBE_UNUSED auto _unusedBlock2 = TL::Allocate(12, 1);

        TL::Release(_unusedBlock, 1);
        TL::Release(_unusedBlock2, 1);

        for (auto i : TL::Span<const int>{1, 2, 3, 4, 5, 6})
        {
            TL_LOG_INFO("{}", i);
        }

        auto stacktrace = TL::CaptureStacktrace(0);

        TL_LOG_WARNNING("Stack report: {}", TL::ReportStacktrace(stacktrace));

        Bar b;
        b.f     = 3.14f;
        b.b     = 2.16f;
        b.n     = "Hello-There";
        b.names = {
            {"Hello", "World"},
            {"one", "1"},
            {"two", "2"},
            {"three", "3"},
            {"four", "4"},
        };
        b.foos = {{1, 2}, {2, 3}, {4, 5}};

        {
            std::fstream fileStream{"Bar.bin", std::ios::binary | std::ios::out};
            auto         encoder = TL::BinaryArchive(fileStream);
            encoder.Encode(b);
        }

        Bar decoded{};
        {
            std::fstream fileStream{"Bar.bin", std::ios::binary | std::ios::in};
            auto         decoder = TL::BinaryArchive(fileStream);
            decoder.Decode(decoded);
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

        struct Foo
        {
            float f[14];
        };

        Foo* f     = TL::Allocate<Foo>(3);
        f[0].f[13] = 1.0f;
        f[1].f[13] = 2.0f;
        f[2].f[13] = 3.0f;
        // TL::Release(f, 3);

        TL::Arena arena = TL::Arena();
        Foo*      f2    = TL::Allocate<Foo>(&arena);

        arena.Collect();

        // FooLibrary fooLib;
        // while (true)
        // {
        //     fooLib.poll();
        //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // }


        TL::FileWatcher watcher;
        watcher.watch("I:/repos/TL/build/TL-Playground", TL::FileEventType::Modified, true);

        watcher.subscribe([](const TL::FileEvent& event)
        {
            auto path = event.path;
            auto type = event.type;
            auto target = event.target;
            auto oldPath = event.oldPath;
            TL_LOG_INFO("File event, path {}, type {}, target {}, oldPath {}", path, (int)type, (int)target, oldPath);
            return false;
        });

        // TL::CodeGen::Builder builder{};
        // auto typeI32 = builder.DeclareType(nullptr, builder.CreateId("I32"), TL::CodeGen::Type::Kind::I32);
        // auto typeSceneView = builder.DeclareType(nullptr, builder.CreateId("SceneView"), TL::CodeGen::Type::Kind::Struct);
        // builder.StructAddField(typeI32, builder.CreateId("foo"));
        // TL_LOG_INFO("{}", builder.DumpCppCode());

        do
        {
            watcher.poll();
        }
        while(true);
    }
}