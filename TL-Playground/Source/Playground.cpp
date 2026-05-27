#include <TL/Allocator/Arena.hpp>
#include <TL/Allocator/MemPlumber.hpp>
#include <TL/Serialization/Binary.hpp>

#include <TL/Serialization/Binary.hpp>
#include <TL/Containers/Vector.hpp>
#include <TL/Containers/Map.hpp>
#include <TL/Containers/Set.hpp>
#include <TL/File/File.hpp>
#include <TL/Log.hpp>
#include <TL/Assert.hpp>
#include <TL/Stacktrace.hpp>
#include <TL/Literals.hpp>
#include <TL/FileSystem/FileSystemWatcher.hpp>

#include <thread>

#include <nlohmann/json.hpp>

#if 0

class FooLibrary final : public TL::LibraryWatcher
{
public:
    FooLibrary()
        : TL::LibraryWatcher("TL_Dll_Playground.dll")
    {
    }

    ~FooLibrary()
    {
        TL::LogInfo("FooLibrary destroyed");
    }

    void onUpdate(TL::LibraryLoaderAction action, void* state) override
    {
        switch (action)
        {
        case TL::LibraryLoaderAction::Load:
            TL::LogInfo("Library loaded");
            break;
        case TL::LibraryLoaderAction::Unload:
            TL::LogInfo("Library unloaded");
            break;
        case TL::LibraryLoaderAction::Reload:
            TL::LogInfo("Library reloaded");
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
            TL::LogInfo("{}", i);
        }

        auto stacktrace = TL::CaptureStacktrace(0);

        TL::LogWarn("Stack report: {}", TL::ReportStacktrace(stacktrace));

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

        TL::LogInfo(" f: {}, b: {}, n: {}", decoded.f, decoded.b, decoded.n);
        for (auto [key, value] : decoded.names)
        {
            TL::LogInfo("{} {}", key, value);
        }
        for (auto f : decoded.foos)
        {
            TL::LogInfo("{} {}", f.i, f.b);
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
            TL::LogInfo("File event, path {}, type {}, target {}, oldPath {}", path, (int)type, (int)target, oldPath);
            return false;
        });

        // TL::CodeGen::Builder builder{};
        // auto typeI32 = builder.DeclareType(nullptr, builder.CreateId("I32"), TL::CodeGen::Type::Kind::I32);
        // auto typeSceneView = builder.DeclareType(nullptr, builder.CreateId("SceneView"), TL::CodeGen::Type::Kind::Struct);
        // builder.StructAddField(typeI32, builder.CreateId("foo"));
        // TL::LogInfo("{}", builder.DumpCppCode());

        do
        {
            watcher.poll();
        }
        while(true);
    }
}

#endif

#include <TL/Context.hpp>
#include <TL/File/File.hpp>
#include <TL/FileSystem/FileSystem.hpp>
#include <TL/FileSystem/FileSystemWatcher.hpp>
#include <TL/Log.hpp>
#include <TL/Ptr.hpp>

#include <chrono>
#include <thread>

int main()
{
    TL::Vector<int>   v = {1, 2, 3, 4, 5};
    TL::Map<int, int> m = {
        {1, 9},
        {8, 2},
        {7, 3},
        {6, 4},
        {5, 3},
        {4, 2},
    };

    TL::Set<int> s = {1, 1, 1, 2, 1, 1, 1, 2, 2, 2, 4, 3};
    TL::Ptr<int> p = TL::CreatePtr<int>(1);

    // struct TestObject
    // {
    //     int   x;
    //     float y;
    //     char _padd[16_mb];
    //     char  tag;

    //     TestObject(int a, float b, char c)
    //         : x(a)
    //         , y(b)
    //         , tag(c)
    //     {
    //     }
    // };

    // TL::Arena     arena;

    // constexpr int kOuterLoops  = 1'000'000;
    // constexpr int kInnerAllocs = 64;

    // for (int i = 0; i < kOuterLoops; ++i)
    // {
    //     // allocate a few objects per iteration
    //     for (int j = 0; j < kInnerAllocs; ++j)
    //     {
    //         int         a = j;
    //         float       b = j * 0.5f;
    //         char        c = 'A' + (j % 26);

    //         TestObject* obj = TL::constructFrom<TestObject>(&arena, a, b, c);
    //         assert(obj);
    //         assert(obj->x == a);
    //         assert(obj->y == b);
    //         assert(obj->tag == c);
    //     }

    //     // After allocations, reset the arena
    //     arena.reset();

    //     // Check that arena is reusing memory properly (no new allocations expected)
    //     // This is indirect: if reset() failed, subsequent allocations would crash/assert.
    //     if ((i % 100000) == 0)
    //         printf("Iteration %d OK\n", i);
    // }

    // printf("Arena stress test completed successfully.\n");
    // return 0;

    TL::FileWatcher watcher;
    auto            events = TL::FileEventType::Added | TL::FileEventType::Removed |
                  TL::FileEventType::Modified | TL::FileEventType::Renamed;
    watcher.watch("./watch-test", events, true);
    watcher.subscribe(
        [](const TL::FileEvent& event)
        {
            TL::LogInfo("File event, path {}, type {}, target {}, oldPath {}",
                        event.path, (int)event.type, (int)event.target, event.oldPath);
            return false;
        });

    bool running = true;
    while (running)
    {
        watcher.poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}